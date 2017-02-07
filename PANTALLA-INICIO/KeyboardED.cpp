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
}

char KeyboardED::getKey()
{
    return key;
}

KeyboardED::KeyboardED(const KeyboardED& orig) {
}

KeyboardED::~KeyboardED() {
}

