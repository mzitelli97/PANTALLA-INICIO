#ifndef GRAPHICPLAYERCARD_H
#define GRAPHICPLAYERCARD_H

#include "GraphicItem.h"

class GraphicPlayerCard : public GraphicItem{
public:
    GraphicPlayerCard(ALLEGRO_BITMAP * image, ALLEGRO_BITMAP * stealthTokenImg,unsigned int lives, std::string name, PlayerId whichPlayer,unsigned int width,unsigned int height);
    void setLivesAndActions(unsigned int livesNumber,unsigned int actions);
    void setFont(ALLEGRO_FONT * font);
    void setTurn(bool turn);
    ItemInfo IAm() override;
    void draw() override;
    virtual ~GraphicPlayerCard() {};
private:
    void setPosition();
    unsigned int lives;
    unsigned int actions;
    bool turn;
    Point actionsPos;
    Point namePos;
    string name;
    ALLEGRO_BITMAP * stealthToken; 
    PlayerId whichPlayer;
    ALLEGRO_FONT * font;
};

#endif /* GRAPHICPLAYERCARD_H */

