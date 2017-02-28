#include "BurgleBrosWrapper.h"


BurgleBrosWrapper::BurgleBrosWrapper() {
    srand(time(NULL));
    //allegro_startup();
    quit=false;
    
    allegroEvent = new AllegroEG;
    CModel *initModel = new CModel;
    CView *initView = new CView(initModel);
    cController *initController = new cController;
    initModel->attach(initView);
    initController->attachView(initView);
    initController->attachModel(initModel);
    initController->attachSound(&sound);
    initView->update();
    gui.attachController(initController);
    gui.attachEventGenerator((EventGenerator *) allegroEvent);
    p2Controller=initController;
    p2Model=initModel;
    p2View=initView;
}

bool 
BurgleBrosWrapper::gameOnCourse() {
    return !quit;
}

void 
BurgleBrosWrapper::getNameAndIp() {

    cController *initController = dynamic_cast<cController *>(p2Controller);
    CModel *initModel = dynamic_cast<CModel *>(p2Model);
    if(initController != nullptr && initModel != nullptr)
    {
        while(!initController->checkIfConnecting() && !quit)
        {
            quit = initController->userQuit();
            if(gui.hayEvento())
                gui.parseEvento();
        }
        name=initModel->getInfo().entries[MY_NAME];
        ipToConnect=initModel->getInfo().entries[IP_TO_CONNECT];
    }
    else
        quit=true;
       
}

void
BurgleBrosWrapper::connect() 
{
    cController *initController = dynamic_cast<cController *>(p2Controller);
    BurgleBrosController *bbcontroller = new BurgleBrosController;
    networkEvent= new NetworkingEG;
    bbcontroller->attachNetworkInterface(&networkInterface);
    
    if(initController != nullptr)
    {
        while(!connect(bbcontroller) && !quit)
        {
            quit = initController->userQuit();
            if(gui.hayEvento())
                gui.parseEvento();
        }
        if(!quit)
        {
            delete p2Controller;
            delete p2Model;
            delete p2View;
            
            networkEvent->attachNetworkingInterface(&networkInterface);
            gui.attachEventGenerator(networkEvent);
            BurgleBrosModel *newModel = new BurgleBrosModel;
            BurgleBrosView *newView= new BurgleBrosView(newModel);
            newModel->attach(newView);
            newModel->attach(&sound);
            newModel->attachController(bbcontroller);
            sound.attachModel(newModel);
            bbcontroller->attachSound(&sound);
            bbcontroller->attachModel(newModel);
            bbcontroller->attachView(newView);
            gui.attachController(bbcontroller);
            p2Controller = bbcontroller;
            p2Model = newModel;
            p2View = newView;
        }
    }    
}

void
BurgleBrosWrapper::playGame() {

    
    allegroEvent->playTimer();
    BurgleBrosController *controller = dynamic_cast<BurgleBrosController *>(p2Controller);
    //gui.setMouseZResolution(MOUSE_Z_RESOL_REQUIRED);
    bool prev=false;
    if( controller != nullptr)
    {
        while(!controller->checkIfGameFinished())
        {
            prev=controller->isWaiting4ack();
            if(gui.hayEvento())
            {
                gui.parseEvento();
                if(controller->hasToResetTimeoutTimer())
                    {allegroEvent->resetTimer(); cout<<"Resetie timer"<<endl;}
                if(controller->isWaiting4ack() && !prev)
                    {allegroEvent->playTimer();cout<<"Playie timer"<<endl;}
                else if (!controller->isWaiting4ack())
                    {allegroEvent->stopTimer(); cout<<"apague timer"<<endl;}
                if(controller->resetZMouse());
                    //gui.resetZMouse();
            }
        }
    }
    delete p2Controller;
    delete p2Model;
    delete p2View; 
    
    
}


bool BurgleBrosWrapper::connect(BurgleBrosController* controller) {
    bool retVal = false;
    if(networkInterface.standardConnectionStart(ipToConnect))
    {
        controller->setCommunicationRoleNThisPlayerName(networkInterface.getCommunicationRole(), name);
        retVal = true;
    }
    if(networkInterface.checkError())    //Si hubo un error tratando de hacer la connection start:
    {
        cout<<networkInterface.getErrorMsg();
        quit=true;
    }
    return retVal;
}



BurgleBrosWrapper::BurgleBrosWrapper(const BurgleBrosWrapper& orig) {
}

BurgleBrosWrapper::~BurgleBrosWrapper() {
  // delete  networkEvent;                     // mirar esto
  // delete  allegroEvent;
}

