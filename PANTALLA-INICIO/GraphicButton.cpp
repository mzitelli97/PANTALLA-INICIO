#include "GraphicButton.h"

/*Constants defined to set the buttons position. Only used on the constructor.
*Main buttons: quit, help, mute. They appear always on the right top of every sceen*/
#define BUTTON_WIDTH (totalWidth/50.0)          //dimesions of the main buttons
#define BUTTON_HEIGHT (totalHeight/28.0)

#define BUTTON_CENTER_Y (this->height*0.8)      //position of the main buttons
#define QUIT_CENTER_X   totalWidth-this->width*1.2//(39*(totalWidth/40))
#define HELP_CENTER_X   QUIT_CENTER_X-this->width*1.2
#define MUTE_CENTER_X   HELP_CENTER_X-this->width*1.2

#define PASS_CENTER_X   (34*(totalWidth/40))
#define PASS_CENTER_Y   (totalWidth/60)
#define PASS_WIDTH      (totalWidth/15)     //the pass button has its own dimensions(its not a main button)
#define PASS_HEIGHT     (totalHeight/20)

#define CONNECT_CENTER_X totalWidth/2.0
#define CONNECT_CENTER_Y (9*(totalHeight/10.0))
#define CONNECT_WIDTH    totalWidth/2.3             //the connect button has its own dimensions(its not a main button)
#define CONNECT_HEIGHT   CONNECT_WIDTH*(4.0/16.0)

GraphicButton::GraphicButton() {
}

GraphicButton::GraphicButton(const GraphicButton& orig) {
}

GraphicButton::GraphicButton(ALLEGRO_BITMAP * buttonImage,ALLEGRO_BITMAP * buttonImage2, buttonAction identifier, unsigned int width, unsigned int height)
{
    this->button=identifier;
    this->image=buttonImage;
    this->image_2=buttonImage2;
    setScreenDimentions(width,height);
    /*Dimension common for the main buttons. Then the specific buttons override this*/
    this->width = BUTTON_WIDTH;
    this->height = BUTTON_HEIGHT;
    /*To make it a square*/
    if(width>height) this->height = this->width;
    else this->width = this->height;
    this->center.y = BUTTON_CENTER_Y;
    
    switch(button)
    {
        case QUIT_BUTTON:
            center.x=QUIT_CENTER_X;
            //center.y=QUIT_CENTER_Y;
            //this->width=BUTTON_WIDTH;
            //this->height=BUTTON_HEIGHT; 
            break;
        case HELP_BUTTON:
            center.x = HELP_CENTER_X;
            //center.y = HELP_CENTER_Y;
            //this->width = BUTTON_WIDTH;
            //this->height = BUTTON_HEIGHT;
            break;
        case MUTE_BUTTON:
            center.x = MUTE_CENTER_X;
            //center.y = MUTE_CENTER_Y;
            //this->width = BUTTON_WIDTH;
            //this->height = BUTTON_HEIGHT;
            break;
        case PASS_BUTTON:
            center.x=PASS_CENTER_X;
            center.y=PASS_CENTER_Y;
            this->width=PASS_WIDTH;
            this->height=PASS_HEIGHT; 
            break;
        case CONNECT_BUTTON:
            center.x=CONNECT_CENTER_X;
            center.y=CONNECT_CENTER_Y;
            this->width=CONNECT_WIDTH;
            this->height=CONNECT_HEIGHT; 
            break;
        default:
            break;
    }
    min.x=center.x - (this->width)/2;
    max.x=center.x + (this->width)/2;
    min.y=center.y - (this->height)/2;
    max.y=center.y + (this->height)/2;
    
}

ItemInfo GraphicButton::IAm()
{
    if(button == MUTE_BUTTON || button==UNMUTE_BUTTON)
        return {VOL_BUTTON_CLICK,nullptr};
    if(button == HELP_BUTTON)
        return {HELP_BUTTON_CLICK,nullptr};
    if(button == ZOOM_BUTTON)
        return {ZOOM_CLICK,&zoomFloor};
    if(button == QUIT_BUTTON)
        return {EXIT_BUTTON_CLICK,nullptr};
    if(button == PASS_BUTTON)
        return {PASS_BUTTON_CLICK,nullptr};
    if(button == CONNECT_BUTTON)    
        return {CONNECT_BUTTON_CLICK,nullptr};
}

buttonAction GraphicButton::getButtonIdentifier()
{
    return button;
}

void GraphicButton::setZoomFloor(unsigned int floor)
{
    this->zoomFloor=floor;
}

void GraphicButton::toggleMute()
{
    ALLEGRO_BITMAP * aux;
    if(button==MUTE_BUTTON)
        button=UNMUTE_BUTTON;
        
    else if(button==UNMUTE_BUTTON)
        button=MUTE_BUTTON;
    aux = image;
    image = image_2;
    image_2 = aux;
        
}

void GraphicButton::setLocation()
{
    double my_size = this->width;       //save my size
    if(button == ZOOM_BUTTON)
    {
        logic2GraphicCardLocation({zoomFloor,3,3});
        min.x += (width + my_size/3.5);
        min.y += (height - my_size);
        width = height = my_size;       //recover my size
        max.x = min.x + width;
        max.y = min.y + height;
    }
}

GraphicButton::~GraphicButton() {
}
