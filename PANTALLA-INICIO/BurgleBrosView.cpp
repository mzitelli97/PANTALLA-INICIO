#include <vector>

#include "BurgleBrosView.h"
#include "GraphicEDices.h"
#include "GraphicGDie.h"
#include "GraphicGuard.h"
#include "GraphicGuardCards.h"
#include "GraphicLoot.h"
#include "GraphicPlayer.h"
#include "GraphicPlayerCard.h"
#include "GraphicTile.h"
#include "GraphicToken.h"
#include "GraphicMenuItem.h"
#include "GraphicWall.h"
#include "GraphicButton.h"
#include "GraphicHelp.h"
#include "LayerItem.h"
#include <time.h>


typedef enum {TILES_LIST, BUTTONS_LIST, CHARACTER_CARDS_LIST, LOOT_SHOW_LIST, EXTRA_DICES_LIST, GUARD_CARDS_LIST} FirstLayerLists;
typedef enum {TOKENS_LIST, GUARD_INFO_LIST, PLAYER_INFO_LIST, STATIC_ITEMS} SecondeLayerLists;
typedef enum {MENU_ITEM_LIST} ThirdLayerLists;

#define SCREEN_W 1200
#define SCREEN_H (SCREEN_W*9/16)
#define TITLE_H al_get_bitmap_height(backScreen)/20.0
#define ACTIONS_FONT_H al_get_bitmap_height(backScreen)/50.0
#define NO_FLOOR_ZOOMED -1
#define NO_GUARD_ZOOMED -1

#define PATH_BACKSCREEN     "Images/Backscreen/fondo.jpg"
#define PATH_FONT           "Font/font3.ttf"
#define PATH_ACTIONFONT     "Font/font6.ttf"  
#define ICON                "icon/burglebros.tga"

BurgleBrosView::BurgleBrosView(BurgleBrosModel * model) {
   
    error=false;
    
    if(imageLoader.initImages())
    {
        showingHelp = false;
        display=nullptr;
        backScreen=nullptr;
        actionsFont=nullptr;
                
        if(model != nullptr)
        {
            this->model = model;
            #ifdef FULLSCREEN
            al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
            #endif
           display =al_create_display(SCREEN_W,SCREEN_H);           
            if(display != nullptr)
            {
                backScreen = al_load_bitmap(PATH_BACKSCREEN);
                if(backScreen != nullptr)
                {
                    al_draw_scaled_bitmap(backScreen,0,0,al_get_bitmap_width(backScreen),al_get_bitmap_height(backScreen),0,0,al_get_display_width(display),al_get_display_height(display),0);
                    actionsFont=al_load_font(PATH_ACTIONFONT,ACTIONS_FONT_H,0);

                    if(actionsFont !=nullptr)
                    {
                        ALLEGRO_FONT * font = al_load_font(PATH_FONT,TITLE_H,0);
                        if(font != nullptr)
                        {
                            al_set_target_bitmap(backScreen);
                            al_draw_text(font,al_map_rgb(0,0,0),al_get_bitmap_width(backScreen)/2.0,TITLE_H/2,ALLEGRO_ALIGN_CENTER, "EDA BURGLE BROS");
                            al_destroy_font(font);
                            al_set_target_backbuffer(display);

                            this->resetZoom();
                            floorZoomed = NO_FLOOR_ZOOMED;
                            guardZoomed = NO_GUARD_ZOOMED;
                            playerZoomed = NON_PLAYER;
                            lootZoomed = NON_PLAYER;
                            GraphicHelp temp(imageLoader.getRules());
                            help=temp;
                            help.setScreenDimentions(al_get_display_width(display),al_get_display_height(display));
                            help.init();
                            showingHelp=false;
                            #ifdef ICON
                            ALLEGRO_BITMAP *icon =nullptr; 
                            icon=al_load_bitmap(ICON);                              
                            if(icon != nullptr)
                            {
                                al_set_display_icon(display,icon);
                                //al_destroy_bitmap(icon);
                            }
                            else cout<<"COMO EL CULO\n";
                            #endif
                            al_set_window_title(display,"EDA Burgle Bros");
                            #ifdef CARDS_CHEAT
                            cheatCards();
                            #endif
                            al_flip_display();
                        }else
                        {
                            //No se pudo crear fuente auxiliar
                            al_destroy_bitmap(backScreen);
                            al_destroy_display(display);
                            al_destroy_font(actionsFont);
                            error=true;
                            errorMsg= " Fonts couldnt be loaded.";
                        }
                    }else
                    {
                     //No se pudo crear actionFont
                        al_destroy_bitmap(backScreen);
                        al_destroy_display(display);
                        error=true;
                        errorMsg= " Background picture couldnt be loaded.";
                    }
                }else
                {
                    //No se pudo crear backscreen
                     al_destroy_display(display);
                     error=true;
                     errorMsg= " Background picture couldnt be loaded.";
                }
            }else
            {
                //No se pudo crear display
                error=true;
                errorMsg= " Display couldnt be created";
            }
        }else
        {
            //Puntero de model vacio
            error=true;
        }
    }else
    {
        error=true;
        errorMsg="ImageLoad failed\n\t"+imageLoader.getError();
    }
}

