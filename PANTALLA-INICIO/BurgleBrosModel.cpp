#include "BurgleBrosModel.h"
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>

#define KEYPAD_CRACK_NUMBER 6
void addWithoutRepeating(list<CardLocation> &alarmList, CardLocation item);
typedef struct
{
    CardLocation target;
    unsigned int length;
}AuxStruct;

bool sortAuxStruct(AuxStruct &item1, AuxStruct &item2)
{
    return item1.length<item2.length;
}

bool BurgleBrosModel::isGuardMoving() 
{
    bool retVal=false;
    if(isGuardsTurn())
    {    
        if(guards[getP2Player(playerOnTurnBeforeGuardMove)->getPosition().floor].hasStepsLeft())
            retVal=true;
    }
    return retVal;
}

list<GuardMoveInfo> BurgleBrosModel::getWGuardPath() 
{
    return gWholePath.first; 
}


BurgleBrosModel::BurgleBrosModel()
{
    BurgleBrosGuard aux1(0);
    BurgleBrosGuard aux2(1);
    BurgleBrosGuard aux3(2);
    guards[0]= aux1;
    guards[1]= aux2;
    guards[2]= aux3;
    board.initBoard();
    gameFinished=false;
    playerSpentFreeAction=false;
    status=WAITING_FOR_ACTION;
    guardFinishedMoving=false;
    rollForLootCount=0;
    specialMotionCase=false;
    iE = NO_IE;
    nmbrOfPendingQuestions=0;
}

bool BurgleBrosModel::isMotionSpecialCase() {
    return specialMotionCase;
}

void BurgleBrosModel::reset()
{
    BurgleBrosGuard aux1(0);
    BurgleBrosGuard aux2(1);
    BurgleBrosGuard aux3(2);
    BurgleBrosBoard auxBoard;
    BurgleBrosLoots auxLoots;
    BurgleBrosTokens auxTokens;
    BurgleBrosDices auxDice;
    BurgleBrosPlayer auxPlayer;
    guards[0]= aux1;
    guards[1]= aux2;
    guards[2]= aux3;
    board=auxBoard;
    board.initBoard();
    loots=auxLoots;
    tokens=auxTokens;
    dice=auxDice;
    auxPlayer.setName(myPlayer.getName());
    myPlayer=auxPlayer;
    auxPlayer.setName(otherPlayer.getName());
    otherPlayer=auxPlayer;
    gameFinished=false;
    playerSpentFreeAction=false;
    status=WAITING_FOR_ACTION;
    guardFinishedMoving=false;
    rollForLootCount=0;
    specialMotionCase=false;
    finishMsg.clear();
    auxMsgsToShow.clear();
    iE = NO_IE;
    nmbrOfPendingQuestions=0;
}

bool BurgleBrosModel::moveRequiresToInitGuard(CardLocation locationToMove)
{
    return !guards[locationToMove.floor].checkIfInitialized();
}

void BurgleBrosModel::initBoard(vector<CardName> &allTiles,CardLocation initPos, unsigned int *safeNumber)
{
    if(allTiles.empty())
    {
        board.initBoard();
        CardLocation aux;
        for(aux.floor=0;aux.floor<BOARD_STANDARD_FLOORS; aux.floor++ )
        {
            for(aux.row=0;aux.row<FLOOR_RAWS; aux.row++ )
            {
                for(aux.column=0;aux.column<FLOOR_COLUMNS; aux.column++ )
                {
                    allTiles.push_back(board.getCardType(aux));
                }
            }
        }
        *safeNumber = board.setCardVisible(initPos);
        if(board.getCardType(initPos)== LAVATORY)
            tokens.lavatoryRevealed(initPos);
    }
    else if(allTiles.size()== BOARD_STANDARD_FLOORS * FLOOR_COLUMNS * FLOOR_RAWS)
    {
        board.initBoard(allTiles);
        board.setCardVisible(initPos, *safeNumber);
        if(board.getCardType(initPos)== LAVATORY)
            tokens.lavatoryRevealed(initPos);
    }
    else
    {
        gameFinished=true;
        finishMsg= "ERROR: BBModel error: Tried to initialize board with an incorrect number of tiles!";
    }
}
void BurgleBrosModel::initPlayer(PlayerId playerId, string playerName, CharacterName playerCharacter, CardLocation playerPos)
{
    BurgleBrosPlayer *p = getP2Player(playerId);
    p->setName(playerName);
    p->setPosition(playerPos);
    //board.setCardVisible(playerPos);
    p->setCharacter(playerCharacter);
    if(!tokens.isThereADownstairToken(playerPos))
        tokens.placeDownstairToken(playerPos);
}
void BurgleBrosModel::setInitTurn(PlayerId playerId)
{
    getP2Player(playerId)->setTurn(true);
}
void BurgleBrosModel::copyGuardInitPos(CardLocation guardPos, CardLocation guardDiePos)
{
    unsigned int i;
    for(i=0; i<BOARD_STANDARD_FLOORS; i++)
    {    
        if(i==BOARD_STANDARD_FLOORS-1 && guards[i].checkIfInitialized())        //Si se llamo a esta función ya habiendo incializado los 3 guardias se cierra el modelo.
            this->gameFinished=true;
        if(!guards[i].checkIfInitialized())     //Si no está inicializado, a ese guardia se va a inicializar.
            break;
    }
    guards[i].init(guardPos, guardDiePos);
    list<CardLocation> path = board.getShortestPath(guardPos, guardDiePos);
    guards[i].setNewPathToTarget(path);
}
void BurgleBrosModel::generateGuardInitPos(CardLocation *guardPos, CardLocation *guardDiePos)
{
    unsigned int i;
    for(i=0; i<BOARD_STANDARD_FLOORS; i++)
    {    
        if(i==BOARD_STANDARD_FLOORS-1 && guards[i].checkIfInitialized())        //Si se llamo a esta función ya habiendo incializado los 3 guardias se cierra el modelo.
            this->gameFinished=true;
        if(!guards[i].checkIfInitialized())     //Si no está inicializado, a ese guardia se va a inicializar.
            break;
    }
    guards[i].init();
    *guardPos = guards[i].getPosition();
    *guardDiePos = guards[i].getTargetPosition();
    list<CardLocation> path = board.getShortestPath(*guardPos, *guardDiePos);
    guards[i].setNewPathToTarget(path);
}
vector<wall> BurgleBrosModel::getInfo2DrawWalls()
{
    vector<wall> aux;
    aux.reserve(BOARD_STANDARD_FLOORS * NUMBER_OF_WALLS);
    board.getWalls(aux);
    return aux;
}
vector<Info2DrawCards> BurgleBrosModel::getInfo2DrawCards()
{
    vector<Info2DrawCards> retVal;
    retVal.reserve(BOARD_STANDARD_FLOORS * FLOOR_RAWS * FLOOR_COLUMNS);
    CardLocation aux;
    for(aux.floor=0; aux.floor < BOARD_STANDARD_FLOORS; aux.floor++)
    {
        for(aux.row=0; aux.row < FLOOR_RAWS; aux.row++)
        {
            for(aux.column=0; aux.column < FLOOR_COLUMNS; aux.column++)
            {
                Info2DrawCards toPush;
                toPush.isVisible = board.isCardVisible(aux);
                toPush.location = aux;
                toPush.safeNumber = board.getCardSafeNumber(aux);
                toPush.type = board.getCardType(aux);
                retVal.push_back(toPush);
            }
        }
    }
    return retVal;
}
list<Info2DrawTokens> BurgleBrosModel::getInfo2DrawTokens()
{
    list<Info2DrawTokens> retVal;
    list<CardLocation> auxList = tokens.getAlarmsList();
    vector<CardLocation> downstairsTokens = tokens.getDownstairs();
    unsigned int aux= tokens.howManyTokensOnCPURoom((CardName)COMPUTER_ROOM_FINGERPRINT);
    unsigned int  i,j;
    Info2DrawTokens toPush;
    
    if(tokens.getCrowToken().first)
    {    
        toPush.token=CROW_TOKEN;
        toPush.position=tokens.getCrowToken().second;
        retVal.push_back(toPush);
    }
    pair<bool, CardLocation> auxiliar= tokens.getPersianKittyToken();
    if(auxiliar.first)
    {
        toPush.token=PERSIAN_KITTY_TOKEN;
        toPush.position=auxiliar.second;
        retVal.push_back(toPush);
    }
    toPush.token=DOWNSTAIRS_TOKEN;
    for(auto& token : downstairsTokens)
    {
        toPush.position=token;
        retVal.push_back(toPush);
    }
    
    toPush.token=ALARM_TOKEN;
    for(auto& token : auxList)
    {
        toPush.position=token;
        retVal.push_back(toPush);
    }
    
    auxList = tokens.getCrackedCards();
    toPush.token= SAFE_TOKEN;
    for(auto& token : auxList)
    {
        toPush.position=token;
        retVal.push_back(toPush);
    }
    pair<CardLocation, unsigned int> temp = tokens.getStealthTokensOnFloor();
    for(unsigned int i=0; i < temp.second; i++)
    {
        toPush.position=temp.first;
        toPush.token= STEALTH_TOKEN;
        retVal.push_back(toPush);
    }
    
    //keypad tokens
    auxList= tokens.getKeypadTokens();
    toPush.token= KEYPAD_TOKEN;
    for(auto& token : auxList)
    {
        toPush.position=token;
        retVal.push_back(toPush);
    }

    
    for(i=0; i < aux; i++)
    {
        toPush.position = board.getComputerRoomLocation(COMPUTER_ROOM_FINGERPRINT);
        toPush.token= HACK_TOKEN;
        retVal.push_back(toPush);
    }
    aux= tokens.howManyTokensOnCPURoom(COMPUTER_ROOM_LASER);
    for(i= 0; i <  aux; i++)
    {
        toPush.position= board.getComputerRoomLocation(COMPUTER_ROOM_LASER);
        toPush.token= HACK_TOKEN;
        retVal.push_back(toPush);
    }
    aux= tokens.howManyTokensOnCPURoom(COMPUTER_ROOM_MOTION);
    for(i= 0; i <  aux; i++)
    {
        toPush.position= board.getComputerRoomLocation(COMPUTER_ROOM_MOTION);
        toPush.token= HACK_TOKEN;
        retVal.push_back(toPush);
    }
    return retVal;
}
list<Info2DrawLoot> BurgleBrosModel::getInfo2DrawLoot()
{
    list<Info2DrawLoot> retVal;
    unsigned int aux= loots.getCurrentLoots();
    for(unsigned int i =0; i< aux; i++)
    {
        Info2DrawLoot loot = loots.getLootInfo(i);
        retVal.push_back(loot);
    }
    return retVal;
}

Info2DrawGuard BurgleBrosModel::getInfo2DrawGuard(unsigned int floor)
{
    Info2DrawGuard info;
    if(floor<=2)
    { 
        info.dieNumber=guards[floor].getDiceNumber();
        info.diePosition=guards[floor].getTargetPosition();
        info.position=guards[floor].getPosition();
        info.shownDeck=guards[floor].getShownDeck();
        info.initialized=guards[floor].checkIfInitialized();
        info.isTopOfNotShownDeckVisible=guards[floor].isTopOfNotShownDeckVisible();
        info.topOfNotShownDeck=guards[floor].getTopCard();
    }
    return info;
}
Info2DrawPlayer BurgleBrosModel:: getInfo2DrawPlayer(PlayerId player)
{
    Info2DrawPlayer info;
    BurgleBrosPlayer * p = getP2Player(player);
    info.name=p->getName();
    info.character=p->getCharacter();
    info.lives=p->getCurrLifes();
    info.position=p->getPosition();
    info.currActions=p->getcurrentActions();
    info.turn = p->isItsTurn();
    info.isOnBoard = p->isOnBoard();
    return info;
}

