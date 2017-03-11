#include "GUI.h"


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

    for(auto& ev : evento)
    {
        retVal=ev->hayEvento();
        eventType=ev->getType();
        data=ev->getData();
        if(retVal == true) break;
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

