#include "MouseED.h"

MouseED:: MouseED(bool click,unsigned int x,unsigned int y) {
    this->click= click;
    this->x=x;
    this->y=y;
}
MouseED::MouseED(int z)
{
    this->click= false;
    this->z=z;
}


MouseED::MouseED(const MouseED& orig) {
}

MouseED::~MouseED() {
}

unsigned int MouseED::getX()
{
    return x;
}

unsigned int MouseED::getY()
{
    return y;
}

int MouseED::getZ() 
{
    return z;
}

bool MouseED::isClicked()
{
    return click;
}


