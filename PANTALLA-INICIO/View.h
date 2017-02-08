/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   View.h
 * Author: javier
 *
 * Created on December 24, 2016, 5:09 PM
 */

#ifndef VIEW_H
#define VIEW_H

#include "Model.h"

typedef enum {  TILE_CLICK = 0, LOOT_CARDS_CLICK, CHAR_CARD_CLICK, GUARD_CARDS_CLICK, ZOOM_CLICK,TEXT_BOX, MENU_ITEM_CLICK, VOL_BUTTON_CLICK, HELP_BUTTON_CLICK, PASS_BUTTON_CLICK, EXIT_BUTTON_CLICK, NO_ITEM_CLICK} clickItem;
typedef enum{ ZOOM_BUTTON, /*FULLSCREEN_BUTTON,*/CONNECT_BUTTON, MUTE_BUTTON, UNMUTE_BUTTON, PASS_BUTTON, HELP_BUTTON, QUIT_BUTTON} buttonAction;

class View {
public:
    View();
    View(const View& orig);
    virtual ~View();
    virtual void update(Model * model) = 0;
private:

};

#endif /* VIEW_H */

