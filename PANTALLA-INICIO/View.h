/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   View.h
 * Author: javier
 *
 * Created on December 24, 2016, 5:09 PM
 */

#ifndef VIEW_H
#define VIEW_H

class View {
public:
    View();
    View(const View& orig);
    virtual ~View();
    virtual void update() = 0;
private:

};

#endif /* VIEW_H */

