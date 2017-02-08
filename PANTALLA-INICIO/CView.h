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

#include "View.h"
#include "GraphicItem.h"
#include "ImageLoader.h"


class CView: public View {
public:
    CView();
    CView(const CView& orig);
    virtual ~CView();
    void update(Model* model) override;
    ItemInfo itemFromClick(Point point);
private:
    std:: list<GraphicItem*> gList;
    ImageLoader imageLoader;
    ALLEGRO_BITMAP * backScreen;
    ALLEGRO_DISPLAY * display;
    ALLEGRO_FONT * font;

};

#endif /* CVIEW_H */

