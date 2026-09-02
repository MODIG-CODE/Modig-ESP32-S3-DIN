
#ifndef _PROTXTTOOLS_H_
#define _PROTXTTOOLS_H_

#include "inttypes.h"
#include "Arduino.h"
#include "pro_buff.h"

class TxtTools{
    public:
        TxtTools();
        static int32_t txt_len_get(char *dst);
        static  int32_t txt_meat_get(char *dst);
        static void txt_fill(char *dst, uint8_t data, uint32_t offset, uint32_t endset);
        static void txt_clear(char *dst, uint32_t len);
        static int32_t txt_copy(char *dst, uint32_t doffset, char *src, uint32_t slen);
        static int32_t txt_compare(char *dst, char *cmp);
        static int32_t txt_compare(String dst, char* cmp);
        static int32_t txt_excopy(char *dst, uint32_t dlen, char *src, uint32_t slen);
        static int32_t txt_excompare(char *dst, uint32_t dlen, char *cmp, uint32_t clen);

        static uint32_t val2txt(ProBuff* dbuff, uint32_t val);
        static uint32_t has_val(uint8_t* src);
        static uint32_t has_val(String* src);
        static bool txt2val(uint8_t* src, uint32_t* dst);
        static bool txt2val(uint8_t* src, uint8_t* dst8);
        static bool txt2val(String src, uint32_t* dst);
        static bool txt2val(String src, uint8_t* dst);

        static bool bytes2valtxt(ProBuff* dbuff, uint8_t* arr, uint32_t alen, char separator);
};


#endif