bool BurgleBrosView::didAnErrorStarting() {
    return error;
}

string BurgleBrosView::getErrorMsg() {
    
    return errorMsg;
}



BurgleBrosView::~BurgleBrosView()
{
    if(error != true)
    {
        al_destroy_display(display);
        al_destroy_bitmap(backScreen);
        al_destroy_font(actionsFont);
    }
}

void BurgleBrosView::reset()
{
    list<LayerItem>::iterator it_layers;
    
    for( it_layers = graphics.begin(); it_layers != graphics.end(); it_layers++)
        it_layers->erase();
    graphics.clear();
    
    resetZoom();
    floorZoomed = NO_FLOOR_ZOOMED;
    guardZoomed = NO_GUARD_ZOOMED;
    playerZoomed = NON_PLAYER;
    lootZoomed = NON_PLAYER;
    showingHelp = false;
}

void BurgleBrosView::resetZoom() {

    list<LayerItem>::iterator it_layers;
    onZoom=false;
    
    for( it_layers = graphics.begin(); it_layers != graphics.end(); it_layers++)
        it_layers->resetZoom();
}


void BurgleBrosView::ViewInit(BurgleBrosModel* model)
{  
    //creo los info2draw
    vector<Info2DrawCards> info_tiles= model->getInfo2DrawCards();
    Info2DrawGuard infoGuard[3];
    
    for(int i = 0; i < BOARD_STANDARD_FLOORS; i++)
    {
        infoGuard[i] = model->getInfo2DrawGuard(i);
    }
    list<Info2DrawLoot> infoLoot= model-> getInfo2DrawLoot();
    Info2DrawPlayer infoThisPlayer= model->getInfo2DrawPlayer(THIS_PLAYER);
    Info2DrawPlayer infoOtherPlayer= model->getInfo2DrawPlayer(OTHER_PLAYER);
    list<Info2DrawTokens> infoTokens= model->getInfo2DrawTokens();
    vector<wall> infoWalls = model->getInfo2DrawWalls();
    
    /**************************************************************************/
    //CREO LAS DIFERENTES CAPAS
    
    LayerItem auxLayer;

    list<LayerItem>::iterator it_layer;
    
    for(int i=0; i<INIT_QUANTITY_LAYERS;i++)
        graphics.push_back(auxLayer);
    
    it_layer=graphics.begin();
    
    
    /*********Inicializo la primera capa****************/

    //creo una lista de graphicTiles

    GroupItem auxTiles_item;
    
    for(int i=0; i<info_tiles.size();i++)
    {
         GraphicTile *auxTile_element=new GraphicTile(imageLoader.getImageP(info_tiles[i].type),imageLoader.getImageBackP(info_tiles[i].type),info_tiles[i].location,al_get_display_width(display),al_get_display_height(display));
         auxTiles_item.atachGraphicsItem((GraphicItem *) auxTile_element);
    }
    
    //creo una lista de Buttons
    GroupItem auxButtons_item;
    GraphicButton *auxButton;
    
    /*Los botones de zoom para cada piso*/
    for(int i = 0; i < BOARD_STANDARD_FLOORS; i++)
    {
        auxButton = new GraphicButton(imageLoader.getImageP(ZOOM_BUTTON), nullptr, ZOOM_BUTTON, al_get_display_width(display), al_get_display_height(display));
        auxButton->setZoomFloor(i);
        auxButton->setLocation();
        auxButtons_item.atachGraphicsItem((GraphicItem*) auxButton);
    }
    
    /*El boton del volumen*/
    auxButton = new GraphicButton(imageLoader.getImageP(MUTE_BUTTON), imageLoader.getImageP(UNMUTE_BUTTON), MUTE_BUTTON, al_get_display_width(display), al_get_display_height(display));
    auxButtons_item.atachGraphicsItem((GraphicItem*) auxButton);
    
    /*Los demas botones*/
    for(int i = (int)PASS_BUTTON; i <= (int)QUIT_BUTTON; i++)
    {
        auxButton = new GraphicButton(imageLoader.getImageP((buttonAction)i), nullptr, (buttonAction)i, al_get_display_width(display), al_get_display_height(display));
        auxButtons_item.atachGraphicsItem((GraphicItem*) auxButton);
    }

          
    //creo una lista de graphicCharacterscards
    GroupItem auxCharactersCards_item;
    
    GraphicPlayerCard *auxCharactersCardsThis_element=new GraphicPlayerCard(imageLoader.getImageP(infoThisPlayer.character,true),imageLoader.getImageP(STEALTH_TOKEN), infoThisPlayer.lives,infoThisPlayer.name, THIS_PLAYER,al_get_display_width(display),al_get_display_height(display)); // con true devuelve la carta
    GraphicPlayerCard *auxCharactersCardsOther_element=new GraphicPlayerCard(imageLoader.getImageP(infoOtherPlayer.character,true),imageLoader.getImageP(STEALTH_TOKEN), infoOtherPlayer.lives,infoOtherPlayer.name, OTHER_PLAYER,al_get_display_width(display),al_get_display_height(display)); // con true devuelve la carta
    
    auxCharactersCardsThis_element->setFont(actionsFont);
    auxCharactersCardsOther_element->setFont(actionsFont);
    auxCharactersCardsThis_element->setTurn(infoThisPlayer.turn);
    auxCharactersCardsOther_element->setTurn(infoOtherPlayer.turn);
    auxCharactersCards_item.atachGraphicsItem((GraphicItem *) auxCharactersCardsThis_element);
    auxCharactersCards_item.atachGraphicsItem((GraphicItem *) auxCharactersCardsOther_element);
            
    //creo una lista para graphicLoots
    GroupItem auxLoots_item;
       
    //creo una lista para extra_dies
    GroupItem auxExtraDies_item;
    
    //creo una lista de graphicGuardcards 
    GroupItem auxGuards_item;
    
    for(int i = 0; i < BOARD_STANDARD_FLOORS; i++)
    {
        GraphicGuardCards *auxGuard_card = new GraphicGuardCards(imageLoader.getImageBackP(infoGuard[i].position),i,false);
        auxGuard_card->setScreenDimentions(al_get_display_width(display),al_get_display_height(display));
        GraphicGuardCards *auxGuard_card2 = new GraphicGuardCards(imageLoader.getImageP(infoGuard[i].shownDeck.front()),i,true);
        auxGuard_card2->setScreenDimentions(al_get_display_width(display),al_get_display_height(display));
        auxGuard_card2->push_top_card(imageLoader.getImageP(infoGuard[i].shownDeck.front()));
        auxGuards_item.atachGraphicsItem((GraphicItem *)auxGuard_card);
        auxGuards_item.atachGraphicsItem((GraphicItem *)auxGuard_card2);
    }
    
    //**********push sobre la primera capa 
    it_layer->atachGroupItem(&auxTiles_item);
    it_layer->atachGroupItem(&auxButtons_item);
    it_layer->atachGroupItem(&auxCharactersCards_item);
    it_layer->atachGroupItem(&auxLoots_item);
    it_layer->atachGroupItem(&auxExtraDies_item);
    it_layer->atachGroupItem(&auxGuards_item);
    
    it_layer++;                                     //Paso a la segunda capa

    /*********Inicializo la segunda capa****************/
    
    //creo una lista de token
    GroupItem auxToken_item;
    
    //creo una lista para guardInfo
    GroupItem auxGuardInfo_item;
    
    GraphicGDie * auxGuardDie;
    GraphicGuard * auxGuard;
    for(int i = 0; i < BOARD_STANDARD_FLOORS; i++)
    {
        auxGuardDie = new GraphicGDie(imageLoader.getImageP(RED_DICE, 2));
        auxGuardDie->setScreenDimentions(al_get_display_width(display),al_get_display_height(display));
        auxGuard = new GraphicGuard(imageLoader.getGuardImage());
        auxGuard->setScreenDimentions(al_get_display_width(display),al_get_display_height(display));
       
        auxGuardInfo_item.atachGraphicsItem((GraphicItem *)auxGuard);
        auxGuardInfo_item.atachGraphicsItem((GraphicItem *)auxGuardDie);
    }
    
    
    //creo una lista de graphicPlayer
    GroupItem auxPlayer_item;
    
    GraphicPlayer *auxThisPlayer_element=new GraphicPlayer(imageLoader.getImageP(infoThisPlayer.character,false),infoThisPlayer.position,al_get_display_width(display),al_get_display_height(display)); 
    GraphicPlayer *auxOtherPlayer_element=new GraphicPlayer(imageLoader.getImageP(infoOtherPlayer.character,false),infoOtherPlayer.position,al_get_display_width(display),al_get_display_height(display)); 
    auxPlayer_item.atachGraphicsItem((GraphicItem *)auxThisPlayer_element);
    auxPlayer_item.atachGraphicsItem((GraphicItem *)auxOtherPlayer_element);

    //creo una lista de static item
    GroupItem auxStaticItem;
    /*Paredes de todos los pisos*/
    for(int i = 0; i < NUMBER_OF_WALLS * BOARD_STANDARD_FLOORS; i++)
    {
        GraphicWall * wall_i = new GraphicWall;
        wall_i->setScreenDimentions(al_get_display_width(display), al_get_display_height(display));
        wall_i->setLocation(infoWalls[i].FrontCard, infoWalls[i].RearCard);
        auxStaticItem.atachGraphicsItem(wall_i);
    }
    
    /*Mazo de loots (loot boca abajo, en el medio)*/
    GraphicLoot * auxLoot = new GraphicLoot(imageLoader.getImageBackP(infoLoot.front().loot));
    auxLoot->setScreenDimentions(al_get_display_width(display),al_get_display_height(display));
    auxLoot->setPosition(0);
    auxStaticItem.atachGraphicsItem(auxLoot);

    //**********push sobre la segunda capa 
    it_layer->atachGroupItem(&auxToken_item);
    it_layer->atachGroupItem(&auxGuardInfo_item);
    it_layer->atachGroupItem(&auxPlayer_item);
    it_layer->atachGroupItem(&auxStaticItem);
    
    it_layer++;
    
    /*********Inicializo la tercer capa****************/
    
    //creo una lista de menu_items
    GroupItem auxMenuItem_item;
        
    //**********push sobre la tercer capa 
    it_layer->atachGroupItem(&auxMenuItem_item);
      
    
}

