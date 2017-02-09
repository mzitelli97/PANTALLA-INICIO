/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   cController.h
 * Author: diego
 *
 * Created on February 5, 2017, 9:51 AM
 */

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
    void attachSound(BurgleBrosSound *sound);
    void parseMouseEvent(EventData *mouseEvent);
    void parseNetworkEvent(EventData *evData);
    void parseKeyboardEvent(EventData *evData);
    virtual void parseTimerEvent(EventData *evData);
    string askForSpentOK(vector<string>& message) override;

    bool checkIfConnecting();
    void attachView(CView* view);
    void attachModel(CModel* model);
    bool userQuit();
    cController(const cController& orig);
    virtual ~cController();
private:
    bool connecting;
    CView* view;
    CModel* model;
    BurgleBrosSound *sound;
    bool quit;

};

#endif /* CCONTROLLER_H */

