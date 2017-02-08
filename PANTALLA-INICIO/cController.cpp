#include "cController.h"
#include "MouseED.h"
#include "KeyboardED.h"

cController::cController()
{
    view=nullptr;
}

cController::cController(const cController& orig) {
}

cController::~cController() {
}

bool cController::checkIfConnecting()
{
    return connecting;
}

void cController::parseMouseEvent(EventData* mouseEvent) {
    MouseED *p2MouseData = dynamic_cast<MouseED *> (mouseEvent);
    if( p2MouseData != nullptr)
    {
        ItemInfo temp;
        Point aux = {(double)p2MouseData->getX(),(double)p2MouseData->getY()};
        temp=view->itemFromClick(aux);
        textSelected *auxTextInfo;
        switch(temp.type)
        {
            case TEXT_BOX:
                auxTextInfo = (textSelected*)temp.info;
                model->selectText(*auxTextInfo);
                break;
            case CONNECT_BUTTON_CLICK:
                if(!connecting && !model->isAnEntryEmpty())                 //si se esta conectando no tomo el click en el boton connect
                    connecting = true;
                break;
            case VOL_BUTTON_CLICK:
                /*modelPointer->toggleVol();
                view->toggleVolButton();
                view->update(modelPointer);*/
                break;
            case HELP_BUTTON_CLICK:
                break;
            case EXIT_BUTTON_CLICK:
                //if(view->yesNoMessageBox(exitMsg)==1)
                    quit = true;
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
            model->write(p2KeyData->getKey());
        else 
        {
            SpecialKey key = p2KeyData->getSpecialKey();
            switch(key)
            {
                case BACKSPACE_KEY:
                    model->deleteOneChar();
                    break;
                case UP_KEY:
                    model->selectText((textSelected)((int)(model->getInfo().selected)-1));
                    break;
                case DOWN_KEY:
                    model->selectText((textSelected)((int)(model->getInfo().selected)+1));
                    break;
                case ESCAPE_KEY:
                    quit = true;
                    break;
            }
        }
    }
}


void cController::parseNetworkEvent(EventData* evData) {

}

string cController::askForSpentOK(vector<string>& message) {

}

void cController::attachModel(CModel* model)
{
    this->model = model;
}

void cController::attachView(CView* view)
{
    this->view=view;
}

bool cController::userQuit()
{
    return quit;
}
