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

TextBox::TextBox(double width, double height, ALLEGRO_FONT * font)
{
    data.selected=false;
    data.text="127.0.0.1";
    this->width = width;
    this->height = height;
    this->font = font;
}

TextBox::TextBox(const TextBox& orig) {
}

TextBox::~TextBox() {
}

ItemInfo TextBox::IAm() {
    ItemInfo info={TEXT_BOX,&data};
    return info;
}

void TextBox::select() {
    data.selected=true;
}

void TextBox::unselect() {
    data.selected=false;
}

std::string TextBox::getText() {
    return data.text;
}

void TextBox::setText(std::string text) {//ver si hay que hacer alguna validacion de width y eso
    this->data.text=text;
}

void TextBox::draw()
{
    al_draw_filled_rectangle(min.x,min.y,min.x+width,min.y+height,BACK_COLOR);
    if(font != nullptr)
        al_draw_text(font,TEXT_COLOR,min.x, min.y, ALLEGRO_ALIGN_LEFT, data.text.c_str());
    if(data.selected)
        al_draw_text(font,TEXT_COLOR,min.x+al_get_text_width(font,data.text.c_str()),min.y,0,"|");
}

void TextBox::setPosition(double x, double y)
{
    this->min = {x,y};
}


