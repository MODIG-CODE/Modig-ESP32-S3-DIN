#include <Arduino.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include "harddef.h"
#include "pro_leds.h"
#include "pro_mess.h"
#include "pro_io.h"
#include "pro_rsbus.h"
#include "pro_sett.h"
#include "pro_httpserver.h"

void WiFiprepare(void);
void WiFiEvent(WiFiEvent_t event);

Harddef hd;
Leds leds(2);
ProMess mess;
IOs ios(2,1,0); //2 outputs, 1 input, 0 analog in
RsBus rs485;
Sett sett;
ProHttpServer proserver(80);

//------------------------------------------------------------
void setup() {
  //Serial.begin(115200);
  mess.begin(115200);
  leds.setup(STATUS_LED, LED1_PIN, LED_ON);
  leds.begin();

  ios.setup_outputs(0, OUT1_PIN, 1);
  ios.setup_outputs(1, OUT2_PIN, 1);
  ios.setup_inputs(0, INPUT1_PIN, 0);
  ios.begin();
  ios.iread_click(0);

  rs485.begin(0, 19200, RS485_1_RXD, RS485_1_TXD, RS485_1_RE_DE);
  rs485.begin(1, 19200, RS485_2_RXD, RS485_2_TXD, RS485_2_RE_DE);

  for (int i=0; i<10; i++){
    leds.blink(STATUS_LED, 50);
    delay(200);
  }

  //---PSRAM init---
  if (!psramInit()) {
    mess.print(MESS_NL "PSRAM FAILED..."); 
  }

  //---SETT init---
  if (ios.iread(0) > 0){
    //---SETT restore defaults---
    leds.blink(STATUS_LED, 1000);
    if (!sett.begin(1)) while (1){};
    delay(1000);
  } else {
    if (!sett.begin(0)) while (1){};
  }
  //---deassert key---
  while (ios.iread(0) > 0);
    
  ios.pulse(0, 500);
  ios.pulse(1, 1000);

  sett.temporary_wifi_access(WIFI_DEF_SSID, WIFI_DEF_PASS);
  WiFiprepare();
  delay(100);

  proserver.io_funcs = &ios;
  proserver.pro_sett(&sett);
  proserver.begin();
  //proserver.add_action("/wifisett", HTTP_ANY, ServerTools);
  proserver.start();
  delay(200);
  mess.print(MESS_NL "WWW start");
  
  mess.print(MESS_NL "SYSTEM STARTING...");  
}

//------------------------------------------------------------
void loop() {
  uint32_t i = 0;
  uint32_t rec_len = 0;
  uint32_t rec_del = 11;
  while (1){
    i++;
    if (i > 20){
      i = 0;
      leds.blink(STATUS_LED, 50);
      rs485.print(0, MESS_NL "RS485-1 test");
      delay(200);
      rs485.print(1, MESS_NL "RS485-2 test");
    }

    if (ios.iread_click(0) > 0){
      mess.print(MESS_NL "-------------------------------");
      mess.print(MESS_NL "CHIP: ", (char*)(ESP.getChipModel()));
      mess.print(MESS_NL "CORES: ", (int32_t)(ESP.getChipCores()));
      mess.print(MESS_NL "FLASH: ", (int32_t)(ESP.getFlashChipSize()));
      mess.print(MESS_NL "PSRAM: ", (int32_t)(ESP.getPsramSize()));
      mess.print(MESS_NL "HEAP: ", (int32_t)(ESP.getHeapSize()));
      mess.print(MESS_NL "FL SPEED: ", (int32_t)(ESP.getFlashChipSpeed()));
      
      mess.print(MESS_NL "AP start, SSID: ", sett.ap_ssid);
      mess.print("; PASS: ", sett.ap_pass);
      mess.print_ip4(MESS_NL "Test page IP: ", WiFi.softAPIP());
      
      ios.pulse(0, 500);
      ios.pulse(1, 1000);
      rs485.print(0, MESS_NL "RS485-1 USER test");
      delay(200);
      rs485.print(1, MESS_NL "RS485-2 USER test");
    }
    delay(100);  
  }
}

//------------------------------------------------------------
void WiFiprepare(void){
  WiFi.onEvent(WiFiEvent);
  WiFi.mode(WIFI_AP_STA);

  //-----------
  if (sett.apEN > 0) {
    if(WiFi.softAPConfig(sett.apIP, sett.apGate, sett.apSub)) {
      if (WiFi.softAP(sett.ap_ssid, sett.ap_pass)){
        mess.print(MESS_NL "AP start, SSID: ", sett.ap_ssid);
        mess.print("; PASS: ", sett.ap_pass);
      } else {
        mess.print(MESS_NL "AP Fail...");
      }       
    } else {
      mess.print(MESS_NL "AP config Fail...");
    }
  } else {
    mess.print(MESS_NL "AP OFF");
  }
  
  //-----------
  bool status = true;
  if (sett.staEN > 0) {
    if (sett.staDHCP == 0){
      if(!WiFi.config(sett.staIP, sett.staGate, sett.staSub)) {
        mess.print(MESS_NL "WiFi net config Fail...");
        status = false;
      }
    }
    if (status){
      if (WiFi.begin(sett.sta_ssid, sett.sta_pass)){
        mess.print(MESS_NL "WiFi Conn, SSID: ", sett.sta_ssid);
        mess.print("; PASS: ", sett.sta_pass);
      } else {
        mess.print(MESS_NL "WiFi Fail...");
      }      
    } else {
      mess.print(MESS_NL "WiFi access config Fail...");
    }
  } else {
    mess.print(MESS_NL "WiFi Conn OFF");
  }
}


//------------------------------------------------------------
// Handler zdarzeń
void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    // --- STA (klient) ---
    case SYSTEM_EVENT_STA_CONNECTED:
      mess.print(MESS_NL "[STA] WiFi connected");
      leds.norm(STATUS_LED, LED_ON);
      break;

    case SYSTEM_EVENT_STA_DISCONNECTED:
      //mess.print(MESS_NL "[STA] WiFi disconnected");
      leds.norm(STATUS_LED, LED_OFF);
      break;

    case SYSTEM_EVENT_STA_GOT_IP:
      mess.print_ip4(MESS_NL"[STA] WiFi IP: ", WiFi.localIP());
      sett.sip_new(sett.staDHCP_IP,  WiFi.localIP());
      //leds.norm(STATUS_LED, LED_ON);
      break;

    // --- AP (punkt dostepowy) ---
    case SYSTEM_EVENT_AP_START:
      mess.print(MESS_NL "[AP] Access Point Ready.");
      mess.print(MESS_NL "[AP] SSID: ", sett.ap_ssid);
      mess.print_ip4("[AP] AP IP: ", WiFi.softAPIP());
      break;

    case SYSTEM_EVENT_AP_STACONNECTED:
      mess.print(MESS_NL "[AP] AP new client.");
      leds.norm(STATUS_LED, LED_ON);
      //leds.blink(STATUS_LED, 200);
      break;

    case SYSTEM_EVENT_AP_STADISCONNECTED:
      mess.print(MESS_NL "[AP] AP client disconnected.");
      leds.norm(STATUS_LED, LED_OFF);
      //leds.blink(STATUS_LED, 200);
      break;

    default:
      break;
  }
}