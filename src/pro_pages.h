
#ifndef _PROPAGES_H_
#define _PROPAGES_H_

#include "pro_buff.h"
#include "pro_sett.h"
#include <HTTPClient.h>

class Pages{
	private:
		ProBuff* pagebuff_;
    	ProBuff* tempbuff_;

	public:
		Sett* extsett;

		const uint32_t PAGE_BUFF_LEN = (4*1024);
		const uint32_t PAGE_TBUFF_LEN = (32);
		uint32_t css_len;
		Pages();
		~Pages();

		bool begin();
		void pro_sett(Sett* src);

		bool add(uint8_t* src);
		bool add(const uint8_t* src);
		bool add(uint8_t* src, uint32_t len);
		bool add(char* src);
		bool add(uint32_t val);
		bool add(const char* src);
		bool add(const char* src, uint32_t maxlen);

		void clear();
		uint8_t* buff();
		uint32_t len();

		char* ip4(uint8_t* src);

		void label(const char* text);
		void label(char* text);
		void hiden_input(const char* name, const char* val);
		void link_button(const char* url, const char* text);
		void onclick_button(const char* func, const char* text);
		void submit_button(const char* text);
		void hyperlink(const char* url, const char* text);
		void imglink(const char* url, const char* img_url);
		void form_open(const char* url, const char* method);
		void form_close();
		void input_label(const char* id, const char* label, char* value);
		void input_label_list(const char* id, const char* label, const char* list, char* value);
		void ena_dis(const char* id, const char* label, uint8_t val);

		const char* css(uint32_t* len);
		const uint8_t* favicon(uint32_t* len);
		const uint8_t* logo(uint32_t* len);

		void head();
		void home();
		void inout(uint32_t* inputs);
		void devset();
		void wifi();
		void rs485set();
		void info();
		void addIoScript();
};

extern const uint8_t favicon_ico[];
extern uint32_t favicon_ico_size;
extern const uint8_t logo_png320[];
extern uint32_t logo_png320_size;


#define HTML_VALUE_MAX_LEN		24
#define HTML_INPUT_MAX_LEN		(const char*)"24"
#define HTML_GET				(const char*)"GET"
#define HTML_POST				(const char*)"POST"

#define HTML_HOMEPAGE_URL		(const char*)"https://www.modig.pl/produkt/esp32-s3-din/"
#define HTML_GITHUB_URL			(const char*)"https://github.com/MODIG/ESP32-S3-DIN-test.git"
#define HTML_MODEL				(const char*)"MODIG ESP32-S3 test v1.0"
#define HTML_VERSION			(const char*)" v1.0"
#define HTML_DEV_NAME			(const char*)"MD43001 - ESP32-S3 DIN"

#define	PAGES_MAIN_URL			(const char*)"/"
#define	PAGES_AUTH_URL			(const char*)"/auth"
#define	PAGES_INOUT_URL			(const char*)"/inout"
#define	PAGES_DEVSETT_URL		(const char*)"/devsett"
#define	PAGES_WIFI_URL			(const char*)"/wifisett"
#define	PAGES_COMSETT_URL		(const char*)"/comsett"
#define	PAGES_LOGOUT_URL		(const char*)"/logout"

#define HTML_BODY_OPEN    			(const char*)"<body>\r\n"
#define HTML_HB_CLOSE      			(const char*)"</body></HTML>\r\n\r\n"

#define HTML_BODY_TOPDIV			(const char*)"<div class=\"topnav\">\r\n"
#define HTML_BODY_CONTDIV			(const char*)"<div class=\"content\">\r\n"
#define HTML_BODY_GRIDDIV			(const char*)"<div class=\"cardgrid\">\r\n"
#define HTML_BODY_CARDDIV			(const char*)"<div class=\"card\">\r\n"

#define HTML_DIV_OPEN				(const char*)"<div>\r\n"
#define HTML_DIV_CLOSE				(const char*)"</div>\r\n"

#define HTML_P_OPEN					(const char*)"<p>\r\n"
#define HTML_P_CLOSE				(const char*)"</p>\r\n"

#define HTML_BR						(const char*)"<br>"
#define HTML_HLINE					(const char*)"<hr>"

#define HTML_DO_RECON				(const char*)"<br>(Reconnection will be required)"


#define CSS_STYLE_SHEET (const char*)\
	"html {font-family: system-ui; text-align: center;}\r\n"\
	"body {background-color: #D0D0D0; margin: auto;}\r\n"\
	".topnav {background-color: #101010; overflow: hidden; padding: 1rem;}\r\n"\
	/*".content {padding: 1rem;}\r\n"\*/\
	".cardgrid {max-width: 600px; margin: auto;}\r\n"\
	".card {background-color: white;"\
		" box-shadow: 2px 2px 20px #ED3229;"\
		" border-radius: 8px;"\
		" padding: 1rem;"\
	    " margin: 2rem;}\r\n"\
	"hr {height:3px; border-width:0; background-image:linear-gradient(to right, #FFFFFF, #4CC3B0, #FFFFFF)}\r\n"\
	"input, label, button, select {"\
		" font-size: 1rem;"\
		" font-weight: bold;"\
		" padding: 1rem;"\
	    " margin: 1rem;}\r\n"\
	"input[type=text], input[type=password], select {"\
		" width: 50%;"\
		" border: 2px solid #ED3229;"\
		" border-radius: 10px;"\
		" text-align: center;"\
		" font-weight: normal;"\
		" box-sizing: border-box;}\r\n"\
	"input[type=submit], button {"\
		" width: 35%;"\
	  	" border: none;"\
		" color: white;"\
		" background-color: #ED3229;"\
		" text-decoration: none;"\
		" border-radius: 10px;"\
		" opacity: 0;"\
		" animation: showB 2s ease 0s forwards;}\r\n"\
	"button {"\
		" background-color: #F7B80B;"\
		" opacity: 1;}\r\n"\
	"@keyframes showB{to{opacity: 1;}}\r\n"

#define HTML_HEAD (const char*)\
		"<!DOCTYPE html>\r\n"\
		"<HTML><head>\r\n"\
		"<meta charset=\"UTF-8\">\r\n"\
		"<title></title>\r\n"\
		"<link rel=\"icon\" type=\"image/x-icon\" href=\"favicon.ico\">\r\n"\
		"<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">\r\n"\
		"</head>\r\n"

#endif
