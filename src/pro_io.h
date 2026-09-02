
#ifndef _PROIO_H_
#define _PROIO_H_

#include "inttypes.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define IOS_MAX          8
#define IOS_PROC_DELAY    100
#define IOS_ALWAYS_ON     0xFFFFFFFF
#define IOS_CLICK       3

#define IOS_TASK_PRIO      12
#define IOS_TASK_STACK     (2*1024)

typedef struct {
	uint32_t pin;
	int32_t active;
    uint32_t act_sta;
    volatile uint32_t period;
} PRO_IO_STRUCT;

typedef struct {
	uint32_t pin;
	int32_t active;
    uint32_t act_sta;
    uint32_t click;
} PRO_IOI_STRUCT;

typedef struct {
	uint32_t pin;
    uint32_t ch;
    uint32_t resH;
    uint32_t resL;
	uint32_t raw;
	uint32_t v_val;
} PRO_ANA_STRUCT;

class IOs{
    private:
        uint32_t delay_;
        uint32_t delay_ms_;

        uint32_t adc_range_;
        uint32_t adc_vref_;
        uint32_t aqueue_;

        PRO_IO_STRUCT* obase_;
        PRO_IOI_STRUCT* ibase_;
        PRO_ANA_STRUCT* abase_;
        TaskHandle_t IoTaskHandle;

    public:
        uint32_t ios_;
        uint32_t ins_;
        uint32_t aios_;

        IOs(uint32_t num_of_io, uint32_t num_of_in, uint32_t num_of_ana);
        ~IOs();
        bool begin(uint32_t msdelay = IOS_PROC_DELAY);
        bool setup_outputs(uint32_t index, uint32_t io_pin, uint32_t active, uint32_t od = 0);
        bool setup_inputs(uint32_t index, uint32_t io_pin, uint32_t active, uint32_t pullup = 0);
        bool setup_analog(uint32_t index, uint32_t aio_pin, uint32_t r1 = 0, uint32_t r2 = 1);
        void state(uint32_t index, uint32_t state);
        uint32_t read(uint32_t index);
        bool pulse(uint32_t index, uint32_t msperiod);
        uint32_t iread(uint32_t index);
        uint32_t iread_click(uint32_t index);
        uint32_t adc2v(uint32_t index);
        uint32_t adcraw(uint32_t index);
        uint32_t adcv(uint32_t index);
        static void IOsTask(void* obj);
};

#endif