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
#include "LibsInit.h"
#include "GUI.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    srand(time(NULL));
    allegro_startup();
    BurgleBrosModel model;
    BurgleBrosView view;
    BurgleBrosController controller;
    GUI gui;
    view.ViewInit(&model);
    view.update(&model);
    
    model.attachView(&view);
    controller.attachModel(&model);
    controller.attachView(&view);
    gui.atachController(&controller);
    
    while(true)
    {
        if(gui.hayEvento())
            gui.parseEvento();
    }
    
    /*view.update(&model);
    al_rest(5);
    model.move(THIS_PLAYER_ACTION, {0,1,0});
    view.update(&model);
    al_rest(5);*/
    return 0;
}

