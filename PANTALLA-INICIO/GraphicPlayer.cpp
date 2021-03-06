#include <allegro5/bitmap.h>
#include "GraphicPlayer.h"

GraphicPlayer::GraphicPlayer() 
{
}

GraphicPlayer::GraphicPlayer(ALLEGRO_BITMAP * image, CardLocation location,unsigned int width, unsigned int height)
{
    this->image = image;
    setScreenDimentions(width,height);
    setLocation(location);
    isOnBoard = false;
}

GraphicPlayer::GraphicPlayer(const GraphicPlayer& orig) {
}
void GraphicPlayer::setLocation(CardLocation location)
{
    if(location.floor < BOARD_STANDARD_FLOORS && location.row < FLOOR_RAWS && location.column < FLOOR_COLUMNS)
    {
        if(logic2GraphicCardLocation(location))
        {
            max.x = max.x - width/2;
            min.y = min.y + height/4.5;


            center.x= (min.x+max.x)/2;
            center.y= (min.y+max.y)/2;
            width = max.x-min.x;
            height = max.y- min.y;
        }
    }
}

GraphicPlayer::~GraphicPlayer() {
}

ItemInfo GraphicPlayer::IAm()
{
    return {NO_ITEM_CLICK, nullptr};
}

void GraphicPlayer::setOnBoard(bool isOnBoard)
{
    this->isOnBoard = isOnBoard;
}

void GraphicPlayer::draw()
{
    if(isOnBoard)
        if(image != nullptr)
            al_draw_scaled_bitmap(image,0,0,al_get_bitmap_width(image),al_get_bitmap_height(image),min.x,min.y,width,height,0);
}



