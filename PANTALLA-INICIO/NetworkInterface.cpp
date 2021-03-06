#include "NetworkInterface.h"

#define DIFF_TIME_CLOCK(X,Y) (((double)((X)-(Y)))/((double) CLOCKS_PER_SEC))
#define RAND_IN_RANGE_DOUBLE(x,y) y+((((double)rand())/((double)(RAND_MAX)))*((x)-(y))) //y<x

bool packetHasNoDataField(PerezProtocolHeader h)
{
    return (h==ACK || h==AGREE || h==DISAGREE || h==NAME || h==YOU_START || h==I_START || h==PASS || h==WE_WON || h==WE_LOST || h==GAME_OVER  || h==PLAY_AGAIN || h==QUIT || h==ERRORR || h==PICK_UP_LOOT);
}

NetworkInterface::NetworkInterface()
{
    p2networking = new Networking;
    firstTimeCalled = true;
    timeToBecomeServer = RAND_IN_RANGE_DOUBLE(MAX_WAITING_TIME, MIN_WAITING_TIME);
    currentRole = CLIENT;
    currClock = clock();
    prevClock = currClock;
    error=false;
}

NetworkInterface::NetworkInterface(Networking &networking)
{
	p2networking = &networking;
	firstTimeCalled = true;
	timeToBecomeServer = RAND_IN_RANGE_DOUBLE(MAX_WAITING_TIME, MIN_WAITING_TIME);
	currentRole = CLIENT;
	currClock = clock();
	prevClock = currClock;
        error=false;
}

bool NetworkInterface::standardConnectionStart(string &ip)
{
	bool connected = false;
        double time;
	if (firstTimeCalled == true)
	{
		firstTimeCalled = false;
		error=!(p2networking->prepareToConnect(ip));
                currClock = clock();
                prevClock = currClock;
                if(error)
                    errorMsg = "Networking error:"+ p2networking->getErrorMsg();
	}
	switch (currentRole)
	{
	case CLIENT:
		currClock = clock();
                time = (double)DIFF_TIME_CLOCK(currClock, prevClock);
		if (!error && p2networking->tryToConnect() == true) //El que no haya error es por si en prepare to connect hubo un error.
			connected = true;
		else if (time > 0.1)//timeToBecomeServer )
		{
			currentRole = SERVER;
			p2networking->abortConnecting();
			error=!(p2networking->prepareToListen());
                        if(error)
                            errorMsg = "Networking error:"+ p2networking->getErrorMsg();
                        cout<<"YA SOY SERVER" << endl;
		}
                break;
	case SERVER:
		if (p2networking->listen() == true)
			connected = true;
		break;
	}
	return connected;
}


CommunicationRole NetworkInterface::getCommunicationRole()
{
	return currentRole;
}

bool NetworkInterface::checkError() {
    return error;
}

string NetworkInterface::getErrorMsg() {
    return errorMsg;
}

