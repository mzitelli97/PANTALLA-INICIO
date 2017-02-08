/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TextBox.h
 * Author: diego
 *
 * Created on February 5, 2017, 10:23 AM
 */

#ifndef TEXTBOX_H
#define TEXTBOX_H
#include "GraphicItem.h"

typedef struct {
    bool selected;
    std:: string text;
}boxData;


class TextBox: public GraphicItem  {
public:
    TextBox(double width, double height,ALLEGRO_FONT * font);
    TextBox(const TextBox& orig);
    virtual ~TextBox();
    virtual ItemInfo IAm();
    virtual void draw();
    void setText(std::string text);
    std::string getText();
    void select();
    void unselect();
    void setPosition(double x, double y);
    
private:
    boxData data;
    ALLEGRO_FONT * font;

};

#endif /* TEXTBOX_H */

