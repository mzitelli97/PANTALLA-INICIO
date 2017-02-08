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
#include "BurgleBrosView.h"
#include "BurgleBrosSound.h"
#include "LibsInit.h"
#include "GUI.h"
#include "CView.h"
#include "cController.h"
using namespace std;

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
    initModel.attachView(&initView);
    initController.attachView(&initView);
    initController.attachModel(&initModel);
    initView.update(&initModel);
    while(1);
    
    string name=argv[1];
    string ipToConnect=argv[2];
    string ipToListen=argv[3];
    
    
    BurgleBrosModel model;
    BurgleBrosView view;
    BurgleBrosController controller;
    model.attachView(&view);
    model.attachController(&controller);
    model.attachSoundManager(&sound);
    controller.attachModel(&model);
    controller.attachView(&view);
    gui.atachController(&controller);
    gui.getNameAndIp(name, ipToConnect, ipToListen);
    while(!gui.connect() || !gui.userQuit())
    {
        //if(gui.hayEvento())
          //  gui.parseEvento();
    }
    if(!gui.userQuit())
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
        while(gui.gameStillPlaying())
        {
            if(gui.hayEvento())
                gui.parseEvento();
        }
    }
    return 0;
}

