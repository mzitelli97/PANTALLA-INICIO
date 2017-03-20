#ifndef GROUPITEM_H
#define GROUPITEM_H

#include "GraphicItem.h"
#include <list>

class GroupItem {
public:
    
    GroupItem();
    
    void draw();
    /*borra el contenido de las lista y libera la memoria del puntero*/
    void erase();
    void atachGraphicsItem(GraphicItem * graphicItem);
    list<GraphicItem *>::iterator accessGraphicItems();
    ItemInfo itemFromClick(Point point);
    /*toggle de onZoom*/
    void setZoom();
    /*coloca onZoom en false*/
    void resetZoom();
    virtual ~GroupItem();

private:

    bool onZoom;
    list<GraphicItem *> graphicItem;
   
};

#endif /* GROUPITEM_H */


