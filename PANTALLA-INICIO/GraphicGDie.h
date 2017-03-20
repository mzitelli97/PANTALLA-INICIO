#ifndef GRAPHICGDIE_H
#define GRAPHICGDIE_H

#include "GraphicItem.h"


class GraphicGDie : public GraphicItem{
public:
    GraphicGDie();
    GraphicGDie(ALLEGRO_BITMAP * image);
    GraphicGDie(const GraphicGDie& orig);
    virtual ~GraphicGDie();
    virtual void draw();
    ItemInfo IAm() override;
    void setInitialized(bool state);
    void setPosition(CardLocation location);
    void setNumber(ALLEGRO_BITMAP * die_num);
private:
    bool initialized;
};

#endif /* GRAPHICGDIE_H */

