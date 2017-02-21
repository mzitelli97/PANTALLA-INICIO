#include "BurgleBrosController.h"
#include "MouseED.h"
#include "GraphicMenuItem.h"
#include "GraphicGuardCards.h"
#include "GUI.h"
#include "NetworkED.h"
#include "KeyboardED.h"
#include "TimerED.h"
#include <algorithm>

using namespace std;

BurgleBrosController::BurgleBrosController()
{
    modelPointer=nullptr;
    networkInterface=nullptr;
    view=nullptr;
    quit=false;
    status=INITIALIZING;
    initPacketCount=0;
    aMoveActionPending=false;
    resetTimeoutTimer=false;
    waiting4QuitAck=false;
    firstInitDone=false;
    waiting4ack=false;
    resetMouseZ=false;
}

BurgleBrosController::BurgleBrosController(const BurgleBrosController& orig) 
{
}

bool BurgleBrosController::resetZMouse() {
    bool retVal=resetMouseZ;
    if(resetMouseZ==true)
        resetMouseZ=false;
    return retVal;
}


bool BurgleBrosController::hasToResetTimeoutTimer()
{
    bool retVal=false;
    if(resetTimeoutTimer == true)
    {
        resetTimeoutTimer=false;
        retVal=true;
    }
    return retVal;
}

bool BurgleBrosController::isWaiting4ack()
{
    return waiting4ack;
}
void BurgleBrosController::attachNetworkInterface(NetworkInterface * p2NetworkInterface)
{
    this->networkInterface=p2NetworkInterface;
}
void BurgleBrosController::attachModel(BurgleBrosModel *gamePointer)
{
    if(gamePointer!=nullptr)
        this->modelPointer = gamePointer;
}
void BurgleBrosController::attachView(BurgleBrosView *view)
{
    if(view!=nullptr)
        this->view=view;
}

void BurgleBrosController::attachSound(BurgleBrosSound* sound)
{
    if(sound!=nullptr)
    {
        this->sound=sound;
        sound->reset();
    }       
}

void BurgleBrosController::setCommunicationRoleNThisPlayerName(CommunicationRole communicationRole,string name)
{
    this->communicationRole=communicationRole;
    this->thisPlayerName=name;
    if(communicationRole==SERVER)
        networkInterface->sendPacket(NAME);     //Si es el server, tiene que iniciar la conversación.
}

bool BurgleBrosController::checkIfGameFinished()
{
    return quit;
}

string BurgleBrosController::getUsersResponse(vector<string> &message)
{
    string retVal;
    vector<string> fingerPrint({ENTER_FINGERPRINT_TEXT});
    vector<string> lavatory({LAVATORY_TEXT});
    vector<string> laser({LASER_TEXT});
    vector<string> motion({MOTION_TEXT});
    vector<string> deadbolt({DEADBOLT_TEXT});
    if(modelPointer->getPlayerOnTurn() == THIS_PLAYER)      //A este jugador le pregunta con un cartelito por view
    {
        if(!modelPointer->isMotionSpecialCase())    //Para todos los casos salve el caso especial del motion
            retVal=askThisPlayerAndProcess(message);
        else
            retVal=handleThisPlayerMotionSpecialCase(message);
    }
    else                                        //Al otro jugador le pregunta por el paquete recibido en la queue
    {
        if(!modelPointer->isMotionSpecialCase())
            retVal=processOtherPlayerBasicChoice(message);
        else
            retVal=handleOtherPlayerMotionSpecialCase(message);
    }
    packetToAnalize.clear();
    return retVal;
}

string BurgleBrosController::handleThisPlayerMotionSpecialCase(vector<string> &message)     //En este caso primero se tiene que resolver primero el motion y luego el segundo caso.
{
    vector<string> motion({MOTION_TEXT});
    string retVal;
    string userChoice =view->MessageBox(message);
    checkForNonOrderedPackets();
    resetTimeoutTimer=true;
    if(!quit)       //Si em el tiempo que se escogió lo que quería el usuario no hubo un quit, se procesa las cosas y se mandan los paquetes correspondientes.
    {
        
        if(message[2]==motion[2])            //Se tiene que esperar para un motion porque es su caso especial,
        {
            if(userChoice==USE_HACK_TOKEN_TEXTB)    //Si decidió usar token, se manda un paquete.
            {
                networkInterface->sendUseToken(modelPointer->locationOfComputerRoomOrLavatory(COMPUTER_ROOM_MOTION));
                waiting4ack=true;
                retVal=userChoice;          // si fue token devuelve ya de una esto para que procese el modelo
            }
            else    //SI decide no usar tokens para el motion, le pregunta por segunda vez al player 
            {
                modelPointer->userDecidedTo(userChoice); // primero hace que procese el modelo la wea.
                vector<string> secondMsg= modelPointer->getMsgToShow();
                retVal=askThisPlayerAndProcess(secondMsg);
            }
        }
        else
            quit=true; //Esto no debería pasar nunca.
    }
    return retVal;
}

string BurgleBrosController::handleOtherPlayerMotionSpecialCase(vector<string> &message) //En este caso primero se tiene que resolver primero el motion y luego el segundo caso.
{
    vector<string> motion({MOTION_TEXT});
    vector<string> lavatory({LAVATORY_TEXT});
    string retVal;
    if(packetToAnalize.empty()) //Si no mandaron ningun paquete para complementar la info, es cuando de un move que podian llegar 2 paquetes dsps llego un paquete que no era ni use token, ni spent ok, o sea, no eligio use token cuando se podia solo use token o trigger alarm para los 2 tiles
    {
        modelPointer->userDecidedTo(TRIGGER_ALARM_TEXTB);        //Le dice que no gast tokens para el motionh
        vector<string> newMsg = modelPointer->getMsgToShow();
        if(newMsg[2]==lavatory[2])              //Puede ser que se haya negado usar un token de lavatory
            retVal=USE_MY_STEALTH_TOKEN_TEXTB;      
        else                    //Sino siempre la respuesta negativa es triggerear una alarma.
            retVal=TRIGGER_ALARM_TEXTB;
    }           //Si llego un use token, pero no del computer rooom de motion     O  llego un paquete de spent ok de la 2da pregunta a tile
    else if( (packetToAnalize.front().getHeader()==USE_TOKEN && !modelPointer->isThereACpuRoomOrLavatory(packetToAnalize.front().getTokenPos(), COMPUTER_ROOM_MOTION)) || packetToAnalize.front().getHeader()==SPENT_OK)
    {
        modelPointer->userDecidedTo(TRIGGER_ALARM_TEXTB);        //Le dice que no gast tokens para el motionh
        vector<string> newMsg = modelPointer->getMsgToShow();
        retVal=processOtherPlayerBasicChoice(newMsg); 
    } 
    else
        retVal=processOtherPlayerBasicChoice(message);
    return retVal;    
}