vector<unsigned int> BurgleBrosModel::getInfo2DrawExtraDices()
{
    return dice.getCurrDice();
}

pair<bool,CardLocation> BurgleBrosModel::getGoldBarInfo()
{
    return loots.getGoldBarOnFloor();
}

importantEvents BurgleBrosModel::getInfoOfEvents()
{
    return iE;
}

PlayerId BurgleBrosModel::getPlayerOnTurn()
{
    if(myPlayer.isItsTurn())
        return THIS_PLAYER;
    else if(otherPlayer.isItsTurn())
        return OTHER_PLAYER;
    else
        return NON_PLAYER;
}
bool BurgleBrosModel::isGuardsTurn()
{
    return (getPlayerOnTurn()==NON_PLAYER);
}

bool BurgleBrosModel::hasGameFinished()
{
    return gameFinished;
}
string BurgleBrosModel::getFinishMsg()
{
    return finishMsg;
}
CardLocation BurgleBrosModel::locationOfComputerRoomOrLavatory(CardName computerRoomOrLavatory)
{
    if(computerRoomOrLavatory == LAVATORY)
        return board.getLavatoryLocation();
    else
        return board.getComputerRoomLocation(computerRoomOrLavatory);
}

bool BurgleBrosModel::isThereACpuRoomOrLavatory(CardLocation pos, CardName whichTypeOfTile) {
    if(whichTypeOfTile == LAVATORY)
        return pos == board.getLavatoryLocation();
    else
        return pos == board.getComputerRoomLocation(whichTypeOfTile);
}

ModelStatus BurgleBrosModel::getModelStatus()
{
    return status;
}
vector<string> BurgleBrosModel::getMsgToShow()
{
    return msgsToShow;
}
bool BurgleBrosModel::userDecidedTo(string userChoice)
{   /*Esta funcion se encarga de procesar la elección del usuario a entrar a tiles especiales, del de este jugador y del otro jugador.*/
    bool guardHasToMove=false;
    BurgleBrosPlayer * movingPlayer= getP2Player(getPlayerOnTurn()); 
    vector<string> fingerPrint({ENTER_FINGERPRINT_TEXT});
    vector<string> lavatory({LAVATORY_TEXT});
    vector<string> laser({LASER_TEXT});
    vector<string> motion({MOTION_TEXT});
    vector<string> deadbolt({DEADBOLT_TEXT});
    vector<string> askForLoot({ASK_FOR_LOOT_TEXT});
    vector<string> offerLoot({OFFER_LOOT_TEXT});
    vector<string> spotter({SPOTTER_SPECIAL_ACTION_TEXT});
    if(msgsToShow[2]== motion[2])     //Si era una respuesta para un motion
    {
        if(userChoice==USE_HACK_TOKEN_TEXTB)        // y uso hack tokens
            tokens.removeOneHackTokenOf(COMPUTER_ROOM_MOTION);
        else if(userChoice==TRIGGER_ALARM_TEXTB)        //Sino, triggerea la alarma
        {    
            triggerAlarm(prevLoc);
            if(movingPlayer->getcurrentActions() == 0 && getPlayerOnTurn() == THIS_PLAYER && !specialMotionCase) //SI es de este player justo cuando termina su turno, se tiene que enviar el paquete del guardia.
                guardHasToMove=true;
        }
    }
    else if(msgsToShow[2]==deadbolt[2])
    {
        if(userChoice==SPEND_ACTIONS_TEXTB)//decide gastar las acciones y entra
        {
            for(unsigned int i=0;i<2;++i)//PODRIA SER UN DEFINE EL 2
                movingPlayer->decActions();
        }
        else if(userChoice==GET_BACK_TEXTB)//decide no gastar las acciones y vuelve atras
            movingPlayer->setPosition(prevLoc);
    }
    else if(msgsToShow[2]==fingerPrint[2])
    {
        if(userChoice==USE_HACK_TOKEN_TEXTB)// clickeo "use hack token" 
            tokens.removeOneHackTokenOf(COMPUTER_ROOM_FINGERPRINT);
        else if(userChoice==TRIGGER_ALARM_TEXTB)//clickeo "trigger alarm"
        {
            triggerAlarm(movingPlayer->getPosition());  //Así se pone el dado a donde tiene que ir, este lo necesitaba desde el guardia
            if(movingPlayer->getcurrentActions() == 0 && getPlayerOnTurn() == THIS_PLAYER) //SI es de este player justo cuando termina su turno, se tiene que enviar el paquete del guardia.
                guardHasToMove=true;
        }
    }
    else if(msgsToShow[2]==laser[2])
    {
        if(userChoice==TRIGGER_ALARM_TEXTB)
            triggerAlarm(movingPlayer->getPosition());
        else if(userChoice==USE_HACK_TOKEN_TEXTB)
            tokens.removeOneHackTokenOf(COMPUTER_ROOM_LASER);
        else if(userChoice==SPEND_ACTION_TEXTB)
            movingPlayer->decActions();
        if(movingPlayer->getcurrentActions() == 0 && getPlayerOnTurn() == THIS_PLAYER && msgsToShow.size()==5 && msgsToShow[4]==USE_HACK_TOKEN_TEXTB && userChoice==TRIGGER_ALARM_TEXTB)
            guardHasToMove=true; //En el caso que: this player se mete a un laser y se queda sin acciones, pero tenía token y decide no usar un token, no se manda ningun paquete, por lo cual se tiene que manda el paquete de guardmove.
    }
    else if(msgsToShow[2]==lavatory[2])
    {
        if(status==WAITING_FOR_USER_CONFIRMATION)
        {
            if(userChoice ==USE_MY_STEALTH_TOKEN_TEXTB)
            {
                movingPlayer->decLives();
                if(movingPlayer->getcurrentActions() == 0 && getPlayerOnTurn() == THIS_PLAYER) //SI es de este player justo cuando termina su turno, se tiene que enviar el paquete del guardia.
                        guardHasToMove=true;
            }
            else if(userChoice ==USE_LAVATORY_TOKEN_TEXTB)
                tokens.useLavatoryToken();
            status=WAITING_FOR_ACTION;
        }
        else if(status==DESPUES_VEMOS_A)
        {
            if(userChoice ==USE_MY_STEALTH_TOKEN_TEXTB)
                myPlayer.decLives();
            else if(userChoice ==USE_LAVATORY_TOKEN_TEXTB)
                tokens.useLavatoryToken();
        }
        else if(status==DESPUES_VEMOS_B)
        {
            if(userChoice ==USE_MY_STEALTH_TOKEN_TEXTB)
                otherPlayer.decLives();
            else if(userChoice ==USE_LAVATORY_TOKEN_TEXTB)
                tokens.useLavatoryToken();
            nmbrOfPendingQuestions--; 
            if(nmbrOfPendingQuestions==0)
            {
                status=WAITING_FOR_ACTION;
                endGuardMove();
            }    
        }    
    }
    else if(msgsToShow[0]==askForLoot[0])       //Si se estaba esperando por la respuesta a un ask for loot
    {
        if(userChoice==ACCEPT_TEXTB)        //Y se respondió que se acepta
        {   
            PlayerId playerOnTurn = getPlayerOnTurn();
            getP2Player(playerOnTurn)->attachLoot(lootOfferedOrAskedFor);   //Se le pone el loot al player on turn
            getP2OtherPlayer(playerOnTurn)->deattachLoot(lootOfferedOrAskedFor);    //Se le saca al otro jugador
            loots.setNewLootOwner(lootOfferedOrAskedFor,playerOnTurn);
        }
    }
    else if(msgsToShow[0]== offerLoot[0])
    {
        if(userChoice==ACCEPT_TEXTB)
        {    
            PlayerId playerOnTurn = getPlayerOnTurn();
            getP2OtherPlayer(playerOnTurn)->attachLoot(lootOfferedOrAskedFor);
            getP2Player(playerOnTurn)->deattachLoot(lootOfferedOrAskedFor);
            loots.setNewLootOwner(lootOfferedOrAskedFor,playerOnTurn==THIS_PLAYER?OTHER_PLAYER:THIS_PLAYER);
            notifyAllObservers();
        }
    }
    else if(msgsToShow[2] == spotter[2])
    {
        guards[getP2Player(getPlayerOnTurn())->getPosition().floor].setTopOfNotShownDeckVisible(false); //Dejo de mostrarla.
        if(userChoice==SPOTTER_BOTTOM)
            guards[getP2Player(getPlayerOnTurn())->getPosition().floor].pushTopCardToTheBottom();
        getP2Player(getPlayerOnTurn())->decActions();
        playerSpentFreeAction=true;
        notifyAllObservers();
        checkTurns();
    }
    if(msgsToShow[2]== motion[2] && specialMotionCase)      //Si fue el caso especial de motion salen 2 carteles seguidos, o sea se espera por una confirmación más.
    {
        if(!auxMsgsToShow.empty())
        {
            status=WAITING_FOR_USER_CONFIRMATION;
            this->msgsToShow=auxMsgsToShow;
            auxMsgsToShow.clear();
        }
        else        //Si estaba vacía era el caso especial para el keypad.
            status=WAITING_FOR_DICE;
        specialMotionCase=false;
    }   
    else if(msgsToShow[2]!=lavatory[2])
        status=WAITING_FOR_ACTION;
    notifyAllObservers();
    checkTurns();
    notifyAllObservers();
    return guardHasToMove;
}

