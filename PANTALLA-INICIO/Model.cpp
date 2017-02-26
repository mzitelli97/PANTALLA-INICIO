/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Model.cpp
 * Author: javier
 * 
 * Created on January 6, 2017, 12:16 PM
 */

#include "Model.h"

Model::Model() {
}

Model::Model(const Model& orig) {
}

Model::~Model() {
}

void Model::attach(Observer * view)
{
    observers.push_back(view);
}

void Model::notifyAllObservers() 
{
    std::list<Observer *>::iterator it;
    for(it = observers.begin(); it != observers.end(); it++)
        (*it)->update();
}