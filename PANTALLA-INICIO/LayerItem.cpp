#include "LayerItem.h"


LayerItem::LayerItem() {
    
}

void 
LayerItem::atachGroupItem(GroupItem * group) {

   this->Groups.push_back(*group);
}

ItemInfo 
LayerItem::itemFromClick(Point point) {
    
    ItemInfo retVal = {NO_ITEM_CLICK, nullptr};
    
    for(auto& group : Groups)
    {
        retVal=group.itemFromClick(point);
        if(retVal.type != NO_ITEM_CLICK) break; //si lo encuentro, paro
    }
    return retVal;
}

list<GraphicItem*>::iterator 
LayerItem::accessGraphicItems(unsigned int itemType) {

    list<GroupItem>::iterator it_group;
    it_group=Groups.begin();
    advance(it_group,itemType);
    return it_group->accessGraphicItems();
    
}


void 
LayerItem::setZoom() {
    
    for(auto& item : Groups)
        item.setZoom();
    
}

void
LayerItem::resetZoom() {
   
    for(auto& item : Groups)
        item.resetZoom();
}

void
LayerItem::erase() {

    for(auto& item : Groups)
        item.erase();
    
    Groups.clear();
    
}

list<GroupItem>::iterator 
LayerItem::erese(unsigned int itemList) {

    list<GroupItem>::iterator it_groups;
    it_groups=Groups.begin();
    advance(it_groups,itemList);
    it_groups->erase();
    
    return it_groups;
    
}

void LayerItem::draw() {
    
    for(auto& item : Groups)
    {
        item.draw();
    }
}

LayerItem::~LayerItem() {
    
}


