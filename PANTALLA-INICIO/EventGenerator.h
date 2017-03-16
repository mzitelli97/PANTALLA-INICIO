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
    virtual ~EventGenerator();
    
protected:
    
    EventData *data;
    GuiEvent event; 
    
};

#endif /* EVENTGENERATOR_H */

