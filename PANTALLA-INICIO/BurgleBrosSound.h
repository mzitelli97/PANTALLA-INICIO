/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BurgleBrosSound.h
 * Author: javier
 *
 * Created on January 11, 2017, 2:17 AM
 */

#ifndef BURGLEBROSSOUND_H
#define BURGLEBROSSOUND_H

//#include "SoundManager.h"
#include "View.h"
#include "BurgleBrosModel.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <iostream>
#include <vector>

#define MUSIC_FOLDER "Music/"
#define SOUNDS_FOLDER "Sounds/"


#define THEME_1 "Pink panther"

#define THEMES_EXTENSION ".wav"

using namespace std;

class BurgleBrosSound:public Observer {
public:
    BurgleBrosSound();
    BurgleBrosSound(const BurgleBrosSound& orig);
    
    void toggleMute();
    void reset();
    void update() override;
    void attachModel(BurgleBrosModel *model);

    virtual ~BurgleBrosSound();
private:
    void playSoundEffect(importantEvents effect);
    vector<ALLEGRO_SAMPLE *> samples;
    BurgleBrosModel *model;
    string soundEffect2String(importantEvents effect);
    bool loadAllSamples();
    bool initOK;
    bool onMute;

};

#endif /* BURGLEBROSSOUND_H */

