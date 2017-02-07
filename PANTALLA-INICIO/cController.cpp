#include "cController.h"
#include "MouseED.h"
#include "KeyboardED.h"

cController::cController() {
    view=nullptr;
}

cController::cController(const cController& orig) {
}

cController::~cController() {
}

void cController::parseMouseEvent(EventData* mouseEvent) {
    MouseED *p2MouseData = dynamic_cast<MouseED *> (mouseEvent);
    if( p2MouseData != nullptr)
    {
        ItemInfo temp;
        Point aux = {(double)p2MouseData->getX(),(double)p2MouseData->getY()};
        temp=view->itemFromClick(aux);

    }    
}

void cController::parseKeyboardEvent(EventData* evData)
{
    KeyboardED *p2KeyData = dynamic_cast<KeyboardED *>(evData);
    if(p2KeyData != nullptr)
    {
        
    }
}


void cController::parseNetworkEvent(EventData* evData) {

}

string cController::askForSpentOK(vector<string>& message) {

}



void cController::attachView(CView* view) {
    this->view=view;
}
