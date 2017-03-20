#include "NetworkED.h"
#include <string>
#include <regex>

NetworkED::NetworkED(PerezProtocolHeader header,unsigned char * buffer, unsigned int length) 
{
     error=false;
     this->header=header;
     this->len=length;
     memcpy(this->buffer, buffer, len);
}

bool NetworkED::isPacketOk()
{
    bool retVal = false;
    if(packetHasNoDataField(header))
    {
        if(len == 0) retVal = true;
    }
    else
    {
        regex e("");        //this always fails, it will be set depend on the recieved packet
        string s((const char*)buffer,(size_t)len);
        switch(header)
        {
            case I_AM:
                e = "[\x20-\x26]";
                break;
            case NAME_IS:
                if(len == s[0]+1)           //this checks if the name has the size especify in the packet
                {
                    s = s.substr(1);        //the first character is always a counter(hex value)
                    e = "[\x20-\x7F]+";     //printable characters are from 0x20 to 0x7F
                }
                break;
            case START_INFO:
                e = "([\x01-\x14]){48}[A-D][1-4]F[1-3][0-6]";
                break;
            case INITIAL_G_POS:
                e = "([A-D][1-4]F[1-3]){2}";
                break;
            case MOVE: case PEEK:
                e = "^[A-D][1-4]F[1-4][0-6]$";
                break;
            case SPENT_OK:
                e = "^[YN]$";
                break;
            case ADD_TOKEN: case USE_TOKEN: case CREATE_ALARM: case PLACE_CROW:
                e = "^[A-D][1-4]F[1-3]$";
                break;
            case THROW_DICE:
                e = "([0-6]){6}";
                break;
            case SAFE_OPENED: case OFFER_LOOT: case REQUEST_LOOT:
                e = "^[0-9]$";
                break;
            case SPY_PATROL:
                e = "^[A-D][1-4]F[1-3][TB]$";
                break;
            case ROLL_DICE_FOR_LOOT:
                e = "^[1-6]$";
                break;
            case GUARD_MOVEMENT:
                if(s[0] >= 1)
                {
                    e = "(([A-D][1-4]F[1-3])+[\xFF]?)+";
                    s = s.substr(1);
                }
                break;
            default:
                retVal = false;
                break;
        }
        if(regex_match(s,e))
            retVal = true;
    }
    return retVal;
}
PerezProtocolHeader NetworkED::getHeader()
{
    return header;
}
CharacterName NetworkED::getCharacter()
{
    CharacterName retVal;
    if(header==I_AM)
    {
        retVal=(CharacterName)buffer[0];        //Su primer dato como numero representa justo el character name
    }
    else 
        error=true;
    return retVal;
}

string NetworkED::getName()
{
    buffer[len]= '\0';
    string aux = (char *)(&(buffer[1]));    //Como en el paquete name is se guarda los chars a partir del 2do byte y en el primero su largo es válido esto.
    return aux;
}


void NetworkED::getInitGPos(CardLocation *guardPos, CardLocation *guardsDiePos)
{
    if(header==INITIAL_G_POS)
    {
        buffer[len]= '\0';
        string aux = (char *)buffer;    //Aca quedaría en aux por ejemplo A1F2A2F1
        *guardPos=protocolToCardLocation(aux.substr(0,PROTOCOL_LOCATION_LENGTH));   //Acá se los separa.
        *guardsDiePos=protocolToCardLocation(aux.substr(PROTOCOL_LOCATION_LENGTH,PROTOCOL_LOCATION_LENGTH));
    }
    else 
        error=true;
}

Loot NetworkED::getLoot()
{
    Loot retVal;
    if(header==SAFE_OPENED || header== OFFER_LOOT || header == REQUEST_LOOT)
    {
        retVal= (Loot) buffer[0];
    }
    else 
        error=true;
    return retVal;
}