bool BurgleBrosView::isShowingHelp() {
    return showingHelp;
}

void BurgleBrosView::showHelp(bool yesOrNo) {
    if(yesOrNo != showingHelp)
        help.resetScroll();
    this->showingHelp = yesOrNo;
}

void BurgleBrosView::setHelpScroll(unsigned int scroll) {
    help.setScroll(scroll);
}
void BurgleBrosView::update()
{
    /*Update all*/
    
    updateTiles();
    updateTokens();
    updateCharacters();
    updateCharacterCards();
    updateLoots();
    updateGuards();
    updateExtraDices();
    
    
    //after=clock();
    //cout << "Update tardó: "<< ((double)(after-before))/(double)CLOCKS_PER_SEC<< " segundos."<<endl; 
    
    
    /*Draw all*/
    al_draw_scaled_bitmap(backScreen,0,0,al_get_bitmap_width(backScreen),al_get_bitmap_height(backScreen),0,0,al_get_display_width(display),al_get_display_height(display),0);
    list<LayerItem>::iterator it_layers;
    if(!showingHelp)
    {
       for( it_layers = graphics.begin(); it_layers != graphics.end(); it_layers++)
            it_layers->draw();
    }
    else
        help.draw();
    
    al_flip_display();
    //after=clock();
    //cout << "Draw tardó: "<< ((double)(after-before))/(double)CLOCKS_PER_SEC << " segundos."<<endl; 
}


