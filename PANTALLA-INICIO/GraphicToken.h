#ifndef GRAPHICTOKEN_H
#define GRAPHICTOKEN_H

#include "GraphicItem.h"

class GraphicToken : public GraphicItem{
public:
    GraphicToken(ALLEGRO_BITMAP * image);
    GraphicToken(const GraphicToken& orig);
    virtual ~GraphicToken();
    ItemInfo IAm() override;
    void setPosition(CardLocation location, unsigned int number);

private:
    GraphicToken();
};

#endif /* GRAPHICTOKEN_H */

