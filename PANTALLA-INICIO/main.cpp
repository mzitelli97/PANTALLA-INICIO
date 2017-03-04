#include "LibsInit.h"
#include "BurgleBrosWrapper.h"

int main(int argc, char** argv)
{
    if( allegro_startup() == AL_STARTUP_SUCCESS)
    {
        BurgleBrosWrapper fullGame;


        fullGame.getNameAndIp();

        if(fullGame.gameOnCourse())
        {
            fullGame.connect();
            if(fullGame.gameOnCourse())
                fullGame.playGame();
        }

        if(fullGame.wasAnError())
            fullGame.showError();
    
       allegro_shut_down();
    }
    
    return 0;
}


