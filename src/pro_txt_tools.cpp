
#include "pro_txt_tools.h"

TxtTools::TxtTools(){}

//-----------------------------------------------------------------------------------
int32_t TxtTools::txt_len_get(char* dst){
	int32_t result = 0;
    if (dst == 0){
        return -1;
    } else {
    	while (*dst > 0){
		    dst++; result++;
            if (result >= 1024) break;
	    }
    }
	return result;
}

//-----------------------------------------------------------------------------------
int32_t TxtTools::txt_meat_get(char* dst){
	int32_t result = 0;
    while ((*dst >= '#') && (*dst <= 'z')){
		dst++; result++;
        if (result >= 1024) break;
	}
	return result;
}

//-----------------------------------------------------------------------------------
void TxtTools::txt_fill(char* dst, uint8_t data, uint32_t offset, uint32_t endset){
	dst += offset;
	while (offset < endset){
		*dst = data; dst++; offset++;
	}
}

//-----------------------------------------------------------------------------------
void TxtTools::txt_clear(char* dst, uint32_t len){
	while (len > 0){
		*dst = 0; dst++; len--;
	}
}

//-----------------------------------------------------------------------------------
int32_t TxtTools::txt_copy(char* dst, uint32_t doffset, char* src, uint32_t slen){
	int32_t i = 0;
	dst += doffset;

	while (i < slen){
		*dst = *src; dst++; src++; i++;
	}
	*dst = 0;
	return i;
}

//-----------------------------------------------------------------------------------
//return first char index of find string
int32_t TxtTools::txt_compare(char* dst, char* cmp){
	int32_t result = 1;
	while (1){
		if (*dst != *cmp) {
			result = -1; break;
		}
		if ((*dst == 0) || (*cmp == 0)) break;
		dst++; cmp++;
	}
	return result;
}

//-----------------------------------------------------------------------------------
//return first char index of find string
int32_t TxtTools::txt_compare(String dst, char* cmp){
	int32_t result = 1;
	uint32_t i = 0;
	uint32_t len = dst.length();
	char c;

	while (i < len){
		c = dst.charAt(i);
		if (c != *cmp) {
			result = -1; break;
		}
		if ((c == 0) || (*cmp == 0)) break;
		i++; cmp++;
	}
	return result;
}

//-----------------------------------------------------------------------------------
int32_t TxtTools::txt_excopy(char* dst, uint32_t dlen, char* src, uint32_t slen){
	uint32_t i = 0;
	while (i < dlen){
		if (i < slen){
			*dst = *src; dst++; src++;
		} else {
			*dst = 0; dst++;
		}
		i++;
	}
	return i;
}

//-----------------------------------------------------------------------------------
int32_t TxtTools::txt_excompare(char* dst, uint32_t dlen, char* cmp, uint32_t clen){
	int32_t result = -1;
	uint32_t i = 0;
	while ((i < dlen) || (i < clen)){
		if ((i < dlen) && (i < clen)){
			if (*dst != *cmp){
				result = -1; break;
			} else {
				result = 1;
			}
			dst++; cmp++;
		} else if ((i >= dlen) && (i < clen)){
			if (0 != *cmp){
				result = -1; break;
			}
			cmp++;
		} else if ((i < dlen) && (i >= clen)){
			if (*dst != 0){
				result = -1; break;
			}
			dst++;
		}
		i++;
	}
	return result;
}

//-----------------------------------------------------------------------------------
uint32_t TxtTools::val2txt(ProBuff* dbuff, uint32_t val){
	uint8_t buff[32];
	int16_t i = 0;
	uint32_t result = 0;
	uint32_t d = 0;

	while (i<32){
		d = (val % 10);
		buff[i] = ((uint8_t)d + (uint8_t)'0');
		i++;
		val /= 10;
		if (val == 0) break;
	}

	while (i > 0){
		i--;
		result++;
		if (!dbuff->add(buff[i])) break;
	}

	dbuff->text_add("");
	return result;
}

//-----------------------------------------------------------------------------------
uint32_t TxtTools::has_val(uint8_t* src){
	uint32_t i = 0;
	while ((*src >= '0') && (*src <= '9')){
		i++; src++;
		if (i >= 128) break;
	}
	return i;
}

//-----------------------------------------------------------------------------------
uint32_t TxtTools::has_val(String* src){
	uint32_t i = 0;
	//uint32_t len = src->length();
	while ((src->charAt(i) >= '0') && (src->charAt(i <= '9'))){
		i++;
		 if (i >= 128) break;
	}
	return i;
}

//-----------------------------------------------------------------------------------
bool TxtTools::txt2val(uint8_t* src, uint32_t* dst){
	if ((*src >= '0') && (*src <= '9')){
		uint32_t i = 0;
		uint32_t value = 0;
		
		while ((*src >= '0') && (*src <= '9')){
			value *= 10;
			value += ((*src) - (uint8_t)'0');
			i++; src++;
			if (i >= 128) break;
		}
		*dst = value;
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------
bool TxtTools::txt2val(uint8_t* src, uint8_t* dst8){
	uint32_t value = 0;
	if (txt2val(src, &value)){
		if (value > 255) value = 255;
		*dst8 = (uint8_t)value;
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------
bool TxtTools::txt2val(String src, uint32_t* dst){
	uint32_t i = 0;
	uint32_t len = src.length();
	uint32_t value = 0;
	char c;
	
	while (i < len){
		c = src.charAt(i);
		if ((c >= '0') && (c <= '9')){
			value *= 10;
			value += (c - (uint8_t)'0');
		} else {
			break;
		}
		i++;
	}
	if (i>0) {
		*dst = value;		
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------
bool TxtTools::txt2val(String src, uint8_t* dst8){
	uint32_t value = 0;
	if (txt2val(src, &value)){
		if (value > 255) value = 255;
		*dst8 = (uint8_t)value;
		return true;
	}
	return false;
}


//-----------------------------------------------------------------------------------
bool TxtTools::bytes2valtxt(ProBuff* dbuff, uint8_t* arr, uint32_t alen, char separator){
	bool result = false;
	uint32_t val;
	while (alen > 0){
		val = (uint32_t)(*arr);
		//val = 123;
		val2txt(dbuff, val);
		alen--;
		if (alen > 0){
			result = dbuff->add(separator);
			arr++;
		}
	}
	dbuff->text_add("");
	return result;
}