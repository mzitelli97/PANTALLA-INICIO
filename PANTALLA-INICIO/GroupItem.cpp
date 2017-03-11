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
    bool item_flag = false;
    
    for(auto& item : graphicItem)
    {
        if(item->isZoomed() || !onZoom)              //si el item esta zoomeado o no hay nada en zoom
            if(item->isPointIn(point))
            {
                retVal=item->IAm();
                break;                              //si lo encuentro, salgo
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

    for(auto& item : graphicItem)
    {    
        if(item->isZoomed() || !onZoom)
            item->draw();
    }
}

void GroupItem::erase() {

    for(auto& item : graphicItem)
        delete item;
    
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


