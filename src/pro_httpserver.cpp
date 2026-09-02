
#include "pro_httpserver.h"
#include "pro_mess.h"
#include "pro_sett.h"
#include "pro_rsbus.h"

ProHttpServer::ProHttpServer(uint32_t port){
    server = new WebServer(80);
    page = new Pages;
}

ProHttpServer::~ProHttpServer(){
    server->close();
    server->~WebServer();
    page->~Pages();
    delete[] server;
    delete[] page;
}

//-----------------------------------------------------------------
void ProHttpServer::pro_sett(Sett* src){
    extsett_ = src;
    page->extsett = src;
    //page->pro_sett(src);
}

bool ProHttpServer::begin(){
    page->begin();

    home_enable();
    css_enable();
    favicon_enable();
    logo_enable();
    not_found_enable();

    inout_enable();
    devset_enable();
    wifi_enable();
    rs485set_enable();
    return true;
}

void ProHttpServer::home_enable(){
    server->on("/", [this]{
        page->home();
        server->send_P(200, "text/html", (const char*)page->buff(), page->len());
    });
}

void ProHttpServer::css_enable(){
    server->on("/style.css", [this]{
        uint32_t content_size;
        server->sendHeader("Cache-Control", "public, max-age=3600");
        server->send_P(200, "text/css", (const char*)page->css(&content_size), content_size);
    });
}

void ProHttpServer::favicon_enable(){
    server->on("/favicon.ico", [this]{
        uint32_t content_size;
        server->sendHeader("Cache-Control", "public, max-age=3600");
        server->send_P(200, "image/ico", (const char*)page->favicon(&content_size), content_size);
    });
}

void ProHttpServer::logo_enable(){
    server->on("/logo.jpg", [this]{
        uint32_t content_size;
        server->sendHeader("Cache-Control", "public, max-age=3600");
        server->send_P(200, "image/jpeg", (const char*)page->logo(&content_size), content_size);
    });
}

void ProHttpServer::not_found_enable(){
    server->onNotFound([this]{
        page->home();
        server->send_P(200, "text/html", (const char*)page->buff(), page->len());
    });
}

void ProHttpServer::inout_enable(){
    server->on(PAGES_INOUT_URL, [this]{
        if (server->args() > 0){
            ProMess::print(MESS_NL);
            if (server->hasArg("ioline")){
                String ioline = server->arg("ioline");
                ProMess::print(" IO line: ", ioline);
                if ((TxtTools::txt_compare(ioline, (char*)"io1ON")) > 0){
                    io_funcs->state(0,1);
                } else if ((TxtTools::txt_compare(ioline, (char*)"io1OFF")) > 0){
                    io_funcs->state(0,0);
                } else if ((TxtTools::txt_compare(ioline, (char*)"io2ON")) > 0){
                    io_funcs->state(1,1);
                } else if ((TxtTools::txt_compare(ioline, (char*)"io2OFF")) > 0){
                    io_funcs->state(1,0);
                }
            }           
        }
        uint32_t ins[4];
        //for (uint32_t i = 0; i<4; i++){
        //    ins[i] = io_funcs->adcv(i);
        //}
        for (uint32_t i = 0; i<4; i++){
            ins[i] = io_funcs->read(i);
        }

        page->inout(ins);
        server->send_P(200, "text/html", (const char*)page->buff(), page->len());
    });
}

void ProHttpServer::devset_enable(){
    server->on(PAGES_DEVSETT_URL, [this]{
        if (server->args() > 0){
            ProMess::print(MESS_NL);
            if (server->hasArg("apena")){
                TxtTools::txt2val(server->arg("apena"), &extsett_->apEN);
                ProMess::print("  HTTP WiFiEna: ", extsett_->apEN);
            }
            if (server->hasArg("apssid")){
                extsett_->stxt_new(extsett_->ap_ssid, SSID_MAX_LEN, server->arg("apssid"));
                ProMess::print("  HTTP WiFiID: ", extsett_->ap_ssid);
            }            
            if (server->hasArg("appass")){
                extsett_->stxt_new(extsett_->ap_pass, PASS_MAX_LEN, server->arg("appass"));
                ProMess::print("  HTTP WiFiPass: ", extsett_->ap_pass);
            }
            if (server->hasArg("apip")){
                extsett_->sip_new(extsett_->apIP, server->arg("apip"));
                ProMess::print_ip4("  HTTP WiFiIP: ", extsett_->apIP);
            }        
        }

        page->devset();
        server->send_P(200, "text/html", (const char*)page->buff(), page->len());
    });
}

