#include "CView.h"
#include "TextBox.h"
#include "GraphicButton.h"
#include "CModel.h"

#define N_BOXES 3
#define SCREEN_W 1080
#define SCREEN_H (SCREEN_W*9/16)
#define ACTIONS_FONT_H al_get_bitmap_height(backScreen)/50.0
#define BOX_WIDTH al_get_display_width(display)/3.0
#define BOX_HEIGHT al_get_display_height(display)/20.0

#define BOX_MIN_X al_get_display_width(display)/2.0
#define BOX_MIN_Y al_get_display_height(display)/3.0
#define SPACE_X al_get_display_width(display)/50.0
#define SPACE_Y al_get_display_height(display)/10.0

#define TEXT_COLOR al_map_rgb(0,0,255)

CView::CView()
{
    imageLoader.initImages();           //Falta checkear.
#ifdef FULLSCREEN
    al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
#endif
    display =al_create_display(SCREEN_W,SCREEN_H);           //Falta checkear.
    backScreen = al_load_bitmap("fondo.jpg");
    al_draw_scaled_bitmap(backScreen,0,0,al_get_bitmap_width(backScreen),al_get_bitmap_height(backScreen),0,0,al_get_display_width(display),al_get_display_height(display),0);
    al_set_window_title(display,"EDA Burgle Bros");
    font = al_load_font("title.ttf",BOX_HEIGHT,0);
        
    al_draw_text(font,TEXT_COLOR,SPACE_X,BOX_MIN_Y,ALLEGRO_ALIGN_LEFT,"YOUR IP:");
    al_draw_text(font,TEXT_COLOR,SPACE_X,BOX_MIN_Y + SPACE_Y+BOX_HEIGHT,ALLEGRO_ALIGN_LEFT,"FRIEND'S IP:");
    al_draw_text(font,TEXT_COLOR,SPACE_X,BOX_MIN_Y+ 2*(SPACE_Y+BOX_HEIGHT),ALLEGRO_ALIGN_LEFT,"ENTER YOUR NAME:");
    
    ALLEGRO_FONT * box_font = al_load_font("fonts.ttf",BOX_HEIGHT, 0);
    //Creo las cajas de texto
    for(int i = 0; i < N_BOXES; i++)
    {
        TextBox * box = new TextBox(BOX_WIDTH,BOX_HEIGHT,box_font, (textSelected)i);
        box->setPosition(BOX_MIN_X, BOX_MIN_Y + SPACE_Y* (i%N_BOXES) + (i%N_BOXES) * BOX_HEIGHT);
        gList.push_back((GraphicItem*) box);
    }
    
    //Creo los botones
    /*El boton del volumen*/
    GraphicButton * auxButton = new GraphicButton(imageLoader.getImageP(MUTE_BUTTON), imageLoader.getImageP(UNMUTE_BUTTON), MUTE_BUTTON, al_get_display_width(display), al_get_display_height(display));
    gList.push_back(auxButton);
    /*EL boton de conectar*/
    auxButton = new GraphicButton(imageLoader.getImageP(CONNECT_BUTTON), imageLoader.getImageP(CONNECTING_BUTTON), CONNECT_BUTTON, al_get_display_width(display), al_get_display_height(display));
    gList.push_back(auxButton);
    /*Los demas botones*/
    for(int i = (int)HELP_BUTTON; i <= (int)QUIT_BUTTON; i++)
    {
        auxButton = new GraphicButton(imageLoader.getImageP((buttonAction)i), nullptr, (buttonAction)i, al_get_display_width(display), al_get_display_height(display));
        gList.push_back(auxButton);
    }
    
    al_destroy_font(font);
    //al_destroy_font(box_font);
}

CView::CView(const CView& orig) {
}

CView::~CView()
{
    al_destroy_bitmap(backScreen);
    al_destroy_display(display);
}

void CView::update(Model* auxModel)
{
    CModel * model = (CModel*) auxModel;
    initInfo data = model->getInfo();
    list<GraphicItem *>::iterator it = gList.begin();
    //Update all the text boxes with the info of the model
    for(int i = 0; i < NONE_SELECTED; i++, it++)
    {
        TextBox * box = dynamic_cast<TextBox*>(*it);
        if(box != nullptr)
        {
            box->setText(data.entries[i]);
            if(data.selected == i) box->select();
            else box->unselect();
        }
            
    }
    //Draw all the items
    for(it = gList.begin(); it != gList.end(); it++)
        (*it)->draw();
    al_flip_display();
}


ItemInfo CView::itemFromClick(Point point)
{
    ItemInfo retVal;
    list<GraphicItem *>::iterator it;
    for(it = gList.begin(); it != gList.end(); it++)
    {
        if((*it)->isPointIn(point))
        {
            retVal = (*it)->IAm();
            break;
        }
    }
    return retVal;
}

void CView::toggleConnectButton()
{
    list<GraphicItem *>::iterator it = gList.begin();
    advance(it,N_BOXES + 1);
    GraphicButton * button;
    button = dynamic_cast<GraphicButton *> (*it);
    if(button != nullptr)
            button->toggleMute();
}
