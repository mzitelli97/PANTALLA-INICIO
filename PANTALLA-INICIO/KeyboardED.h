/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   KeyboardED.h
 * Author: maxi
 *
 * Created on 7 de febrero de 2017, 01:45
 */

#ifndef KEYBOARDED_H
#define KEYBOARDED_H

#include "EventData.h"


class KeyboardED : public EventData{
public:
    KeyboardED(char key);
    KeyboardED(const KeyboardED& orig);
    char getKey();
    virtual ~KeyboardED();
private:
    char key;

};

#endif /* KEYBOARDED_H */

