/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TimerED.h
 * Author: maxi
 *
 * Created on 9 de febrero de 2017, 10:12
 */

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

