#include "GraphicEDices.h"

#define SPACE_BETWEEN_DICES (FLOOR_HEIGHT/35.0)
#define DICE_WIDTH (totalWidth/30.0)
#define DICE_HEIGHT (totalHeight/15.0)

GraphicEDices::GraphicEDices(ALLEGRO_BITMAP * image)
{
    this->image = image;
}

GraphicEDices::GraphicEDices(const GraphicEDices& orig) {
}

GraphicEDices::~GraphicEDices() {
}

ItemInfo GraphicEDices::IAm()
{
    ItemInfo retVal = {NO_ITEM_CLICK, nullptr};
    return retVal;
}

void GraphicEDices::setPosition(unsigned int number)
{
    width = DICE_WIDTH;
    height = DICE_HEIGHT;
    if(zoomed)
    {
        width += DICE_WIDTH/2;
        height += DICE_HEIGHT/2;
    }
    if(width < height) width = height;
    else height = width;
    
    min.x = FLOOR_MIN_X + TOTAL_FLOORS_WIDTH;
    if(zoomed) min.x = totalWidth - width - width/10.0;
    min.y = FLOOR_MIN_Y + number*height + (number+1)*SPACE_BETWEEN_DICES;
    max.x = min.x + width;
    max.y = min.y + height;
}