void BurgleBrosModel::setDice(vector<unsigned int> &currDice)
{
    bool keyCracked;
    BurgleBrosPlayer * movingPlayer= getP2Player(getPlayerOnTurn());
    if(currDice.empty())        //Si estaba vacío significa que el jugador que hizo el set dice fue el de esta pc
    {       //Entonces se tiran random en este programa
        if(movingPlayer->getCharacter()==THE_PETERMAN)      //Si es el peterman tira 1 dado más
            keyCracked=dice.throwDiceForKeypadWithExtraDie(movingPlayer->getPosition());
        else
            keyCracked=dice.throwDiceForKeypad(movingPlayer->getPosition());    
        currDice=dice.getCurrDice();
    }
    else
    {
        dice.setDice(currDice); //SI los jhizo el otro jugador se copian.
        keyCracked=dice.didDiceUnlockKeypad();
    }
    if(keyCracked)  //SI se crackeo, se pone un token, o sino se lo devuelve a su posición anterior
    {
        tokens.putKeyPadToken(movingPlayer->getPosition());
        iE = KEYPAD_OPENED;
        notifyAllObservers();
        iE = NO_IE;
        //soundManager->playSoundEffect(KEYPAD_OPENED);
        if(movingPlayer->getPosition() == guards[movingPlayer->getPosition().floor].getPosition())
            movingPlayer->decLives();       //si habia un guardia al entrar al keypad, pierde una vida
    }
    else
    {
        dice.addDieToKeypad(movingPlayer->getPosition());
        movingPlayer->setPosition(prevLoc);
    }
    status=WAITING_FOR_ACTION;
    notifyAllObservers();
    checkTurns();
    notifyAllObservers();
}
void BurgleBrosModel::pass(PlayerId playerId)
{
    bool actionOk=false;
    BurgleBrosPlayer * p=getP2Player(playerId);
    if(p->isItsTurn() && !gameFinished)
    {
        while(p->getcurrentActions())
            p->decActions();
        if(board.getCardType(p->getPosition()) == THERMO && board.isCardVisible(p->getPosition()))
            triggerAlarm(p->getPosition());
        checkTurns();
        notifyAllObservers();
        actionOk=true;
    }
    if(actionOk==false)
    {   gameFinished=true; finishMsg = "ERROR: BBModel error: A pass action was called when it wasnt possible to do it!"; }
}
unsigned int BurgleBrosModel::peek(PlayerId playerId, CardLocation locationToPeek, int safeNumber)
{
    unsigned int retVal;
    bool actionOk=false;
    BurgleBrosPlayer *p=getP2Player(playerId);
    if(isPeekPosible(playerId, locationToPeek) && !gameFinished)
    {
        if(playerId == THIS_PLAYER)
             retVal=board.setCardVisible(locationToPeek);
        else
             retVal=board.setCardVisible(locationToPeek, safeNumber);
        if(p->getCharacter() == THE_HAWK && playerSpentFreeAction==false && board.isAWallBetween(p->getPosition(), locationToPeek))
            playerSpentFreeAction=true;
        else
            p->decActions();
        if(board.getCardType(locationToPeek)==LAVATORY)
            tokens.lavatoryRevealed(locationToPeek); 
        checkTurns();
        notifyAllObservers();
        actionOk=true;      
    }
    if(actionOk==false)
    {   gameFinished=true; finishMsg = "ERROR: BBModel error :A peek action was called when it wasnt possible to do it!"; }
    return retVal;
}
unsigned int BurgleBrosModel::move(PlayerId playerId, CardLocation locationToMove, int safeNumber)
{
    bool actionOk=false;
    unsigned int retVal;
    if(isMovePosible(playerId, locationToMove) && !gameFinished)
    {
        BurgleBrosPlayer* movingPlayer=getP2Player(playerId);
        BurgleBrosPlayer* playerNotMoving=getP2OtherPlayer(playerId);
        CardLocation prevLocation=movingPlayer->getPosition();
        CardName newCardType=board.getCardType(locationToMove);
        bool cardWasVisible=true;
        
        if( !board.isCardVisible(locationToMove) )
        {
            if(playerId == OTHER_PLAYER)
                retVal=board.setCardVisible(locationToMove, safeNumber);
            else
                retVal=board.setCardVisible(locationToMove);
            cardWasVisible=false;
        }    
        else
            retVal=board.getCardSafeNumber(locationToMove);
        movingPlayer->decActions();
        movingPlayer->setPosition(locationToMove);
        
        if(locationToMove.floor != prevLocation.floor && (board.getCardType(prevLocation) == STAIR || tokens.isThereADownstairToken(prevLocation)))
        {
            iE = STAIRS;
            notifyAllObservers();
            iE = NO_IE;
            //soundManager->playSoundEffect(STAIRS_STEPS);
        }
        
        //Cambios segun el lugar desde el que me muevo
        if(board.getCardType(prevLocation)==MOTION && board.isMotionActivated())
        {
            if(tokens.howManyTokensOnCPURoom(COMPUTER_ROOM_MOTION))
            {
                std::vector<string> aux({MOTION_TEXT,USE_HACK_TOKEN_TEXTB,TRIGGER_ALARM_TEXTB});
                this->msgsToShow=aux;
                status=WAITING_FOR_USER_CONFIRMATION; //Ahora el modelo va a esperar la respuesta del usuario.
                this->prevLoc=prevLocation;
                handleSpecialMoveFromMotion(locationToMove,&cardWasVisible); //EN el caso en que se mueve a otra carte que pida cartel, se encarga esta función de arreglarlo.
            }
            else 
                triggerAlarm(prevLocation);
            board.deActivateMotion();
        }
        //cambios segun el charatcer
        if(movingPlayer->getCharacter()==THE_ACROBAT && locationToMove==guards[locationToMove.floor].getPosition())
            movingPlayer->setActions(movingPlayer->getcurrentActions()+1);
        
        //cambios segun loot
        if(movingPlayer->hasLoot(GEMSTONE) && locationToMove==playerNotMoving->getPosition())
            movingPlayer->decActions();
        if(movingPlayer->hasLoot(TIARA) && board.adjacentCards(locationToMove,guards[locationToMove.floor].getPosition()))
            movingPlayer->decLives();
        
        
        notifyAllObservers();
        if(locationToMove==guards[locationToMove.floor].getPosition() && board.getCardType(locationToMove)!= LAVATORY && movingPlayer->getCharacter()!=THE_ACROBAT)
            if( !(board.getCardType(locationToMove) == KEYPAD && !tokens.isThereAKeypadToken(locationToMove)) )     //esto cubre el caso de que te rebote el keypad
                movingPlayer->decLives();
        
        CardLocation downstairsLocationToMove={locationToMove.floor-1, locationToMove.row, locationToMove.column};
        if(prevLocation==downstairsLocationToMove && !tokens.isThereADownstairToken(locationToMove))
            tokens.placeDownstairToken(locationToMove);
        
        
        //Cambios segun el tipo de carta al que me movi
        
        //Si me movi a un atrium y hay un guard arriba o abajo se pierde una vida
        if(newCardType==ATRIUM &&
                ( (locationToMove.floor>0 && board.isCardDownstairs(locationToMove,guards[locationToMove.floor-1].getPosition()) ) || ( locationToMove.floor<2 && board.isCardUpstairs(locationToMove,guards[locationToMove.floor+1].getPosition()) ) ) )
            movingPlayer->decLives();
        //Si me movi a una camara y hay un guardia en otra camara activo una alarma en donde estoy
        if( newCardType==CAMERA && GuardInCamera() && locationToMove!= guards[locationToMove.floor].getPosition() ) 
            triggerAlarm(locationToMove);
        //Si me movi a un foyer y hay un guardia en un tile adyacente me ve (a menos que haya una pared)
        if( newCardType== FOYER && board.adjacentCards(locationToMove, guards[locationToMove.floor].getPosition() ) )
            movingPlayer->decLives();
        //Si me movi a un deadbolt tengo que gastar 3 acciones para entrar o vuelvo a donde estaba
        if( newCardType==DEADBOLT && locationToMove!=guards[locationToMove.floor].getPosition() && (locationToMove!=playerNotMoving->getPosition() || (locationToMove==playerNotMoving->getPosition() && !playerNotMoving->isOnBoard())))
        {   
            if(movingPlayer->getcurrentActions()<2 && !cardWasVisible)
                movingPlayer->setPosition(prevLocation);
            else if(!specialMotionCase)
            {
                std::vector<string> aux({DEADBOLT_TEXT,SPEND_ACTIONS_TEXTB,GET_BACK_TEXTB});
                this->msgsToShow=aux;
                this->status=WAITING_FOR_USER_CONFIRMATION; //Pone al modelo en el estado de espera por la respuesta del usuario.
                this->prevLoc=prevLocation;
            }
                
        }    
        //Si quiero entrar a un keypad y no esta abierto tengo que tirar los dados (el numero de dados se corresponde con los intentos en el mismo turno)
        if( newCardType==KEYPAD && !tokens.isThereAKeypadToken(locationToMove) && !specialMotionCase)
        {
            bool keyCracked=false;
            this->status=WAITING_FOR_DICE;
            this->prevLoc=prevLocation;
        }
        
        if(!tokens.isThereAnAlarmToken(locationToMove) && movingPlayer->getCharacter()!=THE_HACKER && ( playerNotMoving->getCharacter()!=THE_HACKER || (playerNotMoving->getCharacter()==THE_HACKER && (locationToMove!= playerNotMoving->getPosition() || (locationToMove== playerNotMoving->getPosition() && !playerNotMoving->isOnBoard())) ) ) && guards[locationToMove.floor].getPosition()!=locationToMove)
        {    
            if( newCardType==FINGERPRINT)//hay que arreglar el tema de cuando hace click en la cruz del native message
            {
                   if(tokens.howManyTokensOnCPURoom(COMPUTER_ROOM_FINGERPRINT) && !specialMotionCase )//Si hay tokens disponibles
                   {
                      std::vector<string> aux({ENTER_FINGERPRINT_TEXT,USE_HACK_TOKEN_TEXTB,TRIGGER_ALARM_TEXTB});  //Esto contiene el título del cartelito, subtitulo y texto, por eso vector
                      this->msgsToShow=aux;
                      this->status=WAITING_FOR_USER_CONFIRMATION;
                   }
                   else if (!tokens.howManyTokensOnCPURoom(COMPUTER_ROOM_FINGERPRINT))
                       triggerAlarm(locationToMove);
            }   

            if( newCardType==MOTION)
                board.activateMotion();//hay que marcar que se entro en este turno y si sale en el mismo turno tiene que gastar un token o activar una alarma, en el proximo ya puede salir

            if(newCardType==LASER && !movingPlayer->hasLoot(MIRROR))
            {   
                if( !(tokens.howManyTokensOnCPURoom(COMPUTER_ROOM_LASER)) && !(movingPlayer->getcurrentActions()) )
                    triggerAlarm(locationToMove);
                else if(!specialMotionCase)
                {
                    std::vector<string> aux({LASER_TEXT,TRIGGER_ALARM_TEXTB}); 
                    if(tokens.howManyTokensOnCPURoom(COMPUTER_ROOM_LASER))
                        aux.push_back(USE_HACK_TOKEN_TEXTB);
                    if(movingPlayer->getcurrentActions())
                        aux.push_back(SPEND_ACTION_TEXTB);
                    this->msgsToShow=aux;
                    this->status=WAITING_FOR_USER_CONFIRMATION;
                }
            }
        }
        
        if(newCardType==LAVATORY)
        {
            if(!cardWasVisible)
                tokens.lavatoryRevealed(locationToMove);

            if(locationToMove==guards[locationToMove.floor].getPosition() && tokens.isThereAStealthToken(locationToMove))
            {
                if(!specialMotionCase)
                {
                    vector<string> aux({LAVATORY_TEXT,USE_LAVATORY_TOKEN_TEXTB,USE_MY_STEALTH_TOKEN_TEXTB});
                    this->msgsToShow=aux;
                    status=WAITING_FOR_USER_CONFIRMATION;
                }
            }    
            else if(locationToMove==guards[locationToMove.floor].getPosition() && !tokens.isThereAStealthToken(locationToMove))
                movingPlayer->decLives();
        }    
        
        
        if( newCardType==SCANNER_DETECTOR && movingPlayer->carriesLoot())
            triggerAlarm(locationToMove);
        
        if(newCardType==THERMO && ( movingPlayer->getcurrentActions()==0 || movingPlayer->hasLoot(ISOTOPE)))
            triggerAlarm(locationToMove);
        
        if(newCardType==WALKAWAY && !cardWasVisible && locationToMove.floor>0)
        {   
            CardLocation downStairsLocation={locationToMove.floor-1,locationToMove.row,locationToMove.column};
            movingPlayer->setPosition(downStairsLocation);
        }

        checkTurns();
        notifyAllObservers();
        actionOk=true;
    }
    if(actionOk==false)
    {   gameFinished=true; finishMsg = "ERROR: BBModel error: A peek action was called when it wasnt possible to do it!"; }
    return retVal;
}
void BurgleBrosModel::handleSpecialMoveFromMotion(CardLocation movingToTile, bool *cardWasVisible)
{
    CardName moveTo=board.getCardType(movingToTile);
    BurgleBrosPlayer * playerMoving = getP2Player(getPlayerOnTurn());
    BurgleBrosPlayer * playerNotMoving = getP2OtherPlayer(getPlayerOnTurn());
    if(moveTo==FINGERPRINT && tokens.howManyTokensOnCPURoom(COMPUTER_ROOM_FINGERPRINT))
    {
        specialMotionCase=true;
        std::vector<string> aux({ENTER_FINGERPRINT_TEXT,USE_HACK_TOKEN_TEXTB,TRIGGER_ALARM_TEXTB});  //Esto contiene el título del cartelito, subtitulo y texto, por eso vector
        this->auxMsgsToShow=aux;
    }                                                                                                                                   //Mayor igual que dos porque ya se gasto una acción en el move.
    else if(moveTo==DEADBOLT && movingToTile!=guards[movingToTile.floor].getPosition() && movingToTile!=playerNotMoving->getPosition() && playerMoving->getcurrentActions()>=2)
    {   
        specialMotionCase=true;
        std::vector<string> aux({DEADBOLT_TEXT,SPEND_ACTIONS_TEXTB,GET_BACK_TEXTB});  //Esto contiene el título del cartelito, subtitulo y texto, por eso vector
        this->auxMsgsToShow=aux;
    }
    else if(moveTo==KEYPAD && !tokens.isThereAKeypadToken(movingToTile))
    {   
        specialMotionCase=true;
        auxMsgsToShow.clear();      //Para el caso de moverse de un motion que se puede activar y tiene tokens a un keypad que no está abierto:
    }
    else if(moveTo==LAVATORY)
    {
        if(!(*cardWasVisible))
        {   tokens.lavatoryRevealed(movingToTile); *cardWasVisible=true;    }
        if(movingToTile==guards[movingToTile.floor].getPosition() && tokens.isThereAStealthToken(movingToTile))
        {
            specialMotionCase=true;
            vector<string> aux({LAVATORY_TEXT,USE_LAVATORY_TOKEN_TEXTB,USE_MY_STEALTH_TOKEN_TEXTB});
            this->auxMsgsToShow=aux;
        }
    }
    else if(moveTo==LASER &&  !playerMoving->hasLoot(MIRROR) && (tokens.howManyTokensOnCPURoom(COMPUTER_ROOM_LASER) || playerMoving->getcurrentActions()))
    {
        std::vector<string> aux({LASER_TEXT,TRIGGER_ALARM_TEXTB}); 
        if(tokens.howManyTokensOnCPURoom(COMPUTER_ROOM_LASER))
            aux.push_back(USE_HACK_TOKEN_TEXTB);
        if(playerMoving->getcurrentActions())
            aux.push_back(SPEND_ACTION_TEXTB);
        specialMotionCase=true;
        this->auxMsgsToShow=aux;
    }
}

