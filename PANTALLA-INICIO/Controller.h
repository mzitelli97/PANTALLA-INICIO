#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <string>
#include <vector>
#include "EventData.h"

using namespace std;

class Controller {
public:
    virtual void parseMouseEvent(EventData *mouseEvent)=0;
    virtual void parseNetworkEvent(EventData *mouseEvent)=0;
    virtual void parseKeyboardEvent(EventData *mouseEvent)=0;
    virtual void parseTimerEvent(EventData *mouseEvent)=0;
};

#endif /* CONTROLLER_H */

