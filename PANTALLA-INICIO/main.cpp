#include "LibsInit.h"
#include "BurgleBrosWrapper.h"

int main(int argc, char** argv)
{
    if( allegro_startup() == AL_STARTUP_SUCCESS)
    {
       /*  Creo el wrapper, si algun archivo como imagen, fuente, etc. no se 
        * iniciliza como corresponde el error no lo indica por en la funcion 
        * showError, mostrandolo en pantalla y en el archivo de texto que se 
        * encuentra en la carpeta error  */
        
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