ItemInfo BurgleBrosView::itemFromClick(Point point)
{
    ItemInfo retVal = {NO_ITEM_CLICK, nullptr};
    list<LayerItem>::reverse_iterator it_layer;   
    if(!showingHelp)
    {  
        for(it_layer=graphics.rbegin(); it_layer!= graphics.rend() && retVal.type==NO_ITEM_CLICK; it_layer++)
            retVal=it_layer->itemFromClick(point);
    }
    else
        retVal=help.IAm();
   
    return retVal;
}

void 
BurgleBrosView::updateTiles()
{
    vector<Info2DrawCards> aux = model->getInfo2DrawCards();
    list<GraphicItem *>::iterator it = accessGraphicItems(FIRST_LAYER, TILES_LIST);
    for(unsigned int i=0; i < BOARD_STANDARD_FLOORS * FLOOR_RAWS * FLOOR_COLUMNS ; i++, it++)
    {
        GraphicTile *p = dynamic_cast<GraphicTile *>(*it);
        if(p != nullptr)
        {
            if(!(p->isVisible()) && aux[i].isVisible)
                p->setVisible(imageLoader.getImageP(aux[i].safeNumber));
        }
    }
}

void 
BurgleBrosView::updateTokens()
{
    list<Info2DrawTokens> info_tokens = model->getInfo2DrawTokens();
    list<GroupItem>::iterator it_itemType;
    it_itemType = deleteList(SECOND_LAYER, TOKENS_LIST);
    
    map<CardLocation, unsigned int> tokensCount;
    
    list<Info2DrawTokens>::iterator it;
    for( it = info_tokens.begin(); it != info_tokens.end(); it++)
    {
        GraphicToken * token = new GraphicToken(imageLoader.getImageP(it->token));
        token->setScreenDimentions(al_get_display_width(display),al_get_display_height(display));
        if(onZoom && it->position.floor == floorZoomed)
            token->setZoom(true);
        token->setPosition(it->position, tokensCount[it->position]++);
        it_itemType->atachGraphicsItem(token);
    }
}

