/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GUI.cpp
 * Author: user
 * 
 * Created on 6 de enero de 2017, 10:50
 */

#include <allegro5/keyboard.h>
#include <allegro5/mouse.h>
#include <allegro5/timer.h>
#include "GUI.h"
#include "MouseED.h"
#include "NetworkED.h"
#include "KeyboardED.h"
#include "TimerED.h"

#define SECONDS 1.0

GUI::GUI() {
    
    this->EventQueue=al_create_event_queue();
    timer=al_create_timer(SECONDS);     //Pongo el timer a contar segundos.
    if(EventQueue != NULL)
    {
        al_register_event_source(EventQueue,al_get_mouse_event_source());
        al_register_event_source(EventQueue,al_get_keyboard_event_source());
    }
    quit = false;
    networkEventEnabled=false;
}

GUI::GUI(const GUI& orig) {
}

void GUI::attachController(Controller * controller)
{
    this->controller=controller;
}
void GUI::attachNetworkInterface(NetworkInterface * networkingInterface)
{
    this->networkingInterface=networkingInterface;
    networkEventEnabled=true;
}




bool GUI::hayEvento()
{
    ALLEGRO_EVENT rawEvent;
    bool retVal=false;
    unsigned char auxBuffer[BUFSIZE];
    unsigned int len;
    PerezProtocolHeader header;
    if(networkEventEnabled && networkingInterface->recievePacket(&header,auxBuffer,&len))
    {
        NetworkED *auxData=new NetworkED(header,auxBuffer,len);
        if(auxData->isPacketOk())
        {
            eventData= (EventData *) auxData;
            event=GUI_EVENT_NETWORKING;
            retVal=true;
        }
        else
            error=true;
    }
    else if(al_get_timer_count(timer) >= TIMEOUT_TIME)
    {
        TimerED *auxData = new TimerED(TIMEOUT);
        
        eventData=(EventData *) auxData;
        event=GUI_EVENT_TIMER;
        cout<< "TIMEOUT"<<endl;
        retVal=true;
    }
    else if(al_get_next_event(EventQueue,&rawEvent))   //necesito una eventQueue y donde la iniciliza
    {
        if(rawEvent.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        {

            MouseED *auxData= new MouseED(true,rawEvent.mouse.x,rawEvent.mouse.y);
            eventData=(EventData *) auxData;
            event=GUI_EVENT_MOUSE;
            retVal=true;
        }
        else if(rawEvent.type == ALLEGRO_EVENT_KEY_UP)
        {
            KeyboardED *auxData;
            retVal=true;
            if(rawEvent.keyboard.keycode >= ALLEGRO_KEY_A && rawEvent.keyboard.keycode <= ALLEGRO_KEY_Z)
                auxData= new KeyboardED(rawEvent.keyboard.keycode-ALLEGRO_KEY_A+'A');
            else if(rawEvent.keyboard.keycode >= ALLEGRO_KEY_0 && rawEvent.keyboard.keycode <= ALLEGRO_KEY_9)
                auxData= new KeyboardED(rawEvent.keyboard.keycode-ALLEGRO_KEY_0+'0');
            else if(rawEvent.keyboard.keycode >= ALLEGRO_KEY_PAD_0 && rawEvent.keyboard.keycode <= ALLEGRO_KEY_PAD_9)
                auxData= new KeyboardED(rawEvent.keyboard.keycode-ALLEGRO_KEY_PAD_0+'0');
            else if(rawEvent.keyboard.keycode == ALLEGRO_KEY_FULLSTOP || rawEvent.keyboard.keycode == ALLEGRO_KEY_PAD_DELETE)
                auxData= new KeyboardED('.');
            else if(rawEvent.keyboard.keycode == ALLEGRO_KEY_SPACE)
                auxData= new KeyboardED(' ');
            else if(rawEvent.keyboard.keycode == ALLEGRO_KEY_ENTER || rawEvent.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER)
                auxData= new KeyboardED(ENTER_KEY);
            else if(rawEvent.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                auxData= new KeyboardED(ESCAPE_KEY);
            else if(rawEvent.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
                auxData= new KeyboardED(BACKSPACE_KEY);
            else if(rawEvent.keyboard.keycode == ALLEGRO_KEY_DOWN)
                auxData= new KeyboardED(DOWN_KEY);
            else if(rawEvent.keyboard.keycode == ALLEGRO_KEY_UP)
                auxData= new KeyboardED(UP_KEY);
            else retVal = false;                    //si no es ninguna de las teclas que tomamos, no genera evento
            eventData=(EventData *) auxData;
            event=GUI_EVENT_KEYBOARD;
        }
    }
    
    return retVal;
}

void GUI::enableTimer() {
    
}

void GUI::playTimer() {
    al_start_timer(timer);
}

void GUI::resetTimer() {
    al_stop_timer(timer);
    al_set_timer_count(timer,0);
    al_start_timer(timer);
}
void GUI::stopTimer() {
    al_stop_timer(timer);
    al_set_timer_count(timer,0);
}


void GUI::parseEvento(){
    
    switch(event)
    {
        case GUI_EVENT_MOUSE:
            controller->parseMouseEvent(eventData);
            break;
        case GUI_EVENT_KEYBOARD:
            controller->parseKeyboardEvent(eventData);
            break;
        case GUI_EVENT_NETWORKING:
            controller->parseNetworkEvent(eventData);
            break;
        case GUI_EVENT_TIMER:
            controller->parseTimerEvent(eventData);
            break;
    }
    delete eventData;
}

GUI::~GUI() {
}

