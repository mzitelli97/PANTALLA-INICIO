/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GraphicHelp.h
 * Author: javier
 *
 * Created on February 19, 2017, 5:58 PM
 */

#ifndef GRAPHICHELP_H
#define GRAPHICHELP_H

#include "GraphicItem.h"

class GraphicHelp: public GraphicItem {
public:
    GraphicHelp();
    GraphicHelp(vector<ALLEGRO_BITMAP *> d);
    void init(); 
    void setScroll(unsigned int scroll);
    void resetScroll();
    GraphicHelp(const GraphicHelp& orig);
    virtual ItemInfo IAm();
    virtual void draw();
    virtual ~GraphicHelp();
private:
    unsigned int currentScroll;
    vector<ALLEGRO_BITMAP *> allImgs;
};

#endif /* GRAPHICHELP_H */

