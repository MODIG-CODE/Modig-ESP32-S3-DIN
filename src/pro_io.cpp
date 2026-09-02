#include <Arduino.h>
#include "pro_io.h"
#include "pro_mess.h"
#include "driver/adc.h"


IOs::IOs(uint32_t num_of_io, uint32_t num_of_in, uint32_t num_of_ana){
    //---------------------------------
    if (num_of_io > 0) {
        if (num_of_io > IOS_MAX) num_of_io = IOS_MAX;
        ios_ = num_of_io;
        obase_ = new PRO_IO_STRUCT[num_of_io];
        for (uint32_t i=0;i<ios_;i++){
            obase_[i].active = -1;
        }
    } else {
        ios_ = 0;
        obase_ = new PRO_IO_STRUCT[1];
    }

    //---------------------------------
    if (num_of_in > 0) {
        if (num_of_in > IOS_MAX) num_of_in = IOS_MAX;
        ins_ = num_of_in;
        ibase_ = new PRO_IOI_STRUCT[num_of_in];
        for (uint32_t i=0;i<ins_;i++){
            ibase_[i].active = -1;
        }
    } else {
        ins_ = 0;
        ibase_ = new PRO_IOI_STRUCT[1];
    }

    //---------------------------------
    if (num_of_ana > 0) {
        if (num_of_ana > IOS_MAX) num_of_ana = IOS_MAX;
        aios_ = num_of_ana;
        abase_ = new PRO_ANA_STRUCT[num_of_ana];
        aqueue_ = 0;
        adc1_config_width(ADC_WIDTH_BIT_12);
    } else {
        aios_ = 0;
        abase_ = new PRO_ANA_STRUCT[1];
    }
}

IOs::~IOs(){
    delete[] obase_;
    delete[] ibase_;
    delete[] abase_;
}

bool IOs::begin(uint32_t msdelay){  
    delay_ms_ = msdelay;
    delay_ = (msdelay / portTICK_PERIOD_MS);
    if (delay_ == 0) delay_ = 1;

    adc_range_ = 4096;
    adc_vref_ = 1100;

    xTaskCreate(IOsTask, "IOsTask", IOS_TASK_STACK, (void*)this, IOS_TASK_PRIO, &IoTaskHandle);
    return true;
}

bool IOs::setup_outputs(uint32_t index, uint32_t io_pin, uint32_t active, uint32_t od){
    if (index < ios_){
        if (active > 1) active = 1;
        obase_[index].pin = io_pin;
        obase_[index].active = active;
        obase_[index].period = 0;
        obase_[index].act_sta = 0;
        if (od == 0) pinMode(io_pin, OUTPUT);
        else pinMode(io_pin, OUTPUT_OPEN_DRAIN);
        if (active == 0) digitalWrite(io_pin, 1);
        else digitalWrite(io_pin, 0);
        return true;
    }
    return false;
}

bool IOs::setup_inputs(uint32_t index, uint32_t io_pin, uint32_t active, uint32_t pullup){
    if (index < ins_){
        if (active > 1) active = 1;
        ibase_[index].pin = io_pin;
        ibase_[index].active = active;
        ibase_[index].act_sta = 0;
        if (pullup == 0) pinMode(io_pin, INPUT_PULLUP);
        if (pullup == 1) pinMode(io_pin, OUTPUT_OPEN_DRAIN | INPUT_PULLUP);
        if (pullup == 2) pinMode(io_pin, INPUT_PULLDOWN);
        return true;
    }
    return false;
}

bool IOs::setup_analog(uint32_t index, uint32_t aio_pin, uint32_t r1, uint32_t r2){    
    if (index < aios_){
        uint32_t chn = 0;
        if (aio_pin == 0) chn = ADC1_CHANNEL_0;
        if (aio_pin == 1) chn = ADC1_CHANNEL_1;
        if (aio_pin == 2) chn = ADC1_CHANNEL_2;
        if (aio_pin == 3) chn = ADC1_CHANNEL_3;
        if (aio_pin == 4) chn = ADC1_CHANNEL_4;
        abase_[index].pin = aio_pin;
        abase_[index].ch = chn;
        abase_[index].resH = r1;
        abase_[index].resL = r2;
        pinMode(abase_[index].pin, INPUT);
        adc1_config_channel_atten((adc1_channel_t)abase_[index].ch, ADC_ATTEN_DB_0);
        return true;
    }
    return false;
}