string BurgleBrosController::askThisPlayerAndProcess(vector<string> &message)
{
    string retVal;
    bool willWaitForAck=true;
    vector<string> fingerPrint({ENTER_FINGERPRINT_TEXT});
    vector<string> lavatory({LAVATORY_TEXT});
    vector<string> laser({LASER_TEXT});
    vector<string> motion({MOTION_TEXT});
    vector<string> deadbolt({DEADBOLT_TEXT});
    retVal=view->MessageBox(message);   //una vez obtenido lo que el usuario escogió, se tiene que mandar un mensaje con lo que se puso.
    resetTimeoutTimer=true;
    checkForNonOrderedPackets();
    if(!quit)       //Si em el tiempo que se escogió lo que quería el usuario no hubo un quit, se procesa las cosas y se mandan los paquetes correspondientes.
    {
            //EL 2 SIGNIFICA EL TEXTO, los titulos y subtitulos son iguales.
        if(message[2]==fingerPrint[2] && retVal==USE_HACK_TOKEN_TEXTB )  //Si se preguntaba por un fingerprint y el usuario decidió usar un token:
            networkInterface->sendUseToken(modelPointer->locationOfComputerRoomOrLavatory(COMPUTER_ROOM_FINGERPRINT));
        else if (message[2]==laser[2] )   //Si entro a un laser
        {
            if(retVal==USE_HACK_TOKEN_TEXTB)        //y eligio gastar un hack token, se manda con la location del mismo.
                networkInterface->sendUseToken(modelPointer->locationOfComputerRoomOrLavatory(COMPUTER_ROOM_LASER));
            else if (retVal==SPEND_ACTION_TEXTB)
                networkInterface->sendSpent(true);  //Si eligió gastar acciones se manda con este
            else if(message.size()==5 && message[4]==USE_HACK_TOKEN_TEXTB && retVal== TRIGGER_ALARM_TEXTB) // Si solo podia elegir entre use token o trigger alarm y eligio trigger alarm, no manda paquete
                willWaitForAck=false;
            else
                networkInterface->sendSpent(false); //Sino, se manda que no quiso aceptar el gasto de acciones.
        }
        else if(message[2]==lavatory[2] && retVal==USE_LAVATORY_TOKEN_TEXTB)  //si era un lavatory y eligió usar un token se manda un use token
            networkInterface->sendUseToken(modelPointer->locationOfComputerRoomOrLavatory(LAVATORY));
        else if(message[2]==deadbolt[2])          //Si era un deadbolt
        {
            if(retVal==SPEND_ACTIONS_TEXTB)         //Se manda spend actions con true si eligió gastarlas, con no si no gasto nada el rata.
                networkInterface->sendSpent(true);
            else
                networkInterface->sendSpent(false);
        }
        else if(message[2]==motion[2] && retVal==USE_HACK_TOKEN_TEXTB)    //Si salio del motion y uso el token se manda un use token.
            networkInterface->sendUseToken(modelPointer->locationOfComputerRoomOrLavatory(COMPUTER_ROOM_MOTION));
        else
            willWaitForAck=false;
        waiting4ack=willWaitForAck;
    }
    return retVal;
}
string BurgleBrosController::processOtherPlayerBasicChoice(vector<string> &message) 
{
    string retVal;
    vector<string> fingerPrint({ENTER_FINGERPRINT_TEXT});
    vector<string> lavatory({LAVATORY_TEXT});
    vector<string> laser({LASER_TEXT});
    vector<string> motion({MOTION_TEXT});
    vector<string> deadbolt({DEADBOLT_TEXT});
    if(message[2]==fingerPrint[2])  //Si se preguntaba por un fingerprint
    {
        if(packetToAnalize.empty())            //Y no llego ninguna info extra, se devuelve que eligió triggerear una alarma
            retVal=TRIGGER_ALARM_TEXTB;
        else if(packetToAnalize.front().getHeader()==USE_TOKEN)    //Si se uso un token se devuelve eso.
            retVal=USE_HACK_TOKEN_TEXTB;
    }
    else if(message[2]==laser[2])         //SI se preguntaba por la entrada a un tile laser
    {
        if(packetToAnalize.empty()) 
            retVal=TRIGGER_ALARM_TEXTB; 
        else if(packetToAnalize.front().getHeader()==USE_TOKEN)    //Si se uso un token se devuelve eso.
            retVal=USE_HACK_TOKEN_TEXTB;
        else if(packetToAnalize.front().getHeader()==SPENT_OK && packetToAnalize.front().playerAcceptedToSpentMoves())
            retVal=SPEND_ACTION_TEXTB;
        else
            retVal=TRIGGER_ALARM_TEXTB;             //Sino ,llego un  spent ok con el valor "N".
    }
    else if(message[2]==motion[2])            //Si se esperaba para un motion
    {
        if(packetToAnalize.empty()) 
            retVal=TRIGGER_ALARM_TEXTB; 
        else if(packetToAnalize.front().getHeader()==USE_TOKEN)     //Y llego un use token, se devulve como si hubiera presionado el cartel con use hack token text
            retVal=USE_HACK_TOKEN_TEXTB;
        else
            retVal=TRIGGER_ALARM_TEXTB;         //Si era cualquier otro paquete triggerea alarma.
    }
    else if(message[2]==lavatory[2])          //Si se entró a un lavaratory
    {
        if(packetToAnalize.empty())                //Y el siguiente paquete que llegó no fue un use token, se usan los tokens del jugador
            retVal=USE_MY_STEALTH_TOKEN_TEXTB;
        else if(packetToAnalize.front().getHeader()==USE_TOKEN)    //Sino se usan los tokens del lavatory
            retVal=USE_LAVATORY_TOKEN_TEXTB;
    }
    else if(message[2]==deadbolt[2])          //Si se entró a un deadbolt donde no había personas dentro
    {
        if(packetToAnalize.front().getHeader()== SPENT_OK && packetToAnalize.front().playerAcceptedToSpentMoves())    //Si el spent ok llego con yes, se usan las acciones extyra, sino no.
            retVal=SPEND_ACTIONS_TEXTB;
        else
            retVal=GET_BACK_TEXTB;
    }
    packetToAnalize.clear();
    return retVal;
}
void BurgleBrosController::parseMouseEvent(EventData *mouseEvent)
{
    
    if(mouseEvent!=nullptr && status!=INITIALIZING)
    {
        MouseED *p2MouseData = dynamic_cast<MouseED *> (mouseEvent);
        if( p2MouseData != nullptr && p2MouseData->isClicked())
        {
            ItemInfo temp;
            vector<string> exitMsg={"Quit","Confirm quit", "You have pressed the quit button. Are you sure you want to quit?"};
            Point aux={(double)p2MouseData->getX(), (double)p2MouseData->getY()};
            temp=view->itemFromClick(aux);
            CardLocation * auxLocation = nullptr;
            PlayerId * auxPlayer = nullptr;
            auxInfo * menuInfo = nullptr;
            auxInfoGuard * guardInfo = nullptr;
            unsigned int * floor = nullptr;
            string userChoice;
            switch(temp.type)
            {
                case TILE_CLICK:
                    if(!waiting4ack)
                    {
                        auxLocation = (CardLocation *)temp.info;
                        view->showMenu(modelPointer->getPosibleActionsToTile(THIS_PLAYER, *auxLocation), aux, *auxLocation);
                        view->update();
                    }
                    break;
                case MENU_ITEM_CLICK:
                    if(!waiting4ack)
                    {
                        menuInfo = (auxInfo *)temp.info;
                        interpretAction(menuInfo->option, menuInfo->location);
                        view->update();
                    }
                    break;
                case LOOT_CARDS_CLICK:
                    auxPlayer = (PlayerId *)temp.info;
                    view->zoomLoot(*auxPlayer);
                    view->update();
                    break;
                case GUARD_CARDS_CLICK:
                    guardInfo = (auxInfoGuard *)temp.info;
                    if(guardInfo->shownDeck == true)
                        view->zoomGuardDeck(guardInfo->floor);
                    else
                    {
                        if(!waiting4ack && modelPointer->isPeekGuardsCardPossible(THIS_PLAYER,guardInfo->floor))
                        {
                            modelPointer->peekGuardsCard(THIS_PLAYER,&auxLocation,guardInfo->floor,SPOTTER_NO_PREV_CHOICE);
                            vector<string> msg= modelPointer->getMsgToShow();
                            string userChoice = view->MessageBox(msg);
                            modelPointer->userDecidedTo(userChoice);
                            networkInterface->sendSpyPatrol(*auxLocation,userChoice);
                        }
                    }
                    view->update();
                    break;
                case CHAR_CARD_CLICK:
                    auxPlayer = (PlayerId *)temp.info;
                    view->zoomPlayerCard(*auxPlayer);
                    view->update();
                    break;
                case ZOOM_CLICK:
                    floor = (unsigned int *)temp.info;
                    view->zoomFloor(*floor,modelPointer);
                    view->update();
                    break;
                case PASS_BUTTON_CLICK:
                    view->eraseMenu();
                    if(!waiting4ack && modelPointer->getPlayerOnTurn()==THIS_PLAYER)
                    {
                        modelPointer->pass(THIS_PLAYER);
                        networkInterface->sendPacket(PASS);
                        waiting4ack = true;
                    }
                    break;
                case VOL_BUTTON_CLICK:
                    sound->toggleMute();
                    view->toggleVolButton();
                    view->update();
                    break;
                case HELP_BUTTON_CLICK:
                    //view->cheatCards();
//                    view->showHelp(true);
                    resetMouseZ=true;
                    view->update();
                    break;
                case EXIT_BUTTON_CLICK:
                   /* if(view->yesNoMessageBox(exitMsg)==1)
                    {
                        networkInterface->sendPacket(QUIT);
                        waiting4QuitAck=true;
                        waiting4ack = true;
                        quitCause=USER_QUIT;
                    }*/
                    networkInterface->sendPacket(QUIT);
                    waiting4QuitAck=true;
                    waiting4ack = true;
                    quitCause=USER_QUIT;
                    break;
                case HELP_IMAGE_CLICK:
//                    view->showHelp(false);
                    resetMouseZ=true;
                    view->update();
                    break;
                default:
                    view->eraseMenu();
                    view->update();
                    break;
            }
        }
        else if( p2MouseData != nullptr && !p2MouseData->isClicked())
        {
//            if(view->isShowingHelp())
  //              view->setHelpScroll((-1)*p2MouseData->getZ());
            view->update();
        }
    }
    //checkGameStatus();
}
void BurgleBrosController::interpretAction(string action, CardLocation location)
{
    view->eraseMenu();
    if(action == "PEEK")//VER SI ESTE TIPO DE FUNCIONES TIENEN QUE RECIBIR QUE PLAYER O USAR EL QUE ESTA EN TURNO DIRECTAMENTE
    {   
        unsigned int safeNumber = modelPointer->peek(THIS_PLAYER,location,NO_SAFE_NUMBER);
        networkInterface->sendPeek(location, safeNumber);
    }
    else if(action=="MOVE")
    {
        if(modelPointer->moveRequiresToInitGuard(location))     //Si el jugador con el move sube a un piso donde no hay un guardia inicializado:
        {
            CardLocation guardPos,guardDiePos;
            previousMovingToLocation=location;      //Guarda a donde se iba a mover
            aMoveActionPending=true;                
            modelPointer->generateGuardInitPos(&guardPos,&guardDiePos);
            networkInterface->sendInitGPos(guardPos,guardDiePos);
        }
        else
        {
            unsigned int safeNumber = modelPointer->move(THIS_PLAYER,location,NO_SAFE_NUMBER);
            networkInterface->sendMove(location, safeNumber);
        }
    }
    else if(action=="ADD TOKEN")
    {
        modelPointer->addToken(THIS_PLAYER,location);
        networkInterface->sendAddToken(location);
    }
    else if(action=="ADD DIE")
    {
        modelPointer->addDieToSafe(THIS_PLAYER,location);
        networkInterface->sendAddToken(location);           //Add token tmb es para añadir dados al safe
    }
    else if(action=="CRACK")
    {
        vector<unsigned int> diceThrown;
        modelPointer->crackSafe(THIS_PLAYER,diceThrown);
        networkInterface->sendDice(diceThrown);
    }
    else if(action=="CREATE ALARM")
    {
        modelPointer->createAlarm(THIS_PLAYER,location);
        networkInterface->sendCreateAlarm(location);
    }
    else if(action=="PLACE CROW")
    {
        modelPointer->placeCrow(THIS_PLAYER,location);
        networkInterface->sendPlaceCrow(location);
    }
    else if(action=="PICK UP KITTY" || action=="PICK UP GOLD BAR" || action=="PICK UP LOOTS")
    {
        modelPointer->pickLoot(THIS_PLAYER);
        networkInterface->sendPacket(PICK_UP_LOOT);
    }
    else if(action=="ESCAPE")
    {
        modelPointer->escape(THIS_PLAYER,location);
        location.floor++;
        networkInterface->sendMove(location,0);
    }
    else
    {
        for(int i = (int)TIARA; i <= (int)GOLD_BAR; i++)
        {
            string ask = "ASK FOR ";
            string offer = "OFFER ";
            ask += loot2Str((Loot)i);
            offer += loot2Str((Loot)i);
            transform(ask.begin(), ask.end(), ask.begin(), ::toupper);
            transform(offer.begin(), offer.end(), offer.begin(), ::toupper);
            if(action==ask)
            {
                modelPointer->askForLoot(THIS_PLAYER,(Loot)i);
                networkInterface->sendRequestLoot((Loot)i);
                break;
            }
            if(action==offer)
            {
                modelPointer->offerLoot(THIS_PLAYER,(Loot)i);
                networkInterface->sendOfferLoot((Loot)i);
                break;
            }
        }
    }
    waiting4ack=true;
}

