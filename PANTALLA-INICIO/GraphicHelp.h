#ifndef GRAPHICHELP_H
#define GRAPHICHELP_H

#include "GraphicItem.h"

class GraphicHelp: public GraphicItem {
public:
    GraphicHelp();
    GraphicHelp(vector<ALLEGRO_BITMAP *> d);
    void init(); 
    void setScroll(unsigned int scroll);
    void resetScroll();
    GraphicHelp(const GraphicHelp& orig);
    virtual ItemInfo IAm();
    virtual void draw();
    virtual ~GraphicHelp();
private:
    unsigned int currentScroll;
    vector<ALLEGRO_BITMAP *> allImgs;
};

#endif /* GRAPHICHELP_H */

