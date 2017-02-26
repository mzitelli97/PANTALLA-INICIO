/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CView.h
 * Author: diego
 *
 * Created on February 5, 2017, 10:17 AM
 */

#ifndef CVIEW_H
#define CVIEW_H

#include "Observer.h"
#include "CModel.h"
#include "GraphicItem.h"
#include "ImageLoader.h"


class CView: public Observer {
public:
    CView(CModel * model);
    CView(const CView& orig);
    ~CView();
    void update() override;
    ItemInfo itemFromClick(Point point);
    void toggleButton(buttonAction button);
    
private:
    CModel * model;
    std:: list<GraphicItem*> gList;
    ImageLoader imageLoader;
    ALLEGRO_BITMAP * backScreen;
    ALLEGRO_DISPLAY * display;
    ALLEGRO_FONT * font;
    bool error;

};

#endif /* CVIEW_H */

