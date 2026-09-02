#ifndef _PROBUFF_H_
#define _PROBUFF_H_

#include "inttypes.h"
#include "WString.h"

class ProBuff{
    private:
        uint8_t* base_;
        uint32_t len_;
        bool ring_ ;
        volatile uint32_t fill_;
        volatile uint32_t track_;

    public:
        static const int BUFF_MAX_LEN = (8*1024);
        
        ProBuff(uint32_t len, bool ring = false);
        ~ProBuff();
        uint8_t* base();
        uint8_t* base(uint32_t index);
        uint32_t len();
        bool is_ring();
        void clear(bool need_fill = false);
        
        bool add(uint8_t new_data);
        bool add(uint8_t* src);
        bool add(uint8_t* src, uint32_t len);

        uint32_t fill_get();
        uint8_t data_at(uint32_t index);
        uint8_t data_get();
        bool data_xget(uint8_t* dst);

        bool data_check();
        uint32_t track_get();
        void track_set(uint32_t index);

        bool text_add(char new_char);
        bool text_add(char* src);
        bool text_add(const char* src);
        bool text_add(char* src, uint32_t maxlen);
        bool text_add(String* ssrc);
        uint32_t text_add(uint32_t val);
        uint32_t text_add(int32_t val);
        uint32_t text_add(int32_t val, uint32_t len);
        
        bool hex_add(uint8_t val);
        bool hex_add(uint32_t val);

        uint32_t copy(ProBuff* dst, ProBuff* src = nullptr, uint32_t sindex = 0, uint32_t slen = 0);
};
#endif