/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BurgleBrosWrapper.cpp
 * Author: carlos
 * 
 * Created on 10 de febrero de 2017, 18:59
 */

#include "BurgleBrosWrapper.h"

BurgleBrosWrapper::BurgleBrosWrapper() {
    srand(time(NULL));
    //allegro_startup();
    quit=false;
    CModel *initModel = new CModel;
    CView *initView = new CView(initModel);
    cController *initController = new cController;
    initModel->attach(initView);
    initController->attachView(initView);
    initController->attachModel(initModel);
    initController->attachSound(&sound);
    initView->update();
    gui.attachController(initController);
    p2Controller=initController;
    p2Model=initModel;
    p2View=initView;
}

bool BurgleBrosWrapper::gameOnCourse() {
    return !quit;
}

void BurgleBrosWrapper::getNameAndIp() {
    cController *initController = dynamic_cast<cController *>(p2Controller);
    CModel *initModel = dynamic_cast<CModel *>(p2Model);
    if(initController != nullptr && initModel != nullptr)
    {
        while(!initController->checkIfConnecting() && !quit)
        {
            quit = initController->userQuit();
            if(gui.hayEvento())
                gui.parseEvento();
        }
        name=initModel->getInfo().entries[MY_NAME];
        ipToConnect=initModel->getInfo().entries[OTHER_IP];
        ipToListen=initModel->getInfo().entries[MY_IP];
    }
    else
        quit=true;
       
}

void BurgleBrosWrapper::connect() 
{
    cController *initController = dynamic_cast<cController *>(p2Controller);
    BurgleBrosController *bbcontroller = new BurgleBrosController;
    bbcontroller->attachNetworkInterface(&networkInterface);
    if(initController != nullptr)
    {
        while(!connect(bbcontroller) && !quit)
        {
            quit = initController->userQuit();
            if(gui.hayEvento())
                gui.parseEvento();
        }
        if(!quit)
        {
            delete p2Controller;
            delete p2Model;
            delete p2View;
            gui.attachNetworkInterface(&networkInterface);
            BurgleBrosModel *newModel = new BurgleBrosModel;
            BurgleBrosView *newView= new BurgleBrosView(newModel);
            newModel->attach(newView);
            newModel->attach(&sound);
            newModel->attachController(bbcontroller);
            sound.attachModel(newModel);
            bbcontroller->attachSound(&sound);
            bbcontroller->attachModel(newModel);
            bbcontroller->attachView(newView);
            gui.attachController(bbcontroller);
            p2Controller = bbcontroller;
            p2Model = newModel;
            p2View = newView;
        }
    }    
}

void BurgleBrosWrapper::playGame() {
    gui.playTimer();
    BurgleBrosController *controller = dynamic_cast<BurgleBrosController *>(p2Controller);
    bool prev=false;
    if( controller != nullptr)
    {
        while(!controller->checkIfGameFinished())
        {
            prev=controller->isWaiting4ack();
            if(gui.hayEvento())
                gui.parseEvento();
            if(controller->isWaiting4ack() && !prev)
                gui.playTimer();
            else if (!controller->isWaiting4ack())
                gui.resetTimer();
        }
    }
    delete p2Controller;
    delete p2Model;
    delete p2View;
    
    
}


bool BurgleBrosWrapper::connect(BurgleBrosController* controller) {
    bool retVal = false;
    if(networkInterface.standardConnectionStart(ipToConnect,ipToListen))
    {
        controller->setCommunicationRoleNThisPlayerName(networkInterface.getCommunicationRole(), name);
        retVal = true;
    }
    if(networkInterface.checkError())    //Si hubo un error tratando de hacer la connection start:
    {
        cout<<networkInterface.getErrorMsg();
        quit=true;
    }
    return retVal;
}



BurgleBrosWrapper::BurgleBrosWrapper(const BurgleBrosWrapper& orig) {
}

BurgleBrosWrapper::~BurgleBrosWrapper() {
}

