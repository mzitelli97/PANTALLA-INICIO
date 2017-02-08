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
#include "GUI.h"
#include "MouseED.h"
#include "NetworkED.h"
#include "KeyboardED.h"

GUI::GUI() {
    
    this->EventQueue=al_create_event_queue();
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

void GUI::attachController(Controller * Controller)
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
            if(rawEvent.keyboard.keycode >= ALLEGRO_KEY_A && rawEvent.keyboard.keycode <= ALLEGRO_KEY_Z)
                auxData= new KeyboardED(rawEvent.keyboard.keycode-ALLEGRO_KEY_A+'A');
            else if(rawEvent.keyboard.keycode >= ALLEGRO_KEY_0 && rawEvent.keyboard.keycode <= ALLEGRO_KEY_9)
                auxData= new KeyboardED(rawEvent.keyboard.keycode-ALLEGRO_KEY_A+'0');
            else if(rawEvent.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                auxData= new KeyboardED(ESCAPE_KEY);
            else if(rawEvent.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
                auxData= new KeyboardED(BACKSPACE_KEY);
            else if(rawEvent.keyboard.keycode == ALLEGRO_KEY_DOWN)
                auxData= new KeyboardED(DOWN_KEY);
            else if(rawEvent.keyboard.keycode == ALLEGRO_KEY_UP)
                auxData= new KeyboardED(UP_KEY);
            eventData=(EventData *) auxData;
            event=GUI_EVENT_KEYBOARD;
            retVal=true;
        }
    }
    return retVal;
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
    }
    
}

GUI::~GUI() {
}

