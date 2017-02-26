#include "GUI.h"
#include "NetworkED.h"
#include "TimerED.h"
#include "EventGenerator.h"


GUI::GUI() {
}

GUI::GUI(const GUI& orig) {
}

void
GUI::attachController(Controller * controller)
{
    this->controller=controller;
}

void 
GUI::attachEventGenerator(EventGenerator* event)
{
    this->evento.push_back(event);
}

bool 
GUI::hayEvento()
{
    bool retVal=false;
    list<EventGenerator*>::iterator it_evento;

    for(it_evento=evento.begin(); it_evento != evento.end() && !retVal;it_evento++) //No guardo el iterador xq cuando sale de scope desaloca la memoria
    {
            retVal=(*it_evento)->hayEvento();
            eventType=(*it_evento)->getType();
            data=(*it_evento)->getData();
    }
    return retVal;
}

void GUI::parseEvento(){
    
    switch(eventType)
    {
        case GUI_EVENT_MOUSE:
            controller->parseMouseEvent(data);
            break;
        case GUI_EVENT_KEYBOARD:
            controller->parseKeyboardEvent(data);
            break;
        case GUI_EVENT_NETWORKING:
            controller->parseNetworkEvent(data);
            break;
        case GUI_EVENT_TIMER:
            controller->parseTimerEvent(data);
            break;
    }
    //elimino el event data
    delete data;
}

GUI::~GUI() {
}

