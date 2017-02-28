#ifndef ALLEGROEG_H
#define ALLEGROEG_H

#include "EventGenerator.h"
#include <allegro5/events.h>
#include <allegro5/config.h>
#include <allegro5/keyboard.h>
#include <allegro5/mouse.h>
#include <allegro5/timer.h>

#define MOUSE_Z_STD_RESOLUTION 300
#define SECONDS 1.0
#define TIMEOUT_TIME 60

class AllegroEG: public EventGenerator {
public:
    AllegroEG();
    bool hayEvento() override;
    void enableTimer();
    void resetTimer();
    void stopTimer();
    void playTimer();
    void resetZMouse();
    virtual ~AllegroEG();
private:

    bool error;
    int prevMouseZ;
    int mouseZResolution;
    ALLEGRO_EVENT_QUEUE *EventQueue;
    ALLEGRO_TIMER * timer;

};

#endif /* ALLEGROEG_H */

