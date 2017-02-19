/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GraphicHelp.cpp
 * Author: javier
 * 
 * Created on February 19, 2017, 5:59 PM
 */

#include "GraphicHelp.h"
#include "GUI.h"

GraphicHelp::GraphicHelp() {
    image=nullptr;
    
}

GraphicHelp::GraphicHelp(ALLEGRO_BITMAP *helpImage) {
    this->image=helpImage;
    
}

void GraphicHelp::setScroll(unsigned int scroll) {
    currentScroll=scroll;
}


void GraphicHelp::resetScroll() {
    setScroll(0);
}

void GraphicHelp::init() {
    min.x=0;
    min.y=0;
    max.y=totalHeight;
    max.x=totalWidth;
    center.x= totalWidth/2.0;
    center.y= totalHeight/2.0;
    width=totalWidth;
    height=totalHeight;
    currentScroll=0;
}

ItemInfo GraphicHelp::IAm() {
    return {HELP_IMAGE_CLICK, nullptr};
}

void GraphicHelp::draw() {
    
    float sy= (((float)(al_get_bitmap_height(image)- height)) / ((float)MOUSE_Z_RESOL_REQUIRED))*currentScroll ;
    if(image != nullptr)
    {
        if(al_get_bitmap_width(image)<totalWidth)
        {
            al_draw_filled_rectangle(0, 0, totalWidth, totalHeight,al_map_rgba(0,0,0,180));
            al_draw_scaled_bitmap(image,0,sy,al_get_bitmap_width(image),height,(totalWidth-al_get_bitmap_width(image))/2.0,0,al_get_bitmap_width(image),height,0);           
        }
        else
            al_draw_scaled_bitmap(image,0,sy,al_get_bitmap_width(image),height,0,0,totalWidth,height,0);           
    }
}

GraphicHelp::GraphicHelp(const GraphicHelp& orig) {
}

GraphicHelp::~GraphicHelp() {
}

