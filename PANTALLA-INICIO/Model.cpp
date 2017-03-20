#include "Model.h"

Model::Model() {
}

Model::Model(const Model& orig) {
}

Model::~Model() {
}

void Model::attach(Observer * view)
{
    if(view != nullptr)
        observers.push_back(view);
}

void Model::notifyAllObservers() 
{
    for(auto& ob : observers)
        ob->update();
}