void 
BurgleBrosView::updateLoots()
{
    list<Info2DrawLoot> aux = model->getInfo2DrawLoot();
    map<PlayerId, unsigned int> lootsCount;
    pair<bool,CardLocation> goldBar;
       
    list<GroupItem>::iterator itemsList = deleteList(FIRST_LAYER, LOOT_SHOW_LIST);
    for(list<Info2DrawLoot>::iterator newInfo = aux.begin() ; newInfo!= aux.end(); newInfo++)
    {
        GraphicLoot *p = new GraphicLoot(newInfo->owner, imageLoader.getImageP(newInfo->loot),false);
        p->setScreenDimentions(al_get_display_width(display),al_get_display_height(display));
        if(onZoom && (newInfo->owner == lootZoomed) && lootZoomed != NON_PLAYER) 
            p->setZoom(true);
        p->setPosition(lootsCount[newInfo->owner]++);
        itemsList->atachGraphicsItem((GraphicItem *) p);
        if(newInfo->loot == GOLD_BAR)
        {
            goldBar = model->getGoldBarInfo();
            if(goldBar.first == true)           //there is a gold bar on the floor
            {
                GraphicLoot *p = new GraphicLoot(newInfo->owner, imageLoader.getImageP(GOLD_BAR), true);
                p->setScreenDimentions(al_get_display_width(display),al_get_display_height(display));
                if(onZoom && goldBar.second.floor == floorZoomed) p->setZoom(true);
                p->setPosition(goldBar.second);
                itemsList->atachGraphicsItem((GraphicItem *)p);
                newInfo++;        //always after a goldBar there is another goldBar
            }
        }
    }
    
}
void BurgleBrosView::updateButtons()
{
    
}


void BurgleBrosView::updateCharacters() {
    
    list<GraphicItem*>::iterator it = accessGraphicItems(SECOND_LAYER, PLAYER_INFO_LIST);
    //First Player
    Info2DrawPlayer player = model->getInfo2DrawPlayer(THIS_PLAYER);
    GraphicPlayer* gPlayer = dynamic_cast<GraphicPlayer*> (*it); //ASUMI QUE EL PRIMERO ES THIS_PLAYER
    
    if(gPlayer!=NULL)
    {
        if(onZoom && player.position.floor == floorZoomed) gPlayer->setZoom(true);
        else gPlayer->setZoom(false);
        gPlayer->setLocation(player.position);
        gPlayer->setOnBoard(player.isOnBoard);
    }
    //Second Player
    player = model->getInfo2DrawPlayer(OTHER_PLAYER);
    gPlayer = dynamic_cast<GraphicPlayer*> (*(++it));
    if(gPlayer!=NULL)
    {
        if(onZoom && player.position.floor == floorZoomed) gPlayer->setZoom(true);
        else gPlayer->setZoom(false);
        gPlayer->setLocation(player.position);
        gPlayer->setOnBoard(player.isOnBoard);
    }
}