bool NetworkInterface::recievePacket(PerezProtocolHeader *header, unsigned char * msg, unsigned int *len)
{
    return p2networking->recievePacket(header,(char *)msg,len);
}
bool NetworkInterface::sendPacket(PerezProtocolHeader header)
{
    bool retVal=false;
    unsigned char buff[1];
    unsigned int i=0;
    if(packetHasNoDataField(header))
        retVal=p2networking->sendPacket(header,(const char *)buff,i);
    else
    {
        error=true;
        errorMsg= "NetworkInterface error: tried to send a message that has data field without data field.";
    }
    return retVal;
}
bool NetworkInterface::sendName(string name)
{
    bool retVal=false;
    unsigned char auxBuffer[BUFSIZE];
    auxBuffer[0] = '\0';
    auxBuffer[0] = name.length();
    memcpy(&(auxBuffer[1]), name.c_str(), name.length());
    retVal = p2networking->sendPacket(NAME_IS, (const char *)auxBuffer, name.length() + 1);
    return retVal;
}
bool NetworkInterface::sendChar(CharacterName characterName)
{
    bool retVal=false;
    unsigned char auxBuffer[1];
    auxBuffer[0]=(unsigned char) characterName;
    retVal = p2networking->sendPacket(I_AM, (const char *)auxBuffer, 1);
    return retVal;
}
bool NetworkInterface::sendInitGPos(CardLocation guardPos, CardLocation guardDiePos)
{
    bool retVal=false;
    string aux = cardLocationToProtocol(guardPos) + cardLocationToProtocol(guardDiePos); 
    retVal = p2networking->sendPacket(INITIAL_G_POS, aux.c_str(), aux.length());
    return retVal;
}
bool NetworkInterface::sendStartInfo(vector<CardName> &tiles, CardLocation &initTile, unsigned int safeNumber)
{
    bool retVal=false;
    unsigned char auxBuffer[BUFSIZE];
    if(tiles.size() == BOARD_STANDARD_FLOORS*FLOOR_RAWS * FLOOR_COLUMNS)
    {
        unsigned int i;
        for(i=0; i<BOARD_STANDARD_FLOORS*FLOOR_RAWS * FLOOR_COLUMNS; i++)
        {
            auxBuffer[i]=(unsigned char) tiles[i];
        }
        char buf[2]; buf[0] = '0' + safeNumber; buf[1] = '\0';
        string initPos=cardLocationToProtocol(initTile) +  buf;
        memcpy(&(auxBuffer[i]), initPos.c_str(), initPos.length());
        retVal = p2networking->sendPacket(START_INFO,(const char *) auxBuffer, BOARD_STANDARD_FLOORS*FLOOR_RAWS * FLOOR_COLUMNS+ initPos.length());
    }
    return retVal;
}

bool NetworkInterface::sendDice(vector<unsigned int> &dice)
{
    bool retVal=false;
    char buffer[MAX_NMBR_OF_EXTRA_DICES];
    for(unsigned int i=0; i<MAX_NMBR_OF_EXTRA_DICES; i++)
        buffer[i]=(char)((unsigned int)'0'+dice[i]);
    retVal=p2networking->sendPacket(THROW_DICE, buffer, MAX_NMBR_OF_EXTRA_DICES);
    return retVal;
}


bool NetworkInterface::sendPeek(CardLocation tileToPeek, unsigned int safeNumber)
{
    bool retVal=false;
    char buffer[2];
    buffer[0]='0'+safeNumber;
    buffer[1]='\0';
    string fullmsg= cardLocationToProtocol(tileToPeek) + buffer;
    retVal=p2networking->sendPacket(PEEK, fullmsg.c_str(), fullmsg.length());
    return retVal;
}
bool NetworkInterface::sendMove(CardLocation tileToPeek, unsigned int safeNumber)
{
    bool retVal=false;
    char buffer[2];
    buffer[0]='0'+safeNumber;
    buffer[1]='\0';
    string fullmsg= cardLocationToProtocol(tileToPeek) + buffer;
    retVal=p2networking->sendPacket(MOVE, fullmsg.c_str(), fullmsg.length());
    return retVal;
}

bool NetworkInterface::sendUseToken(CardLocation location)
{
    bool retVal=false;
    string msg=cardLocationToProtocol(location);
    retVal=p2networking->sendPacket(USE_TOKEN, msg.c_str(), msg.length());
    return retVal;
}
bool NetworkInterface::sendAddToken(CardLocation location)
{
    bool retVal=false;
    string msg=cardLocationToProtocol(location);
    retVal=p2networking->sendPacket(ADD_TOKEN, msg.c_str(), msg.length());
    return retVal;
}

bool NetworkInterface::sendPlaceCrow(CardLocation location)
{
    bool retVal=false;
    string msg=cardLocationToProtocol(location);
    retVal=p2networking->sendPacket(PLACE_CROW, msg.c_str(), msg.length());
    return retVal;
    
}