void BurgleBrosModel::addToken(PlayerId playerId, CardLocation locationToAddToken)
{
    bool actionOk=false;
    BurgleBrosPlayer* movingPlayer=getP2Player(playerId);
    if(isAddTokenPosible(playerId, locationToAddToken))
    {
        tokens.addHackTokenOn(board.getCardType(locationToAddToken));
        movingPlayer->decActions();
        checkTurns();
        notifyAllObservers();
        actionOk=true;
    }
    if(actionOk==false)
    {   gameFinished=true; finishMsg = "ERROR: BBModel error: An add token action was called when it wasnt possible to do it!"; }
}
void BurgleBrosModel::addDieToSafe(PlayerId playerId, CardLocation safe)
{
    bool actionOk=false;
    BurgleBrosPlayer* p= getP2Player(playerId);
    if(isAddDieToSafePossible(playerId, safe) && !gameFinished)
    {
        p->decActions();
        p->decActions();
        dice.addDieToSafe(safe.floor); 
        checkTurns();
        notifyAllObservers();
        actionOk=true;
    }
    if(actionOk==false)
    {   gameFinished=true; finishMsg = "ERROR: BBModel error: An add die to safe action was called when it wasnt possible to do it!"; }
}
void BurgleBrosModel::crackSafe(PlayerId playerId,vector<unsigned int> &diceThrown)
{
    bool actionOk=false;
    BurgleBrosPlayer* p= getP2Player(playerId);
    if(isCrackSafePossible(playerId,p->getPosition()) && !gameFinished)
    {
        CardLocation safe= p->getPosition();
        p->decActions();                //Cuesta una acción hacer un crack
        if(playerId==THIS_PLAYER)       //Para este jugador:
        {
            if(p->getCharacter() == THE_PETERMAN)       //Si es peterman tira con un dado extra
                diceThrown=dice.throwDiceForSafeWithExtraDie(safe.floor);
            else                                        //Sino tira los dados normales
                diceThrown=dice.throwDiceForSafe(safe.floor);
        }
        else dice.setDice(diceThrown);  //Si fue el otro jugador se copian
        list<CardLocation> tilesCrackedOnThisAction = board.tilesWithCracked(diceThrown,safe.floor);   //Obtengo las cartas que tienen como safe number uno de los numeros que salio en el dado
        tokens.addCrackTokenOn(tilesCrackedOnThisAction);
        if(tokens.isSafeOpened(safe.floor))
        {
            status=WAITING_FOR_LOOT;    //Espero a que me confirmen el loot.
            prevLoc=safe;           //Guardo a que safe crackeo
            iE = SAFE_CRACKED;
            notifyAllObservers();
            iE = NO_IE;
            //soundManager->playSoundEffect(SAFE_CRACKED);    //check if this needs to be here or in the controller
        }
        notifyAllObservers();
        checkTurns();
        actionOk=true;
    }
    if(actionOk==false)
    {   gameFinished=true; finishMsg = "ERROR: BBModel error: A crack safe action was called when it wasnt possible to do it!"; }
}
void BurgleBrosModel::setLoot(PlayerId playerId, Loot *loot)
{
    BurgleBrosPlayer *p=getP2Player(playerId);
    CardLocation safe= prevLoc;
    if(tokens.isSafeOpened(safe.floor) && status== WAITING_FOR_LOOT)
    {
        Loot lootGotten;
        if(playerId==THIS_PLAYER)
            lootGotten =loots.getLoot(playerId);
        else if(playerId==OTHER_PLAYER)
            lootGotten =loots.getLoot(playerId, *loot);
        p->attachLoot(lootGotten);
        if(lootGotten==GOLD_BAR)
            loots.setGoldBardLocation(safe);
        if(lootGotten==CURSED_GOBLET && p->getCurrLifes()>1)
            p->decLives();
        board.setSafeCracked(safe.floor);
        triggerSilentAlarm(safe.floor);
        *loot=lootGotten;
        status=WAITING_FOR_ACTION;
        notifyAllObservers();
        checkTurns();
    }
    else
    {   gameFinished=true; finishMsg = "ERROR: BBModel error: A set Loot action was called when it wasnt possible to do it!"; }
}

void BurgleBrosModel::createAlarm(PlayerId playerId, CardLocation tile)
{
    bool actionOk=false;
    if(isCreateAlarmPossible(playerId,tile) && !gameFinished)
    {
        playerSpentFreeAction=true;
        triggerAlarm(tile);
        actionOk=true;
        notifyAllObservers();
    }
    if(actionOk==false)
    {   gameFinished=true; finishMsg = "ERROR: BBModel error: A create alarm action was called when it wasnt possible to do it!"; }
}
void BurgleBrosModel::placeCrow(PlayerId playerId, CardLocation tile)
{
    bool actionOk=false;
    if(isPlaceCrowPossible(playerId,tile) && !gameFinished)
    {
        tokens.placeCrowToken(tile);
        playerSpentFreeAction=true;
        actionOk=true;
        notifyAllObservers();
    }
    if(actionOk==false)
    {   gameFinished=true; finishMsg = "ERROR: BBModel error: A place crow action was called when it wasnt possible to do it!"; }
}
void BurgleBrosModel::pickLoot(PlayerId playerId)
{
    bool actionOk=false;
    BurgleBrosPlayer *p= getP2Player(playerId);
    if(!gameFinished)
    {
        if(isPickLootPossible(playerId,PERSIAN_KITTY))          //Si es el persian kitty
        {
            pair<bool, CardLocation> kittyInfo;
            kittyInfo.first= false;
            tokens.placePersianKittyToken(kittyInfo);
            loots.setNewLootOwner(PERSIAN_KITTY, playerId);
            p->attachLoot(PERSIAN_KITTY);
            notifyAllObservers();
            actionOk=true;
        }
        if(isPickLootPossible(playerId, GOLD_BAR))
        {
            loots.pickGoldBarOnFloor(playerId, p->getPosition());
            p->attachLoot(GOLD_BAR);
            notifyAllObservers();
            actionOk=true;
        }
    }
    if(actionOk==false)
    {   gameFinished=true; finishMsg = "ERROR: BBModel error: A pick loot action was called when it wasnt possible to do it!"; }
}

string BurgleBrosModel::peekGuardsCard(PlayerId playerId, CardLocation **guardCard, unsigned int guardsFloor, string prevChoice)
{
    bool actionOk = false;
    string userChoice;
    if(isPeekGuardsCardPossible(playerId, guardsFloor) && !gameFinished)
    {
        if(*guardCard != nullptr) 
            guards[guardsFloor].pushCardToTheTop(**guardCard);  //significa que guardCard es la carta que espio el otro jugador
        else
        {
            spyGuardCard = guards[guardsFloor].getTopCard();
            *guardCard=&spyGuardCard;        //sino, es un movimiento mio, asi que guardo la carta que espio para mandarla
        }
        guards[guardsFloor].setTopOfNotShownDeckVisible(true);      //Muestro la carta de arriba
        notifyAllObservers();
        
        if(prevChoice == SPOTTER_NO_PREV_CHOICE && playerId==THIS_PLAYER)   //Para este jugador le pregunta por patalla
        {
            vector<string>msgToShow({SPOTTER_SPECIAL_ACTION_TEXT,SPOTTER_TOP,SPOTTER_BOTTOM});
            this->msgsToShow=msgToShow;
            status=WAITING_FOR_USER_CONFIRMATION;
        }
        else
        {
            userChoice=prevChoice;      //Sino es lo pasado por argumento.
            std::chrono::seconds second(1);
            std::this_thread::sleep_for(second); //Se duerme un segundo para mostrar la carta que saco el otro pj.
            guards[guardsFloor].setTopOfNotShownDeckVisible(false); //Dejo de mostrarla.
            if(userChoice==SPOTTER_BOTTOM)
                guards[guardsFloor].pushTopCardToTheBottom();
            getP2Player(playerId)->decActions();
            playerSpentFreeAction=true;
            notifyAllObservers();
            checkTurns();
        }
        actionOk=true;
    }
    if(actionOk==false)
    {   gameFinished=true; finishMsg = "ERROR: BBModel error: A peek guards card action was called when it wasnt possible to do it!"; }
    return userChoice;
}

void BurgleBrosModel::askForLoot(PlayerId playerId, Loot loot)
{
    bool actionOk = false;
    if(isAskForLootPossible(playerId,getP2Player(playerId)->getPosition(),loot) && !gameFinished)
    {
        
        std::vector<string> aux({ASK_FOR_LOOT_TEXT+loot2Str(loot),ACCEPT_TEXTB,DECLINE_TEXTB}); 
        this->msgsToShow=aux;
        this->status=IN_LOOT_EXCHANGE;
        this->lootOfferedOrAskedFor=loot;
        actionOk=true;
    }
    if(actionOk==false)
    {   gameFinished=true; finishMsg = "ERROR: BBModel error: An ask for loot action was called when it wasnt possible to do it!"; }
}

