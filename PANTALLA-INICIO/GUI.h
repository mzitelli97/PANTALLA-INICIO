#ifndef GUI_H
#define GUI_H
#include <list>
#include "Controller.h"
#include "NetworkInterface.h"
#include "EventGenerator.h"


using namespace std;

class GUI {
public:
    
    /*Si guardo los Handle EventGenerator, puedo realizar modificaciones sobre las
     * condiciones que generan un evento que responden a situaciones externas,
     * sin modificar la GUI. 
     * Ejemplo: Tiempo de un timer para generar el evento.
     */
    
    GUI();
    GUI(const GUI& orig);
    /*recorre la lista de eventGenerator y chequea si hubo un evento*/
    bool hayEvento(void);
    /*Segun el evento trancurrido entrega un eventData al controller*/
    void parseEvento(void);
    void attachController (Controller * Controller);
    void attachEventGenerator(EventGenerator * evento);
    virtual ~GUI();
    
private:
    
    Controller *controller;
    list<EventGenerator *> evento;
    GuiEvent eventType;
    EventData *data;
    bool error;          //para implementar un chequeo de error
    
};

#endif /* GUI_H */

