
#ifndef _PROHTTPSERWER_H_
#define _PROHTTPSERWER_H_

#include <WebServer.h>
//#include <AsyncTCP.h>
//#include <ESPAsyncWebServer.h>
#include "pro_txt_tools.h"
#include "pro_pages.h"
#include "pro_io.h"


#define SERVER_TASK_PRIO      5
#define SERVER_TASK_STACK     (8*1024)
#define SERVER_TASK_DELAY     25

class ProHttpServer{
    using IOFunc = std::function<void(uint32_t, uint32_t)>;
    using HFunc = std::function<void(WebServer*, Pages*)>;  // handler dostaje referencję do serwera
    
    private:
        TaskHandle_t serverTaskHandle_;
        Sett* extsett_;
        static void serverTask(void* obj);

    public:
        IOs* io_funcs;
        //ProRTC* rtc_funcs;
        WebServer* server;
        Pages* page;
        ProHttpServer(uint32_t port);
        ~ProHttpServer();
        void pro_sett(Sett* src);
        bool begin();
        void home_enable();
        void css_enable(); 
        void favicon_enable();
        void logo_enable();
        void not_found_enable();
        void inout_enable();
        void devset_enable();
        void wifi_enable();
        void rs485set_enable();
        void start();
        void add_action(const char* url, HTTPMethod met, HFunc action_handler);
        //void io_exe(IOFunc iofunc);
       // void io_exe(IOs* iofuncs);
};



#endif