
#include "pro_txt_tools.h"
#include "pro_rsbus.h"

HardwareSerial* RsBus::ser_[];
ProBuff* RsBus::buff_[];
ProBuff* RsBus::rbuff_[];

SemaphoreHandle_t RsBus::rsbusMutex[];
uint32_t RsBus::tx_en_pin_[];

TaskHandle_t RsBus::rsbusTaskHandle_ = NULL;

RsBus::RsBus(){
    xTaskCreate(rsbusTask, "RSbusTask", RSBUS_TASK_STACK, (void*)NULL, RSBUS_TASK_PRIO, &rsbusTaskHandle_);
}

RsBus::~RsBus(){
    delete[] RsBus::buff_;
    delete[] RsBus::rbuff_;
}

uint32_t RsBus::begin(uint32_t bus, uint32_t baud, uint32_t rx_pin, uint32_t tx_pin, uint32_t txen_pin){
    if (bus < RSBUS_BUSES){
        RsBus::buff_[bus] = new ProBuff(RSBUS_BUFF_LEN);
        RsBus::buff_[bus]->clear();
        RsBus::rbuff_[bus] = new ProBuff(RSBUS_BUFF_LEN);
        RsBus::rbuff_[bus]->clear();
        
        if (bus == 0) ser_[bus] = &Serial;
        if (bus == 1) ser_[bus] = &Serial1;

        RsBus::tx_en_pin_[bus] = txen_pin;
        pinMode(RsBus::tx_en_pin_[bus], OUTPUT);
        digitalWrite(RsBus::tx_en_pin_[bus], 0);
        RsBus::ser_[bus]->setMode(UART_MODE_RS485_HALF_DUPLEX);
        RsBus::ser_[bus]->begin(baud, SERIAL_8N1, rx_pin, tx_pin);
        //gpio_pullup_dis(rx_pin);
        gpio_pullup_en((gpio_num_t)rx_pin);
        rsbusMutex[bus] = xSemaphoreCreateMutex(); 
        return 1;
    }
    return 0;
}

ProBuff* RsBus::rxbase(uint32_t bus){
    return RsBus::rbuff_[bus];
}

ProBuff* RsBus::txbase(uint32_t bus){
    return RsBus::buff_[bus];
}

void RsBus::rsbusTask(void* obj){
    uint32_t i = 0;
    uint32_t bus = 0;
    uint32_t rec_delay = 0;
    int32_t c;

    while(1){
        for (bus = 0; bus < RSBUS_BUSES; bus++){
            if (RsBus::ser_[bus]){
                if (RsBus::ser_[bus]->available()){
                    i = 0;
                    while (1) {
                        c = RsBus::ser_[bus]->read();
                        if (c > -1) 
                            RsBus::rbuff_[bus]->add((uint8_t)c);
                        else
                            break;
                        i++;
                        if (i > RSBUS_TASK_MAX_SEND) break;                    
                    }
                }

                i = 0;
                if (RsBus::buff_[bus]->data_check()){
                    if (xSemaphoreTake(rsbusMutex[bus], 1000)){
                        RsBus::ser_[bus]->flush();
                        digitalWrite(RsBus::tx_en_pin_[bus], 1);
                        RsBus::ser_[bus]->write(RsBus::buff_[bus]->base(), buff_[bus]->fill_get());
                        RsBus::buff_[bus]->clear();
                        RsBus::ser_[bus]->flush(true);
                        digitalWrite(RsBus::tx_en_pin_[bus], 0);
                        xSemaphoreGive(rsbusMutex[bus]);
                    }
                }
            }
        }
        vTaskDelay(RSBUS_TASK_DELAY / portTICK_PERIOD_MS);
    }
}

//-----------------------------------------------------------------------------------
void RsBus::print(uint32_t bus, const char* mess){
    if (bus < RSBUS_BUSES){
        if (xSemaphoreTake(rsbusMutex[bus], portMAX_DELAY)){
            RsBus::buff_[bus]->text_add(mess);
            xSemaphoreGive(rsbusMutex[bus]);
        }
    }
}

void RsBus::print(uint32_t bus, char* mess){
    RsBus::print(bus, (const char*)mess);
}

//-----------------------------------------------------------------------------------
void RsBus::print(uint32_t bus, ProBuff* srcBuff){
    if (bus < RSBUS_BUSES){
        if (xSemaphoreTake(rsbusMutex[bus], portMAX_DELAY)){
            uint8_t u;
            while (srcBuff->data_xget(&u)){
                RsBus::buff_[bus]->add(u);
            }
            xSemaphoreGive(rsbusMutex[bus]);
        }
    }
}

void RsBus::print(uint32_t bus, String* smess){
    if (bus < RSBUS_BUSES){
        if (xSemaphoreTake(rsbusMutex[bus], portMAX_DELAY)){
            uint32_t len = smess->length();
            uint32_t i = 0;
            while(i<len){
                buff_[bus]->add(smess->charAt(i)); i++;
            }
            xSemaphoreGive(rsbusMutex[bus]);
        }
    }
}

void RsBus::print(uint32_t bus, uint32_t val){
    if (bus < RSBUS_BUSES){
        if (xSemaphoreTake(rsbusMutex[bus], portMAX_DELAY)){
            buff_[bus]->text_add(val);
            xSemaphoreGive(rsbusMutex[bus]);
        }
    }
}

//-----------------------------------------------------------------------------------
void RsBus::print(uint32_t bus, const char* mess, char* txt){
    if (bus < RSBUS_BUSES){
        if (xSemaphoreTake(rsbusMutex[bus], portMAX_DELAY)){
            RsBus::buff_[bus]->text_add(mess);
            RsBus::buff_[bus]->text_add(txt);
            xSemaphoreGive(rsbusMutex[bus]);
        }
    }
}

void RsBus::print(uint32_t bus, const char* mess, uint8_t* txt){
    if (bus < RSBUS_BUSES){
        if (xSemaphoreTake(rsbusMutex[bus], portMAX_DELAY)){
            RsBus::buff_[bus]->text_add(mess);
            RsBus::buff_[bus]->add(txt);
            xSemaphoreGive(rsbusMutex[bus]);
        }
    }
}

void RsBus::print(uint32_t bus, const char* mess, String stxt){
    if (bus < RSBUS_BUSES){
        if (xSemaphoreTake(rsbusMutex[bus], portMAX_DELAY)){
            RsBus::buff_[bus]->text_add(mess);
            uint32_t len = stxt.length();
            uint32_t i = 0;
            while(i<len){
                buff_[bus]->add(stxt.charAt(i)); i++;
            }
            xSemaphoreGive(rsbusMutex[bus]);
        }
    }
}

void RsBus::print(uint32_t bus, const char* mess, uint32_t val){
    if (bus < RSBUS_BUSES){
        if (xSemaphoreTake(rsbusMutex[bus], portMAX_DELAY)){
            RsBus::buff_[bus]->text_add(mess);
            buff_[bus]->text_add(val);
            xSemaphoreGive(rsbusMutex[bus]);
        }
    }
}

//-----------------------------------------------------------------------------------