void BurgleBrosController::parseKeyboardEvent(EventData *evData)
{
    KeyboardED *p2KeyData = dynamic_cast<KeyboardED *>(evData);
    if(p2KeyData != nullptr)
    {
        if(p2KeyData->isASpecialKey()) 
        {
            SpecialKey key = p2KeyData->getSpecialKey();
            vector<string> exitMsg={"Quit","Confirm quit", "You have pressed the quit button. Are you sure you want to quit?"};
            if(key == ESCAPE_KEY)
            {
                if(view->yesNoMessageBox(exitMsg)==1)
                {
                    networkInterface->sendPacket(QUIT);
                    waiting4QuitAck=true;
                    waiting4ack = true;
                    quitCause=USER_QUIT;
                }
            }
        }
    }
}

void BurgleBrosController::parseNetworkEvent(EventData *networkEvent)
{
    NetworkED *p2NetworkData = dynamic_cast<NetworkED *> (networkEvent);
    vector<string> gameOverMsg({DEFAULT_GAME_OVER_MSG});
    vector<string> quitMsg({DEFAULT_QUIT_MSG});
    if(p2NetworkData!=nullptr)
    {
        switch(status)      //Depende de en que estado está el juego, los paquetes de internet significan distintas cosas
        {
            case INITIALIZING:          //En la inicialización importa si es el cliente o el server en cuanto al orden de los mensajes
                if(p2NetworkData->getHeader() != GAME_OVER && p2NetworkData->getHeader() != QUIT)
                {
                    if(!firstInitDone)
                    {
                        if(communicationRole==CLIENT)
                            clientInitRoutine(p2NetworkData);
                        else if(communicationRole==SERVER)
                            serverInitRoutine(p2NetworkData);
                    }
                    else
                    {
                        if(whichPlayer==FIRST_DECIDING_PLAYER)
                            firstDecidedRoutine(p2NetworkData);
                        else
                            secondDecidedRoutine(p2NetworkData);
                    }
                }
                else
                {
                    quit=true;
                    networkInterface->sendPacket(ACK);
                    if(p2NetworkData->getHeader() == GAME_OVER) view->MessageBox(gameOverMsg);
                    else view->MessageBox(quitMsg);
                }
                
                break;
            case PLAYING:
                interpretNetworkAction(p2NetworkData);  //Si esta jugando, se interpreta la jugada del otro jugador.
                break;
            default:
                break;
              
        }
    }
    else
        quit=true;
}

