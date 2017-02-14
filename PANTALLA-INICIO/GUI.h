/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GUI.h
 * Author: user
 *
 * Created on 6 de enero de 2017, 10:50
 */

#ifndef GUI_H
#define GUI_H
#include <list>
#include <allegro5/events.h>
#include <allegro5/config.h>
#include "Controller.h"
#include "NetworkInterface.h"

#define TIMEOUT_TIME 60

typedef enum{GUI_EVENT_MOUSE, GUI_EVENT_KEYBOARD,GUI_EVENT_NETWORKING,GUI_EVENT_TIMER,GUI_EVENT_NOEVENT} GuiEvent;

using namespace std;

class GUI {
public:
    GUI();
    GUI(const GUI& orig);
    bool hayEvento(void);
    void parseEvento(void);
    void attachController (Controller * Controller);
    void attachNetworkInterface(NetworkInterface * networkingInterface);
    void enableTimer();
    void resetTimer();
    void stopTimer();
    void playTimer();
    virtual ~GUI();
    
private:
    Controller *controller;
    bool networkEventEnabled;
    bool error;
    GuiEvent event;
    ALLEGRO_TIMER * timer;
    EventData *eventData;
    ALLEGRO_EVENT_QUEUE *EventQueue;     
    NetworkInterface * networkingInterface;
    bool quit;
};

#endif /* GUI_H */

