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
#include "BurgleBrosSound.h"
#include "BurgleBrosModel.h"
#include "BurgleBrosController.h"
#include "BurgleBrosView.h"
#include "LibsInit.h"
#include "GUI.h"
#include "CView.h"
#include "cController.h"
using namespace std;

/*Estas funciones luego pertenecerían a un wrapper que las une para simplificar el main.*/
bool connect(bool *quit, NetworkInterface * networkInterface, BurgleBrosController * Controller, string &ipToConnect, string &ipToListen, string &userName );
bool gameStillPlaying(BurgleBrosController &controller);
/*
 * 
 */
int main(int argc, char** argv) {

    srand(time(NULL));
    allegro_startup();
    bool quit=false;
    
    GUI gui;
    BurgleBrosSound sound;
    CModel initModel;
    CView initView(&initModel);
    cController initController;
    NetworkInterface networkInterface;
    
    
    initModel.attach(&initView);
    initController.attachView(&initView);
    initController.attachModel(&initModel);
    initController.attachSound(&sound);
    initView.update();
    string name;
    string ipToConnect;
    string ipToListen;
   
    gui.attachController(&initController);
    while(!initController.checkIfConnecting() && !quit)
    {
        quit = initController.userQuit();
        if(gui.hayEvento())
            gui.parseEvento();
    }
    if(!quit)  //SI no fue por un quit
    {
        name=initModel.getInfo().entries[MY_NAME];
        ipToConnect=initModel.getInfo().entries[OTHER_IP];
        ipToListen=initModel.getInfo().entries[MY_IP];
        
        BurgleBrosController controller;
    
        controller.attachNetworkInterface(&networkInterface);
        while(!connect(&quit,&networkInterface,&controller, ipToConnect, ipToListen, name) && !quit)
        {
            quit = initController.userQuit();
            if(gui.hayEvento())
                gui.parseEvento();
        }

        gui.attachNetworkInterface(&networkInterface);
        if(!quit)
        {
            BurgleBrosModel model;
            BurgleBrosView view(&model);
            model.attach(&view);
            model.attachController(&controller);
            sound.attachModel(&model);
            controller.attachSound(&sound);
            controller.attachModel(&model);
            controller.attachView(&view);
            gui.attachController(&controller);
            gui.playTimer();
            bool prev=false;
            while(gameStillPlaying(controller))
            {
                prev=controller.isWaiting4ack();
                if(gui.hayEvento())
                    gui.parseEvento();
                if(controller.isWaiting4ack() && !prev)
                    gui.playTimer();
                else if (!controller.isWaiting4ack())
                    gui.resetTimer();
            }
        }
    }
    
    return 0;
}

bool connect(bool *quit, NetworkInterface * networkInterface, BurgleBrosController * Controller, string &ipToConnect, string &ipToListen, string &userName )
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

