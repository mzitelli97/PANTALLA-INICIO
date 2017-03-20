#include "TimerED.h"

TimerED::TimerED(TimerEventType type) {
    this->type=type;
}

TimerEventType TimerED::getType()
{
    return type;
}

TimerED::TimerED(const TimerED& orig) {
}

TimerED::~TimerED() {
}