void BurgleBrosController::parseTimerEvent(EventData* mouseEvent) {
    TimerED *p2TimerData = dynamic_cast<TimerED *> (mouseEvent);
    vector<string> timeoutMessage({DEFAULT_TIMEOUT_MSG});
    if(p2TimerData!=nullptr && p2TimerData->getType()==TIMEOUT)
    {
        networkInterface->sendPacket(ERRORR);
        view->MessageBox(timeoutMessage);
        quit=true;
    }
}

void BurgleBrosController::interpretNetworkAction(NetworkED *networkEvent)
{
    vector<string> message, quitMsg({DEFAULT_QUIT_MSG}), gameOverMsg({DEFAULT_GAME_OVER_MSG}), spotterDecision({DEFAULT_SPOTTER_MSG});
    vector<string> timeoutMessage({DEFAULT_TIMEOUT_MSG});
    Loot loot;
    bool guardHasToMove;
    CardLocation guardPosition, guardDice,auxLoc;
    CardLocation * spyGuardCard = &auxLoc;
    list<GuardMoveInfo> guardMovement;
    vector<unsigned int> dice;
    analizeIfModelRequiresMoreActions(networkEvent);
    switch(networkEvent->getHeader())       //Depende de que acción se ejecutan distintas funciones.
    {
        case PEEK:
            modelPointer->peek(OTHER_PLAYER, networkEvent->getPos(),networkEvent->getSafeNumber());
            networkInterface->sendPacket(ACK);
            break;
        case PASS:
            modelPointer->pass(OTHER_PLAYER);   
            networkInterface->sendPacket(ACK);
            break;
        case MOVE:    
            if(networkEvent->getPos().floor==3)     //Si es un movimiento al 4to piso, es un escape.
            {   
                auxLoc=networkEvent->getPos();
                auxLoc.floor--;         //Apunto al stair tile
                modelPointer->escape(OTHER_PLAYER, auxLoc);
            }
            else
                modelPointer->move(OTHER_PLAYER, networkEvent->getPos(),networkEvent->getSafeNumber());
            
            if(modelPointer->hasGameFinished())
            {
                vector<string> toShow;
                if(modelPointer->getFinishMsg() == "WON")
                {
                    networkInterface->sendPacket(WE_WON);
                    vector<string> aux({DEFAULT_WIN_MSG});
                    toShow=aux;
                }
                else if(modelPointer->getFinishMsg()== "LOST")
                {    
                    networkInterface->sendPacket(WE_LOST);
                    vector<string> aux({DEFAULT_LOST_MSG});
                    toShow=aux;
                }
                string userChoice=view->MessageBox(toShow);
                if(userChoice == "Play again")
                {
                    whichPlayer=SECOND_DECIDING_PLAYER;
                    resetGame();
                } 
            }
            
            else
                networkInterface->sendPacket(ACK);
            break;
                
        case ADD_TOKEN:
            if(modelPointer->isAddDieToSafePossible(OTHER_PLAYER,networkEvent->getTokenPos()))      //SI el add token era para añadir un dado al safe
                modelPointer->addDieToSafe(OTHER_PLAYER,networkEvent->getTokenPos());
            else
                modelPointer->addToken(OTHER_PLAYER,networkEvent->getTokenPos());   //Sino era un token para computer room
            networkInterface->sendPacket(ACK);
            break;
        case ACK:
            waiting4ack=false;
            if(waiting4QuitAck) //quit = waiting4QuitAck;
                quit=true;
            else if(aMoveActionPending)      /*SI se tuvo que inicializar un guardia por un move, ya esta inicializado antes de mandar el move (el ack es al INIT_G_POS)
                                              *  y ahora se manda la acción move.*/       
            {
                unsigned int safeNumber = modelPointer->move(THIS_PLAYER,previousMovingToLocation,NO_SAFE_NUMBER);      //Se ejecuta el move y se hace.
                networkInterface->sendMove(previousMovingToLocation, safeNumber);
                aMoveActionPending=false;   resetTimeoutTimer=true;     waiting4ack=true;
            }
            else if(modelPointer->getModelStatus()== WAITING_FOR_ACTION && modelPointer->isGuardsTurn())    //Si el jugador gastó todas las acciones y, para casos especiales metió lo que se preguntaba (deadbolt, etc) se procede a mover el guardia.
            {
                modelPointer->guardMove(guardMovement);         //Se hace la movida del guardia, y se guarda por referencia en guardMovement 
                networkInterface->sendGMove(guardMovement);     //Se envía esa información.
                resetTimeoutTimer=true;
            }
            else if(modelPointer->getModelStatus()==WAITING_FOR_USER_CONFIRMATION)   //Si se esperaba la confirmación del usuario para una accion propia del jugador de esta cpu:
            {
                message=modelPointer->getMsgToShow(); //Se obtiene el mensaje a mostrar,
                guardHasToMove = modelPointer->userDecidedTo(getUsersResponse(message));//Esta función devuelve lo que elige el jugador en el cartelito. y le pasa la respuesta al modelo.
                if(guardHasToMove)      //Si se termino una jugada que no mando paquete y termino su turno, se tiene que enviar el paquete del guardia.
                {   modelPointer->guardMove(guardMovement); networkInterface->sendGMove(guardMovement);}
            }
            else if(modelPointer->getModelStatus()== WAITING_FOR_DICE)
            {
                modelPointer->setDice(dice);    //le paso unos dados vacíos y ahí pone los dados que salieron tirando para el keypad.
                networkInterface->sendDice(dice); //Se lo envía a la otra pc.
                resetTimeoutTimer=true;     waiting4ack=true;
            }
            else if(modelPointer->getModelStatus()== WAITING_FOR_GUARD_INIT)
            {
                modelPointer->generateGuardInitPos(&guardPosition, &guardDice);
                networkInterface->sendInitGPos(guardPosition, guardDice);
                resetTimeoutTimer=true;     waiting4ack=true;
            }
            else if(modelPointer->getModelStatus()==WAITING_FOR_LOOT)   //Si se envió un throw dice que habría el safe
            {
                modelPointer->setLoot(THIS_PLAYER, &loot);          //Al llegar el ack se manda el safeopened con el loot obtenido.
                networkInterface->sendSafeOpened(loot);
                resetTimeoutTimer=true;     waiting4ack=true;
            }
            else if(modelPointer->getModelStatus()== WAITING_DICE_FOR_LOOT) //Si llegó el ack a un tiro de dados
            {
                if(modelPointer->dieForLootNeeded())            //Si se necesitaba tirar 1 dado más (caso que tenga chihuahua y persian kitty juntos)
                {
                    unsigned int die=modelPointer->rollDieForLoot(NO_DIE);
                    networkInterface->sendRollDiceForLoot(die);
                    resetTimeoutTimer=true;     waiting4ack=true;
                }
                else
                    modelPointer->continueGame();       //SIno se sigue con el juego normal.
            }
            break;
        case SPENT_OK:case USE_TOKEN: 
            if(modelPointer->getModelStatus()==WAITING_FOR_USER_CONFIRMATION)   //Si se esperaba la confirmación del usuario para una accion propia del jugador de esta cpu:
            {
                packetToAnalize.push_back(*networkEvent);   //Acá se guarda para tratar el paquete en la función getUsersResponse
                message=modelPointer->getMsgToShow(); 
                modelPointer->userDecidedTo(getUsersResponse(message));
                networkInterface->sendPacket(ACK);
            }
            else
               quit=true;
            break;
        case AGREE: case DISAGREE: case REQUEST_LOOT: case OFFER_LOOT:
            handleLootsExchange(networkEvent);
            break;
           
        case THROW_DICE:
            if(modelPointer->getModelStatus() == WAITING_FOR_DICE)      //Si se esperaba para un keypad
            {
                networkEvent->getDice(dice);    //Obtiene los dados que tiro el otro para el keypad
                modelPointer->setDice(dice);    //Y se los pasa al modelo para que procese.
                networkInterface->sendPacket(ACK);
            }
            else                //Sino se tira para el safe.
            {
                networkEvent->getDice(dice);
                modelPointer->crackSafe(OTHER_PLAYER, dice);
                networkInterface->sendPacket(ACK);
            }
            break;
        case GUARD_MOVEMENT:
            networkEvent->getGuardMovement(guardMovement);  //Obtengo el movimiento del guardia
            modelPointer->guardMove(guardMovement); //Y hago que el modelo lo procese.
            if(modelPointer->hasGameFinished() && modelPointer->getFinishMsg()== "LOST")
            {
                networkInterface->sendPacket(WE_LOST);
                vector<string> aux({DEFAULT_LOST_MSG});
                string userChoice=view->MessageBox(aux);
                if(userChoice == "Play again")
                {
                    whichPlayer=SECOND_DECIDING_PLAYER;
                    resetGame();
                }
            }    
            else if(modelPointer->dieForLootNeeded())        //Si se necesita tirar un dado para los loots (chihuahua o persian kitty).
            {
                unsigned int die=modelPointer->rollDieForLoot(NO_DIE);
                networkInterface->sendRollDiceForLoot(die);
                waiting4ack=true;
            }
            break;
        case INITIAL_G_POS:
            networkEvent->getInitGPos(&guardPosition, &guardDice);
            modelPointer->copyGuardInitPos(guardPosition, guardDice);
            networkInterface->sendPacket(ACK);
            break;
        case SAFE_OPENED:
            loot=networkEvent->getLoot();
            modelPointer->setLoot(OTHER_PLAYER, &loot);
            networkInterface->sendPacket(ACK);
            break;
        case PICK_UP_LOOT:
            modelPointer->pickLoot(OTHER_PLAYER);
            networkInterface->sendPacket(ACK);
            break;       
        case CREATE_ALARM:
            modelPointer->createAlarm(OTHER_PLAYER, networkEvent->getCreateAlarmPos());
            networkInterface->sendPacket(ACK);
            break;
        case PLACE_CROW:
            modelPointer->placeCrow(OTHER_PLAYER, networkEvent->getPlaceCrowPos());
            networkInterface->sendPacket(ACK);
            break;
        case ROLL_DICE_FOR_LOOT:
            modelPointer->rollDieForLoot(networkEvent->getDieForLoot());
            if(!modelPointer->dieForLootNeeded())            //Si no se necesita más roll for dices, continua el juego. O sea si el otro tenía el persian kitty y el chihuahua pasa 2 veces por aca y la segunda solo pasa por el continue.
                modelPointer->continueGame();
            networkInterface->sendPacket(ACK);
            break;
        case SPY_PATROL:
            auxLoc=networkEvent->getSpyPatrolPos();
            modelPointer->peekGuardsCard(OTHER_PLAYER,&spyGuardCard,auxLoc.floor,networkEvent->getSpyPatrolChoice());
            spotterDecision[spotterDecision.size()-2]+=networkEvent->getSpyPatrolChoice();
            view->MessageBox(spotterDecision);      //Le informo al usuario lo que el otro jugador hizo con la carta espiada
            networkInterface->sendPacket(ACK);
            break;
        case WE_WON: case WE_LOST:
            handleWonOrLost(networkEvent->getHeader());
            break;
        case QUIT:
            quit=true;
            networkInterface->sendPacket(ACK);
            view->MessageBox(quitMsg);
            break;
        case PLAY_AGAIN:
            handlePlayAgain();
            break;
        case GAME_OVER:
            quit=true;
            networkInterface->sendPacket(ACK);
            if(!modelPointer->hasGameFinished())
                view->MessageBox(gameOverMsg);
            break;
        case ERRORR:
            quit=true;
            view->MessageBox(timeoutMessage);
        default:
            break;

    }
}
void BurgleBrosController::handlePlayAgain()
{
    /*vector<string> toShow({DEFAULT_PLAY_AGAIN_MSG});
    string userChoice=view->MessageBox(toShow);
    if(userChoice == "Play again")
    {
        resetGame();
        auxInitInfo[THIS_PLAYER].playersCharacter=getRandomCharacter();
        networkInterface->sendChar(auxInitInfo[THIS_PLAYER].playersCharacter);//sendCharacter()
        this->status=INITIALIZING;
        whichPlayer=SECOND_DECIDING_PLAYER;
    }
    else
    {
        waiting4QuitAck=true;   
        networkInterface->sendPacket(GAME_OVER);
    }*/
    if(!modelPointer->hasGameFinished())
    {
        auxInitInfo[THIS_PLAYER].playersCharacter=getRandomCharacter();
        networkInterface->sendChar(auxInitInfo[THIS_PLAYER].playersCharacter);//sendCharacter()
        this->status=INITIALIZING;
    }
    else
    {
        waiting4QuitAck=true;   
        networkInterface->sendPacket(GAME_OVER);
    }    
}
void BurgleBrosController::handleLootsExchange(NetworkED *networkEvent)
{
    if(modelPointer->getModelStatus()==WAITING_FOR_ACTION)      //Si el otro usuario mando un offer o un request
    {
        if(networkEvent->getHeader()==REQUEST_LOOT)         //Interpreta la acción pasandola por el modelo
            modelPointer->askForLoot(OTHER_PLAYER, networkEvent->getLoot());
        else if(networkEvent->getHeader() == OFFER_LOOT)
            modelPointer->offerLoot(OTHER_PLAYER, networkEvent->getLoot());
        vector<string> msgToShow= modelPointer->getMsgToShow();
        string userChoice =view->MessageBox(msgToShow);     //Le pregunta al usuario su elección
        modelPointer->userDecidedTo(userChoice);            //Se la pone al modelo
        if(userChoice==ACCEPT_TEXTB)                        //Se lo comunica a la otra pc
            networkInterface->sendPacket(AGREE);
        else
            networkInterface->sendPacket(DISAGREE);
    }
    else        //Si se esperaba por un agree o un disagree de la otra persona:
    {
        if(networkEvent->getHeader()== AGREE)
            modelPointer->userDecidedTo(ACCEPT_TEXTB);      //Si era un agree se lo comunica al modelo, sino le comunica un disagree.
        else if (networkEvent->getHeader()== DISAGREE)
            modelPointer->userDecidedTo(DECLINE_TEXTB);
        if(waiting4ack)
                waiting4ack=false;
    }
}
void BurgleBrosController::handleWonOrLost(PerezProtocolHeader msg)
{
    vector<string> toShow;
    if(msg==WE_WON)
    {
        vector<string> aux({DEFAULT_WIN_MSG});
        toShow=aux;
    }
    else if (msg==WE_LOST)
    {
        vector<string> aux({DEFAULT_LOST_MSG});
        toShow=aux;
    }
    string userChoice=view->MessageBox(toShow);
    if(userChoice == "Play again")
    {
        networkInterface->sendPacket(PLAY_AGAIN);
        this->status=INITIALIZING;
        whichPlayer=FIRST_DECIDING_PLAYER;
        resetGame();
    }
    else
    {
        waiting4QuitAck=true;   
        networkInterface->sendPacket(GAME_OVER);
    }
    
}



