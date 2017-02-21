#ifndef BURGLEBROSWRAPPER_H
#define BURGLEBROSWRAPPER_H

#include "GUI.h"
#include "BurgleBrosController.h"
#include "cController.h" 
#include "AllegroEG.h"

class BurgleBrosWrapper {
public:
    BurgleBrosWrapper();
    BurgleBrosWrapper(const BurgleBrosWrapper& orig);
    void getNameAndIp();
    void connect();
    void playGame();
    bool gameOnCourse();
    
    virtual ~BurgleBrosWrapper();
private:
    bool connect(BurgleBrosController * controller);
    bool quit;
    GUI gui;
    Model *p2Model;
    Observer *p2View;
    Controller *p2Controller;
    /*Guardo el handle para realizar modificaciones sobre el timer que 
     controla los TimeOuts, que responden a variables externas*/
    AllegroEG *allegroEvent; 
    NetworkInterface networkInterface;
    BurgleBrosSound sound;
    string name, ipToConnect;
};

#endif /* BURGLEBROSWRAPPER_H */

