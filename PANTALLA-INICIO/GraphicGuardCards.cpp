#include "GraphicGuardCards.h"

#define CARD_WIDTH (totalWidth/11.0)
#define CARD_HEIGHT (totalHeight/7.0)
#define SEPARATION (FLOOR_HEIGHT-2*CARD_WIDTH)
#define RADIX (totalWidth/250.0)
#define RED al_map_rgb(200,10,10)

GraphicGuardCards::GraphicGuardCards(ALLEGRO_BITMAP * image, unsigned int floor, bool isShownDeck)
{
    this->image = image;
    this->properties.floor = floor;
    topOfNonVisibleDeckShown=false;
    this->properties.shownDeck = isShownDeck;
    isVisible = !isShownDeck;        //si es el mazo de las cartas que no salieron, lo inicializo visible
}

GraphicGuardCards::GraphicGuardCards(const GraphicGuardCards& orig) {
}

GraphicGuardCards::~GraphicGuardCards() {
}

ItemInfo GraphicGuardCards::IAm()
{
    ItemInfo retVal = {NO_ITEM_CLICK,nullptr};      //si no estoy visible no dejo que me vean
    if(isVisible)
    {
        retVal.type = GUARD_CARDS_CLICK;
        retVal.info = &properties;
    }
    return retVal;
}

void GraphicGuardCards::push_top_card(ALLEGRO_BITMAP* card)
{
    cards.push_front(card);
}

void GraphicGuardCards::clearShownCards()
{
    cards.clear();
}
void GraphicGuardCards::setTopOfNonVisibleDeck(bool visible, ALLEGRO_BITMAP *target)
{
    topOfNonVisibleDeckShown=visible;
    topOfNonVisibleDeckTarget=target;
}

void GraphicGuardCards::setVisible(bool isVisible)
{
    this->isVisible = isVisible;
}

void GraphicGuardCards::draw()
{
    if(isVisible)       //solo la dibujo si esta visible
    {
        setPosition();
        if(!zoomed)
        {
            if(properties.shownDeck)
            {
                if(!cards.empty())
                    al_draw_scaled_bitmap(cards.front(),0,0,al_get_bitmap_width(cards.front()),al_get_bitmap_height(cards.front()),
                            min.x, min.y, width, height, 0);
            }
            else
            {
                if(image != nullptr)
                    al_draw_scaled_bitmap(image,0,0,al_get_bitmap_width(image),al_get_bitmap_width(image),
                            min.x, min.y, width, height, 0);
            }

            if(topOfNonVisibleDeckShown && topOfNonVisibleDeckTarget !=NULL)
                al_draw_scaled_bitmap(topOfNonVisibleDeckTarget,0,0,al_get_bitmap_width(topOfNonVisibleDeckTarget),al_get_bitmap_width(topOfNonVisibleDeckTarget),
                        min.x - (SEPARATION + CARD_WIDTH) / 2.0, min.y + CARD_HEIGHT / 2.0, width, height, 0);
        }
        else
            drawOnZoom();
    }
}

void GraphicGuardCards::setPosition()
{
    if(!zoomed)
    {
        min.x = properties.floor * FLOOR_WIDTH + (properties.floor+1) * SPACE_BETWEEN_FLOORS + CARD_WIDTH + SEPARATION;
        min.y = FLOOR_MIN_Y + FLOOR_HEIGHT;
        max.y = min.y + CARD_HEIGHT;
        if(!properties.shownDeck)
            min.x = min.x - SEPARATION - CARD_WIDTH;
        max.x = min.x + CARD_WIDTH;
        width = max.x - min.x;
        height = max.y - min.y;
    }
    else
    {
        width = CARD_WIDTH * 2;
        height = CARD_HEIGHT * 2;
        min.x = width/4;
        min.y = height/4;
        max.x = min.x + width;
        max.y = min.y + height;
    }
}

void GraphicGuardCards::drawOnZoom()
{
    int i = 0;
    for(auto& item : cards)
    {
        al_draw_scaled_bitmap(item,0,0,al_get_bitmap_width(item),al_get_bitmap_height(item),
                min.x +i%4 * width, min.y + (int)i/4 * height, width, height, 0);
        i++;
    }
    al_draw_rounded_rectangle(min.x,min.y,max.x,max.y,RADIX,RADIX,RED,RADIX);
}


