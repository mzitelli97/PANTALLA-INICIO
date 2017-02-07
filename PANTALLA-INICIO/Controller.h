#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <string>
#include <vector>
#include "EventData.h"

using namespace std;

class Controller {
public:
    Controller();
    Controller(const Controller& orig);
    virtual string askForSpentOK(vector<string> &message)=0;
    virtual void parseMouseEvent(EventData *mouseEvent)=0;
    virtual void parseNetworkEvent(EventData *mouseEvent)=0;
    virtual void parseKeyboardEvent(EventData *mouseEvent)=0;
    virtual ~Controller();
private:

};

#endif /* CONTROLLER_H */

