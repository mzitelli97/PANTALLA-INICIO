#include "BurgleBrosSound.h"
//#define MUTE

using namespace std;

BurgleBrosSound::BurgleBrosSound() {
    initOK=loadAllSamples();
    al_reserve_samples(3);              //para poder reproducir 3 sonidos a la vez(no va a haber mas)
#ifndef MUTE
    if(initOK)
        al_play_sample(samples.front(), 1.0, 0.0, 1.0,ALLEGRO_PLAYMODE_LOOP, NULL);   //musica de fondo
#endif   
    onMute = false;
}

BurgleBrosSound::BurgleBrosSound(const BurgleBrosSound& orig) {
    
}

void BurgleBrosSound::attachModel(BurgleBrosModel* model) 
{
    this->model = model;
}


void BurgleBrosSound::playSoundEffect(importantEvents effect)
{
    if(!onMute && initOK && effect != NO_IE)
        al_play_sample(samples[(int)effect+1], 1.5, 0.0, 1.0,ALLEGRO_PLAYMODE_ONCE, NULL);  //+1 por la musica de fondo
}
bool BurgleBrosSound::loadAllSamples()
{
    bool retVal=false;
    string fullPath = (string)MUSIC_FOLDER + (string)THEME_1 + (string)THEMES_EXTENSION;
    ALLEGRO_SAMPLE * temp = al_load_sample(fullPath.c_str());
    if(temp != nullptr)
    {
        samples.push_back(temp);
        retVal=true;
    }
    for(int i = (int)ALARM_TRIGGERED; i <= (int)WON; i++)
    {
        fullPath = (string)SOUNDS_FOLDER + soundEffect2String((importantEvents)i) + (string)THEMES_EXTENSION;
        temp = al_load_sample(fullPath.c_str());
        if(temp != nullptr)
        {
            samples.push_back(temp);
            retVal=true;
        }
        else { retVal = false; break; }
    }
    return retVal;
}

string BurgleBrosSound::soundEffect2String(importantEvents effect)
{
    string retVal = "";
    switch(effect)
    {
        case ALARM_TRIGGERED: retVal += "Alarm"; break;
        case SAFE_CRACKED: retVal += "Safe"; break;
        case KEYPAD_OPENED: retVal += "Keypad"; break;
        case STAIRS: retVal += "Stairs"; break;
        case CHIHUAHUA_BARKS: retVal += "Bark"; break;
        case KITTY_ESCAPED: retVal += "Kitty"; break;
        case LOST: retVal += "Lost"; break;
        case WON: retVal += "Chopper"; break;
        default: retVal += "ERROR"; break;
    }
    return retVal;
}

void BurgleBrosSound::toggleMute()
{
    onMute ^= true;
    if(!onMute)
        al_play_sample(samples.front(), 1.0, 0.0, 1.0,ALLEGRO_PLAYMODE_LOOP, NULL);
    else
        al_stop_samples();
}

void BurgleBrosSound::reset()
{
    onMute = false;
    al_stop_samples();              //paro todos los sonidos que pueda haber
#ifndef MUTE    
    al_play_sample(samples.front(), 1.0, 0.0, 1.0,ALLEGRO_PLAYMODE_LOOP, NULL); //reinicio la musica de fondo
#endif
}

void BurgleBrosSound::update()
{
    importantEvents iE = model->getInfoOfEvents();
    playSoundEffect(iE);
}

BurgleBrosSound::~BurgleBrosSound() {
}

