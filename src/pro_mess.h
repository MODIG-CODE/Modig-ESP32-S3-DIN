#ifndef _PROMESS_H_
#define _PROMESS_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <Arduino.h>
#include "inttypes.h"
#include "pro_buff.h"

#define MESS_TASK_PRIO        1
#define MESS_TASK_STACK       (2*1024)
#define MESS_TASK_DELAY       100
#define MESS_BUFF_LEN         (uint32_t)(512)  

#define MESS_ALL_MASK         (uint32_t)0xFFFFFFFF
#define MESS_FLAG_READY       (uint32_t)(1<<0)
#define MESS_FLAG_PAGE        (uint32_t)(1<<1)
#define MESS_NL                "\r\n"

#define MESS_DEF_BAUD         115200

#define MPRIO_MAIN            1
#define MPRIO_IO              2

class ProMess{
    private:
        //static HardwareSerial* ser_;
        static HWCDC* ser_;
        uint32_t baud_;
        static ProBuff* buff_;
        static uint32_t ena_mask_;
        static TaskHandle_t messTaskHandle_;
        static SemaphoreHandle_t messMutex;
        static void messTask(void* obj);

    public:
        ProMess();
        ~ProMess();
        void begin(uint32_t baud = MESS_DEF_BAUD);
        static ProBuff* txbase();

        static void ena_mask(uint32_t flags);

        static void print(const char* mess, uint32_t mask = MPRIO_MAIN);
        static void print(char* mess, uint32_t mask = MPRIO_MAIN);
        static void print(String* smess, uint32_t mask = MPRIO_MAIN);

        static void print(const char* mess, char* txt, uint32_t mask = MPRIO_MAIN);
        static void print(const char* mess, uint8_t* txt, uint32_t mask = MPRIO_MAIN);
        static void print(const char* mess, String stxt, uint32_t mask = MPRIO_MAIN);
        static void print(const char* mess, int32_t val, uint32_t mask = MPRIO_MAIN);

        static void print_ip4(const char* mess, uint8_t* ip, uint32_t mask = MPRIO_MAIN);
        static void print_ip4(const char* mess, IPAddress ip, uint32_t mask = MPRIO_MAIN);
};



#endif