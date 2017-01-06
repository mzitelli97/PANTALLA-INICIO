/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BurgleBrosView.h
 * Author: javier
 *
 * Created on December 24, 2016, 12:58 PM
 */

#ifndef BURGLEBROSVIEW_H
#define BURGLEBROSVIEW_H
#include "BurgleBrosModel.h"
#include "ImageLoader.h"
#include "View.h"
#include "GraphicItem.h"
#include <list>
typedef enum {FIRST_LAYER, SECOND_LAYER, THIRD_LAYER} Layers;
typedef enum {TILES_LIST, BUTTONS_LIST, CHARACTER_CARDS_LIST, LOOT_SHOW_LIST, EXTRA_DICES_LIST, GUARD_CARDS_LIST} FirstLayerLists;
typedef enum {TOKENS_LIST, GUARD_INFO_LIST, PLAYER_INFO_LIST, STATIC_ITEMS} SecondeLayerLists;
typedef enum {MENU_ITEM_LIST} ThirdLayerLists;
#define DROPDOWNS_MENUS_LIST 0
#define INIT_QUANTITY_LAYERS 3

class BurgleBrosView : public View{
public:
    BurgleBrosView();
    BurgleBrosView(const BurgleBrosView& orig);
    virtual void update(BurgleBrosModel * model);
    clickItem itemFromClick(Point point);
    void ViewInit(BurgleBrosModel* model);
    void showMenu(list<string> options, Point click, CardLocation tile);
    string getDDMenuOption(Point aux);
    CardLocation getDDMenuLocation(Point aux);
    void eraseMenu();
    virtual ~BurgleBrosView();
private:
    list<GraphicItem *>::iterator accessGraphicItems(Layers layer, unsigned int itemType);
    list<list<GraphicItem *>>::iterator deleteList(Layers layer, unsigned int itemList);
    void updateCharacters(BurgleBrosModel *model);
    void updateCharacterCards(BurgleBrosModel *model);
    void updateTiles(BurgleBrosModel * model);
    void updateLoots(BurgleBrosModel * model);
    void updateGuards(BurgleBrosModel * model);
    void updateTokens(BurgleBrosModel * model);
    void updateExtraDices(BurgleBrosModel * model);
    list<list<list<GraphicItem *>>> graphicInterface;                   //layers structure of graphical interface
    ALLEGRO_DISPLAY * display;
    ALLEGRO_BITMAP * backScreen;
    ImageLoader imageLoader;

};

#endif /* BURGLEBROSVIEW_H */