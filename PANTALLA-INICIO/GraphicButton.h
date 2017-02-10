#ifndef GRAPHICBUTTON_H
#define GRAPHICBUTTON_H
#include "GraphicItem.h"

class GraphicButton: public GraphicItem {
public:
    GraphicButton();
    GraphicButton(const GraphicButton& orig);
    GraphicButton(ALLEGRO_BITMAP * buttonImage,ALLEGRO_BITMAP * buttonImage2, buttonAction identifier, unsigned int width, unsigned int height);
    void setZoomFloor(unsigned int floor);
    buttonAction getButtonIdentifier();
    void toggleMute();
    void setLocation();
    ItemInfo IAm() override;
    virtual ~GraphicButton();
    
private:
    buttonAction button;
    unsigned int zoomFloor;
    ALLEGRO_BITMAP * image_2;
};

#endif /* GRAPHICBUTTON_H */