void BurgleBrosController::analizeIfModelRequiresMoreActions(NetworkED *networkEvent)
{
    PerezProtocolHeader h = networkEvent->getHeader();
    vector<string> message;
    if(modelPointer->getModelStatus()==WAITING_FOR_USER_CONFIRMATION && h!=SPENT_OK && h!=USE_TOKEN && modelPointer->getPlayerOnTurn()==OTHER_PLAYER) //Si se hizo un move que podía llegar un use token o un spent ok luego, pero no llegó
    {
        message=modelPointer->getMsgToShow(); //Se obtiene el mensaje que se mostraria si saltar el cartel
        modelPointer->userDecidedTo(getUsersResponse(message));//Y esta funcion "emula" lo elegido por el otro jugador. por ejemplo si no gasto las acciones del deadbolt simula como que eligio no gastarlas en el cartel, pero siendo el jugador desde la otra pc.
       /* if(modelPointer->getModelStatus()==WAITING_FOR_USER_CONFIRMATION ) //Si seguía esperando por una segunda respuesta (el caso especial del motion):
        {
            message=modelPointer->getMsgToShow(); //Se obtiene el mensaje que se mostraria si saltar el cartel
            modelPointer->userDecidedTo(getUsersResponse(message));
        }*/
    }
}

void BurgleBrosController::checkGameStatus()
{
    if(modelPointer->hasGameFinished())
    {
        string aux = modelPointer->getFinishMsg();
        if(aux=="WON")
        {
            vector<string> auxmsg({DEFAULT_WIN_MSG});
            while(1)
                view->MessageBox(auxmsg);
        }
    }
}
void BurgleBrosController::clientInitRoutine(NetworkED *networkEvent)
{
    unsigned int packetCountCopy=initPacketCount;   //Saco una copia del número de paquete de inicialización
    unsigned int initSafeNumber = 0;
    CardLocation guardPos,guardsDiePos, playersPos;
    vector<CardName> tiles;
    switch(initPacketCount)
    {
        case 0: 
            if(networkEvent->getHeader() == NAME)   //Primero el client recibe el paquete name
            {
                networkInterface->sendName(thisPlayerName);             //Al cual le responde con su nombre 
                auxInitInfo[THIS_PLAYER].PlayerName = thisPlayerName;   //Y lo guarda para el modelo
                initPacketCount++;
            }
            break;
        case 1:
            if(networkEvent->getHeader() == ACK)        //Una vez que el server recibió la informacion y confirmó con un ack
            {
                networkInterface->sendPacket(NAME);     //Se le envía name para saber el nombre del server.
                initPacketCount++;
            }
            break;
        case 2:
            if(networkEvent->getHeader() == NAME_IS)    //El server responde con su nombre.
            {
                auxInitInfo[OTHER_PLAYER].PlayerName=networkEvent->getName(); //Se guarda en una estructura auxiliar para luego pasarle toda la información junta de inicialización de jugadores al modelo.
                networkInterface->sendPacket(ACK);     //Se le envía un ack para confirmar la información recibida.
                initPacketCount++;
            }
            break;
        case 3:
            if(networkEvent->getHeader() == I_AM)   //Luego el server manda que character escogió
            {
                auxInitInfo[OTHER_PLAYER].playersCharacter=networkEvent->getCharacter();        //Se guarda su jugador
                auxInitInfo[THIS_PLAYER].playersCharacter=getRandomCharacter(auxInitInfo[OTHER_PLAYER].playersCharacter);    //Se elige uno random que no sea el que escogió el server
                networkInterface->sendChar(auxInitInfo[THIS_PLAYER].playersCharacter);      //Y se le manda la info de cual es el que escogió el client.
                initPacketCount++;
            }
            break;
        case 4:
            if(networkEvent->getHeader() == INITIAL_G_POS)      //El server crea a los guardias y le pasa al cliente donde están
            {
                networkEvent->getInitGPos(&guardPos, &guardsDiePos);    //Geteo su info y se la paso al modelo
                modelPointer->copyGuardInitPos(guardPos,guardsDiePos);
                networkInterface->sendPacket(ACK);                      //Respondo con un ack
                initPacketCount++;
            }
            break;
        case 5:
            if(networkEvent->getHeader() == START_INFO)         //Luego vienen de que tipo es cada tile, y la posición inicial de los jugadores
            {   
                networkEvent->getStartInfo(&tiles, &playersPos);    
                initSafeNumber = networkEvent->getSafeNumber();
                modelPointer->initBoard(tiles,playersPos, &initSafeNumber);                 //Se inicializa ya el board
                auxInitInfo[OTHER_PLAYER].initPos=playersPos;   //Y se pasa toda la info de los players al modelo
                auxInitInfo[THIS_PLAYER].initPos=playersPos;
                modelPointer->initPlayer(THIS_PLAYER,auxInitInfo[THIS_PLAYER].PlayerName,auxInitInfo[THIS_PLAYER].playersCharacter,auxInitInfo[THIS_PLAYER].initPos);
                modelPointer->initPlayer(OTHER_PLAYER,auxInitInfo[OTHER_PLAYER].PlayerName,auxInitInfo[OTHER_PLAYER].playersCharacter,auxInitInfo[OTHER_PLAYER].initPos);
                networkInterface->sendPacket(ACK);              //Se responde con un ack
                initPacketCount++;
            }
            break;
        case 6:
            if(networkEvent->getHeader() == YOU_START)          //Luego el server decide quien empieza.
            {
                initPacketCount=0;
                modelPointer->setInitTurn(THIS_PLAYER);         //Como el server dijo que el cliente empiece empieza el jugador de esta maquina
                status=PLAYING;
                view->ViewInit(modelPointer);
                view->update();
                firstInitDone=true;
                iStarted=false;
            }
            else if(networkEvent->getHeader() == I_START)
            {
                initPacketCount=0;
                modelPointer->setInitTurn(OTHER_PLAYER);         //Como el server dijo que el cliente empiece empieza el, the other player comienza jugando.
                networkInterface->sendPacket(ACK);              //Se le manda un ack para que el server sepa que le llegó el msj al cliente.
                status=PLAYING;
                view->ViewInit(modelPointer);
                view->update();
                firstInitDone=true;
                iStarted=true;
            }
            break;
        default:
            break;
    }
    if(initPacketCount==packetCountCopy)    //Si no fue modificado significa que no entró a un if, por ende hubo un paquete que no siguió las reglas del protocolo -> se cierra el programa.
        quit=true;
}

