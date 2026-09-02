#ifndef _HARDDEF_H_
#define _HARDDEF_H_

#include "inttypes.h"


//RS485-1
#define RS485_1_TXD       43 //UART0
#define RS485_1_RXD       44 //UART0
#define RS485_1_RE_DE     38 //RE & DE PIN

//RS485-2
#define RS485_2_TXD       17 //UART1
#define RS485_2_RXD       18 //UART1
#define RS485_2_RE_DE     16 //RE & DE PIN

//I2C
#define I2C_SDA         10
#define I2C_SCL         9

//OUTPUTS
#define OUT1_PIN        48
#define OUT2_PIN        47
#define OUT_ON           1
#define OUT_OFF          0

//INPUTS
#define INPUT1_PIN      0   //ISP/BOOT switch
#define IN_ON           0
#define IN_OFF          1

//LEDs
#define LED1_PIN        11
#define STATUS_LED      0
#define LED_ON          0
#define LED_OFF         1

class Harddef{

};

#endif