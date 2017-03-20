#ifndef GRAPHICPLAYER_H
#define GRAPHICPLAYER_H

#include "GraphicItem.h"

using namespace std;

class GraphicPlayer : public GraphicItem{
public:
    GraphicPlayer(ALLEGRO_BITMAP * image, CardLocation location,unsigned int width, unsigned int height);
    GraphicPlayer(const GraphicPlayer& orig);
    virtual ~GraphicPlayer();
    void setLocation(CardLocation l);
    void setOnBoard(bool isOnBoard);
    virtual ItemInfo IAm();
    void draw() override;

private:
    GraphicPlayer();
    bool isOnBoard;

};

#endif /* GRAPHICPLAYER_H */

