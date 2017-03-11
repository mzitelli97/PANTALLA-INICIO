#include "AllegroEG.h"
#include "MouseED.h"
#include "KeyboardED.h"
#include "TimerED.h"
#include <iostream>
#include <allegro5/display.h>

AllegroEG::AllegroEG() {
    
    error=false;
    prevMouseZ=0;
    mouseZResolution=MOUSE_Z_STD_RESOLUTION;
    
    this->EventQueue=al_create_event_queue();
    timer=al_create_timer(SECONDS);     //Pongo el timer a contar segundos.
    if(EventQueue != nullptr)
    {
        al_register_event_source(EventQueue,al_get_mouse_event_source());
        al_register_event_source(EventQueue,al_get_keyboard_event_source());
        //al_register_event_source(EventQueue,al_get_display_event_source(al_get_current_display()));
    }
    else { error=true; errorMsg="Error al crear EventQueue";}
    
}

bool 
AllegroEG::initOk() {
    return !error;
}

std::string 
AllegroEG::getErrorMsg() {
    return errorMsg;
}


bool
AllegroEG::hayEvento() {

    bool retVal=false;
    ALLEGRO_EVENT rawEvent;
    
    if(al_get_next_event(EventQueue,&rawEvent))
    {
        if(rawEvent.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            KeyboardED *auxData= new KeyboardED(ESCAPE_KEY);
            data=(EventData *) auxData;
            event=GUI_EVENT_KEYBOARD;
            retVal=true;
        }
        else if(rawEvent.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
        {
            MouseED *auxData= new MouseED(true,rawEvent.mouse.x,rawEvent.mouse.y);
            data=(EventData *) auxData;
            event=GUI_EVENT_MOUSE;
            retVal=true;
        }
        else if(rawEvent.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            int z=rawEvent.mouse.z;
            if(z > 0)
            {
                al_set_mouse_z(0);
                z=0;
            }
            else if(z < ((-1)* mouseZResolution))
            {
                al_set_mouse_z(((-1)* mouseZResolution));
                z=((-1)* mouseZResolution);
            }
            if(z != prevMouseZ)
            {
                MouseED *auxData= new MouseED(z);
                data=(EventData *) auxData;
                event=GUI_EVENT_MOUSE;
                prevMouseZ=z;
                retVal=true;
            }
        }
        else if(rawEvent.type == ALLEGRO_EVENT_KEY_CHAR)
        {
            KeyboardED *auxData;
            retVal=true;
            if(rawEvent.keyboard.keycode >= ALLEGRO_KEY_A && rawEvent.keyboard.keycode <= ALLEGRO_KEY_Z)
            {
                if((rawEvent.keyboard.modifiers & ALLEGRO_KEYMOD_SHIFT) || (rawEvent.keyboard.modifiers & ALLEGRO_KEYMOD_CAPSLOCK))
                    auxData= new KeyboardED(rawEvent.keyboard.keycode-ALLEGRO_KEY_A+'A');
                else
                    auxData= new KeyboardED(rawEvent.keyboard.keycode-ALLEGRO_KEY_A+'a');
            }
            else if(rawEvent.keyboard.keycode >= ALLEGRO_KEY_0 && rawEvent.keyboard.keycode <= ALLEGRO_KEY_9)
                auxData= new KeyboardED(rawEvent.keyboard.keycode-ALLEGRO_KEY_0+'0');
            else if(rawEvent.keyboard.keycode >= ALLEGRO_KEY_PAD_0 && rawEvent.keyboard.keycode <= ALLEGRO_KEY_PAD_9)
                auxData= new KeyboardED(rawEvent.keyboard.keycode-ALLEGRO_KEY_PAD_0+'0');
            else if(rawEvent.keyboard.keycode == ALLEGRO_KEY_FULLSTOP || rawEvent.keyboard.keycode == ALLEGRO_KEY_PAD_DELETE)
                auxData= new KeyboardED('.');
            else if(rawEvent.keyboard.keycode == ALLEGRO_KEY_SPACE)
                auxData= new KeyboardED(' ');
            else if(rawEvent.keyboard.keycode == ALLEGRO_KEY_ENTER || rawEvent.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER)
                auxData= new KeyboardED(ENTER_KEY);
            else if(rawEvent.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                auxData= new KeyboardED(ESCAPE_KEY);
            else if(rawEvent.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
                auxData= new KeyboardED(BACKSPACE_KEY);
            else if(rawEvent.keyboard.keycode == ALLEGRO_KEY_DOWN)
                auxData= new KeyboardED(DOWN_KEY);
            else if(rawEvent.keyboard.keycode == ALLEGRO_KEY_UP)
                auxData= new KeyboardED(UP_KEY);
            else retVal = false;                    //si no es ninguna de las teclas que tomamos, no genera evento
            data=(EventData *) auxData;
            event=GUI_EVENT_KEYBOARD;
        }
        else if(al_get_timer_count(timer) >= TIMEOUT_TIME)
        {
            TimerED *auxData = new TimerED(TIMEOUT);
            data=(EventData *) auxData;
            event=GUI_EVENT_TIMER;
            std::cout<< "TIMEOUT"<<std::endl;
            retVal=true;
        }
   }
    
    return retVal;
}

void 
AllegroEG::playTimer() {
    al_start_timer(timer);
}

void AllegroEG::resetZMouse() {
    al_set_mouse_z(0);
}

void 
AllegroEG::resetTimer() {
    al_stop_timer(timer);
    al_set_timer_count(timer,0);
    al_start_timer(timer);
    al_stop_timer(timer);
    al_set_timer_count(timer,0);
}

void 
AllegroEG::stopTimer() {
    al_stop_timer(timer);
    al_set_timer_count(timer,0);
}

void 
AllegroEG::enableTimer() {

}


AllegroEG::~AllegroEG() {
}

