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