void BurgleBrosModel::offerLoot(PlayerId playerId, Loot loot)
{
    bool actionOk = false;
    if(isOfferLootPossible(playerId,getP2Player(playerId)->getPosition(),loot) && !gameFinished)
    {
        std::vector<string> aux({OFFER_LOOT_TEXT+loot2Str(loot),ACCEPT_TEXTB,DECLINE_TEXTB}); 
        this->msgsToShow=aux;
        this->status=IN_LOOT_EXCHANGE;
        this->lootOfferedOrAskedFor=loot;
        actionOk=true;
    }
    if(actionOk==false)
    {   gameFinished=true; finishMsg = "ERROR: BBModel error: An offer loot action was called when it wasnt possible to do it!"; }
}
void BurgleBrosModel::escape(PlayerId playerId, CardLocation stairTile)
{
    bool actionOk=false;
    BurgleBrosPlayer *p = getP2Player(playerId);
    if(isEscapePossible(playerId,stairTile) && !gameFinished)
    {   
        while(p->getcurrentActions())
            p->decActions();
        p->getToDaChoppa(); 
        iE = STAIRS;
        notifyAllObservers();
        iE = NO_IE;
        //soundManager->playSoundEffect(STAIRS_STEPS);
        notifyAllObservers();
        checkTurns();
        notifyAllObservers();
        actionOk=true;
    }
    if(actionOk==false)
    {   gameFinished=true; finishMsg = "ERROR: BBModel error: An escape action was called when it wasnt possible to do it!"; }
}

bool BurgleBrosModel::GuardInCamera() 
{
    bool GuardOnCamera=false;
    for(unsigned int i=0; i<BOARD_STANDARD_FLOORS; ++i)
    {
        if(guards[i].checkIfInitialized() && board.getCardType(guards[i].getPosition())== CAMERA && board.isCardVisible(guards[i].getPosition()))//chequeo que la camara este dada vuelta
        {    
            GuardOnCamera=true;
            break;
        }    
    }
    return GuardOnCamera;
} 

void BurgleBrosModel::checkTurns()
{
    if(myPlayer.isItsTurn() && myPlayer.getcurrentActions() == 0 && status==WAITING_FOR_ACTION)
    {
        if(board.getCardType(myPlayer.getPosition()) == THERMO && board.isCardVisible(myPlayer.getPosition()))
            triggerAlarm(myPlayer.getPosition());
        myPlayer.setTurn(false);
        playerOnTurnBeforeGuardMove=THIS_PLAYER;
      /*  moveGuard(myPlayer.getPosition().floor); //Se comenta para probar los moves entre las 2 pcs
        if(!otherPlayer.isOnHelicopter())
            otherPlayer.setTurn(true);
        else
            myPlayer.setTurn(true);
        handlePersianKittyMove(OTHER_PLAYER);
        handleChihuahuaMove(OTHER_PLAYER);*/
        playerSpentFreeAction=false;
        board.deActivateMotion();
    }
    else if(otherPlayer.isItsTurn() && otherPlayer.getcurrentActions() == 0 && status==WAITING_FOR_ACTION)
    {
        if(board.getCardType(otherPlayer.getPosition()) == THERMO && board.isCardVisible(otherPlayer.getPosition()))
            triggerAlarm(otherPlayer.getPosition());
        otherPlayer.setTurn(false);
        playerOnTurnBeforeGuardMove=OTHER_PLAYER;
        //moveGuard(otherPlayer.getPosition().floor);
      /*  if(!myPlayer.isOnHelicopter())
            myPlayer.setTurn(true);
        else
            otherPlayer.setTurn(true);
        handlePersianKittyMove(THIS_PLAYER);
        handleChihuahuaMove(THIS_PLAYER);*/
        playerSpentFreeAction=false;
        board.deActivateMotion();
    }
    else if(isGuardsTurn() && guardFinishedMoving==true)
    {
        BurgleBrosPlayer *nextPlayerOnTurn=getP2OtherPlayer(playerOnTurnBeforeGuardMove); //El jugador que termina ahora el turno va a ser el que no terminó antes que el guardia
        if(nextPlayerOnTurn->isOnHelicopter())
            nextPlayerOnTurn=getP2Player(playerOnTurnBeforeGuardMove);  //Si el otro jugador estaba en el helicopter vuelve a jugar el primero.
        if(!nextPlayerOnTurn->isOnBoard() && !nextPlayerOnTurn->isOnHelicopter()&& guards[nextPlayerOnTurn->getPosition().floor].getPosition()== nextPlayerOnTurn->getPosition())
            nextPlayerOnTurn->decLives();
        nextPlayerOnTurn->setTurn(true);
        nextPlayerOnTurn->setActions(INIT_NMBR_OF_ACTIONS);       //Se le resetean las acciones.
        if(nextPlayerOnTurn->hasLoot(MIRROR))
            nextPlayerOnTurn->decActions();     //SI tenía un mirror pierde 1 acción
        if(nextPlayerOnTurn->hasLoot(CHIHUAHUA) || nextPlayerOnTurn->hasLoot(PERSIAN_KITTY))
            status=WAITING_DICE_FOR_LOOT;
        // handlePersianKittyMove(nextPlayerOnTurn);
       // handleChihuahuaMove(nextPlayerOnTurn);
        guardFinishedMoving=false;
        dice.resetKeypadsDice();
        notifyAllObservers();
    }
    if(!gameFinished)
        checkIfWonOrLost();
}

void BurgleBrosModel::checkIfWonOrLost()
{
    if(myPlayer.isOnHelicopter() && otherPlayer.isOnHelicopter())
    {
        gameFinished=true;
        finishMsg= "WON";
        iE = WON;
        notifyAllObservers();
        iE = NO_IE;
        //soundManager->playSoundEffect(WON);     //check if this needs to be here or in the controller
        myPlayer.setTurn(false);
        otherPlayer.setTurn(false);
    }
#ifndef INMORTAL
    else if(myPlayer.getCurrLifes()==0 || otherPlayer.getCurrLifes()==0)
    {
        gameFinished=true;
        finishMsg= "LOST";
        iE = LOST;
        notifyAllObservers();
        iE = NO_IE;
        //soundManager->playSoundEffect(LOST);    //check if this needs to be here or in the controller
        myPlayer.setTurn(false);
        otherPlayer.setTurn(false);
    }
    
#endif
}

bool BurgleBrosModel::isMovePosible(PlayerId playerId,CardLocation tileToMove)
{
    BurgleBrosPlayer * playerMoving = getP2Player(playerId);
    CardLocation playerMovingPos= playerMoving->getPosition();
    bool retVal=false;
    if(playerMoving->isItsTurn()&& status == WAITING_FOR_ACTION)
    {
        if(board.adjacentCards(playerMovingPos, tileToMove))
            retVal=true;
        if(board.isCardVisible(playerMovingPos))
        {
            if(board.getCardType(playerMovingPos) == STAIR  && board.isCardUpstairs(playerMovingPos, tileToMove))
               retVal=true;
            if(board.isCardVisible(tileToMove) && board.getCardType(playerMovingPos) == WALKAWAY  &&  board.isCardDownstairs(playerMovingPos, tileToMove))
               retVal=true;
            if(board.isCardVisible(tileToMove) && board.getCardType(playerMovingPos) == SERVICE_DUCT && (board.getOtherServiceDuctPos(playerMovingPos)== tileToMove))
               retVal=true;
        }
        if(board.getCardType(tileToMove) == SECRET_DOOR && board.isAWallBetween(playerMovingPos,tileToMove) && board.isCardVisible(tileToMove))//No hay que ver que tambien la card sea visible?
            retVal=true;
        if(tokens.isThereAToken(playerMovingPos, DOWNSTAIRS_TOKEN) && board.isCardDownstairs(playerMovingPos, tileToMove))
            retVal=true;
        if((playerMoving->getcurrentActions() <3 )&& board.isCardVisible(tileToMove) &&(board.getCardType(tileToMove) == DEADBOLT )&& !(guards[playerMoving->getPosition().floor].getPosition() == tileToMove) && (!(getP2OtherPlayer(playerId)->getPosition() == tileToMove) || ((getP2OtherPlayer(playerId)->getPosition() == tileToMove) && !getP2OtherPlayer(playerId)->isOnBoard())))
            retVal=false;
        if(board.getCardType(playerMovingPos) == SERVICE_DUCT && (board.getOtherServiceDuctPos(playerMovingPos)== tileToMove) && playerMoving->hasLoot(PAINTING) && board.isCardVisible(tileToMove))
            retVal=false;
        if(board.getCardType(tileToMove) == SECRET_DOOR && board.isAWallBetween(playerMovingPos,tileToMove) && playerMoving->hasLoot(PAINTING))
            retVal=false;
         //Falta la del persian kitty o chihuahua
        if(playerMoving->getcurrentActions() == 1 && playerMoving->hasLoot(GEMSTONE) && (getP2OtherPlayer(playerId)->getPosition() == tileToMove))
           retVal=false;
    }
    return retVal;
} 
bool BurgleBrosModel:: isPeekPosible(PlayerId player, CardLocation tile)
{
    bool retVal=false;
    BurgleBrosPlayer* p=getP2Player(player);
    if(p->isItsTurn() && (!board.isCardVisible(tile)) && status == WAITING_FOR_ACTION)
    {
        if(board.adjacentCards(p->getPosition(),tile))
            retVal=true;
        else if(p->getCharacter()==THE_HAWK && board.isAWallBetween(p->getPosition(),tile)&& !playerSpentFreeAction)//HACER FUNCION WALLLSEPARATES
            retVal=true;
        else if( board.getCardType(p->getPosition())== ATRIUM && ( board.isCardUpstairs(p->getPosition(),tile) || board.isCardDownstairs(p->getPosition(),tile) ) )
            retVal=true;
        else if( board.getCardType(p->getPosition())== STAIR && board.isCardUpstairs(p->getPosition(),tile)  )
            retVal=true;
    }
    return retVal;
}
bool BurgleBrosModel::isAddTokenPosible(PlayerId player, CardLocation tile)
{
    bool retVal=false;
    BurgleBrosPlayer* p;
    p = getP2Player(player);
    if(p->isItsTurn() && (board.isCardVisible(tile)) && p->getPosition() == tile && status == WAITING_FOR_ACTION)
    {
        if(IS_COMPUTER_ROOM(board.getCardType(tile)) && tokens.howManyTokensOnCPURoom(board.getCardType(tile)) < MAX_HACK_TOKENS_ON_COMPUTER_ROOM)
            retVal=true;
    }
    return retVal;
}
bool BurgleBrosModel::isAddDieToSafePossible(PlayerId player, CardLocation tile)
{
    bool retVal=false;
    BurgleBrosPlayer* p;
    p = getP2Player(player);
    if(p->isItsTurn() && p->getcurrentActions()>= 2 && board.getCardType(p->getPosition())==SAFE && p->getPosition()==tile && status == WAITING_FOR_ACTION)
    {
        if(!board.isSafeCracked(tile.floor) && dice.getSafeDiceCount(tile.floor) < MAX_NMBR_OF_EXTRA_DICES)
            retVal=true;
    }
    return retVal;
}
bool BurgleBrosModel::isCrackSafePossible(PlayerId playerId,CardLocation safe)
{
    bool retVal=false;
    BurgleBrosPlayer* p = getP2Player(playerId);
    
    if(p->isItsTurn()&& board.getCardType(p->getPosition())==SAFE  && p->getPosition()==safe && status == WAITING_FOR_ACTION)
    {
        if(board.canSafeBeCracked(safe.floor) && !board.isSafeCracked(safe.floor))
        {
            if(dice.getSafeDiceCount(safe.floor)!= 0)
                retVal=true;
            else if (p->getCharacter()==THE_PETERMAN && dice.getSafeDiceCount(safe.floor)==0)
                retVal=true;
            if(getP2OtherPlayer(playerId)->hasLoot(KEYCARD) && getP2OtherPlayer(playerId)->getPosition()!=safe)
                retVal=false;
        }  
    }
    return retVal;
}
bool BurgleBrosModel::isCreateAlarmPossible(PlayerId playerId, CardLocation tile)
{
    bool retVal=false;
    BurgleBrosPlayer* p=getP2Player(playerId);
    if(p->isItsTurn() && p->getCharacter() == THE_JUICER && playerSpentFreeAction==false && board.adjacentCards(p->getPosition(),tile)&& guards[tile.floor].getPosition()!=tile && status == WAITING_FOR_ACTION)
        retVal=true;
    return retVal;
}
bool BurgleBrosModel::isPlaceCrowPossible(PlayerId playerId, CardLocation tile)
{
    bool retVal=false;
    BurgleBrosPlayer* p=getP2Player(playerId);
    if(p->isItsTurn() && p->getCharacter()== THE_RAVEN && tile.floor == p->getPosition().floor && board.getShortestPathLength(p->getPosition(), tile) <= 2 && playerSpentFreeAction==false && status == WAITING_FOR_ACTION)
    {
        retVal=true;
        if(tokens.getCrowToken().first && tokens.getCrowToken().second==tile)
            retVal=false;
    }
    return retVal;
}

