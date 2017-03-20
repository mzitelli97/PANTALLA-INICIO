#ifndef TEXTBOX_H
#define TEXTBOX_H
#include "GraphicItem.h"
#include "CModel.h"

class TextBox: public GraphicItem  {
public:
    TextBox(double width, double height,ALLEGRO_FONT * font, textSelected id);
    TextBox(const TextBox& orig);
    virtual ~TextBox();
    virtual ItemInfo IAm() override;
    virtual void draw() override;
    void setText(std::string text);
    void select();
    void unselect();
    void setPosition(double x, double y);
    
private:
    bool selected;
    std::string text;
    textSelected identifier;
    ALLEGRO_FONT * font;

};

#endif /* TEXTBOX_H */

