#ifndef BURGLEBROSMODEL_H
#define BURGLEBROSMODEL_H
#include "BurgleBrosBoard.h"
#include "BurgleBrosTokens.h"
#include "BurgleBrosGuard.h"
#include "BurgleBrosLoots.h"
#include "BurgleBrosPlayer.h"
#include "BurgleBrosDices.h"
#include "Model.h"

#define NUMBER_OF_PLAYERS 2
#define NO_SAFE_NUMBER -1

/*Mensajes para dialog box tienen:  TITULO                  SUBTITULO               TEXTO */
#define ENTER_FINGERPRINT_TEXT      "Alert",            "Confirm Action", "You have entered a Fingerprint tile, so you will trigger an alarm unless you use a hack token"
#define LASER_TEXT                  "Alert",            "Confirm Action", "You have entered a Laser tile, you can either spend and extra action or use a hack token to avoid triggering an alarm"
#define DEADBOLT_TEXT               "Alert",            "Confirm action", "You moved to a deadbolt, in order to enter you must spend 3 actions, otherwise you will go back to your previous tile"
#define LAVATORY_TEXT               "Alert",            "Confirm Action", "You have encountered a guard! Luckily you are in the lavatory so you can choose between using one of the stealth tokens in it or one of your own"
#define MOTION_TEXT                 "Alert",            "Confirm Action", "You are leaving an activated motion sensor. An alarm will be triggered unless you use a hack token"
#define OFFER_LOOT_TEXT             "Loot Offer",       "Confirm Offer",  "The other player offers you the loot: "
#define ASK_FOR_LOOT_TEXT           "Loot Request",     "Confirm Request","The other player ask you for the loot: "
#define SPOTTER_SPECIAL_ACTION_TEXT "Spotter guard peek", "Complete action", "You have spotted the top of the guard deck. Would you like to let it on the top or put it at the bottom?"
/*Botones de los dialog box:*/
#define ACCEPT_TEXTB "Accept"
#define DECLINE_TEXTB "Decline"
#define USE_LAVATORY_TOKEN_TEXTB "Use Lavatory stealth token"
#define USE_MY_STEALTH_TOKEN_TEXTB "Use one of my stealth tokens"
#define SPEND_ACTIONS_TEXTB "Spend actions"
#define GET_BACK_TEXTB  "Go back"
#define SPEND_ACTION_TEXTB "Spend action"
#define USE_HACK_TOKEN_TEXTB "Use hack token"
#define TRIGGER_ALARM_TEXTB "Trigger alarm"
#define SPOTTER_TOP         "Top"
#define SPOTTER_BOTTOM      "Bottom"
#define SPOTTER_NO_PREV_CHOICE "..."

#define INMORTAL //Comentar para perder cuando se te terminan las vidas.

typedef enum {WAITING_FOR_ACTION, WAITING_FOR_USER_CONFIRMATION, WAITING_FOR_DICE, WAITING_FOR_GUARD_INIT, WAITING_FOR_LOOT, IN_LOOT_EXCHANGE, WAITING_DICE_FOR_LOOT, DESPUES_VEMOS_A, DESPUES_VEMOS_B} ModelStatus;
typedef enum {GUARD_STEP_TO, GUARD_CARD_PICK} LocationMeaning;
typedef struct{
    LocationMeaning meaning;
    CardLocation cardLocation;
}GuardMoveInfo;


typedef enum{ALARM_TRIGGERED, SAFE_CRACKED, KEYPAD_OPENED, STAIRS, CHIHUAHUA_BARKS, KITTY_ESCAPED, LOST, WON, NO_IE}importantEvents;

