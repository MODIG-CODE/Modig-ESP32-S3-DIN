
#include "pro_sett.h"

Sett::Sett(){}

//-----------------------------------------------------------------------------------
bool Sett::begin(uint32_t erase){
    if (ipreferences.begin("nvmblock", false)){
        if (erase > 0) ipreferences.clear();

        //--------------------------------------------------
        if (!ipreferences.isKey("ap_ssid")){
            ipreferences.putBytes("ap_ssid", ap_ssid, SSID_MAX_LEN);
        }
        ipreferences.getBytes("ap_ssid", ap_ssid, SSID_MAX_LEN);
        ap_ssid[SSID_MAX_LEN] = 0;

        //--------------------------------------------------
        if (!ipreferences.isKey("ap_pass")){
            ipreferences.putBytes("ap_pass", ap_pass, PASS_MAX_LEN);
        }
        ipreferences.getBytes("ap_pass", ap_pass, PASS_MAX_LEN);
        ap_pass[PASS_MAX_LEN] = 0;

        //--------------------------------------------------
        //if ap ssid or pass is empty then fill default
        if ((stxt_check((uint8_t*)ap_ssid) < SSID_MIN_LEN) ||
            (stxt_check((uint8_t*)ap_pass) < PASS_MIN_LEN)){            
                stxt_new((uint8_t*)ap_ssid, SSID_MAX_LEN, (char*)AP_DEF_SSID);
                stxt_new((uint8_t*)ap_pass, PASS_MAX_LEN, (char*)AP_DEF_PASS);
                ipreferences.putBytes("ap_ssid", ap_ssid, SSID_MAX_LEN);
                ipreferences.putBytes("ap_pass", ap_pass, PASS_MAX_LEN);        
        }

        //--------------------------------------------------
        if (!ipreferences.isKey("sta_ssid")){
            ipreferences.putBytes("sta_ssid", sta_ssid, SSID_MAX_LEN);
        }
        ipreferences.getBytes("sta_ssid", sta_ssid, SSID_MAX_LEN);
        sta_ssid[SSID_MAX_LEN] = 0;

        //--------------------------------------------------
        if (!ipreferences.isKey("sta_pass")){
            ipreferences.putBytes("sta_pass", sta_pass, PASS_MAX_LEN);
        }
        ipreferences.getBytes("sta_pass", sta_pass, PASS_MAX_LEN);
        sta_pass[PASS_MAX_LEN] = 0;

        if ((stxt_check((uint8_t*)sta_ssid) > 0) && 
            (stxt_check((uint8_t*)sta_ssid) < SSID_MIN_LEN)){
                clear((uint8_t*)sta_ssid, SSID_MAX_LEN);
                clear((uint8_t*)sta_pass, PASS_MAX_LEN);
                ipreferences.putBytes("sta_ssid", sta_ssid, SSID_MAX_LEN);
                ipreferences.putBytes("sta_pass", sta_pass, PASS_MAX_LEN);
        }
        if ((stxt_check((uint8_t*)sta_pass) > 0) && 
            (stxt_check((uint8_t*)sta_pass) < PASS_MIN_LEN)){
                clear((uint8_t*)sta_ssid, SSID_MAX_LEN);
                clear((uint8_t*)sta_pass, PASS_MAX_LEN);
                ipreferences.putBytes("sta_ssid", sta_ssid, SSID_MAX_LEN);
                ipreferences.putBytes("sta_pass", sta_pass, PASS_MAX_LEN);
        }

        //--------------------------------------------------
        if (!ipreferences.isKey("ap_en")){
            ipreferences.putBytes("ap_en", &apEN, 1);
        }
        ipreferences.getBytes("ap_en", &apEN, 1);
        if (apEN > 1) {
            apEN = 1;
            ipreferences.putBytes("ap_en", &apEN, 1);
        }

        if (!ipreferences.isKey("ap_ip")){
            ipreferences.putBytes("ap_ip", apIP, SETT_IP_LEN);
        }
        ipreferences.getBytes("ap_ip", apIP, SETT_IP_LEN);
        
        if (!ipreferences.isKey("ap_gate")){
            ipreferences.putBytes("ap_gate", apGate, SETT_IP_LEN);
        }
        ipreferences.getBytes("ap_gate", apGate, SETT_IP_LEN);
        
        if (!ipreferences.isKey("ap_sub")){
            ipreferences.putBytes("ap_sub", apSub, SETT_IP_LEN);
        }
        ipreferences.getBytes("ap_sub", apSub, SETT_IP_LEN);

        //--------------------------------------------------
        if (!ipreferences.isKey("sta_en")){
            ipreferences.putBytes("sta_en", &staEN, 1);
        }
        ipreferences.getBytes("sta_en", &staEN, 1);
        if (staEN > 1) {
            staEN = 1;
            ipreferences.putBytes("sta_en", &staEN, 1);
        }
        
        if (!ipreferences.isKey("sta_dhcp")){
            ipreferences.putBytes("sta_dhcp", &staDHCP, 1);
        }
        ipreferences.getBytes("sta_dhcp", &staDHCP, 1);
        if (staDHCP > 1) {
            staDHCP = 1;
            ipreferences.putBytes("sta_dhcp", &staDHCP, 1);
        }

        if (!ipreferences.isKey("sta_ip")){
            ipreferences.putBytes("sta_ip", staIP, SETT_IP_LEN);
        }
        ipreferences.getBytes("sta_ip", staIP, SETT_IP_LEN);
        
        if (!ipreferences.isKey("sta_gate")){
            ipreferences.putBytes("sta_gate", staGate, SETT_IP_LEN);
        }
        ipreferences.getBytes("sta_gate", staGate, SETT_IP_LEN);
        
        if (!ipreferences.isKey("sta_sub")){
            ipreferences.putBytes("sta_sub", staSub, SETT_IP_LEN);
        }
        ipreferences.getBytes("sta_sub", staSub, SETT_IP_LEN);        

        //--------------------------------------------------
        ipreferences.end();
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------
bool Sett::temporary_wifi_access(const char* temp_ssid, const char* temp_pass){
    if ((stxt_check((uint8_t*)temp_ssid) >= SSID_MIN_LEN) && 
         (stxt_check((uint8_t*)temp_pass) >= PASS_MIN_LEN)){
            stxt_new((uint8_t*)sta_ssid, SSID_MAX_LEN, (char*)temp_ssid);
            stxt_new((uint8_t*)sta_pass, PASS_MAX_LEN, (char*)temp_pass);
            return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------
uint32_t Sett::stxt_check(uint8_t* src){
	uint32_t result = 0;
    while ((*src >= '#') && (*src <= 'z')){
		src++; result++;
        if (result >= 1024) break;
	}
	return result;
}

//-----------------------------------------------------------------------------------
//container must be dlen + 1 -> last field fill by 0
uint32_t Sett::stxt_new(uint8_t* dst, uint32_t dlen, char* src){
	uint32_t i = 0;
	while (dlen > 0){
		if (*src > 0){
			*dst = *src; dst++; src++;
            i++;
		} else {
			*dst = 0; dst++;
		}
        dlen--;        
	}
    *dst = 0;
	return i;
}

//-----------------------------------------------------------------------------------
//container must be dlen + 1 -> last field fill by 0
uint32_t Sett::stxt_new(uint8_t* dst, uint32_t dlen, char* src, uint32_t slen){
	uint32_t i = 0;
	while (dlen > 0){
		if (i < slen) {
			*dst = *src; dst++; src++;
            i++;
		} else {
			*dst = 0; dst++;
		}
		dlen--;
	}
    *dst = 0;
	return i;
}

//-----------------------------------------------------------------------------------
//container must be dlen + 1 -> last field fill by 0
uint32_t Sett::stxt_new(char* dst, uint32_t dlen, String src){
    uint32_t len = src.length();
	uint32_t i = 0;
    char c;
    while (dlen > 0){
        if (i < len) {
            c = src.charAt(i); i++;
            *dst = c;
        } else{
            *dst = 0;
        }
        dst++; dlen--;
	}
    *dst = 0;
	return i;
}

//-----------------------------------------------------------------------------------
bool Sett::sip_new(uint8_t* dst, String src){
    uint32_t len = src.length();
   	uint32_t si = 0;
    uint32_t value = 0;
    uint32_t val_det = 0;
    uint32_t cycle = 0;
    uint8_t temp[4];
    char c;

    while(cycle < 4){
        value = 0;
        val_det = 0;
        
        while(1){
            if (si < len){
                c = src.charAt(si); si++;
            } else {
                c = 0;
            }

            if ((c >= '0') && (c <= '9')){
                value *= 10;
                value += ((uint8_t)c - (uint8_t)'0');
                val_det++;
            } else {
                si++;
                break;
            }
        }

        if (val_det > 0){
            if (value > 255) value = 255;
            temp[cycle] = (uint8_t)value;
            //*dst = (uint8_t)value; dst++;
            cycle++;
        } else {
            break;
        }
    }
    if (cycle == 4){
        for (cycle=0; cycle<4; cycle++){
            *dst = temp[cycle]; dst++;
        }
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------------
bool Sett::sip_new(uint8_t* dst, IPAddress ip){
    uint32_t nip = ip;
    uint8_t* src = (uint8_t*)&nip; 
    for (uint32_t i=0; i<4; i++){
        *dst = *src;
        dst++; src++;
    }
    return true;
}

//-----------------------------------------------------------------------------------
void Sett::clear(uint8_t* dst, uint32_t len){
	while (len > 0){
		*dst = 0; dst++; len--;
	}
    *dst = 0;
}


