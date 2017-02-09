/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TimerED.cpp
 * Author: maxi
 * 
 * Created on 9 de febrero de 2017, 10:12
 */

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

