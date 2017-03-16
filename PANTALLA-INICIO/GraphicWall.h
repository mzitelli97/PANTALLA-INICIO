#ifndef GRAPHICWALL_H
#define GRAPHICWALL_H

#include "GraphicItem.h"

class GraphicWall : public GraphicItem{
public:
    GraphicWall();
    GraphicWall(const GraphicWall& orig);
    virtual ~GraphicWall();
    ItemInfo IAm() override;
    void setLocation(CardLocation front, CardLocation rear);
    virtual void draw();

private:

};

#endif /* GRAPHICWALL_H */

