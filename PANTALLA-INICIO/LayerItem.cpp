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
    list<GroupItem>::iterator it_groups;
    
    for(it_groups=Groups.begin(); it_groups != Groups.end() && retVal.type == NO_ITEM_CLICK; it_groups++)
        retVal=it_groups->itemFromClick(point);
        
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
    
    list<GroupItem>::iterator it_groups;
    for(it_groups=Groups.begin(); it_groups!=Groups.end(); it_groups++)
        it_groups->setZoom();
    
}

void
LayerItem::resetZoom() {
   
    list<GroupItem>::iterator it_groups;
    for(it_groups=Groups.begin(); it_groups!=Groups.end(); it_groups++)
        it_groups->resetZoom();
}

void
LayerItem::erase() {

    list<GroupItem>::iterator it_groups;
    
    for(it_groups=Groups.begin();it_groups!= Groups.end();it_groups++)
        it_groups->erase();
    
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
    
    list<GroupItem>::iterator it_groups;
    for(it_groups=Groups.begin(); it_groups!=Groups.end(); it_groups++)
    {
        it_groups->draw();
  //      std::cout<<"entre a grupo \n";
    }
}

LayerItem::~LayerItem() {
    
}


