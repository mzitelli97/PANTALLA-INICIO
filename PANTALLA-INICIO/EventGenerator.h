/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   EventGenerator.h
 * Author: user
 *
 * Created on 20 de febrero de 2017, 02:15
 */

#ifndef EVENTGENERATOR_H
#define EVENTGENERATOR_H

#include "EventData.h"

typedef enum{GUI_EVENT_MOUSE, GUI_EVENT_KEYBOARD,GUI_EVENT_NETWORKING,GUI_EVENT_TIMER,GUI_EVENT_NOEVENT} GuiEvent;

class EventGenerator {
public:
    
    EventGenerator();
    virtual bool hayEvento()=0;
    GuiEvent getType();
    EventData * getData();
    void deleteEventData();
    virtual ~EventGenerator();
    
protected:
    
    EventData *data;
    GuiEvent event;

    
    
    
};

#endif /* EVENTGENERATOR_H */