void BurgleBrosController::serverInitRoutine(NetworkED *networkEvent)
{
    unsigned int packetCountCopy=initPacketCount;   //Saco una copia del número de paquete de inicialización
    unsigned int initSafeNumber = 0;
    CardLocation guardPos,guardsDiePos, playersPos;
    vector<CardName> tiles;
    switch(initPacketCount)
    {
        case 0:
            if(networkEvent->getHeader() == NAME_IS)   //Primero el client recibe el paquete name is del cliente. (Había mandado name antes el server).
            {
                auxInitInfo[OTHER_PLAYER].PlayerName=networkEvent->getName();   //Guarda el nombre del otro
                networkInterface->sendPacket(ACK);      //Al cual le responde con su nombre 
                initPacketCount++;
            }
            break;
        case 1:
            if(networkEvent->getHeader() == NAME)       //El cliente le pide al server su name
            {
                networkInterface->sendName(thisPlayerName);     //El server se lo devuelve.
                auxInitInfo[THIS_PLAYER].PlayerName = thisPlayerName;   //Me guardo mi nombre
                initPacketCount++;
            }
            break;
        case 2:
            if(networkEvent->getHeader() == ACK)        //El cliente ya sabe el nombre del server
            {
                auxInitInfo[THIS_PLAYER].playersCharacter=getRandomCharacter(); //ENtonces se obtiene un character aleatorio
                networkInterface->sendChar(auxInitInfo[THIS_PLAYER].playersCharacter); // se lo manda al client
                initPacketCount++;
            }
            break;
        case 3:
            if(networkEvent->getHeader() == I_AM)       //EL cliente manda el character que escogió el.
            {
                auxInitInfo[OTHER_PLAYER].playersCharacter= networkEvent->getCharacter();     //Se obtiene ese character
                modelPointer->generateGuardInitPos(&guardPos, &guardsDiePos);           //Se inicializa el guardia del primer piso
                networkInterface->sendInitGPos(guardPos,guardsDiePos);          //Le manda al cliente donde se inicializó el guardia y su dado.
                initPacketCount++;
            }
            break;
        case 4:
            if(networkEvent->getHeader() == ACK)
            {
                auxInitInfo[THIS_PLAYER].initPos=getRandomCardLocation(modelPointer->getInfo2DrawGuard(0).position, 0); // EN el piso 0 empiezan
                auxInitInfo[OTHER_PLAYER].initPos= auxInitInfo[THIS_PLAYER].initPos;    //Los dos empiezan en la misma posición
                modelPointer->initBoard(tiles,auxInitInfo[THIS_PLAYER].initPos,&initSafeNumber);
                networkInterface->sendStartInfo(tiles, auxInitInfo[THIS_PLAYER].initPos,initSafeNumber);   //Se manda la información inicial de los tiles y la posición inicial de los jugadores
                modelPointer->initPlayer(THIS_PLAYER,auxInitInfo[THIS_PLAYER].PlayerName,auxInitInfo[THIS_PLAYER].playersCharacter,auxInitInfo[THIS_PLAYER].initPos); //Se inicializan los jugadores con los datos que veníamos guardando en los anteriores mensajes.
                modelPointer->initPlayer(OTHER_PLAYER,auxInitInfo[OTHER_PLAYER].PlayerName,auxInitInfo[OTHER_PLAYER].playersCharacter,auxInitInfo[OTHER_PLAYER].initPos);
                initPacketCount++;
            }
            break;
        case 5:
            if(networkEvent->getHeader() == ACK)        //Al recibir este ack, ya hay que decidir quien juega
            {
                if(rand()%2)
                {
                    networkInterface->sendPacket(YOU_START);    //Si empieza el cliente, se lo dice y ya termina su fase de inicialización.
                    initPacketCount=0;
                    modelPointer->setInitTurn(OTHER_PLAYER);
                    status=PLAYING;
                    view->ViewInit(modelPointer);
                    view->update();
                    firstInitDone=true;
                    iStarted=false;
                }
                else
                {
                    networkInterface->sendPacket(I_START);    //Si empieza el server, espera a que le llegue el ack para empezar a jugar
                    initPacketCount++;
                }
            }
            break;
        case 6:
            if(networkEvent->getHeader() == ACK)
            {
                modelPointer->setInitTurn(THIS_PLAYER);
                initPacketCount=0;
                status=PLAYING;
                view->ViewInit(modelPointer);
                view->update();
                firstInitDone=true;
                iStarted=true;
            }
            break;
        default:
            break;
    }
    if(initPacketCount==packetCountCopy)    //Si no fue modificado significa que no entró a un if, por ende hubo un paquete que no siguió las reglas del protocolo -> se cierra el programa.
        quit=true;
}

