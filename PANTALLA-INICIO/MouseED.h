#ifndef MOUSEED_H
#define MOUSEED_H

#include "EventData.h"

class MouseED : public EventData{
public:
    
    MouseED(bool click,unsigned int x,unsigned int y);
    MouseED(int z);
    MouseED(const MouseED& orig);
    unsigned int getX();
    unsigned int getY();
    int getZ();
    bool isClicked();
    virtual ~MouseED();
    
private:
    unsigned int x;
    unsigned int y;
    int z;
    bool click;
};

#endif /* MOUSEED_H */

