#include "NetworkingEG.h"
#include "NetworkED.h"

NetworkingEG::NetworkingEG() {
    
    error=false;
    NetworkEV=false;
    networkingInterface=nullptr;
}

void NetworkingEG::attachNetworkingInterface(NetworkInterface* networkingInterface) {

    this->networkingInterface=networkingInterface;
    NetworkEV=true;
}

bool NetworkingEG::hayEvento() {

    bool retVal=false;
    unsigned char auxBuffer[BUFSIZE];
    unsigned int len;
    
    PerezProtocolHeader header;
    if(NetworkEV && networkingInterface->recievePacket(&header,auxBuffer,&len))  //si no se attacheo un networkingIterface no permite desreferenciarlo
    {
        NetworkED *auxData=new NetworkED(header,auxBuffer,len);
        if(auxData->isPacketOk()) //implementacion de errores de paquete
        {
            data= (EventData *) auxData;
            event=GUI_EVENT_NETWORKING;
            retVal=true;
        }
        else
            error=true;
    }    
    return retVal;
}


NetworkingEG::~NetworkingEG() {
}

