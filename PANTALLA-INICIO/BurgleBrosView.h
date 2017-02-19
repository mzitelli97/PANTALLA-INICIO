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
#include "Observer.h"
#include "GraphicItem.h"
#include "GraphicHelp.h"
#include <list>
typedef enum {FIRST_LAYER, SECOND_LAYER, THIRD_LAYER} Layers;
#define DROPDOWNS_MENUS_LIST 0
#define INIT_QUANTITY_LAYERS 3




#define FULLSCREEN

class BurgleBrosView : public Observer{
public:
    BurgleBrosView();
    BurgleBrosView(BurgleBrosModel * model);
    
    /*Devuelve sobre que item fue el click, junto con informacion del item.
     TILE -> CARDLOCATION
     LOOT_CARD -> OWNER(actionOrigin)
     PLAYER_CARDS -> ACTIONORIGIN
     GUARD_CARD -> AUXGUARDINFO(unsigned int y bool)
     MENU_ITEM -> AUXINFO (string y cardLocation)*/
    ItemInfo itemFromClick(Point point);
    
    virtual void update();
    void reset();
    void ViewInit(BurgleBrosModel* model);
    void showMenu(list<string> options, Point click, CardLocation tile);
    void showMenu(list<string> options, Point click, unsigned int floor);
    void eraseMenu();
    void zoomFloor(unsigned int floor, Model * auxModel);
    void zoomLoot(PlayerId owner);
    void zoomPlayerCard(PlayerId player);
    void zoomGuardDeck(unsigned int floor);
    void toggleVolButton();
    string MessageBox(vector<string> &message);
    int yesNoMessageBox(vector<string> &message);
    void showHelp(bool yesOrNo);
    void setHelpScroll(unsigned int scroll);
    bool isShowingHelp();
    void cheatCards();
    virtual ~BurgleBrosView();
private:
    BurgleBrosModel * model;
    list<GraphicItem *>::iterator accessGraphicItems(Layers layer, unsigned int itemType);
    list<list<GraphicItem *>>::iterator deleteList(Layers layer, unsigned int itemList);
    void drawScreen();
    void updateButtons();
    void updateCharacters();
    void updateCharacterCards();
    void updateTiles();
    void updateLoots();
    void updateGuards();
    void updateTokens();
    void updateExtraDices();
    list<list<list<GraphicItem *>>> graphicInterface;                   //layers structure of graphical interface
    vector<GraphicItem*> walls;
    ALLEGRO_DISPLAY * display;
    ALLEGRO_BITMAP * backScreen;
    ALLEGRO_FONT * actionsFont;
    ImageLoader imageLoader;
    bool showingHelp;
    bool onZoom;
    int floorZoomed;
    int guardZoomed;
    PlayerId lootZoomed;
    PlayerId playerZoomed;
    GraphicHelp help;

};

#endif /* BURGLEBROSVIEW_H */
