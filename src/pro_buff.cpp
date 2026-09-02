 
#include "pro_buff.h"
 
ProBuff::ProBuff(uint32_t len, bool ring){
    if (len < 16) len = 16;
    if (len > BUFF_MAX_LEN) len = BUFF_MAX_LEN;
    len_ = len;
    ring_ = ring;
    base_ = new uint8_t[len_];
}

ProBuff::~ProBuff(){
    delete[] base_;
}

//-------------------------------------------------------------
uint8_t* ProBuff::base() {return base_;}
uint8_t* ProBuff::base(uint32_t index){return &base_[index];}
uint32_t ProBuff::len() {return len_;}
bool ProBuff::is_ring() {return ring_;}

void ProBuff::clear(bool need_fill){
    *base_ = 0;
    fill_ = 0;
    track_ = 0;
    if (need_fill){
        uint8_t* dst = base_;
        for (uint32_t i=0; i<len_; i++){
            *dst = 0; dst++;
        }
    }
}

//-------------------------------------------------------------
//data append with not overwrite on track pointer
bool ProBuff::add(uint8_t new_data){
	//if (base_ > 0){
        if (ring_){
            uint32_t f = fill_;
            fill_++;
            if (fill_ == len_) fill_ = 0;
            if (fill_ != track_){
                base_[f] = new_data;
                return true;
            } else {
                fill_ = f;
            }  
        } else {
            if (fill_ < len_){
                base_[fill_] = new_data;
                fill_++;
                return true;
            }
        }
	//}
	return false;
}

///data append and add "0" at the end. Not overwrite on track pointer
bool ProBuff::add(uint8_t* src){
	bool result;
	while (*src > 0){
		if (!add((uint8_t)(*src))) break;
		src++;
	}
	base_[fill_] = 0;
	return result;
}

bool ProBuff::add(uint8_t* src, uint32_t len){
	bool result;
	while (len > 0){
		result = add((uint8_t)(*src));
		if (!result) break;
		src++; len--;
	}
	base_[fill_] = 0;
	return result;
}

//-------------------------------------------------------------
uint32_t ProBuff::fill_get(){
    if (fill_ >= track_){
        return (fill_-track_);
    } else {
        return ((len_ - fill_) + track_);
    }
}

uint8_t ProBuff::data_at(uint32_t index){
	if (index < len_) return base_[index];
	else return 0;
}

uint8_t ProBuff::data_get(){
	uint8_t data;
	if (track_ != fill_){
		data = data_at(track_);
		track_++;
        if (ring_){
            if (track_ == len_) track_ = 0;
        }
		return data;
	}
	return 0;
}

bool ProBuff::data_xget(uint8_t* dst){
    if (track_ != fill_){
        *dst = base_[track_];
        track_++;
        if (ring_){
            if (track_ == len_) track_ = 0;
        }
        return true;
    }
    return false;
}

//-------------------------------------------------------------
bool ProBuff::data_check(){
	if (track_ != fill_) return true;
	return false;
}

uint32_t ProBuff::track_get(){return track_;}
void ProBuff::track_set(uint32_t index){track_ = index;}


//-------------------------------------------------------------
bool ProBuff::text_add(char new_char){
	return add((uint8_t)new_char);
}

bool ProBuff::text_add(char* src){
	return add((uint8_t*)src);
}

bool ProBuff::text_add(const char* src){
	return add((uint8_t*)src);
}

bool ProBuff::text_add(char* src, uint32_t maxlen){
    return add((uint8_t*)(src), maxlen);
}

bool ProBuff::text_add(String* ssrc){
    uint32_t len = ssrc->length();
    uint32_t i = 0;
    while(i<len){
        add(ssrc->charAt(i)); i++;
    }
	return add((uint8_t)0);
}

//-----------------------------------------------------------------------------------
uint32_t ProBuff::text_add(uint32_t val){
	uint8_t buff[32];
	int16_t i = 0;
	uint32_t result = 0;

	for (i = 0; i < 30; i++){
		buff[i] = (val % 10) + '0';
		val /= 10;
		result++;
		if (val == 0) break;
	}

	for (; i >= 0; i--){
		add(buff[i]);
	}
	base_[fill_] = 0;
	return result;
}

//-----------------------------------------------------------------------------------
uint32_t ProBuff::text_add(int32_t val){
	uint32_t result = 0;
	if (val < 0){
		val *= -1;
		add('-');
		result++;
	}
	result += text_add((uint32_t) val);
	return result;
}

//-----------------------------------------------------------------------------------
uint32_t ProBuff::text_add(int32_t val, uint32_t len){
	uint8_t buff[32];
	int16_t i = 0;
	uint32_t result = 0;

	if (val < 0){
		val *= -1;
		add('-');
		result++;
	}

	for (; i < 30; i++){
		buff[i] = (val % 10) + '0';
		val /= 10;
		result++;
		if (val == 0) break;
	}

	while (result < len){
		i++;
		buff[i] = ' ';
		result++;
	}
	if (result > len){
		i = (len - 1);
	}

	for (; i >= 0; i--){
		add(buff[i]);
	}
	base_[fill_] = 0;
	return result;
}

//-----------------------------------------------------------------------------------
bool ProBuff::hex_add(uint8_t val){
	uint8_t half;
	bool result;

	half = (val & 0xF0);
	half >>= 4;
	if (half <= 9) {
		result = add((uint8_t)(half + '0'));
	} else {
		result = add((uint8_t)(half - 10 + 'A'));
	}

	half = (val & 0x0F);
	if (half <= 9) {
		result = add((uint8_t)(half + '0'));
	} else {
		result = add((uint8_t)(half - 10 + 'A'));
	}
	return result;
}

//-----------------------------------------------------------------------------------
bool ProBuff::hex_add(uint32_t val){
	add('x');
	hex_add((uint8_t)((val>>24) & 0xFF));
	hex_add((uint8_t)((val>>16) & 0xFF));
	hex_add((uint8_t)((val>>8) & 0xFF));
	return hex_add((uint8_t)(val & 0xFF));
}

//-----------------------------------------------------------------------------------
uint32_t ProBuff::copy(ProBuff* dst, ProBuff* src, uint32_t sindex, uint32_t slen) {
    uint32_t i = 0;
	if (src == nullptr) src = this;
    if (sindex < src->len()){
        uint8_t* start_pt = src->base(sindex);
        src->track_set(sindex);
        if (slen == 0) slen = src->fill_get();
        
        uint8_t data;
        while (slen > 0){
            if (src->data_xget(&data)){
                dst->add(data);
                slen--;
                sindex++;
                i++;
            } else break;
        }
    }
    return i;
}