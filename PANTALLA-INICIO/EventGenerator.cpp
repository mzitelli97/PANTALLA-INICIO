
#include "EventGenerator.h"

EventGenerator::EventGenerator() {
    event=GUI_EVENT_NOEVENT;
    data=nullptr;
}

EventData* 
EventGenerator::getData() {

    return this->data;
}

GuiEvent 
EventGenerator::getType() {
    return this->event;
}

void EventGenerator::deleteEventData() {

    delete data;
    
}

EventGenerator::~EventGenerator() {
}