bool BurgleBrosModel::isAskForLootPossible(PlayerId playerId, CardLocation tile, Loot loot)
{
    bool retVal = false;
    BurgleBrosPlayer * p = getP2Player(playerId);
    BurgleBrosPlayer * o = getP2OtherPlayer(playerId);
    if(p->isItsTurn() && p->getPosition() == tile && o->getPosition() == tile && o->hasLoot(loot) && status == WAITING_FOR_ACTION)
    {
        if(!o->isOnHelicopter())
            retVal = true;
        if(loot == GOLD_BAR && p->hasLoot(loot))
            retVal = false;
    }
    return retVal;
}

bool BurgleBrosModel::isOfferLootPossible(PlayerId playerId, CardLocation tile, Loot loot)
{
    bool retVal = false;
    BurgleBrosPlayer * p = getP2Player(playerId);
    BurgleBrosPlayer * o = getP2OtherPlayer(playerId);
    if(p->isItsTurn() && p->getPosition() == tile && o->getPosition() == tile && p->hasLoot(loot) && status == WAITING_FOR_ACTION)
    {
        if(!o->isOnHelicopter())
            retVal = true;
        if(loot == GOLD_BAR && o->hasLoot(loot))
            retVal = false;
    }
    return retVal;
}
bool BurgleBrosModel::isPickLootPossible(PlayerId playerId, Loot lootToPick)
{
    bool retVal = false;
    BurgleBrosPlayer * p = getP2Player(playerId);
    if(p->isItsTurn() && status == WAITING_FOR_ACTION)
    {
        if(lootToPick == PERSIAN_KITTY && tokens.isThereAPersianKittyToken(p->getPosition()))
            retVal=true;
        else if(lootToPick == GOLD_BAR && loots.canPlayerPickUpGoldBarOnFloor(playerId,p->getPosition()))
            retVal=true;
    }
    return retVal;
}
bool BurgleBrosModel::isEscapePossible(PlayerId playerId, CardLocation tile)
{
    bool retVal = false;
    BurgleBrosPlayer * p = getP2Player(playerId);
    BurgleBrosPlayer * o = getP2OtherPlayer(playerId);
    if(p->isItsTurn() && p->getPosition()==tile && board.getCardType(tile)==STAIR && tile.floor==2 && tokens.areAllSafesOpen() && status == WAITING_FOR_ACTION )
    {
        if(!loots.areLootsOnFloor())    //Si no hay ningun loot en el piso, se puede escapar
            retVal=true;
        else if(p->hasLoot(GOLD_BAR) && !o->hasLoot(GOLD_BAR))  //Si el que se escapar tiene un gold bar y el otro no la agarró todavia, puede escapar.
            retVal=true;
        else if(!o->isOnHelicopter() && tokens.getPersianKittyToken().first)    //Si se quiere escapar, el persian kitty está en el piso, pero el otro jugador todavia no subio al heli, puede.
            retVal=true;
    }
    return retVal;
}
bool BurgleBrosModel::isPeekGuardsCardPossible(PlayerId playerId, unsigned int guardsFloor)
{
    bool retVal = false;
    BurgleBrosPlayer * p = getP2Player(playerId);
    if(p->isItsTurn() && p->getCharacter()== THE_SPOTTER && p->getPosition().floor == guardsFloor && status == WAITING_FOR_ACTION && playerSpentFreeAction==false)
        retVal=true;
    return retVal;
}
list<string> BurgleBrosModel::getPosibleActionsToTile(PlayerId player, CardLocation tile)
{
    list<string> aux;
    if(isMovePosible(player, tile))
        aux.push_back("MOVE");
    if(isPeekPosible(player, tile))
        aux.push_back("PEEK");
    if(isAddTokenPosible(player, tile))
        aux.push_back("ADD TOKEN");
    if(isAddDieToSafePossible(player, tile))
        aux.push_back("ADD DIE");
    if(isCrackSafePossible(player, tile))
        aux.push_back("CRACK");
    if(isCreateAlarmPossible(player,tile))
        aux.push_back("CREATE ALARM");
    if(isPlaceCrowPossible(player,tile))
        aux.push_back("PLACE CROW");
    for(int i = (int)TIARA; i <= (int)GOLD_BAR; i++)
    {
        string ask = "ASK FOR ";
        string offer = "OFFER ";
        ask += loot2Str((Loot)i);
        offer += loot2Str((Loot)i);
        transform(ask.begin(), ask.end(), ask.begin(), ::toupper);
        transform(offer.begin(), offer.end(), offer.begin(), ::toupper);
        if(isAskForLootPossible(player,tile,(Loot)i))
            aux.push_back(ask);
        if(isOfferLootPossible(player,tile,(Loot)i))
            aux.push_back(offer);
    }
    if(isPickLootPossible(player, GOLD_BAR) && isPickLootPossible(player,PERSIAN_KITTY) )
        aux.push_back("PICK UP LOOTS");
    else
    {
        if(isPickLootPossible(player,PERSIAN_KITTY))
            aux.push_back("PICK UP KITTY");
        else if(isPickLootPossible(player,GOLD_BAR))
            aux.push_back("PICK UP GOLD BAR");
    }
    if(isEscapePossible(player,tile))
        aux.push_back("ESCAPE");
    return aux;
}
list<string> BurgleBrosModel::getPosibleActionsToGuard(PlayerId player, unsigned int guardsFloor)
{
    list<string> aux;
    if(isPeekGuardsCardPossible(player, guardsFloor))
        aux.push_back("PEEK TOP CARD");
    return aux;
}
 
BurgleBrosPlayer * BurgleBrosModel::getP2Player(PlayerId playerId)
{
    if(playerId==THIS_PLAYER)
        return &myPlayer;
    else
        return &otherPlayer;
}
BurgleBrosPlayer * BurgleBrosModel::getP2OtherPlayer(PlayerId playerId)
{
    if(playerId==THIS_PLAYER)
        return &otherPlayer;
    else
        return &myPlayer;
}    
void BurgleBrosModel::guardMove()
{
    if(gWholePath.first.empty())    //Si se mandó una lista vacía, se hace el movimiento del guardia
    {
        gWholePath.first = generateGuardPath();
        gWholePath.second = gWholePath.first.begin();
    }
    copyGuardMove();
    //guardFinishedMoving=true;
    checkTurns();
}

void BurgleBrosModel::setGuardWholePath(list<GuardMoveInfo> wholePath)
{
    this->gWholePath.first = wholePath;
}

bool BurgleBrosModel::anotherLavatoryInGPath()
{
    bool retVal = false;
    list<GuardMoveInfo>::iterator it = gWholePath.second; it++;
    for(; it != gWholePath.first.end(); it++)
        if(it->meaning == GUARD_STEP_TO && board.getCardType(it->cardLocation) == LAVATORY)
        {
            retVal = true;
            break;
        }
    return retVal;
}

void BurgleBrosModel::endGuardMove() 
{
    unsigned int guardFloor= getP2Player(playerOnTurnBeforeGuardMove)->getPosition().floor;   //Obtengo la posición del 
    BurgleBrosGuard *guardMoving = &(guards[guardFloor]);
    if(tokens.isThereAnAlarmToken(guardMoving->getPosition()))     //Si hay una alarma en su posición ya la desactiva y busca un nuevo camino.
        tokens.turnOffAlarm(guardMoving->getPosition());
    if(getP2Player(playerOnTurnBeforeGuardMove)->getCharacter() == THE_ACROBAT && getP2Player(playerOnTurnBeforeGuardMove)->getPosition() == guardMoving->getPosition())
        getP2Player(playerOnTurnBeforeGuardMove)->decLives();
    for(; gWholePath.second != gWholePath.first.end() && !gameFinished; gWholePath.second++)
    {
        if(gWholePath.second->meaning==GUARD_STEP_TO)
        {
            guardMoving->setPosition(gWholePath.second->cardLocation);
            if(tokens.isThereAnAlarmToken(guardMoving->getPosition()))     //Si hay una alarma en su posición ya la desactiva y busca un nuevo camino.
                tokens.turnOffAlarm(guardMoving->getPosition());
            
            if(guardMoving->getPosition() == myPlayer.getPosition() && myPlayer.isOnBoard())   //Si el guardia entra al tile del player, el mismo pierde una vida.
                myPlayer.decLives();
            if(guardMoving->getPosition() == otherPlayer.getPosition() && otherPlayer.isOnBoard())
                otherPlayer.decLives();
            if(board.isCardVisible(guardMoving->getPosition()))
            {   
                if(board.getCardType(guardMoving->getPosition()) == CAMERA && board.getCardType(myPlayer.getPosition()) == CAMERA && board.isCardVisible(myPlayer.getPosition()))   //Si un guardia se mueve a una camara y hay un player en una camara
                    if(guardMoving->getPosition() != myPlayer.getPosition())      //Y ese player no está en la misma camara que el guardia
                        triggerAlarm(myPlayer.getPosition());   //Triggerea una alarma en el piso que se encuentra 
                if(board.getCardType(guardMoving->getPosition()) == CAMERA && board.getCardType(otherPlayer.getPosition()) == CAMERA && board.isCardVisible(otherPlayer.getPosition()))
                    if(guardMoving->getPosition() != otherPlayer.getPosition())
                        triggerAlarm(otherPlayer.getPosition());
            }
            /*Si un player esta sobre un atrium dado vuelta, y el guardia pasa un piso arriba o abajo de ese player, este pierde una vida*/
            if(board.getCardType(myPlayer.getPosition()) == ATRIUM && board.isCardVisible(myPlayer.getPosition()) && (board.isCardDownstairs(myPlayer.getPosition(), guardMoving->getPosition()) || board.isCardUpstairs(myPlayer.getPosition(), guardMoving->getPosition())))
                myPlayer.decLives();
            if(board.getCardType(otherPlayer.getPosition()) == ATRIUM && board.isCardVisible(otherPlayer.getPosition()) && (board.isCardDownstairs(otherPlayer.getPosition(), guardMoving->getPosition()) || board.isCardUpstairs(otherPlayer.getPosition(), guardMoving->getPosition())))
                otherPlayer.decLives();
            /*Si un player esta sobre un Foyer dado vuelta, y el guardia se encuentra en un tile adyacente, este pierde una vida*/
            if(board.getCardType(myPlayer.getPosition()) == FOYER && board.isCardVisible(myPlayer.getPosition()) && board.adjacentCards(myPlayer.getPosition(), guardMoving->getPosition()))
                myPlayer.decLives();
            if(board.getCardType(otherPlayer.getPosition()) == FOYER && board.isCardVisible(otherPlayer.getPosition()) && board.adjacentCards(otherPlayer.getPosition(), guardMoving->getPosition()))
                otherPlayer.decLives();   
        }
        else if(gWholePath.second->meaning==GUARD_CARD_PICK)
            guardMoving->drawCardTarget(gWholePath.second->cardLocation);
        notifyAllObservers();
        checkIfWonOrLost();
        std::chrono::milliseconds milliseconds(500);
        std::this_thread::sleep_for(milliseconds);
    }
    setGuardsNewPath(guardFloor, guardMoving->getTargetPosition());//Para que no quede sin un camino  si la próxima vez se ejecuta desde esta cpu.
}

