/*
 * Created on February 19, 2017, 5:59 PM
 */

#include "GraphicHelp.h"
#include "GUI.h"
#include "ImageLoader.h"

GraphicHelp::GraphicHelp() {
    image=nullptr;
    
}

GraphicHelp::GraphicHelp(vector<ALLEGRO_BITMAP *> helpImages) {
    allImgs=helpImages;
    
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
    //Se supone que todas las imgs tienen el mismo height
    int sy= currentScroll * (int)((double)(RULES_NMBR_OF_PAGES * al_get_bitmap_height(allImgs[0])- height) / ((double)MOUSE_Z_RESOL_REQUIRED)) ;
    int currPhoto = sy/al_get_bitmap_height(allImgs[0]);
    int currDisplacement=sy%al_get_bitmap_height(allImgs[0]);
    if(al_get_bitmap_width(allImgs[currPhoto])<totalWidth)
    {
        al_draw_filled_rectangle(0, 0, totalWidth, totalHeight,al_map_rgba(0,0,0,180));
        if((al_get_bitmap_height(allImgs[currPhoto]) - currDisplacement) > height)
            al_draw_scaled_bitmap(allImgs[currPhoto],0,currDisplacement,al_get_bitmap_width(allImgs[currPhoto]),height,(totalWidth-al_get_bitmap_width(allImgs[0]))/2.0,0,al_get_bitmap_width(allImgs[0]),height,0);           
        else if((al_get_bitmap_height(allImgs[currPhoto]) - currDisplacement) < height && currPhoto!= RULES_NMBR_OF_PAGES-1)
        {
            al_draw_scaled_bitmap(allImgs[currPhoto],0,currDisplacement,al_get_bitmap_width(allImgs[currPhoto]),(al_get_bitmap_height(allImgs[currPhoto]) - currDisplacement),(totalWidth-al_get_bitmap_width(allImgs[0]))/2.0,0,al_get_bitmap_width(allImgs[currPhoto]),(al_get_bitmap_height(allImgs[currPhoto]) - currDisplacement),0);  
            if(currPhoto!= RULES_NMBR_OF_PAGES-1)
                al_draw_scaled_bitmap(allImgs[currPhoto+1],0,0,al_get_bitmap_width(allImgs[currPhoto]),height -(al_get_bitmap_height(allImgs[currPhoto]) - currDisplacement) ,(totalWidth-al_get_bitmap_width(allImgs[0]))/2.0,(al_get_bitmap_height(allImgs[currPhoto]) - currDisplacement),al_get_bitmap_width(allImgs[currPhoto]),height -(al_get_bitmap_height(allImgs[currPhoto]) - currDisplacement),0);
        }
    }
    else
    {
        al_draw_filled_rectangle(0, 0, totalWidth, totalHeight,al_map_rgba(0,0,0,180));
        if((al_get_bitmap_height(allImgs[currPhoto]) - currDisplacement) > height)
            al_draw_scaled_bitmap(allImgs[currPhoto],0,currDisplacement,al_get_bitmap_width(allImgs[currPhoto]),height,0,0,totalWidth,height,0);           
        else if((al_get_bitmap_height(allImgs[currPhoto]) - currDisplacement) <= height )
        {
            al_draw_scaled_bitmap(allImgs[currPhoto],0,currDisplacement,al_get_bitmap_width(allImgs[currPhoto]),(al_get_bitmap_height(allImgs[currPhoto]) - currDisplacement),0,0,totalWidth,(al_get_bitmap_height(allImgs[currPhoto]) - currDisplacement),0);  
            if(currPhoto!= RULES_NMBR_OF_PAGES-1)
                al_draw_scaled_bitmap(allImgs[currPhoto+1],0,0,al_get_bitmap_width(allImgs[currPhoto]),height -(al_get_bitmap_height(allImgs[currPhoto]) - currDisplacement) ,0,(al_get_bitmap_height(allImgs[currPhoto]) - currDisplacement),totalWidth,height -(al_get_bitmap_height(allImgs[currPhoto]) - currDisplacement),0);
        }
    }
}

GraphicHelp::GraphicHelp(const GraphicHelp& orig) {
}

GraphicHelp::~GraphicHelp() {
}

