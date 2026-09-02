#include <Arduino.h>
#include "pro_leds.h"

Leds::Leds(uint32_t num_of_leds){
    if (num_of_leds == 0) num_of_leds = 1;
    if (num_of_leds > LEDS_MAX) num_of_leds = LEDS_MAX;
    leds_ = num_of_leds;
    base_ = new PRO_LEDS_STRUCT[num_of_leds];
    //fill not setup
    for (uint32_t i=0;i<leds_;i++){
        base_[i].active = -1;
    }
}

Leds::~Leds(){
    delete[] base_;
}

bool Leds::begin(){  
    return begin(LEDS_PROC_DELAY);
}

bool Leds::begin(uint32_t msdelay){  
    delay_ms_ = msdelay;
    delay_ = (msdelay / portTICK_PERIOD_MS);
    if (delay_ == 0) delay_ = 1;

    xTaskCreate(LedTask, "LedTask", LEDS_TASK_STACK, (void*)this, LEDS_TASK_PRIO, &LedTaskHandle);
    return true;
}

bool Leds::setup(uint32_t index, uint32_t led_pin, uint32_t active){
    if (index < leds_){
        if (active < 0) active = 0;
        if (active > 1) active = 1;
        base_[index].pin = led_pin;
        base_[index].active = active;
        base_[index].period = 0;

        if (active == 0) pinMode(led_pin, OUTPUT_OPEN_DRAIN | INPUT);
        else pinMode(led_pin, OUTPUT);

        if (active == 1) digitalWrite(led_pin, 0);
        else digitalWrite(led_pin, 1);
        return true;
    }
    return false;
}

bool Leds::state(uint32_t index, uint32_t state){
    if (index < leds_){
        if (state == 0){
            if (base_[index].active == 0) digitalWrite(base_[index].pin, 1);
            else if (base_[index].active == 1) digitalWrite(base_[index].pin, 0);
        } else {
            if (base_[index].active == 0) digitalWrite(base_[index].pin, 0);
            else if (base_[index].active == 1) digitalWrite(base_[index].pin, 1);
        }
        return true;
    }
    return false;
}

bool Leds::blink(uint32_t index, uint32_t msperiod){
    if (index < leds_){
        base_[index].period = (msperiod + delay_ms_);
        state(index, 1);
        return true;
    }
    return false;
}

bool Leds::norm(uint32_t index, uint32_t normal_state){
    if (index < leds_){
        if (normal_state == 0){
            base_[index].active = 1;
        } else {
            base_[index].active = 0;
        }
        state(index, 0);
        return true;
    }
    return false;
}

void Leds::LedTask(void* obj){
    Leds* my_leds = (Leds*)obj;
    while(1){
        for (uint32_t i=0;i<my_leds->leds_;i++){
            if (my_leds->base_[i].period > 0){
                if (my_leds->base_[i].period < LEDS_ALWAYS_ON){
                    if (my_leds->base_[i].period >= my_leds->delay_ms_){
                        my_leds->base_[i].period -= my_leds->delay_ms_;
                    } else {
                        my_leds->base_[i].period = 0;
                    }
                    if (my_leds->base_[i].period == 0){
                        my_leds->state(i, 0);
                    }
                }
            }
        }
        vTaskDelay(my_leds->delay_); 
    }
}



