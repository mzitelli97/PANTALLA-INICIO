#ifndef BURGLEBROSSOUND_H
#define BURGLEBROSSOUND_H

#include "Observer.h"
#include "BurgleBrosModel.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <iostream>
#include <vector>

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