void ProHttpServer::wifi_enable(){
    server->on(PAGES_WIFI_URL, [this]{
        if (server->args() > 0){
            ProMess::print(MESS_NL);
            if (server->hasArg("staena")){
                TxtTools::txt2val(server->arg("staena"), &extsett_->staEN);
                ProMess::print("  HTTP WiFiEna: ", extsett_->staEN);
            }
            if (server->hasArg("stassid")){
                extsett_->stxt_new(extsett_->sta_ssid, SSID_MAX_LEN, server->arg("stassid"));
                ProMess::print("  HTTP WiFiID: ", extsett_->sta_ssid);
            }                
            if (server->hasArg("stapass")){
                extsett_->stxt_new(extsett_->sta_pass, PASS_MAX_LEN, server->arg("stapass"));
                ProMess::print("  HTTP WiFiPass: ", extsett_->sta_pass);
            }      
            if (server->hasArg("stadhcp")){
                TxtTools::txt2val(server->arg("stadhcp"), &extsett_->staDHCP);
                ProMess::print("  HTTP WiFiDHCP: ", extsett_->staDHCP);
            }
            
            if (server->hasArg("statip")){
                extsett_->sip_new(extsett_->staIP, server->arg("statip"));
                ProMess::print_ip4("  HTTP WiFiIP: ", extsett_->staIP);
            } 
            if (server->hasArg("gateip")){
                extsett_->sip_new(extsett_->staGate, server->arg("gateip"));
                ProMess::print_ip4("  HTTP WiFiGate: ", extsett_->staGate);
            }     
            if (server->hasArg("subip")){
                extsett_->sip_new(extsett_->staSub, server->arg("subip"));
                ProMess::print_ip4("  HTTP WiFiSubnet: ", extsett_->staSub);
            }                          
        }

        page->wifi();
        server->send_P(200, "text/html", (const char*)page->buff(), page->len());
    });
}

void ProHttpServer::rs485set_enable(){
    server->on(PAGES_COMSETT_URL, [this]{
        if (server->args() > 0){
            ProMess::print(MESS_NL);
            if (server->hasArg("rs485-1")){
                String ioline = server->arg("ioline");
                ProMess::print("RS485-1 test");
                RsBus::print(0, MESS_NL "RS485-1 WWW test");
            }
            if (server->hasArg("rs485-2")){
                String ioline = server->arg("ioline");
                ProMess::print("RS485-2 test");
                RsBus::print(0, MESS_NL "RS485-2 WWW test");
            }               
        }
        page->rs485set();
        server->send_P(200, "text/html", (const char*)page->buff(), page->len());
    });
}


void ProHttpServer::start(){
    server->begin();
    xTaskCreate(serverTask, "ServerTask", SERVER_TASK_STACK, (void*)this, SERVER_TASK_PRIO, &serverTaskHandle_);
}

void ProHttpServer::serverTask(void* obj){
    ProHttpServer* ps = (ProHttpServer*)obj;
    while(1){
        ps->server->handleClient();
        vTaskDelay(SERVER_TASK_DELAY / portTICK_PERIOD_MS);
    }
}

void ProHttpServer::add_action(const char* url, HTTPMethod met, HFunc action_handler){
    server->on(url, met, [this, action_handler]{
        page->clear();
        action_handler(server, page);
        server->send_P(200, "text/html", (const char*)page->buff(), page->len());
    });
}    

//void ProHttpServer::io_exe(IOs* iofuncs){
//    io_funcs = iofuncs;
//}




