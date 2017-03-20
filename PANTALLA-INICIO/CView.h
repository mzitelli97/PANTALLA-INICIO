#ifndef CVIEW_H
#define CVIEW_H

#include "Observer.h"
#include "CModel.h"
#include "GraphicItem.h"
#include "ImageLoader.h"


class CView: public Observer {
public:
    CView(CModel * model);
    CView(const CView& orig);
    ~CView();
    void update() override;
    ItemInfo itemFromClick(Point point);
    void toggleButton(buttonAction button);
    string geterrorMsg();
    bool DidAnErrorStarting();
    
private:
    CModel * model;
    std:: list<GraphicItem*> gList;
    ImageLoader imageLoader;
    ALLEGRO_BITMAP * backScreen;
    ALLEGRO_DISPLAY * display;
    ALLEGRO_FONT * font;
    bool error;
    string errorMsg;
};

#endif /* CVIEW_H */

