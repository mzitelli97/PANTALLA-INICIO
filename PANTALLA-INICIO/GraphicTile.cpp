#include "GraphicTile.h"

GraphicTile::GraphicTile(ALLEGRO_BITMAP * front,ALLEGRO_BITMAP * back,CardLocation location,unsigned int width,unsigned int height)
{
    this->image = front;
    this->back = back;
    this->location=location;
    Visible = false;
    setScreenDimentions(width, height);
    setPosition(location);
}
bool GraphicTile::isVisible()
{
    return Visible;
}
void GraphicTile::setVisible(ALLEGRO_BITMAP * safeNumber)
{
    this->safeNumber=safeNumber;
    Visible=true;
}
void GraphicTile::setPosition(CardLocation location)
{
    /*Convert logic location to graphic location*/
    logic2GraphicCardLocation(location);
    
    center.x= (min.x+max.x)/2;
    center.y= (min.y+max.y)/2;
    width = max.x-min.x;
    height = max.y- min.y;
}
void GraphicTile::draw()
{
    if(Visible)
    {
        al_draw_scaled_bitmap(image,0,0,al_get_bitmap_width(image),al_get_bitmap_height(image),min.x,min.y,width,height,0);
        if(safeNumber != nullptr)
            al_draw_scaled_bitmap(safeNumber,0,0,al_get_bitmap_width(safeNumber),al_get_bitmap_height(safeNumber),min.x,min.y,width,height,0);
    }
    else
        al_draw_scaled_bitmap(back,0,0,al_get_bitmap_width(back),al_get_bitmap_height(back),min.x,min.y,width,height,0);
}
CardLocation GraphicTile::getLocation()
{
    return location;
}
GraphicTile::~GraphicTile() 
{
}

ItemInfo GraphicTile::IAm()
{
    ItemInfo retVal;
    retVal.type = TILE_CLICK;
    retVal.info = &location;
    return retVal;
}

void GraphicTile::toggleZoom()
{
    zoomed ^= true;
    setPosition(location);
}