void BurgleBrosView::updateCharacterCards() {
    Info2DrawPlayer player = model->getInfo2DrawPlayer(THIS_PLAYER);
    //FirstPlayer
    list<GraphicItem *>::iterator it = accessGraphicItems(FIRST_LAYER, CHARACTER_CARDS_LIST);
    GraphicPlayerCard* gPlayerCard = dynamic_cast<GraphicPlayerCard *> (*it);
    if(gPlayerCard!=NULL)
    {
        if(onZoom && playerZoomed == THIS_PLAYER) gPlayerCard->setZoom(true);
        else gPlayerCard->setZoom(false);
        gPlayerCard->setLivesAndActions(player.lives,player.currActions);
        gPlayerCard->setTurn(player.turn);
    }
    //SecondPlayer
    player = model->getInfo2DrawPlayer(OTHER_PLAYER);
    gPlayerCard = dynamic_cast<GraphicPlayerCard *> (*(++it));
    if(gPlayerCard!=NULL)
    {
        if(onZoom && playerZoomed == OTHER_PLAYER) gPlayerCard->setZoom(true);
        else gPlayerCard->setZoom(false);
        gPlayerCard->setLivesAndActions(player.lives,player.currActions);
        gPlayerCard->setTurn(player.turn);
    }
}
void BurgleBrosView::updateGuards()
{
    list<GraphicItem *>:: iterator guard = accessGraphicItems(SECOND_LAYER, (unsigned int) GUARD_INFO_LIST);
    list<GraphicItem *>::iterator it = accessGraphicItems(FIRST_LAYER, (unsigned int) GUARD_CARDS_LIST);
    bool zoom;
 
    for(int i = 0; i < BOARD_STANDARD_FLOORS; i++)
    {
        /*Here update all things related to a guard on a floor*/
        Info2DrawGuard info_guard = model->getInfo2DrawGuard(i);
        
        if(*guard != NULL )
        {
            /*Suppose the first item is the guard item*/
            GraphicGuard * guard_item = dynamic_cast<GraphicGuard*> (*guard);
            GraphicGDie * guard_die = dynamic_cast<GraphicGDie*> (*(++guard));
            if( guard_item == nullptr)
            {
                /*It means the die was the first*/
                guard_item = dynamic_cast<GraphicGuard*>(*guard);
                guard_die = dynamic_cast<GraphicGDie*> (*(--guard));
                guard++;
            }
            guard++;    //point to next floor
            
            if(onZoom && i == floorZoomed) zoom = true;
            else zoom = false;
            
            guard_item->setZoom(zoom);
            guard_item->setInitialized(info_guard.initialized);
            guard_item->setPosition(info_guard.position);
            guard_die->setZoom(zoom);
            guard_die->setInitialized(info_guard.initialized);
            guard_die->setPosition(info_guard.diePosition);
            guard_die->setNumber(imageLoader.getImageP(RED_DICE, info_guard.dieNumber));

            GraphicGuardCards * it_cards = dynamic_cast<GraphicGuardCards *> (*it);
            /*First the non shown deck*/
            if(info_guard.shownDeck.size() >= NUMBER_OF_CARDS_TO_PLAY) it_cards->setVisible(false);
            else it_cards->setVisible(true);
            
            /*Then the shown deck*/
            it_cards = dynamic_cast<GraphicGuardCards *> (*(++it));
            it_cards->setVisible(info_guard.initialized);
            if(onZoom && i == guardZoomed) it_cards->setZoom(true);
            else it_cards->setZoom(false);
            it_cards->setTopOfNonVisibleDeck(info_guard.isTopOfNotShownDeckVisible, imageLoader.getImageP(info_guard.topOfNotShownDeck));

            it_cards->clearShownCards();
            while(!info_guard.shownDeck.empty())
            {         
                it_cards->push_top_card(imageLoader.getImageP(info_guard.shownDeck.back()));
                info_guard.shownDeck.pop_back();
            } 
            it++;
        }
    }
}

void BurgleBrosView::updateExtraDices()
{
    vector<unsigned int> info_dices = model->getInfo2DrawExtraDices();
    list<GroupItem>::iterator it_itemType;
    it_itemType = deleteList(FIRST_LAYER, EXTRA_DICES_LIST);
       
    for( int i = 0; i < info_dices.size(); i++)
    {
        if(info_dices[i] != 0)      //if its 0 it means there is no such extra die
        {
            GraphicEDices * dice = new GraphicEDices(imageLoader.getImageP(WHITE_DICE, info_dices[i]));
            dice->setScreenDimentions(al_get_display_width(display),al_get_display_height(display));
            if(onZoom && floorZoomed != NO_FLOOR_ZOOMED) dice->toggleZoom();
            dice->setPosition(i);
            it_itemType->atachGraphicsItem(dice);
        }
        else break;                 //after a die with 0, the following dices are also 0
    }
}

list<GraphicItem *>::iterator BurgleBrosView::accessGraphicItems(Layers layer, unsigned int itemType)
{
    
    list<LayerItem>::iterator it_layers= graphics.begin();
    advance(it_layers,layer);
    return it_layers->accessGraphicItems(itemType);
}


list<GroupItem>::iterator BurgleBrosView::deleteList(Layers layer, unsigned int itemList)
{
    list<LayerItem>::iterator aux= graphics.begin();
    advance(aux, layer);
    return aux->erese(itemList);
}