void IOs::state(uint32_t index, uint32_t state){
    if (index < ios_){
        if (state == 0){
            obase_[index].act_sta = 0;
            if (obase_[index].active == 0) digitalWrite(obase_[index].pin, 1);
            else if (obase_[index].active == 1) digitalWrite(obase_[index].pin, 0);
        } else {
            if (obase_[index].active == 0) digitalWrite(obase_[index].pin, 0);
            else if (obase_[index].active == 1) digitalWrite(obase_[index].pin, 1);
            obase_[index].act_sta = 1;
        }
        //return true;
    }
    //return false;
}

uint32_t IOs::read(uint32_t index){
    if (index < ios_){
        return obase_[index].act_sta;
    }
    return 0;
}

bool IOs::pulse(uint32_t index, uint32_t msperiod){
    if (index < ios_){
        obase_[index].period = (msperiod + delay_ms_);
        state(index, 1);
        return true;
    }
    return false;
}

uint32_t IOs::iread(uint32_t index){
    if (index < ins_){
        return ibase_[index].act_sta;
    }
    return 0;
}

uint32_t IOs::iread_click(uint32_t index){
    if (index < ins_){
        if (ibase_[index].click > 0){
            ibase_[index].click = 0;
            return 1;
        }
    }
    return 0;
}

//--------------------------------------------------------------------------
uint32_t IOs::adc2v(uint32_t index){
	//uint64_t result = 0;
	double result;
	if (index < aios_){
		result = abase_[index].raw;
		result *= (abase_[index].resH + abase_[index].resL);
        result /= (abase_[index].resL);
		result *= adc_vref_;
		result /= adc_range_;
		//result += 5; //poprawia dokądność przy dzieleniu
		//result /= 10;
        abase_[index].v_val = (uint32_t)result;
	}
	return (uint32_t)result;
}

//--------------------------------------------------------------------------
uint32_t IOs::adcraw(uint32_t index){
	if (index < aios_){
        return abase_[index].raw;
	}
	return 0;
}

//--------------------------------------------------------------------------
uint32_t IOs::adcv(uint32_t index){
	if (index < aios_){
        return abase_[index].v_val;
	}
	return 0;
}

//--------------------------------------------------------------------------
void IOs::IOsTask(void* obj){
    IOs* my_ios = (IOs*)obj;
    uint32_t i;
    uint32_t ai;
    uint32_t m = 0;

    while(1){
        //menage outputs
        for (i=0;i<my_ios->ios_;i++){
            if (my_ios->obase_[i].period > 0){
                if (my_ios->obase_[i].period < IOS_ALWAYS_ON){
                    if (my_ios->obase_[i].period >= my_ios->delay_ms_){
                        my_ios->obase_[i].period -= my_ios->delay_ms_;
                    } else {
                        my_ios->obase_[i].period = 0;
                    }
                    if (my_ios->obase_[i].period == 0){
                        my_ios->state(i, 0);
                    }
                }
            }
        }

        //menage inputs
        for (i=0;i<my_ios->ins_;i++){
            if (digitalRead(my_ios->ibase_[i].pin) == my_ios->ibase_[i].active){
                if (my_ios->ibase_[i].act_sta < 0xFFFF){
                    my_ios->ibase_[i].act_sta++;
                    if (my_ios->ibase_[i].act_sta == IOS_CLICK){
                        my_ios->ibase_[i].click = 1;
                        ProMess::print("\r\nClick");
                    }
                }
            } else {
                my_ios->ibase_[i].act_sta = 0;
            }
        }

        //menage analogs
        if (my_ios->aios_ > 0) {
            if (ai >= my_ios->aios_) ai = 0;
            my_ios->abase_[ai].raw = adc1_get_raw((adc1_channel_t)my_ios->abase_[ai].ch);
            my_ios->adc2v(ai);
            ai++;
        }
        /*m++;
        if (m > 50){
            m = 0;
            ProMess::print("\r\nRaw1=", my_ios->abase_[0].raw);
            ProMess::print("\r\nV1=", my_ios->abase_[0].v_val);
            ProMess::print("\r\nRaw2=", my_ios->abase_[1].raw);
            ProMess::print("\r\nV2=", my_ios->abase_[1].v_val);
        }*/
        vTaskDelay(my_ios->delay_); 
    }
}