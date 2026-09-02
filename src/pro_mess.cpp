
#include "pro_txt_tools.h"
#include "pro_mess.h"

//HardwareSerial* ProMess::ser_ = NULL;
HWCDC* ProMess::ser_ = NULL;
ProBuff* ProMess::buff_ = NULL;
uint32_t ProMess::ena_mask_ = 0;
TaskHandle_t ProMess::messTaskHandle_ = NULL;
SemaphoreHandle_t ProMess::messMutex = NULL;

ProMess::ProMess(){
    //ProMess::messbuff_ = new ProBuff(MESS_BUFF_LEN);
}

ProMess::~ProMess(){
    delete[] ProMess::buff_;
}

void ProMess::begin(uint32_t baud){
    ProMess::buff_ = new ProBuff(MESS_BUFF_LEN);
    ProMess::buff_->clear();
    ProMess::ena_mask_ = MESS_ALL_MASK;
    ser_ = &USBSerial;
    //ProMess::ser_ = &USBSerial;
    ser_->begin(baud);
    baud_ = baud;
    messMutex = xSemaphoreCreateMutex(); 
    xTaskCreate(messTask, "MessTask", MESS_TASK_STACK, (void*)NULL, 9, &ProMess::messTaskHandle_);
}

ProBuff* ProMess::txbase(){
    return ProMess::buff_;
}

void ProMess::ena_mask(uint32_t ena_mask){
    ProMess::ena_mask_ = ena_mask;
}

void ProMess::messTask(void* obj){
    uint32_t i = 0;
    uint8_t u;
    ProMess* oser = (ProMess*)obj;
    //oser->ser_->begin(oser->baud_);

    while(1){
        i = 0;
        if (buff_->data_check()){
            if (xSemaphoreTake(messMutex, 1000)){
                /*while (ProMess::buff_->data_xget(&u)){
                    oser->ser_->write(u);
                    i++; if (i > MESS_TASK_MAX_SEND) break;
                }*/
                oser->ser_->write(buff_->base(), buff_->fill_get());
                buff_->clear();
                xSemaphoreGive(messMutex);
            }
        }
        vTaskDelay(MESS_TASK_DELAY / portTICK_PERIOD_MS);
    }
}

//-----------------------------------------------------------------------------------
void ProMess::print(const char* mess, uint32_t mask){
    if ((ProMess::ena_mask_ & mask) > 0){
        if (xSemaphoreTake(messMutex, portMAX_DELAY)){
            ProMess::buff_->text_add(mess);
            xSemaphoreGive(messMutex);
        }
    }
}

void ProMess::print(char* mess, uint32_t mask){
    ProMess::print((const char*)mess, mask);
}

void ProMess::print(String* smess, uint32_t mask){
    if ((ProMess::ena_mask_ & mask) > 0){
        if (xSemaphoreTake(messMutex, portMAX_DELAY)){
            ProMess::buff_->text_add(smess);
            xSemaphoreGive(messMutex);
        }
    }
}

//-----------------------------------------------------------------------------------
void ProMess::print(const char* mess, char* txt, uint32_t mask){
    if ((ProMess::ena_mask_ & mask) > 0){
        if (xSemaphoreTake(messMutex, portMAX_DELAY)){
            ProMess::buff_->text_add(mess);
            ProMess::buff_->text_add(txt);
            xSemaphoreGive(messMutex);
        }
    }
}

void ProMess::print(const char* mess, uint8_t* txt, uint32_t mask){
    if ((ProMess::ena_mask_ & mask) > 0){
        if (xSemaphoreTake(messMutex, portMAX_DELAY)){
            ProMess::buff_->text_add(mess);
            ProMess::buff_->add(txt);
            xSemaphoreGive(messMutex);
        }
    }
}

void ProMess::print(const char* mess, String stxt, uint32_t mask){
    if ((ProMess::ena_mask_ & mask) > 0){
        if (xSemaphoreTake(messMutex, portMAX_DELAY)){
            ProMess::buff_->text_add(mess);
            ProMess::buff_->text_add(&stxt);
            xSemaphoreGive(messMutex);
        }
    }
}

void ProMess::print(const char* mess, int32_t val, uint32_t mask){
    if ((ProMess::ena_mask_ & mask) > 0){
        if (xSemaphoreTake(messMutex, portMAX_DELAY)){
            ProMess::buff_->text_add(mess);
            ProMess::buff_->text_add(val);
            xSemaphoreGive(messMutex);
        }
    }    
}

void ProMess::print_ip4(const char* mess, uint8_t* ip, uint32_t mask){
    if ((ProMess::ena_mask_ & mask) > 0){
       if (xSemaphoreTake(messMutex, portMAX_DELAY)){
            ProMess::buff_->text_add(mess);
            TxtTools::bytes2valtxt(buff_, ip, 4, '.');
        }
        xSemaphoreGive(messMutex);
    }        
}

void ProMess::print_ip4(const char* mess, IPAddress ip, uint32_t mask){
    uint32_t nip = ip;
    print_ip4(mess, (uint8_t*)&nip, mask);
}
//-----------------------------------------------------------------------------------



