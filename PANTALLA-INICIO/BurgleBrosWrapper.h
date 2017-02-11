/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BurgleBrosWrapper.h
 * Author: carlos
 *
 * Created on 10 de febrero de 2017, 18:59
 */

#ifndef BURGLEBROSWRAPPER_H
#define BURGLEBROSWRAPPER_H

#include "GUI.h"
#include "BurgleBrosController.h"
#include "cController.h"



class BurgleBrosWrapper {
public:
    BurgleBrosWrapper();
    BurgleBrosWrapper(const BurgleBrosWrapper& orig);
    void getNameAndIp();
    void connect();
    void playGame();
    bool gameOnCourse();
    
    virtual ~BurgleBrosWrapper();
private:
    bool connect(BurgleBrosController * controller);
    bool quit;
    GUI gui;
    Model *p2Model;
    Observer *p2View;
    Controller *p2Controller;
    NetworkInterface networkInterface;
    BurgleBrosSound sound;
    string name, ipToConnect, ipToListen;
};

#endif /* BURGLEBROSWRAPPER_H */