class BurgleBrosModel : public Model
{
    public:
	BurgleBrosModel();
        void reset();
        PlayerId getPlayerOnTurn();
        bool isGuardsTurn();
        bool hasGameFinished();
        string getFinishMsg();
        /* Funciones para inicializar al modelo.*/
        void initBoard(vector<CardName> &allTiles,CardLocation initPos, unsigned int *safeNumber);     //Dado un vector de cardNames: Si está vacío, se genera el board y se le pasa toda la información, si tiene 48 cartas, se copian las cartas, sino genera un error.
        void initPlayer(PlayerId playerId, string playerName, CharacterName playerCharacter, CardLocation playerPos);   
        void setInitTurn(PlayerId playerId);
        void copyGuardInitPos(CardLocation guardPos, CardLocation guardDiePos);          //Esta función es inteligente y si se inicializó el guardia del primer piso y se la llama, inicializa el del segundo piso, etc.
        void generateGuardInitPos(CardLocation *guardPos, CardLocation *guardDiePos);   //Esta función es inteligente y si se inicializó el guardia del primer piso y se la llama, inicializa el del segundo piso, etc.
        /* Funciones para obtener información para los observers. */
        pair<bool,CardLocation> getGoldBarInfo();
        vector<wall> getInfo2DrawWalls();
	vector<Info2DrawCards> getInfo2DrawCards();
        list<Info2DrawLoot> getInfo2DrawLoot();
	Info2DrawPlayer getInfo2DrawPlayer(PlayerId player);
        Info2DrawGuard getInfo2DrawGuard(unsigned int floor);
	list<Info2DrawTokens> getInfo2DrawTokens();
        vector<unsigned int> getInfo2DrawExtraDices();
        importantEvents getInfoOfEvents();
        /*Funciones para cuando pide cartelito con info extra.*/
        bool isMotionSpecialCase();
        CardLocation locationOfComputerRoomOrLavatory(CardName computerRoomOrLavatory);
        bool isThereACpuRoomOrLavatory(CardLocation pos, CardName whichTypeOfTile);
        ModelStatus getModelStatus();
        vector<string> getMsgToShow();
        PlayerId getPlayerOnTurnBeforeGuardMove();
        bool userDecidedTo(string decision); // Devuelve si hay que llamar a move guard.
        /*Otras funciones*/
        void setDice(vector<unsigned int> &dice);
        bool dieForLootNeeded();      //Si es necesario tirar un dado para el chihuahua o persian kitty
        void continueGame();        //Si el juego estaba parado por los dados para el loot, se le dice que continúe.
        unsigned int rollDieForLoot(unsigned int die);          //Tira un dado para los loots y devuelve el número que salió. en el caso que tenga chihuahua y persian kitty el player, primero se tira por chihuahua y luego por persian kitty.
        /* Acciones que se puede llamar públicamente*/
        void pass(PlayerId playerId);
        unsigned int peek(PlayerId playerId, CardLocation locationToPeek,int safeNumber); //Recibe que jugador a que tile va a mirar, y si se sabe de antemano el número de safe, se lo asigna a la carta que estaba dada vuelta. devuelve el numero de safe de esa carta.
        unsigned int move(PlayerId playerId, CardLocation locationToMove, int safeNumber); //Si es del otro jugador, recibe el safeNumber desde afuera, devuelve el safe number de la carta a la que se mueve.
        bool moveRequiresToInitGuard(CardLocation locationToMove);
        void addToken(PlayerId playerId, CardLocation locationToAddToken);
        void crackSafe(PlayerId playerId,vector<unsigned int> &dice); 
        void setLoot(PlayerId playerId, Loot *loot);
        void addDieToSafe(PlayerId playerId, CardLocation safe);
        void createAlarm(PlayerId playerId, CardLocation tile);
        void placeCrow(PlayerId playerId, CardLocation tile);
        void pickLoot(PlayerId playerId);
        void askForLoot(PlayerId playerId, Loot loot);
        void offerLoot(PlayerId playerId, Loot loot);
        void escape(PlayerId playerId, CardLocation stairTile);
        string peekGuardsCard(PlayerId playerId, CardLocation **guardCard, unsigned int floor, string prevChoice);
        bool isGuardMoving();
        void guardMove();
        void setGuardWholePath(list<GuardMoveInfo> wholePath);
        list<GuardMoveInfo> getGuardWholePath();
        void clearGuardWholePath();
        list<GuardMoveInfo> generateGuardPath();//ver si tiene que ir aca o afuera
        /*Prueba para ver si se pueden realizar ciertas acciones*/
        bool isMovePosible(PlayerId playerId,CardLocation tileToMove);  //Pregunta si una movida es posible
        bool isPeekPosible(PlayerId player, CardLocation tile);         //Pregunta si un peek es posible
        bool isAddTokenPosible(PlayerId player, CardLocation tile);
        bool isAddDieToSafePossible(PlayerId player, CardLocation tile);
        bool isCrackSafePossible(PlayerId playerId,CardLocation tile);
        bool isCreateAlarmPossible(PlayerId playerId, CardLocation tile);
        bool isPlaceCrowPossible(PlayerId playerId, CardLocation tile);
        bool isPickLootPossible(PlayerId playerId, Loot loot);
        bool isAskForLootPossible(PlayerId playerId, CardLocation tile, Loot loot);
        bool isOfferLootPossible(PlayerId playerId, CardLocation tile, Loot loot);
        bool isEscapePossible(PlayerId playerId, CardLocation tile);
        bool isPeekGuardsCardPossible(PlayerId playerId, unsigned int guardsFloor);
        list<string> getPosibleActions(PlayerId player, CardLocation tile);   //Devuelve que acciones puede realizar el jugador indicado en esa tile

