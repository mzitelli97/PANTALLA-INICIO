#ifndef KEYBOARDED_H
#define KEYBOARDED_H

#include "EventData.h"

typedef enum {ENTER_KEY, UP_KEY, DOWN_KEY, BACKSPACE_KEY, ESCAPE_KEY, NO_SPECIAL}SpecialKey;

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

