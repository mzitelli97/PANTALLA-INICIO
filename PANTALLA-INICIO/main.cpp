/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: javier
 *
 * Created on December 29, 2016, 4:15 PM
 */

#include <cstdlib>
#include "BurgleBrosModel.h"
#include "BurgleBrosController.h"
#include "BurgleBrosView.h"
#include "BurgleBrosSound.h"
#include "LibsInit.h"
#include "GUI.h"
#include "CView.h"
#include "cController.h"
using namespace std;

/*Estas funciones luego pertenecerían a un wrapper que las une para simplificar el main.*/
bool connect(bool *quit, NetworkInterface * networkInterface, BurgleBrosController * Controller, string ipToConnect, string ipToListen, string userName );
bool gameStillPlaying(BurgleBrosController &controller);
/*
 * 
 */
int main(int argc, char** argv) {

    srand(time(NULL));
    allegro_startup();
    
    GUI gui;
    BurgleBrosSound sound;
    CModel initModel;
    CView initView;
    cController initController;
    NetworkInterface networkInterface;
    bool quit=false;
    
    initModel.attachView(&initView);
    initController.attachView(&initView);
    initController.attachModel(&initModel);
    initView.update(&initModel);
    string name;
    string ipToConnect;
    string ipToListen;
    
    while(1);
    /*
     while(!initModel.IpsAndNameGotten() || quit)
    {
        if(gui.hayEvento())
            gui.parseEvento();
    }
    if(!quit)  //SI no fue por un quit
    {
        name=initModel.getName();
        ipToConnect=initModel.getipToConnect();
        ipToListen=initModel.ipToListen();
    }
    */
    
    
    BurgleBrosModel model;
    BurgleBrosView view;
    BurgleBrosController controller;
    model.attachView(&view);
    model.attachController(&controller);
    model.attachSoundManager(&sound);
    controller.attachModel(&model);
    controller.attachView(&view);
    gui.attachController(&controller);
    while(!connect(&quit,&networkInterface,&controller, ipToConnect, ipToListen, name) || quit)
    {
        if(gui.hayEvento())
            gui.parseEvento();
    }
    gui.attachNetworkInterface(&networkInterface);
    if(!quit)
    {
        /*BurgleBrosModel model;
        BurgleBrosView view;
        BurgleBrosController controller;
        model.attachView(&view);
        model.attachController(&controller);
        model.attachSoundManager(&sound);
        controller.attachModel(&model);
        controller.attachView(&view);
        gui.atachController(&controller);*/
        while(gameStillPlaying(controller))
        {
            if(gui.hayEvento())
                gui.parseEvento();
        }
    }
    return 0;
}

bool connect(bool *quit, NetworkInterface * networkInterface, BurgleBrosController * Controller, string ipToConnect, string ipToListen, string userName )
{
    bool retVal = false;
    if(networkInterface->standardConnectionStart(ipToConnect,ipToListen))
    {
        Controller->setCommunicationRoleNThisPlayerName(networkInterface->getCommunicationRole(), userName);
        retVal = true;
    }
    if(networkInterface->checkError())    //Si hubo un error tratando de hacer la connection start:
    {
        cout<<networkInterface->getErrorMsg();
        *quit=true;
    }
    return retVal;
}

bool gameStillPlaying(BurgleBrosController &controller)
{
    return !(controller.checkIfGameFinished());
}

