#include "GraphicToken.h"

GraphicToken::GraphicToken() {
}

GraphicToken::GraphicToken(ALLEGRO_BITMAP* image)
{
    this->image = image;
}

GraphicToken::GraphicToken(const GraphicToken& orig) {
}

GraphicToken::~GraphicToken() {
}

ItemInfo GraphicToken::IAm() 
{
    return {NO_ITEM_CLICK, nullptr};
}

void GraphicToken::setPosition(CardLocation location, unsigned int number)
{
    /*Convert logic location to graphic location*/
    logic2GraphicCardLocation(location);
    
    min.x = min.x + width/1.4;
    min.y = min.y + height/7.5 * (number%6);
    max.y = max.y - height/1.4 + height/7.5 * (number%6);
    
    width = max.x-min.x;
    height = max.y- min.y;
    if(number>=6)
    {
        min.x = min.x - width;
        max.x = max.x - width;
    }
    center.x= (min.x+max.x)/2;
    center.y= (min.y+max.y)/2;
}
