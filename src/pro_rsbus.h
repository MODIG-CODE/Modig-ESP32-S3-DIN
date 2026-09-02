#ifndef _PRORSBUS_H_
#define _PRORSBUS_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <Arduino.h>
#include "inttypes.h"
#include "pro_buff.h"

#define RSBUS_BUSES            2
#define RSBUS_TASK_PRIO        12
#define RSBUS_TASK_STACK       (2*1024)
#define RSBUS_TASK_DELAY       10
#define RSBUS_TASK_MAX_SEND    512
#define RSBUS_BUFF_LEN         (uint32_t)(1*256)  
#define RSBUS_RBUFF_LEN         (uint32_t)(1*256) 

#


class RsBus{
    private:
        static HardwareSerial* ser_[RSBUS_BUSES];
        static ProBuff* buff_[RSBUS_BUSES];
        static ProBuff* rbuff_[RSBUS_BUSES];
        static TaskHandle_t rsbusTaskHandle_;
        static void rsbusTask(void* obj);
        static uint32_t tx_en_pin_[RSBUS_BUSES];
        static SemaphoreHandle_t rsbusMutex[RSBUS_BUSES];

    public:
        RsBus();
        ~RsBus();
        static uint32_t begin(uint32_t bus, uint32_t baud, uint32_t rx_pin, uint32_t tx_pin, uint32_t txen_pin);
        static ProBuff* rxbase(uint32_t bus);
        static ProBuff* txbase(uint32_t bus);

        static void print(uint32_t bus, const char* mess);
        static void print(uint32_t bus, char* mess);
        static void print(uint32_t bus, ProBuff* srcBuff);
        static void print(uint32_t bus, String* smess);
        static void print(uint32_t bus, uint32_t val);

        static void print(uint32_t bus, const char* mess, char* txt);
        static void print(uint32_t bus, const char* mess, uint8_t* txt);
        static void print(uint32_t bus, const char* mess, String stxt);
        static void print(uint32_t bus, const char* mess, uint32_t val);
};



#endif