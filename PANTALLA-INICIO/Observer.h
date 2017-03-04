/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Observer.h
 * Author: javier
 *
 * Created on December 24, 2016, 5:09 PM
 */

#ifndef VIEW_H
#define VIEW_H

class Observer {
public:
    Observer() {};
    Observer(const Observer& orig) {};
    virtual void update() = 0;
    virtual ~Observer() {};
};

#endif /* VIEW_H */

