/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   KeyboardED.cpp
 * Author: maxi
 * 
 * Created on 7 de febrero de 2017, 01:45
 */

#include "KeyboardED.h"

KeyboardED::KeyboardED(char key)
{
    this->key = key;
    this->isSpecialKey = false;
    this->specialKey = NO_SPECIAL;
}

KeyboardED::KeyboardED(SpecialKey specialKey)
{
    this->key = '\0';
    this->isSpecialKey = true;
    this->specialKey = specialKey;
}
char KeyboardED::getKey()
{
    return key;
}

SpecialKey KeyboardED::getSpecialKey()
{
    return specialKey;
}

bool KeyboardED::isASpecialKey()
{
    return isSpecialKey;
}

KeyboardED::KeyboardED(const KeyboardED& orig) {
}

KeyboardED::~KeyboardED() {
}

