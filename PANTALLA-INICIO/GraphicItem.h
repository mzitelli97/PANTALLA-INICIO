/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GraphicItem.h
 * Author: carlos
 *
 * Created on 29 de diciembre de 2016, 18:37
 */

#ifndef GRAPHICITEM_H
#define GRAPHICITEM_H

//#include "ImageLoader.h"        //check if this is necessary(this include the model)
#include "BurgleBrosModel.h"
#include "LibsInit.h"

typedef enum {  TILE_CLICK = 0, LOOT_CARDS_CLICK, CHAR_CARD_CLICK, GUARD_CARDS_CLICK, ZOOM_CLICK,TEXT_BOX, MENU_ITEM_CLICK, VOL_BUTTON_CLICK, HELP_BUTTON_CLICK, CONNECT_BUTTON_CLICK, PASS_BUTTON_CLICK, EXIT_BUTTON_CLICK, NO_ITEM_CLICK} clickItem;
typedef enum{ ZOOM_BUTTON, /*FULLSCREEN_BUTTON,*/CONNECT_BUTTON, CONNECTING_BUTTON, MUTE_BUTTON, UNMUTE_BUTTON, PASS_BUTTON, HELP_BUTTON, QUIT_BUTTON} buttonAction;

typedef struct
{
    double x;
    double y;
}Point;

typedef struct
{
    clickItem type;
    void * info;
}ItemInfo;

#define FLOOR_WIDTH (totalWidth/4.5)
#define FLOOR_HEIGHT (totalHeight/2.25)
#define SPACE_BETWEEN_FLOORS (totalWidth/20.0)
#define TOTAL_FLOORS_WIDTH (3.0 * FLOOR_WIDTH + 3.0 * SPACE_BETWEEN_FLOORS)         //no configurable
#define FLOOR_MIN_X (totalWidth/20.0)
#define FLOOR_MIN_Y (totalHeight/10.0)
#define TILES_HEIGHT (FLOOR_HEIGHT/(float)FLOOR_RAWS)
#define TILES_WIDTH  (FLOOR_WIDTH/(float)FLOOR_COLUMNS)

class GraphicItem {
public:
    GraphicItem();
    GraphicItem(const GraphicItem& orig);
    virtual ~GraphicItem();
    void setScreenDimentions(unsigned int totalWidth, unsigned int totalHeight);
    bool isPointIn(Point point);
    bool isZoomed();
    void setZoom(bool zoom);
    virtual void draw();
    virtual void toggleZoom();
    virtual ItemInfo IAm() = 0;
protected:
    bool logic2GraphicCardLocation(CardLocation location);
    ALLEGRO_BITMAP * image;
    Point min;
    Point max;
    Point center;
    double width;
    double height;
    unsigned int totalWidth, totalHeight;
    bool zoomed;
};

#endif /* GRAPHICITEM_H */

