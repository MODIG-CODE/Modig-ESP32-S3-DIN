
#ifndef _PROSETT_H_
#define _PROSETT_H_

#include <Preferences.h>
#include "inttypes.h"

#define SSID_MAX_LEN        32
#define SSID_MIN_LEN        4
#define PASS_MAX_LEN        32
#define PASS_MIN_LEN        8
#define SETT_IP_LEN         4
#define RTC_SETT_LEN         4

#define AP_DEF_SSID     "ESP32-S3-AP"
#define AP_DEF_PASS     "12345678"

#define WIFI_DEF_SSID     "MY_WIFI_SSID"
#define WIFI_DEF_PASS     "MY_WIFI_PASS"

class Sett{
    protected:
        Preferences ipreferences;
    
    public:
        char ap_ssid[SSID_MAX_LEN+1] = {0};
        char ap_pass[PASS_MAX_LEN+1] = {0};
        char sta_ssid[SSID_MAX_LEN+1] = {0};
        char sta_pass[PASS_MAX_LEN+1] = {0};

        uint8_t apEN = 1;
        uint8_t apIP[SETT_IP_LEN] = {192,168,10,1};
        uint8_t apGate[SETT_IP_LEN] = {192,168,10,1};
        uint8_t apSub[SETT_IP_LEN] = {255,255,255,0};
        
        uint8_t staEN = 0;
        uint8_t staDHCP = 1;
        uint8_t staDHCP_IP[SETT_IP_LEN] = {0,0,0,0};
        uint8_t staIP[SETT_IP_LEN] = {10,1,0,162};
        uint8_t staGate[SETT_IP_LEN] = {10,1,0,1};
        uint8_t staSub[SETT_IP_LEN] = {255,255,255,0};
        
        Sett();
        bool begin(uint32_t erase = 0);
        bool temporary_wifi_access(const char* temp_ssid, const char* temp_pass);
        static uint32_t stxt_check(uint8_t* src);
        static uint32_t stxt_new(uint8_t* dst, uint32_t dlen, char* src);
        static uint32_t stxt_new(uint8_t* dst, uint32_t dlen, char* src, uint32_t slen);
        static uint32_t stxt_new(char* dst, uint32_t dlen, String src);
        bool sip_new(uint8_t* dst, String src);
        bool sip_new(uint8_t* dst, IPAddress ip);
        static void clear(uint8_t* dst, uint32_t len);
};

#endif