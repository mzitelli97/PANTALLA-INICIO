/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   NetworkingEG.h
 * Author: user
 *
 * Created on 20 de febrero de 2017, 11:48
 */

#ifndef NETWORKINGEG_H
#define NETWORKINGEG_H

#include "EventGenerator.h"
#include "NetworkED.h"


class NetworkingEG: public EventGenerator {
public:
    NetworkingEG();
    void attachNetworkingInterface(NetworkInterface * networkingInterface);
    bool hayEvento() override;
    virtual ~NetworkingEG();
private:
    NetworkInterface * networkingInterface;
    bool NetworkEV;
    bool error;
};

#endif /* NETWORKINGEG_H */