void BurgleBrosModel::copyGuardMove()
{
    if(board.getCardType(guards[getP2Player(playerOnTurnBeforeGuardMove)->getPosition().floor].getPosition()) == LAVATORY)
    {    
        if(!(tokens.isThereAStealthToken(guards[getP2Player(playerOnTurnBeforeGuardMove)->getPosition().floor].getPosition())))
            status=WAITING_FOR_ACTION;
        else if(status == DESPUES_VEMOS_A && myPlayer.getPosition() != otherPlayer.getPosition())
            status = WAITING_FOR_ACTION;
        else if(status == DESPUES_VEMOS_A && myPlayer.getPosition() == otherPlayer.getPosition())
        {
            if(anotherLavatoryInGPath())
            {
                status=WAITING_FOR_ACTION;
                if(playerOnTurnBeforeGuardMove == THIS_PLAYER)      //i am sending the guard movement
                    nmbrOfPendingQuestions++;
            }
            else
            {
                if(playerOnTurnBeforeGuardMove == THIS_PLAYER)      //i am sending the guard movement
                    status=DESPUES_VEMOS_B;
                else
                    status=WAITING_FOR_ACTION;
            }
        }
    }    
    unsigned int guardFloor= getP2Player(playerOnTurnBeforeGuardMove)->getPosition().floor;   //Obtengo la posición del 
    BurgleBrosGuard *guardMoving = &(guards[guardFloor]);
    if(tokens.isThereAnAlarmToken(guardMoving->getPosition()))     //Si hay una alarma en su posición ya la desactiva y busca un nuevo camino.
        tokens.turnOffAlarm(guardMoving->getPosition());
    if(getP2Player(playerOnTurnBeforeGuardMove)->getCharacter() == THE_ACROBAT && getP2Player(playerOnTurnBeforeGuardMove)->getPosition() == guardMoving->getPosition())
        getP2Player(playerOnTurnBeforeGuardMove)->decLives();
    for(; gWholePath.second != gWholePath.first.end() && !gameFinished; gWholePath.second++)
    {
        if(gWholePath.second->meaning==GUARD_STEP_TO)
        {
            guardMoving->setPosition(gWholePath.second->cardLocation);
            if(tokens.isThereAnAlarmToken(guardMoving->getPosition()))     //Si hay una alarma en su posición ya la desactiva y busca un nuevo camino.
                tokens.turnOffAlarm(guardMoving->getPosition());
            
            if(guardMoving->getPosition() == myPlayer.getPosition() && board.getCardType(myPlayer.getPosition())==LAVATORY && tokens.isThereAStealthToken(myPlayer.getPosition()))
                status=DESPUES_VEMOS_A;
            else if(guardMoving->getPosition() == myPlayer.getPosition() && myPlayer.isOnBoard())   //Si el guardia entra al tile del player, el mismo pierde una vida.
                myPlayer.decLives();
            
            if(guards[getP2Player(playerOnTurnBeforeGuardMove)->getPosition().floor].getPosition() == otherPlayer.getPosition() && board.getCardType(otherPlayer.getPosition())==LAVATORY && tokens.isThereAStealthToken(otherPlayer.getPosition()))
            {    
                if(anotherLavatoryInGPath())    
                    nmbrOfPendingQuestions++;
                else
                    status=DESPUES_VEMOS_B;
            }
            else if(guardMoving->getPosition() == otherPlayer.getPosition() && otherPlayer.isOnBoard())
                otherPlayer.decLives();
            if(board.isCardVisible(guardMoving->getPosition()))
            {   
                if(board.getCardType(guardMoving->getPosition()) == CAMERA && board.getCardType(myPlayer.getPosition()) == CAMERA && board.isCardVisible(myPlayer.getPosition()))   //Si un guardia se mueve a una camara y hay un player en una camara
                    if(guardMoving->getPosition() != myPlayer.getPosition())      //Y ese player no está en la misma camara que el guardia
                        triggerAlarm(myPlayer.getPosition());   //Triggerea una alarma en el piso que se encuentra 
                if(board.getCardType(guardMoving->getPosition()) == CAMERA && board.getCardType(otherPlayer.getPosition()) == CAMERA && board.isCardVisible(otherPlayer.getPosition()))
                    if(guardMoving->getPosition() != otherPlayer.getPosition())
                        triggerAlarm(otherPlayer.getPosition());
            }
            /*Si un player esta sobre un atrium dado vuelta, y el guardia pasa un piso arriba o abajo de ese player, este pierde una vida*/
            if(board.getCardType(myPlayer.getPosition()) == ATRIUM && board.isCardVisible(myPlayer.getPosition()) && (board.isCardDownstairs(myPlayer.getPosition(), guardMoving->getPosition()) || board.isCardUpstairs(myPlayer.getPosition(), guardMoving->getPosition())))
                myPlayer.decLives();
            if(board.getCardType(otherPlayer.getPosition()) == ATRIUM && board.isCardVisible(otherPlayer.getPosition()) && (board.isCardDownstairs(otherPlayer.getPosition(), guardMoving->getPosition()) || board.isCardUpstairs(otherPlayer.getPosition(), guardMoving->getPosition())))
                otherPlayer.decLives();
            /*Si un player esta sobre un Foyer dado vuelta, y el guardia se encuentra en un tile adyacente, este pierde una vida*/
            if(board.getCardType(myPlayer.getPosition()) == FOYER && board.isCardVisible(myPlayer.getPosition()) && board.adjacentCards(myPlayer.getPosition(), guardMoving->getPosition()))
                myPlayer.decLives();
            if(board.getCardType(otherPlayer.getPosition()) == FOYER && board.isCardVisible(otherPlayer.getPosition()) && board.adjacentCards(otherPlayer.getPosition(), guardMoving->getPosition()))
                otherPlayer.decLives();   
        }
        else if(gWholePath.second->meaning==GUARD_CARD_PICK)
            guardMoving->drawCardTarget(gWholePath.second->cardLocation);
        notifyAllObservers();
        checkIfWonOrLost();
        std::chrono::milliseconds milliseconds(500);
        std::this_thread::sleep_for(milliseconds);
    }
    setGuardsNewPath(guardFloor, guardMoving->getTargetPosition());//Para que no quede sin un camino  si la próxima vez se ejecuta desde esta cpu.
}
list<GuardMoveInfo> BurgleBrosModel::generateGuardPath() 
{
    BurgleBrosGuard *guardMoving= new BurgleBrosGuard;
    unsigned int guardFloor = getP2Player(playerOnTurnBeforeGuardMove)->getPosition().floor;
    *guardMoving = guards[guardFloor];
    list<CardLocation> alarmList = tokens.getAlarmsList();
    GuardMoveInfo auxiliarInfoToReport;
    list<GuardMoveInfo> retVal;
    //unsigned int stepsToMove= tokens.howManyAlarmsOnFloor(guardFloor) + guardMoving->getDiceNumber(); //El guardia se mueve en su turno los pasos de sus dados + la cantidad de alarmas en su piso
    if(status==WAITING_FOR_ACTION)
        guardMoving->setSteps(tokens.howManyAlarmsOnFloor(guardFloor) + guardMoving->getDiceNumber());
    
    bool targetReached=false;
    while(guardMoving->hasStepsLeft() && !gameFinished)
    {
        targetReached = guardMoving->step();//step ya decrementa los steps left
  
        if(board.isCardVisible(guardMoving->getPosition()))
        {   
            if(board.getCardType(guardMoving->getPosition()) == CAMERA && board.getCardType(myPlayer.getPosition()) == CAMERA && board.isCardVisible(myPlayer.getPosition()))   //Si un guardia se mueve a una camara y hay un player en una camara
                if(guardMoving->getPosition() != myPlayer.getPosition())      //Y ese player no está en la misma camara que el guardia
                    addWithoutRepeating(alarmList, myPlayer.getPosition());
            if(board.getCardType(guardMoving->getPosition()) == CAMERA && board.getCardType(otherPlayer.getPosition()) == CAMERA && board.isCardVisible(otherPlayer.getPosition()))
                if(guardMoving->getPosition() != otherPlayer.getPosition())
                    addWithoutRepeating(alarmList, otherPlayer.getPosition());
        }

        /*Armo el paquete para informar del movimiento:*/
        auxiliarInfoToReport.meaning=GUARD_STEP_TO;        //Se hizo un movimiento de guardia
        auxiliarInfoToReport.cardLocation=guardMoving->getPosition();   //A la posición que está ahora
        retVal.push_back(auxiliarInfoToReport);  //pusheo
        /*Si el guardia llegó a la posición objetivo, busca un nuevo objetivo*/
        if(targetReached)
        {
            for(auto& alarm : alarmList)
            {
                if(alarm == guardMoving->getPosition()) alarmList.remove(alarm);
            }
            list<CardLocation> cardsTaken=setGuardsNewPath(alarmList, guardMoving); //Setea un nuevo path y devuelve la lista de cartas que tomó del mazo. ( pueden ser 1 o 2 si tomo una carta que apuntaba a donde estaba parado).
            auxiliarInfoToReport.meaning=GUARD_CARD_PICK;   
            for(auto& card : cardsTaken)
            {
                auxiliarInfoToReport.cardLocation=card;
                retVal.push_back(auxiliarInfoToReport);  //Guardo que cartas se tomaron del mazo.
            }
        }
        /*Si había un crow token en el tile donde se encuentra*/
        if(tokens.isThereAToken(guardMoving->getPosition(), CROW_TOKEN))
            guardMoving->decSteps();//si no habia steps la funcion no hace nada
    }
}
void addWithoutRepeating(list<CardLocation> &alarmList, CardLocation item)
{
    bool isRepeated = false;
    for(auto& alarm : alarmList)
    {
        if(alarm == item)
        {
            isRepeated = true;
            break;
        }
    }
    if(!isRepeated) alarmList.push_back(item);
}