        void incOtherPlayerTokensUsed();
	~BurgleBrosModel();
    private:

        void copyGuardMove();
        void endGuardMove();
        bool anotherLavatoryInGPath();
        
        void checkTurns();
        void checkIfWonOrLost();
        void handlePersianKittyMove(unsigned int die);
        void handleChihuahuaMove(unsigned int die);
        void triggerAlarm(CardLocation tile);
        void triggerSilentAlarm(unsigned int floor);
        void handleSpecialMoveFromMotion(CardLocation movingTo,bool *cardWasVisible);
        bool GuardInCamera();
        list<CardLocation> setGuardsNewPath(unsigned int floor);
        list<CardLocation> setGuardsNewPath(unsigned int floor,CardLocation thisTarget);
        list<CardLocation> setGuardsNewPath(list<CardLocation> &alarmList, BurgleBrosGuard *p2Guard);
        bool playerSpentFreeAction;
        BurgleBrosPlayer * getP2Player(PlayerId playerId);
        BurgleBrosPlayer * getP2OtherPlayer(PlayerId playerId);

        BurgleBrosPlayer myPlayer;
	BurgleBrosPlayer otherPlayer;
	BurgleBrosBoard board;
	BurgleBrosGuard guards[BOARD_STANDARD_FLOORS];
	BurgleBrosTokens tokens;
	BurgleBrosLoots loots;
	BurgleBrosDices dice;
        importantEvents iE;
        bool gameFinished;
        string finishMsg;       //Si el juego terminó indica como termino (por ejemplo WON, LOST o MODEL ERROR:"(errormsg)"
        ModelStatus status;         //Para las preguntas al usuario
        vector<string> msgsToShow;      //Contiene el texto y sus respuestas.
        CardLocation prevLoc;
        PlayerId playerOnTurnBeforeGuardMove;
        Loot lootOfferedOrAskedFor;
        unsigned int rollForLootCount;
        bool specialMotionCase;
        vector<string> auxMsgsToShow;
        CardLocation spyGuardCard;
        pair<list<GuardMoveInfo>,list<GuardMoveInfo>::iterator> gWholePath;
        unsigned int nmbrOfPendingQuestions;
        unsigned int otherPlayerTokensUsed;
};
#endif

