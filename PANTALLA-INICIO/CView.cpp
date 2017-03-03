#include "CView.h"
#include "TextBox.h"
#include "GraphicButton.h"
#include "CModel.h"

#define N_BOXES 2
#define SCREEN_W 400
#define SCREEN_H SCREEN_W*(3/2.0)
#define BOX_WIDTH al_get_display_width(display)/1.15
#define BOX_HEIGHT al_get_display_height(display)/17.0

#define BOX_MIN_X (al_get_display_width(display)-BOX_WIDTH)/2.0
#define BOX_MIN_Y al_get_display_height(display)/5.0
#define SPACE_X al_get_display_width(display)/50.0
#define SPACE_Y al_get_display_height(display)/5.7

#define TEXT_COLOR al_map_rgb(0,0,0)

#define PATH_BACKSCREEN     "Images/Begin/begin.jpg"
#define PATH_TITLEFONT      "Font/font2.ttf"
#define PATH_FONT           "Font/font6.ttf"  


CView::CView(CModel * model)
{
    error=false;
    if(model != nullptr)                       
    {    
        this->model = model;               //attach model
        backScreen=nullptr;                 
        display=nullptr;
        font=nullptr;

        if(imageLoader.initImages())
        {
            display =al_create_display(SCREEN_W,SCREEN_H);           
            if(display != nullptr)
            {
                backScreen = al_load_bitmap(PATH_BACKSCREEN);
                if(backScreen != nullptr)
                {
                    al_draw_scaled_bitmap(backScreen,0,0,al_get_bitmap_width(backScreen),al_get_bitmap_height(backScreen),0,0,al_get_display_width(display),al_get_display_height(display),0);
                    al_set_window_title(display,"EDA Burgle Bros");
                    
                    ALLEGRO_FONT * auxFont =nullptr;
                    auxFont = al_load_font(PATH_TITLEFONT,BOX_HEIGHT,0);

                    if(auxFont != nullptr)
                    {
                        al_draw_text(auxFont,TEXT_COLOR,BOX_MIN_X +BOX_WIDTH/2,BOX_MIN_Y/1.1-BOX_HEIGHT,ALLEGRO_ALIGN_CENTER,"FRIEND'S IP:");
                        al_draw_text(auxFont,TEXT_COLOR,BOX_MIN_X +BOX_WIDTH/2,BOX_MIN_Y/1.1 + SPACE_Y+BOX_HEIGHT-BOX_HEIGHT,ALLEGRO_ALIGN_CENTER,"ENTER YOUR NAME:");
                        al_destroy_font(auxFont);
                        
                        font = al_load_font(PATH_FONT,BOX_HEIGHT/1.2, 0);
                        if(font != nullptr)
                        {
                            //Creo las cajas de texto
                            for(int i = 0; i < N_BOXES; i++)
                            {
                                TextBox * box = new TextBox(BOX_WIDTH,BOX_HEIGHT,font, (textSelected)i);
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
                        }else 
                        {   
                            //error al crear la fuente
                            error=true;
                            errorMsg="No se pudo crear la fuente\n";
                            al_destroy_bitmap(backScreen);
                            al_destroy_display(display);
                        }
                    }else
                    {   
                        //error al crear la fuente auxiliar
                        errorMsg="No se pudo crear la fuente auxiliar\n";
                        error=true;
                        al_destroy_bitmap(backScreen);
                        al_destroy_display(display);
                    }
                }else
                {
                    //Error al crear el backScreen
                    errorMsg="No se pudo crear backScreen\n";
                    error=true;
                    al_destroy_display(display);
                }
            }else
            {
                //error al crear el display
                errorMsg="No se pudo crear display\n";
                error=true;
            }
        }else
        {
            //error al inicializar las imagenes 
            errorMsg="No se pudo incializar imagenes\n"+imageLoader.getError();
            error=true;
        }
    }else
    {
        //Error al attachear el modelo, se encontraba vacio.
        errorMsg="Error al attachear modelo\n";
        error=true;
    }
    
}

CView::CView(const CView& orig) {
}

CView::~CView()
{
    if(error != true)   // si no hay error 
    {
        al_destroy_font(font);
        list<GraphicItem*>::iterator it;
        for(it = gList.begin(); it != gList.end(); it++)
            if(*it != nullptr) delete *it;
        al_destroy_bitmap(backScreen);
        al_destroy_display(display);
    }
}

string CView::geterrorMsg() {
    return errorMsg;
}

bool CView::DidAnErrorStarting() {
    return error;
}


void CView::update()
{
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

void CView::toggleButton(buttonAction button)
{
    list<GraphicItem *>::iterator it = gList.begin();
    advance(it,N_BOXES);                                //avanzo hasta el boton del volumen
    GraphicButton * auxButton;
    if(button == CONNECT_BUTTON) it++;                  //el boton de connect esta despues del volumen
    auxButton = dynamic_cast<GraphicButton *> (*it);
    if(auxButton != nullptr)
            auxButton->toggleMute();
}