string BurgleBrosView::MessageBox(vector<string> &msg)
{
    int aux=0;
    if(msg.size()>=4)
    {
        string buttons="";
        for(unsigned int i=3;i<msg.size();++i)
            buttons+=msg[i]+"|";
        buttons.pop_back();
        //cout<<buttons<<endl;
        while(!(aux=al_show_native_message_box(display, msg[0].c_str(),msg[1].c_str(),msg[2].c_str(),buttons.c_str(), ALLEGRO_MESSAGEBOX_QUESTION)));
    }
    //cout<<2+aux<<endl;
    //cout<<msg[2+aux]<<endl;
    return msg[2+aux];
}
int BurgleBrosView::yesNoMessageBox(vector<string> &msg)
{
    return al_show_native_message_box(display, msg[0].c_str(),msg[1].c_str(),msg[2].c_str(),nullptr, ALLEGRO_MESSAGEBOX_YES_NO);
}

void BurgleBrosView::showMenu(list<string> options, Point click, CardLocation tile)
{
    list<GroupItem>::iterator menu_items;
    menu_items = deleteList(THIRD_LAYER,(unsigned int) MENU_ITEM_LIST);
    
    list<string>::iterator it;
    int i = 0;
    for( it = options.begin(); it != options.end(); i++, it++)
    {
        GraphicMenuItem * option_i = new GraphicMenuItem(click, tile, i);
        option_i->setOption(*it);
        option_i->setScreenDimentions(al_get_display_width(display),al_get_display_height(display));
        if(onZoom) option_i->toggleZoom();
        menu_items->atachGraphicsItem((GraphicItem *) option_i);
    }
}
void BurgleBrosView::showMenu(list<string> options, Point click, unsigned int floor)
{
    list<GroupItem>::iterator menu_items;
    menu_items = deleteList(THIRD_LAYER,(unsigned int) MENU_ITEM_LIST);
    CardLocation tile;
    tile.floor=floor;
    list<string>::iterator it;
    int i = 0;
    for( it = options.begin(); it != options.end(); i++, it++)
    {
        GraphicMenuItem * option_i = new GraphicMenuItem(click, tile, i);
        option_i->setOption(*it);
        option_i->setScreenDimentions(al_get_display_width(display),al_get_display_height(display));
        if(onZoom) option_i->toggleZoom();
        menu_items->atachGraphicsItem((GraphicItem *) option_i);
    }
}

void BurgleBrosView::eraseMenu()
{
    list<GroupItem>::iterator it_itemType;
    it_itemType = deleteList(THIRD_LAYER,(unsigned int) MENU_ITEM_LIST);
}

void BurgleBrosView::setZoom() {
    
    onZoom ^= true;
    list<LayerItem>::iterator it_layer;
    
    for(it_layer=graphics.begin();it_layer != graphics.end();it_layer++)
        it_layer->setZoom();
       
}


void BurgleBrosView::zoomFloor(unsigned int floor, Model * auxModel)
{
    setZoom();
    floorZoomed = floor;
    guardZoomed = NO_GUARD_ZOOMED;
    lootZoomed = NON_PLAYER;
    playerZoomed = NON_PLAYER;
    eraseMenu();
    BurgleBrosModel * model = (BurgleBrosModel *) auxModel;
    list<GraphicItem *>::iterator it = accessGraphicItems(FIRST_LAYER, (unsigned int) TILES_LIST);
    for(unsigned int i=0; i < BOARD_STANDARD_FLOORS * FLOOR_RAWS * FLOOR_COLUMNS ; i++, it++)
    {
        GraphicTile * tile = dynamic_cast<GraphicTile *>(*it);
        if(tile->getLocation().floor == floor)
            tile->toggleZoom();
    }
    
    it = accessGraphicItems(SECOND_LAYER, STATIC_ITEMS);
    advance(it,NUMBER_OF_WALLS * floor);
    vector<wall> infoWalls = model->getInfo2DrawWalls();
    for(int i = 0; i < NUMBER_OF_WALLS; it++, i++)
    {
        GraphicWall * wall = dynamic_cast<GraphicWall *> (*it);
        if(wall !=nullptr)
        {
            wall->toggleZoom();
            wall->setLocation(infoWalls[i+NUMBER_OF_WALLS * floor].FrontCard, infoWalls[i+NUMBER_OF_WALLS * floor].RearCard);
        }
    }
    
    //Info2DrawPlayer player = model->getInfo2DrawPlayer(model->getPlayerOnTurn());
    it = accessGraphicItems(FIRST_LAYER, BUTTONS_LIST);
    advance(it, floor);                                             //go to the zoom icon of the floor zoomed
    GraphicButton * button = dynamic_cast<GraphicButton *> (*it);
    if(button != nullptr)
    {
        button->toggleZoom();
        button->setLocation();
        advance(it,BOARD_STANDARD_FLOORS-floor);                            //go to the rest of the buttons
        for(int i = (int)MUTE_BUTTON; i <= (int)QUIT_BUTTON; i++, it++)
        {
            button = dynamic_cast<GraphicButton *> (*it);
            button->toggleZoom();
            if(i == (int)MUTE_BUTTON) i++;         //this is because there are the MUTE and the UNMUTE buttons
        }
    }
}

