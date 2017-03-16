#include "cController.h"
#include "MouseED.h"
#include "KeyboardED.h"
#include "BurgleBrosSound.h"

cController::cController()
{
    view = nullptr;
    model = nullptr;
    connecting = false;
    quit = false;
}

cController::cController(const cController& orig) {
}

cController::~cController() {
}

bool cController::checkIfConnecting()
{
    return connecting;
}

void cController::attachSound(BurgleBrosSound* sound) {
    if(sound !=nullptr)
        this->sound = sound;
}

void cController::parseTimerEvent(EventData* evData) {
//No utilizamos esta función 
}

void cController::parseMouseEvent(EventData* mouseEvent) {
    
    MouseED *p2MouseData = dynamic_cast<MouseED *> (mouseEvent);
    if( p2MouseData != nullptr && p2MouseData->isClicked()) //Si fue un click
    {
        ItemInfo temp;
        Point aux = {(double)p2MouseData->getX(),(double)p2MouseData->getY()};
        temp=view->itemFromClick(aux);
        textSelected *auxTextInfo;
        switch(temp.type)
        {
            case TEXT_BOX:
                auxTextInfo = (textSelected*)temp.info;
                if(!connecting) model->selectText(*auxTextInfo);
                break;
            case CONNECT_BUTTON_CLICK:
                if(!connecting && !model->isAnEntryEmpty())  //si se esta conectando no tomo el click en el boton connect
                {
                    connecting = true;
                    view->toggleButton(CONNECT_BUTTON);
                    view->update();
                }
                break;
            case VOL_BUTTON_CLICK:
                view->toggleButton(MUTE_BUTTON);
                sound->toggleMute();
                view->update();
                break;
            case HELP_BUTTON_CLICK:
                break;
            case EXIT_BUTTON_CLICK:
                quit = true;
                break;
            default:
                break;
        }
    } 
}

void cController::parseKeyboardEvent(EventData* evData)
{
    KeyboardED *p2KeyData = dynamic_cast<KeyboardED *>(evData);
    if(p2KeyData != nullptr)
    {
        if(!p2KeyData->isASpecialKey())
        {   if(!connecting) model->write(p2KeyData->getKey());}
        else 
        {
            SpecialKey key = p2KeyData->getSpecialKey();
            switch(key)
            {
                case BACKSPACE_KEY:
                    if(!connecting) model->deleteOneChar();
                    break;
                case UP_KEY:
                    if(!connecting) model->selectText((textSelected)((int)(model->getInfo().selected)-1));
                    break;
                case DOWN_KEY:
                    if(!connecting) model->selectText((textSelected)((int)(model->getInfo().selected)+1));
                    break;
                case ENTER_KEY:
                    if(!connecting && !model->isAnEntryEmpty())  //si se esta conectando no tomo el click en el boton connect
                    {
                        connecting = true;
                        view->toggleButton(CONNECT_BUTTON);
                        view->update();
                    }
                break;
                case ESCAPE_KEY:
                    quit = true;
                    break;
                default:
                    break;
            }
        }
    }else
    { 
       // no es un eventData
    }
}


void cController::parseNetworkEvent(EventData* evData) {
    //THIS CONTROLLER DOES NOT USE NETOWRK EVENTS
}


void cController::attachModel(CModel* model)
{
    if(model !=nullptr)
        this->model = model;
}

void cController::attachView(CView* view)
{   
    if(view != nullptr)
        this->view=view;
}

bool cController::userQuit()
{
    return quit;
}
