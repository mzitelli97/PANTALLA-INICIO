#ifndef GRAPHICGUARD_H
#define GRAPHICGUARD_H

#include "GraphicItem.h"

class GraphicGuard : public GraphicItem{
public:
    GraphicGuard();
    GraphicGuard(ALLEGRO_BITMAP * image);
    GraphicGuard(const GraphicGuard& orig);
    virtual ~GraphicGuard();
    ItemInfo IAm() override;
    virtual void draw();
    void setInitialized(bool setInitialized);
    void setPosition(CardLocation location);
private:
    bool initialized;
};

#endif /* GRAPHICGUARD_H */

