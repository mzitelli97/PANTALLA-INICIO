#ifndef MODEL_H
#define MODEL_H

#include "Observer.h"
#include <list>

class Model {
public:
    Model();
    Model(const Model& orig);
    virtual ~Model();
    void attach(Observer * view);
    void deAttach(Observer * view);
protected:
    void notifyAllObservers();
private:
    std::list<Observer *> observers;

};

#endif /* MODEL_H */

