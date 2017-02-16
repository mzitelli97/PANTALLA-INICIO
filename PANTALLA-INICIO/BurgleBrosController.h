#ifndef BURGLEBROSCONTROLLER_H
#define BURGLEBROSCONTROLLER_H
#include "BurgleBrosModel.h"
#include "BurgleBrosView.h"
#include "BurgleBrosSound.h"
#include "EventData.h"
#include "NetworkED.h"
#include "Controller.h"
#include "NetworkInterface.h"

typedef enum{INITIALIZING, PLAYING}GameStatus;
typedef enum{FIRST_DECIDING_PLAYER, SECOND_DECIDING_PLAYER}PlayAgainId;//Para cuando se decide jugar de vuelta y se toma como server o cliente, para diferenciarlos esta esto.
typedef enum{USER_QUIT, GAME_WON}QuitCause;

typedef struct
{
    string PlayerName;
    CharacterName playersCharacter;
    CardLocation initPos;
}AuxInitInfo;


#define DEFAULT_WIN_MSG "WON!","You have won the game","Now that youve won the game, you can choose if either play again or quit.","Play again","Quit"
#define DEFAULT_LOST_MSG "LOST!","You have lost the game","Now that youve lost the game, you can choose if either play again or quit.","Play again","Quit"
#define DEFAULT_PLAY_AGAIN_MSG "Play again?", "The game has finished", "The other Player has chosen to play again, do you agree?", "Play again", "Quit"
#define DEFAULT_QUIT_MSG "Quit", "The game will close", "The other Player has closed the game, this game will close itself", "OK"
#define DEFAULT_GAME_OVER_MSG "Game over", "The game will close", " The other player decided not to play again, so this game will close itself", "OK"
#define DEFAULT_SPOTTER_MSG "Hability used", "The Spotter","The other player uses The Spotter hability and choose to put the card at the ","Ok"
#define DEFAULT_TIMEOUT_MSG "Error", "A timeout has ocurred (60 secs without a response)", "After 60 secs of not recieving a confirmation of your action in the other cpu, the game closes itself", "OK"
#define DEFAULT_OTHERS_ERROR_MSG "Error", "An error ocurred", "The other player detected an error, so the game will close", "ok"

class BurgleBrosController:public Controller {
public:
    BurgleBrosController();
    BurgleBrosController(const BurgleBrosController& orig);
    void attachNetworkInterface(NetworkInterface * p2NetworkInterface);
    void setCommunicationRoleNThisPlayerName(CommunicationRole communicationRole, string name);
    void attachModel(BurgleBrosModel *gamePointer);
    void attachView(BurgleBrosView *view);
    void attachSound(BurgleBrosSound *sound);
    bool checkIfGameFinished();
    void parseMouseEvent(EventData *mouseEvent);
    void parseNetworkEvent(EventData *networkEvent);
    void parseKeyboardEvent(EventData *keyboardEvent) override;
    void parseTimerEvent(EventData* mouseEvent) override;
    bool hasToResetTimeoutTimer();
    bool isWaiting4ack();

    virtual string askForSpentOK(vector<string> &message);
    string getUsersResponse(vector<string> &message);
    
    virtual ~BurgleBrosController();
private:
    string askThisPlayerAndProcess(vector<string> &message);
    string handleThisPlayerMotionSpecialCase(vector<string> &message);
    string handleOtherPlayerMotionSpecialCase(vector<string> &message);
    string processOtherPlayerBasicChoice(vector<string> &message);
    void handlePlayAgain();
    void handleLootsExchange(NetworkED * networkEvent);
    void clientInitRoutine(NetworkED *networkEvent);
    void serverInitRoutine(NetworkED *networkEvent);
    void firstDecidedRoutine(NetworkED *networkEvent);
    void secondDecidedRoutine(NetworkED *networkEvent);
    void interpretNetworkAction(NetworkED *networkEvent);
    void interpretAction(string action, CardLocation location);
    void analizeIfModelRequiresMoreActions(NetworkED *networkEvent);
    void handleWonOrLost(PerezProtocolHeader msg);
    void checkGameStatus();
    void resetGame();
    void checkForNonOrderedPackets();
    NetworkInterface * networkInterface;
    bool firstInitDone;
    CommunicationRole communicationRole;
    unsigned int initPacketCount;
    AuxInitInfo auxInitInfo[NUMBER_OF_PLAYERS]; 
    list<NetworkED> packetToAnalize; //Para eventos como move, que necesita varios paquetes para ejecutarse completamente, esta ésta queue.
    BurgleBrosModel *modelPointer;
    BurgleBrosView *view;
    BurgleBrosSound *sound;
    bool aMoveActionPending;
    bool iStarted;
    CardLocation previousMovingToLocation;//tiene pinta de model
    string thisPlayerName;
    bool quit;
    PlayAgainId whichPlayer;
    bool waiting4ack;
    GameStatus status;
    QuitCause quitCause;
    bool waiting4QuitAck;
    bool resetTimeoutTimer;
};

#endif /* BURGLEBROSCONTROLLER_H */

