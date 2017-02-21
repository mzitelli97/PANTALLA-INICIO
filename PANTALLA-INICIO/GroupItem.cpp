#include "GroupItem.h"


GroupItem::GroupItem() {
    onZoom=false;
}

void 
GroupItem::atachGraphicsItem(GraphicItem* graphicItem){
     this->graphicItem.push_back(graphicItem);
}

ItemInfo
GroupItem::itemFromClick(Point point) {

    ItemInfo retVal = {NO_ITEM_CLICK, nullptr};
    list<GraphicItem *>::iterator it_items;
    bool item_flag = false;
    
    
    for(it_items=graphicItem.begin(); it_items != graphicItem.end() && !item_flag; it_items++)
    {
       // std::cout<<"itemFromClick \n";
        if((*it_items)->isZoomed() || !onZoom)
            if(item_flag=(*it_items)->isPointIn(point))
            {
                retVal=(*it_items)->IAm();
                item_flag=true;
            }
    }    
    return retVal;    
                
}

list<GraphicItem*>::iterator 
GroupItem::accessGraphicItems() {
    
    return graphicItem.begin();
    
}

void 
GroupItem::draw() {

    list<GraphicItem *>::iterator it_items;
        
    for(it_items=graphicItem.begin(); it_items != graphicItem.end(); it_items++)
    {    
        if((*it_items)->isZoomed() || !onZoom)
               (*it_items)->draw();
        //std::cout<<"entre a item \n";
    }
}

void GroupItem::erase() {

    list<GraphicItem *>::iterator it_items;
    for(it_items=graphicItem.begin(); it_items != graphicItem.end() ; it_items++)
        delete *it_items;
    
    graphicItem.clear();   
        
}

void GroupItem::setZoom() {
    onZoom^=true;
}

void GroupItem::resetZoom() {
    onZoom=false;
}

GroupItem::~GroupItem() {
}