void NetworkED::getStartInfo(vector<CardName> *tiles, CardLocation *playersStartingPos)
{
    tiles->clear();
    if(header==START_INFO)
    {
        buffer[len]= '\0';
        unsigned int i;
        for(i=0;i<BOARD_STANDARD_FLOORS*FLOOR_RAWS*FLOOR_COLUMNS; i++)
            tiles->push_back((CardName)buffer[i]);      //Se pasa el tipo de cada tile
        string aux= (char *)(&(buffer[i]));             //Luego la posición el protocolo se obtiene al finalizar los tiles
        *playersStartingPos=protocolToCardLocation(aux);
    }
    else 
        error=true;
}
CardLocation NetworkED::getPos()
{
    CardLocation retVal;
    if(header==MOVE || header == PEEK)
    {
        char temp =  buffer[len-1];   //Guardo el numero del safe.
        buffer[len-1]= '\0';    //Pongo un terminador en su lugar
        string aux =(char *) buffer;
        retVal=protocolToCardLocation(aux); // Lo paso por string y lo convierto a card locatioon
        buffer[len-1]=temp;         //Le vuelvo a asignar su número del safe.
    }
    else 
        error=true;
    return retVal;
}
CardLocation NetworkED::getTokenPos()
{
    CardLocation retVal;
    if(header==ADD_TOKEN || header==USE_TOKEN)
    {
        buffer[len]= '\0';               
        string aux =(char *) buffer;
        retVal=protocolToCardLocation(aux);
    }
    else
        error=true;
    return retVal;
}
unsigned int NetworkED::getDieForLoot()
{
    unsigned int retVal;
    
    if(header == ROLL_DICE_FOR_LOOT)
        retVal=(unsigned int) buffer[0];
    else
        error=true;
    return retVal;
}

CardLocation NetworkED::getCreateAlarmPos()
{
    CardLocation retVal;
    
    if(header == CREATE_ALARM)
    { 
        buffer[len]= '\0'; 
        string aux =(char *) buffer;
        retVal=protocolToCardLocation(aux); 
    }
    else
        error=true;
    
    return retVal;
}


CardLocation NetworkED::getPlaceCrowPos()
{
    CardLocation retVal;
    
    if(header == PLACE_CROW)
    {
        buffer[len]= '\0'; 
        string aux =(char *) buffer;
        retVal=protocolToCardLocation(aux);
    }
    else
        error=true;
    
    return retVal;
}

bool NetworkED::playerAcceptedToSpentMoves()
{
    bool retVal=false;
    if(header==SPENT_OK)
    {
        if(buffer[0]=='Y')
            retVal=true;
    }
    else 
        error=true;
    return retVal;
}
unsigned int NetworkED::getSafeNumber()
{
    unsigned int retVal;
    if(header==MOVE || header == PEEK || header == START_INFO)
    {
        retVal=buffer[len-1]- '0';       //El numero de safe se guarda en el ultimo byte del msg.
    }
    else 
        error=true;
    return retVal;
}
void NetworkED::getDice(vector<unsigned int> &dice)
{
    dice.clear();
    dice.reserve(MAX_NMBR_OF_EXTRA_DICES);
    if(header==THROW_DICE)
    {
        for(unsigned int i=0; i<MAX_NMBR_OF_EXTRA_DICES; i++)
            dice.push_back((unsigned int)(buffer[i]-'0'));
    }
    else 
        error=true;
}


void NetworkED::getGuardMovement(list<GuardMoveInfo> &guardInfo)
{
    unsigned int i=1;
    unsigned char temp;
    GuardMoveInfo auxgInfo;
    LocationMeaning currMeaning=GUARD_STEP_TO;
    while(i<len)
    {
        if(buffer[i] == 0xFF)
        {
            if(currMeaning==GUARD_STEP_TO)
                currMeaning=GUARD_CARD_PICK;
            else
                currMeaning=GUARD_STEP_TO;
            i++;
        }
        else
        {
            temp=buffer[i+PROTOCOL_LOCATION_LENGTH];
            buffer[i+PROTOCOL_LOCATION_LENGTH]='\0';
            auxgInfo.cardLocation=protocolToCardLocation((const char*)&(buffer[i]));
            auxgInfo.meaning=currMeaning;
            guardInfo.push_back(auxgInfo);
            buffer[i+PROTOCOL_LOCATION_LENGTH]=temp;
            i += PROTOCOL_LOCATION_LENGTH;
        }
        
    }
}

string NetworkED::getSpyPatrolChoice()
{
    string retVal;
    if(header==SPY_PATROL)
    {
        if(buffer[len-1]=='T')
            retVal=SPOTTER_TOP;
        else if (buffer[len-1]=='B')
            retVal=SPOTTER_BOTTOM;
    }
    else 
        error=true;
    return retVal;
}
CardLocation NetworkED::getSpyPatrolPos()
{
    CardLocation retVal;
    if(header==SPY_PATROL)
    {
        char temp =  buffer[len-1];   //Guardo el numero del safe.
        buffer[len-1]= '\0';    //Pongo un terminador en su lugar
        string aux =(char *) buffer;
        retVal=protocolToCardLocation(aux); // Lo paso por string y lo convierto a card locatioon
        buffer[len-1]=temp;         //Le vuelvo a asignar su número del safe.
    }
    else 
        error=true;
    return retVal;
}
NetworkED::~NetworkED() 
{
}

