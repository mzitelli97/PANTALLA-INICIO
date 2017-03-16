#ifndef GRAPHICEDICES_H
#define GRAPHICEDICES_H

#include "GraphicItem.h"


class GraphicEDices : public GraphicItem{
public:
    GraphicEDices(ALLEGRO_BITMAP * image);
    GraphicEDices(const GraphicEDices& orig);
    virtual ~GraphicEDices();
    ItemInfo IAm() override;
    void setPosition(unsigned int number);
private:

};

#endif /* GRAPHICEDICES_H */