bool NetworkInterface::sendCreateAlarm(CardLocation location)
{
    bool retVal=false;
    string msg=cardLocationToProtocol(location);
    retVal=p2networking->sendPacket(CREATE_ALARM, msg.c_str(), msg.length());
    return retVal;
}

bool NetworkInterface::sendSpent(bool YesOrNo)
{
    char buffer[1];
    if(YesOrNo==true)
        buffer[0]='Y';
    else
        buffer[0]='N';
    return p2networking->sendPacket(SPENT_OK, buffer, 1);
}

bool NetworkInterface::sendSafeOpened(Loot loot)
{
    char buffer[1];
    buffer[0]=(char) loot;
    return p2networking->sendPacket(SAFE_OPENED, buffer, 1);
}

bool NetworkInterface::sendGMove(list<GuardMoveInfo> guardMovement)
{
    unsigned char buffer[BUFSIZE];
    buffer[0]=guardMovement.size();
    list<GuardMoveInfo>::iterator lastInfo=guardMovement.begin();
    list<GuardMoveInfo>::iterator it;
    unsigned int i=1;
    for(it=guardMovement.begin(); it != guardMovement.end(); it++ )
    {
        if(it->meaning== GUARD_STEP_TO)     //Si fue un step le pongo el formato de la carta
        {
            if(lastInfo->meaning==GUARD_CARD_PICK)   //SI hubo un mensaje previo y fue un levante de carta, pongo el 0xff para separar.
            {
                buffer[i]= 0xFF;
                i++;
            }
            cardLocationToProtocol(it->cardLocation).copy((char *)&(buffer[i]), PROTOCOL_LOCATION_LENGTH);
            i += PROTOCOL_LOCATION_LENGTH;
        }
        else
        {
            if(lastInfo->meaning==GUARD_STEP_TO)   //SI hubo un mensaje previo y fue un guard step pongo el separador 0xFF
            {
                buffer[i]= 0xFF;
                i++;
            }
            cardLocationToProtocol(it->cardLocation).copy((char *)&(buffer[i]), PROTOCOL_LOCATION_LENGTH);
            i += PROTOCOL_LOCATION_LENGTH;
        }
        lastInfo=it;
    }
    return p2networking->sendPacket(GUARD_MOVEMENT, (char *) buffer, i);
}

bool NetworkInterface::sendRequestLoot(Loot i)
{
    unsigned char buffer[1];
    buffer[0]= (unsigned char) i;
    return p2networking->sendPacket(REQUEST_LOOT,(char *) buffer, 1);
}
bool NetworkInterface::sendOfferLoot(Loot i)
{
    unsigned char buffer[1];
    buffer[0]= (unsigned char) i;
    return p2networking->sendPacket(OFFER_LOOT,(char *) buffer, 1);
}
bool NetworkInterface::sendRollDiceForLoot(unsigned int die)
{
    unsigned char buffer[1];
    buffer[0]= (unsigned char) die;
    return p2networking->sendPacket(ROLL_DICE_FOR_LOOT,(char *) buffer, 1);
}

bool NetworkInterface::sendSpyPatrol(CardLocation topOfNotShown,string userChoice)
{
    unsigned char buffer[BUFSIZE];
    string pos=cardLocationToProtocol(topOfNotShown);
    memcpy(buffer, pos.c_str(), pos.length());
    if(userChoice==SPOTTER_BOTTOM)
        buffer[PROTOCOL_LOCATION_LENGTH]='B';
    else
        buffer[PROTOCOL_LOCATION_LENGTH]='T';
    return p2networking->sendPacket(SPY_PATROL,(char *) buffer, PROTOCOL_LOCATION_LENGTH+1);
}


NetworkInterface::~NetworkInterface()
{
    if(p2networking != nullptr) delete p2networking;
}
