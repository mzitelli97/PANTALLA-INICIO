#ifndef TIMERED_H
#define TIMERED_H


#include "EventData.h"
typedef enum {TIMEOUT} TimerEventType;

class TimerED: public EventData {
public:
    TimerED(TimerEventType type);
    TimerED(const TimerED& orig);
    TimerEventType getType();
    virtual ~TimerED();
private:
    TimerEventType type;
};

#endif /* TIMERED_H */