void BurgleBrosController::firstDecidedRoutine(NetworkED *networkEvent)
{
    unsigned int packetCountCopy=initPacketCount;   //Saco una copia del número de paquete de inicialización
    unsigned int initSafeNumber;
    CardLocation guardPos,guardsDiePos, playersPos;
    vector<CardName> tiles;
    switch(initPacketCount)
    {
        case 0:
            if(networkEvent->getHeader() == I_AM)   //Luego el server manda que character escogió
            {
                auxInitInfo[OTHER_PLAYER].playersCharacter=networkEvent->getCharacter();        //Se guarda su jugador
                auxInitInfo[THIS_PLAYER].playersCharacter=getRandomCharacter(auxInitInfo[OTHER_PLAYER].playersCharacter);    //Se elige uno random que no sea el que escogió el server
                networkInterface->sendChar(auxInitInfo[THIS_PLAYER].playersCharacter);      //Y se le manda la info de cual es el que escogió el client.
                initPacketCount++;
            }
            break;
        case 1:
            if(networkEvent->getHeader() == INITIAL_G_POS)      //El server crea a los guardias y le pasa al cliente donde están
            {
                networkEvent->getInitGPos(&guardPos, &guardsDiePos);    //Geteo su info y se la paso al modelo
                modelPointer->copyGuardInitPos(guardPos,guardsDiePos);
                networkInterface->sendPacket(ACK);                      //Respondo con un ack
                initPacketCount++;
            }
            break;
        case 2:
            if(networkEvent->getHeader() == START_INFO)         //Luego vienen de que tipo es cada tile, y la posición inicial de los jugadores
            {   
                networkEvent->getStartInfo(&tiles, &playersPos);    
                initSafeNumber = networkEvent->getSafeNumber();
                modelPointer->initBoard(tiles, playersPos, &initSafeNumber);                 //Se inicializa ya el board
                auxInitInfo[OTHER_PLAYER].initPos=playersPos;   //Y se pasa toda la info de los players al modelo
                auxInitInfo[THIS_PLAYER].initPos=playersPos;
                modelPointer->initPlayer(THIS_PLAYER,auxInitInfo[THIS_PLAYER].PlayerName,auxInitInfo[THIS_PLAYER].playersCharacter,auxInitInfo[THIS_PLAYER].initPos);
                modelPointer->initPlayer(OTHER_PLAYER,auxInitInfo[OTHER_PLAYER].PlayerName,auxInitInfo[OTHER_PLAYER].playersCharacter,auxInitInfo[OTHER_PLAYER].initPos);
                networkInterface->sendPacket(ACK);              //Se responde con un ack
                initPacketCount++;
            }
            break;
        case 3:
            if(networkEvent->getHeader() == YOU_START)          //Luego el server decide quien empieza.
            {
                initPacketCount=0;
                modelPointer->setInitTurn(THIS_PLAYER);         //Como el server dijo que el cliente empiece empieza el jugador de esta maquina
                status=PLAYING;
                view->ViewInit(modelPointer);
                view->update();
                iStarted=true;
            }
            else if(networkEvent->getHeader() == I_START)
            {
                initPacketCount=0;
                modelPointer->setInitTurn(OTHER_PLAYER);         //Como el server dijo que el cliente empiece empieza el, the other player comienza jugando.
                networkInterface->sendPacket(ACK);              //Se le manda un ack para que el server sepa que le llegó el msj al cliente.
                status=PLAYING;
                view->ViewInit(modelPointer);
                view->update();
                iStarted=false;
            }
            break;
            
            
            
    }
    if(initPacketCount==packetCountCopy)    //Si no fue modificado significa que no entró a un if, por ende hubo un paquete que no siguió las reglas del protocolo -> se cierra el programa.
        quit=true;
}
void BurgleBrosController::secondDecidedRoutine(NetworkED *networkEvent)
{
    unsigned int packetCountCopy=initPacketCount;   //Saco una copia del número de paquete de inicialización
    unsigned int initSafeNumber;
    CardLocation guardPos,guardsDiePos, playersPos;
    vector<CardName> tiles;
    switch(initPacketCount)
    {
        case 0:
            if(networkEvent->getHeader() == I_AM)       //EL cliente manda el character que escogió el.
            {
                auxInitInfo[OTHER_PLAYER].playersCharacter= networkEvent->getCharacter();     //Se obtiene ese character
                modelPointer->generateGuardInitPos(&guardPos, &guardsDiePos);           //Se inicializa el guardia del primer piso
                networkInterface->sendInitGPos(guardPos,guardsDiePos);          //Le manda al cliente donde se inicializó el guardia y su dado.
                initPacketCount++;
            }
            break;
        case 1:
            if(networkEvent->getHeader() == ACK)
            {
                auxInitInfo[THIS_PLAYER].initPos=getRandomCardLocation(modelPointer->getInfo2DrawGuard(0).position, 0); // EN el piso 0 empiezan
                auxInitInfo[OTHER_PLAYER].initPos= auxInitInfo[THIS_PLAYER].initPos;    //Los dos empiezan en la misma posición
                modelPointer->initBoard(tiles, auxInitInfo[THIS_PLAYER].initPos, &initSafeNumber);
                networkInterface->sendStartInfo(tiles, auxInitInfo[THIS_PLAYER].initPos, initSafeNumber);   //Se manda la información inicial de los tiles y la posición inicial de los jugadores
                modelPointer->initPlayer(THIS_PLAYER,auxInitInfo[THIS_PLAYER].PlayerName,auxInitInfo[THIS_PLAYER].playersCharacter,auxInitInfo[THIS_PLAYER].initPos); //Se inicializan los jugadores con los datos que veníamos guardando en los anteriores mensajes.
                modelPointer->initPlayer(OTHER_PLAYER,auxInitInfo[OTHER_PLAYER].PlayerName,auxInitInfo[OTHER_PLAYER].playersCharacter,auxInitInfo[OTHER_PLAYER].initPos);
                initPacketCount++;
            }
            break;
        case 2:
            if(networkEvent->getHeader() == ACK)        //Al recibir este ack, ya hay que decidir quien juega
            {
                if(iStarted)
                {
                    networkInterface->sendPacket(YOU_START);    //Si antes habia empezado esta cpu, ahora empieza la otra.
                    initPacketCount=0;
                    modelPointer->setInitTurn(OTHER_PLAYER);
                    status=PLAYING;
                    view->ViewInit(modelPointer);
                    view->update();
                    firstInitDone=true;
                    iStarted=false;
                }
                else
                {
                    networkInterface->sendPacket(I_START);    //Si empieza el server, espera a que le llegue el ack para empezar a jugar
                    initPacketCount++;
                }
            }
            break;
        case 3:
            if(networkEvent->getHeader() == ACK)
            {
                modelPointer->setInitTurn(THIS_PLAYER);
                initPacketCount=0;
                status=PLAYING;
                view->ViewInit(modelPointer);
                view->update();
                firstInitDone=true;
                iStarted=true;
            }
            break;
        default:
            break;
    }
    if(initPacketCount==packetCountCopy)    //Si no fue modificado significa que no entró a un if, por ende hubo un paquete que no siguió las reglas del protocolo -> se cierra el programa.
        quit=true;
}

