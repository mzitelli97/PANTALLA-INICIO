/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SoundManager.h
 * Author: javier
 *
 * Created on January 11, 2017, 2:14 AM
 */

#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

class SoundManager {
public:
    //virtual void playSoundEffect(SoundEffect effect)=0;
    virtual void toggleMute()=0;
    virtual void reset()=0;

};

#endif /* SOUNDMANAGER_H */

