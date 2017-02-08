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

typedef enum {UP_KEY, DOWN_KEY, BACKSPACE_KEY, ESCAPE_KEY, NO_SPECIAL}SpecialKey;

class KeyboardED : public EventData{
public:
    KeyboardED(char key);
    KeyboardED(SpecialKey specialKey);
    KeyboardED(const KeyboardED& orig);
    char getKey();
    SpecialKey getSpecialKey();
    bool isASpecialKey();
    virtual ~KeyboardED();
private:
    char key;
    bool isSpecialKey;
    SpecialKey specialKey;

};

#endif /* KEYBOARDED_H */

