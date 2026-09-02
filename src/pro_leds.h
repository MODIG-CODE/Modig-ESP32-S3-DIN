
#ifndef _PROLEDS_H_
#define _PROLEDS_H_

#include "inttypes.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LEDS_MAX            16
#define LEDS_PROC_DELAY     50
#define LEDS_ALWAYS_ON      0xFFFF

#define LEDS_TASK_PRIO      1
#define LEDS_TASK_STACK     (2*1024)


typedef struct {
	uint32_t pin;
	int32_t active;
	volatile uint32_t period;
} PRO_LEDS_STRUCT;

class Leds{
    private:
        uint32_t delay_;
        uint32_t delay_ms_;
        uint32_t leds_;
        PRO_LEDS_STRUCT* base_;
        TaskHandle_t LedTaskHandle;

    public:
        Leds(uint32_t num_of_leds);
        ~Leds();
        bool begin();
        bool begin(uint32_t msdelay);
        bool setup(uint32_t index, uint32_t led_pin, uint32_t active);
        bool state(uint32_t index, uint32_t state);
        bool blink(uint32_t index, uint32_t msperiod);
        bool norm(uint32_t index, uint32_t normal_state);
        static void LedTask(void* obj);
};

#endif