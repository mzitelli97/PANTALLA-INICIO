#ifndef GRAPHICGUARDCARDS_H
#define GRAPHICGUARDCARDS_H

#include "GraphicItem.h"

typedef struct
{
    unsigned int floor;
    bool shownDeck;
}auxInfoGuard;

class GraphicGuardCards : public GraphicItem{
public:
    GraphicGuardCards(ALLEGRO_BITMAP * image, unsigned int floor, bool isShownDeck);
    GraphicGuardCards(const GraphicGuardCards& orig);
    virtual ~GraphicGuardCards();
    ItemInfo IAm() override;
    void push_top_card(ALLEGRO_BITMAP * card);
    void clearShownCards();
    virtual void draw() override;
    void setTopOfNonVisibleDeck(bool visible, ALLEGRO_BITMAP *target);
    void setVisible(bool isVisible);
private:
    list<ALLEGRO_BITMAP *> cards;
    void setPosition();
    void drawOnZoom();
    auxInfoGuard properties;
    bool topOfNonVisibleDeckShown;
    ALLEGRO_BITMAP * topOfNonVisibleDeckTarget;
    bool isVisible;
};

#endif /* GRAPHICGUARDCARDS_H */

