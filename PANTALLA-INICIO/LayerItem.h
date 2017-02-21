#ifndef LAYERITEM_H
#define LAYERITEM_H

#include <list>
#include "GroupItem.h"

class LayerItem {
public:
   
   LayerItem();
   void atachGroupItem(GroupItem * group);
   list<GraphicItem *>::iterator accessGraphicItems(unsigned int itemType);
   /*Toggle de onzoom*/
   void setZoom();
   /*Coloca el onZoom en false*/
   void resetZoom();
   /*Borra todo el contenido de un group determinado y devuelve el iterador al comienzo de la lista */
   list<GroupItem>:: iterator erese(unsigned int itemList);
   /*Se encarga de borra toda la lista*/
   void erase();
   /*Busca en sus lista el item que contenga esas coordenadas y devuelve su informacion en un ItemInfo*/
   ItemInfo itemFromClick(Point point);
   void draw();
   virtual ~LayerItem();

private:

    list<GroupItem> Groups;
    
};

#endif /* LAYERITEM_H */
