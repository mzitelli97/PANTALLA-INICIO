/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TextBox.cpp
 * Author: diego
 * 
 * Created on February 5, 2017, 10:23 AM
 */

#include "TextBox.h"
#define BACK_COLOR al_map_rgb(255,255,255)
#define TEXT_COLOR al_map_rgb(0,0,0)
//#define CLICK_COLOR al_map_rgb

TextBox::TextBox(double width, double height, ALLEGRO_FONT * font, textSelected id)
{
    selected=false;
    text="";
    this->width = width;
    this->height = height;
    this->font = font;
    identifier = id;
}

TextBox::TextBox(const TextBox& orig) {
}

TextBox::~TextBox() {
}

ItemInfo TextBox::IAm() {
    ItemInfo info={TEXT_BOX,&identifier};
    return info;
}

void TextBox::select() {
    selected=true;
}

void TextBox::unselect() {
    selected=false;
}

void TextBox::setText(std::string text) {
    this->text=text;
}

void TextBox::draw()
{
    al_draw_filled_rectangle(min.x,min.y,min.x+width,min.y+height,BACK_COLOR);
    if(font != nullptr)
        al_draw_text(font,TEXT_COLOR,min.x, (min.y+max.y)/2.0-font->height/2.0, ALLEGRO_ALIGN_LEFT, text.c_str());
    if(selected)
        al_draw_text(font,TEXT_COLOR,min.x+al_get_text_width(font,text.c_str()),(min.y+max.y)/2.0-font->height/2.0,0,"|");
}

void TextBox::setPosition(double x, double y)
{
    this->min = {x,y};
    max.x = min.x + width;
    max.y = min.y + height;
}


