#ifndef NETWORKINGEG_H
#define NETWORKINGEG_H

#include "EventGenerator.h"
#include "NetworkED.h"


class NetworkingEG: public EventGenerator {
public:
    NetworkingEG();
    void attachNetworkingInterface(NetworkInterface * networkingInterface);
    bool hayEvento() override;
    virtual ~NetworkingEG();
private:
    NetworkInterface * networkingInterface;
    bool NetworkEV;
    bool error;
};

#endif /* NETWORKINGEG_H */

