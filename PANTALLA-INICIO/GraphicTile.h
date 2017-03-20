#ifndef GRAPHICTILE_H
#define GRAPHICTILE_H

#include "GraphicItem.h"
#include "BurgleBrosCard.h"

class GraphicTile : public GraphicItem{
public:
    GraphicTile(ALLEGRO_BITMAP * front,ALLEGRO_BITMAP * back,CardLocation location,unsigned int width,unsigned int height);
    bool isVisible();
    void setVisible(ALLEGRO_BITMAP * safeNumber);
    CardLocation getLocation();
    virtual ~GraphicTile();
    virtual ItemInfo IAm();
    virtual void draw();
    void toggleZoom();
private:
    void setPosition(CardLocation location);
    CardLocation location;
    bool Visible;
    ALLEGRO_BITMAP * back;
    ALLEGRO_BITMAP * safeNumber;
};

#endif /* GRAPHICTILE_H */