void BurgleBrosController::resetGame()
{
    auxInitInfo[THIS_PLAYER].PlayerName=modelPointer->getInfo2DrawPlayer(THIS_PLAYER).name;
    auxInitInfo[OTHER_PLAYER].PlayerName=modelPointer->getInfo2DrawPlayer(OTHER_PLAYER).name;
    modelPointer->reset();
    sound->reset();
    view->reset();
    waiting4ack=false;
    aMoveActionPending=false;
    waiting4QuitAck=false;
}

void BurgleBrosController::checkForNonOrderedPackets() 
{
    bool retVal=false;
    PerezProtocolHeader header;
    unsigned char buffer[BUFSIZE];
    unsigned int len;
    vector<string> quitMsg({DEFAULT_QUIT_MSG});
    vector<string> othersErrorMessage({DEFAULT_OTHERS_ERROR_MSG});
    if(networkInterface->recievePacket(&header, buffer, &len))
    {
        if(header == QUIT)      //Si llego un quit mientras esta pc estaba en modo bloqueante por un native message
        {
            quit=true;
            networkInterface->sendPacket(ACK);
            view->MessageBox(quitMsg);
        }
        else if(header == ERRORR)       //o un error
        {
            quit=true;
            view->MessageBox(othersErrorMessage);
        }
        else        //En el caso que llego un paquete que no era ninguno de los dos, hubo un error de sincronización, por lo que se debe mandar error!
        {
            quit=true;
            networkInterface->sendPacket(ERRORR);
        }
    }
}


BurgleBrosController::~BurgleBrosController() {
}

