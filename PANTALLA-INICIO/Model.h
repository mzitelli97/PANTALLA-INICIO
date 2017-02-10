/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Model.h
 * Author: javier
 *
 * Created on January 6, 2017, 12:16 PM
 */

#ifndef MODEL_H
#define MODEL_H

#include "View.h"
#include <list>

class Model {
public:
    Model();
    Model(const Model& orig);
    virtual ~Model();
    void attach(Observer * view);
    void deAttach(Observer * view);
protected:
    void notifyAllObservers();
private:
    std::list<Observer *> observers;

};

#endif /* MODEL_H */

