#ifndef CCONTROLLER_H
#define CCONTROLLER_H

#include "Controller.h"
#include "EventData.h"
#include "CView.h"
#include "CModel.h"
#include "BurgleBrosSound.h"

class cController:public Controller {
public:
    cController();

    void parseMouseEvent(EventData *mouseEvent);
    void parseKeyboardEvent(EventData *evData);
    bool checkIfConnecting();
    void attachSound(BurgleBrosSound *sound);
    void attachView(CView* view);
    void attachModel(CModel* model);
    bool userQuit();
    cController(const cController& orig);
    virtual ~cController();
    
    virtual void parseTimerEvent(EventData *evData);        //No utilizamos las siguientes funciones en este controller
    virtual void parseNetworkEvent(EventData *evData);      //

private:
    bool connecting;
    CView* view;
    CModel* model;
    BurgleBrosSound *sound;
    bool quit;

};

#endif /* CCONTROLLER_H */