list<CardLocation> BurgleBrosModel::setGuardsNewPath(list<CardLocation> &alarmList, BurgleBrosGuard *p2Guard)
{
    list<CardLocation> retVal;
    list<AuxStruct> alarmsOnSameFloor;
    CardLocation newTargetLocation;
    unsigned int aux=0;
    for(auto& alarm : alarmList)
    {
        AuxStruct aux;
        if(alarm.floor == p2Guard->getPosition().floor)
        {
             aux.target=alarm;
             alarmsOnSameFloor.push_back(aux);       //Obtengo las alarmas que están en el mismo piso  
        }
    }
    if(!alarmsOnSameFloor.empty())          //Si hay alarmas en su piso
    {
        for(auto& alarm : alarmsOnSameFloor)
            alarm.length=board.getShortestPathLength(p2Guard->getPosition(), alarm.target);    //obtengo el largo de cada camino.
        alarmsOnSameFloor.sort(sortAuxStruct);  //las ordeno por el camino mas corto
        for(auto& alarm : alarmsOnSameFloor)
        {
            if(alarm.length == alarmsOnSameFloor.front().length)
                aux++;  //Cuento cuantas alarmas estan a la misma distancia.
        }
        if(aux==1)  //Si hay una que este a la minima distancia
            newTargetLocation=alarmsOnSameFloor.front().target; //Esa va a ser el nuevo objetivo
        else
        {
            int temp = rand() % aux;            //Sino, se elige random entre las que tienen el mismo largo de camino
            list<AuxStruct>::iterator it = alarmsOnSameFloor.begin();
            advance(it, temp);
            newTargetLocation = it->target;
        }
    }
    else        //Si no había alarmas, se toma una carta  que no sea en la que esté parado
    {
        do{
            newTargetLocation = p2Guard->drawCardTarget();     //Toma una nueva carta objetivo
            retVal.push_back(newTargetLocation);                //la guarda en el retval
        }while(newTargetLocation == p2Guard->getPosition());      //si era la misma posición que la del guardia, saca otra carta del mazo.
    }
    p2Guard->setNewTarget(newTargetLocation);
    list<CardLocation> temp = board.getShortestPath(p2Guard->getPosition(), newTargetLocation);
    p2Guard->setNewPathToTarget(temp);
    return retVal;
}

list<CardLocation> BurgleBrosModel::setGuardsNewPath(unsigned int floor)
{
    list<CardLocation> retVal;
    list<CardLocation> alarmList = tokens.getAlarmsList();
    list<AuxStruct> alarmsOnSameFloor;
    CardLocation newTargetLocation;
    unsigned int aux=0;
    for(auto& alarm : alarmList)
    {
        AuxStruct aux;
        if(alarm.floor == floor)
        {
             aux.target=alarm;
             alarmsOnSameFloor.push_back(aux);       //Obtengo las alarmas que están en el mismo piso  
        }
    }
    if(!alarmsOnSameFloor.empty())          //Si hay alarmas en su piso
    {
        for(auto& alarm : alarmsOnSameFloor)
            alarm.length=board.getShortestPathLength(guards[floor].getPosition(), alarm.target);    //obtengo el largo de cada camino.
        alarmsOnSameFloor.sort(sortAuxStruct);  //las ordeno por el camino mas corto
        for(auto& alarm : alarmsOnSameFloor)
        {
            if(alarm.length == alarmsOnSameFloor.front().length)
                aux++;  //Cuento cuantas alarmas estan a la misma distancia.
        }
        if(aux==1)  //Si hay una que este a la minima distancia
            newTargetLocation=alarmsOnSameFloor.front().target; //Esa va a ser el nuevo objetivo
        else
        {
            int temp = rand() % aux;            //Sino, se elige random entre las que tienen el mismo largo de camino
            list<AuxStruct>::iterator it = alarmsOnSameFloor.begin();
            advance(it, temp);
            newTargetLocation = it->target;
        }
    }
    else        //Si no había alarmas, se toma una carta  que no sea en la que esté parado
    {
        do{
            newTargetLocation = guards[floor].drawCardTarget();     //Toma una nueva carta objetivo
            retVal.push_back(newTargetLocation);                //la guarda en el retval
        }while(newTargetLocation == guards[floor].getPosition());      //si era la misma posición que la del guardia, saca otra carta del mazo.
    }
    guards[floor].setNewTarget(newTargetLocation);
    list<CardLocation> temp = board.getShortestPath(guards[floor].getPosition(), newTargetLocation);
    guards[floor].setNewPathToTarget(temp);
    return retVal;
}
list<CardLocation> BurgleBrosModel::setGuardsNewPath(unsigned int floor, CardLocation thisTarget)
{
    list<CardLocation> retVal;
    list<CardLocation> alarmList = tokens.getAlarmsList();
    list<AuxStruct> alarmsOnSameFloor;
    CardLocation newTargetLocation;
    unsigned int aux=0;
    for(auto& alarm : alarmList)
    {
        AuxStruct aux;
        if(alarm.floor == floor)
        {
             aux.target=alarm;
             alarmsOnSameFloor.push_back(aux);       //Obtengo las alarmas que están en el mismo piso  
        }
    }
    if(!alarmsOnSameFloor.empty())          //Si hay alarmas en su piso
    {
        for(auto& alarm : alarmsOnSameFloor)
            alarm.length=board.getShortestPathLength(guards[floor].getPosition(), alarm.target);    //obtengo el largo de cada camino.
        alarmsOnSameFloor.sort(sortAuxStruct);  //las ordeno por el camino mas corto
        for(auto& alarm : alarmsOnSameFloor)
        {
            if(alarm.length == alarmsOnSameFloor.front().length)
                aux++;  //Cuento cuantas alarmas estan a la misma distancia.
        }
        if(aux==1)  //Si hay una que este a la minima distancia
            newTargetLocation=alarmsOnSameFloor.front().target; //Esa va a ser el nuevo objetivo
        else
        {
            int temp = rand() % aux;            //Sino, se elige random entre las que tienen el mismo largo de camino
            list<AuxStruct>::iterator it = alarmsOnSameFloor.begin();
            advance(it, temp);
            newTargetLocation = it->target;
        }
    }
    else        //Si no había alarmas, se toma una carta  que no sea en la que esté parado
        newTargetLocation = thisTarget;  //si era la misma posición que la del guardia, saca otra carta del mazo.
    guards[floor].setNewTarget(newTargetLocation);
    list<CardLocation> temp = board.getShortestPath(guards[floor].getPosition(), newTargetLocation);
    guards[floor].setNewPathToTarget(temp);
    return retVal;
}
void BurgleBrosModel::triggerSilentAlarm(unsigned int floor)
{
    for(int i=floor; i >= 0; i--)
        guards[i].incDiceNumber();
}

bool BurgleBrosModel::dieForLootNeeded() //Si es necesario tirar un dado para el chihuahua o persian kitty
{
    bool retVal=false;
    BurgleBrosPlayer *p=getP2Player(getPlayerOnTurn());
    if((p->hasLoot(PERSIAN_KITTY) || p->hasLoot(CHIHUAHUA))  && status==WAITING_DICE_FOR_LOOT && rollForLootCount==0) //Si se esperaba por el dice for loot y todavía no se tiraron los dados si
        retVal=true;
    else if(p->hasLoot(PERSIAN_KITTY) && p->hasLoot(CHIHUAHUA) && status==WAITING_DICE_FOR_LOOT && rollForLootCount==1 && board.canKittyMove(p->getPosition()))
        retVal=true; //Tambien si tenía los dos loots y tiró por el chihuahua, y ahora si el kitty puede moverse se tiene
    return retVal;
}
void BurgleBrosModel::continueGame()        //Si el juego estaba parado por los dados para el loot, se le dice que continúe.
{
    rollForLootCount=0;
    status=WAITING_FOR_ACTION;
}

unsigned int BurgleBrosModel::rollDieForLoot(unsigned int die)
{
    BurgleBrosPlayer *p=getP2Player(getPlayerOnTurn());
    if(p->hasLoot(PERSIAN_KITTY) && !p->hasLoot(CHIHUAHUA))     //Si tiene persian kitty y no chihuahua
        handlePersianKittyMove(die);
    else if(!p->hasLoot(PERSIAN_KITTY) && p->hasLoot(CHIHUAHUA))    //SI tiene chihuahua y no tiene persian kitty
        handleChihuahuaMove(die);
    else if(p->hasLoot(PERSIAN_KITTY) && p->hasLoot(CHIHUAHUA))     //Si tiene los 2.
    {
        if(rollForLootCount==0)
            handleChihuahuaMove(die);
        else
            handlePersianKittyMove(die);
    }
    return dice.getCurrDice().front();
}

void BurgleBrosModel::handlePersianKittyMove(unsigned int die)
{
    BurgleBrosPlayer *p=getP2Player(getPlayerOnTurn());
    if(loots.isLootDrawn(PERSIAN_KITTY) && board.canKittyMove(p->hasLoot(PERSIAN_KITTY) ? p->getPosition() : tokens.getPersianKittyToken().second))   
    {
        bool itWillMove;
        if(getPlayerOnTurn()==THIS_PLAYER)      //Si es este jugador tira los dados
            itWillMove=dice.persianKittyShallMove();
        else if(getPlayerOnTurn()==OTHER_PLAYER)    //Sino pone el dado recibido
            itWillMove=dice.persianKittyShallMove(die);
        if(itWillMove)
        {
            if(p->hasLoot(PERSIAN_KITTY))
            {
                p->deattachLoot(PERSIAN_KITTY);
                loots.setNewLootOwner(PERSIAN_KITTY,NON_PLAYER);
            }
            pair<bool, CardLocation> persianKittyToken;
            persianKittyToken.first = true;
            persianKittyToken.second = board.getKittyMovingPos(p->getPosition());
            tokens.placePersianKittyToken(persianKittyToken);
            iE = KITTY_ESCAPED;
            notifyAllObservers();
            iE = NO_IE;
        }
        notifyAllObservers();
    }
    rollForLootCount++;
}
void BurgleBrosModel::handleChihuahuaMove(unsigned int die)
{
    BurgleBrosPlayer *p=getP2Player(getPlayerOnTurn());
    if(p->isItsTurn() && p->hasLoot(CHIHUAHUA))
    {
        bool itWillBark;
        if(getPlayerOnTurn()==THIS_PLAYER)  //SI es este jugador tira los dados
            itWillBark=dice.chihuahuaBarks();
        else if(getPlayerOnTurn()==OTHER_PLAYER)
            itWillBark=dice.chihuahuaBarks(die);    //SIno, tira los dados recibidos.
        if(itWillBark)
        {
            iE = CHIHUAHUA_BARKS;
            notifyAllObservers();
            iE = NO_IE;
            //soundManager->playSoundEffect(CHIHUAHUA_BARKS);
            triggerAlarm(p->getPosition());
            //tokens.triggerAlarm(p->getPosition());
            //setGuardsNewPath(p->getPosition().floor);
        }
        notifyAllObservers();
    }
    rollForLootCount++;
}

void BurgleBrosModel::triggerAlarm(CardLocation tile)
{
    if(guards[tile.floor].getPosition() != tile)
    {
        tokens.triggerAlarm(tile);
        setGuardsNewPath(tile.floor);
        iE = ALARM_TRIGGERED;
        notifyAllObservers();
        iE = NO_IE;
        //soundManager->playSoundEffect(ALARM_TRIGGERED);
    }
}

BurgleBrosModel::~BurgleBrosModel()
{
}