void BurgleBrosView::zoomLoot(PlayerId owner)
{
    if(owner != NON_PLAYER)     setZoom();

    floorZoomed = NO_FLOOR_ZOOMED;
    guardZoomed = NO_GUARD_ZOOMED;
    lootZoomed = owner;
    playerZoomed = NON_PLAYER;
    list<GraphicItem *>::iterator it = accessGraphicItems(FIRST_LAYER, (unsigned int) BUTTONS_LIST);
    advance(it,BOARD_STANDARD_FLOORS);      //the zoom buttons do not appear here
    GraphicButton * button=nullptr;
    for(int i = (int)MUTE_BUTTON; i <= (int)QUIT_BUTTON; i++, it++)
    {
        if( i != (int)PASS_BUTTON)      //the pass button do not appear here
        {
            button = dynamic_cast<GraphicButton *> (*it);
            if(button !=nullptr) button->toggleZoom();
            if(i == (int)MUTE_BUTTON) i++;         //this is because there are the MUTE and the UNMUTE buttons
            
        }
    }   
}

void BurgleBrosView::zoomPlayerCard(PlayerId player)
{
    if(player != NON_PLAYER)     setZoom();
    floorZoomed = NO_FLOOR_ZOOMED;
    guardZoomed = NO_GUARD_ZOOMED;
    lootZoomed = NON_PLAYER;
    playerZoomed = player;
    list<GraphicItem *>::iterator it = accessGraphicItems(FIRST_LAYER, (unsigned int) BUTTONS_LIST);
    advance(it,BOARD_STANDARD_FLOORS);      //the zoom buttons do not appear here
    GraphicButton * button=nullptr;
    for(int i = (int)MUTE_BUTTON; i <= (int)QUIT_BUTTON; i++, it++)
    {
        if( i != (int)PASS_BUTTON)      //the pass button do not appear here
        {
            button = dynamic_cast<GraphicButton *> (*it);
            if(button !=nullptr)    button->toggleZoom();
            if(i == (int)MUTE_BUTTON) i++;         //this is because there are the MUTE and the UNMUTE buttons
            
        }
    }   
}

void BurgleBrosView::zoomGuardDeck(unsigned int floor)
{
    setZoom();
    floorZoomed = NO_FLOOR_ZOOMED;
    guardZoomed = floor;
    lootZoomed = NON_PLAYER;
    playerZoomed = NON_PLAYER;
    list<GraphicItem *>::iterator it = accessGraphicItems(FIRST_LAYER, (unsigned int) BUTTONS_LIST);
    advance(it,BOARD_STANDARD_FLOORS);      //the zoom buttons do not appear here
    GraphicButton * button;
    for(int i = (int)MUTE_BUTTON; i <= (int)QUIT_BUTTON; i++, it++)
    {
        if( i != (int)PASS_BUTTON)      //the pass button do not appear here
        {
            button = dynamic_cast<GraphicButton *> (*it);
            if(button != nullptr)     button->toggleZoom();
            if(i == (int)MUTE_BUTTON) i++;         //this is because there are the MUTE and the UNMUTE buttons
        }
    }   
}

void BurgleBrosView::toggleVolButton()
{
    list<GraphicItem *>::iterator it = accessGraphicItems(FIRST_LAYER, (unsigned int) BUTTONS_LIST);
    advance(it,BOARD_STANDARD_FLOORS);      //avanzo  hasta el boton del volumen(primero estan los de zoom)
    GraphicButton * button=nullptr;
    button = dynamic_cast<GraphicButton *> (*it);
    if(button != nullptr)   button->toggleMute();
}

void BurgleBrosView::cheatCards()
{
    list<GraphicItem *>::iterator it = accessGraphicItems(FIRST_LAYER, (unsigned int) TILES_LIST);
    for(unsigned int i=0; i < BOARD_STANDARD_FLOORS * FLOOR_RAWS * FLOOR_COLUMNS ; i++, it++)
    {
        GraphicTile * tile = dynamic_cast<GraphicTile *>(*it);
        if(tile != nullptr) tile->setVisible(imageLoader.getImageP(3));
    }
}
