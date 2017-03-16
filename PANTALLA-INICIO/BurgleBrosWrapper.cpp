#include "BurgleBrosWrapper.h"
#include <ostream>
#include <fstream>
#include <ctime>


BurgleBrosWrapper::BurgleBrosWrapper() {
    srand(time(NULL));
    
    quit=false;
    error=false;
    allegroEvent=nullptr;
    networkEvent=nullptr;
    p2Controller=nullptr;
    p2Model=nullptr;
    p2View=nullptr;
    errorMsg="Errores ocurridos: \n";        
    
    allegroEvent = new AllegroEG;
        if(allegroEvent == nullptr){error=true; quit=true; errorMsg+="Error al crear AllegroEvent\n";}
    CModel *initModel = new CModel;
        if( initModel == nullptr){error=true; quit=true; errorMsg+="Error al crear el CModel\n";}
    CView *initView = new CView(initModel);
        if(initView == nullptr){error=true; quit=true; errorMsg+="Error al crear CView\n";}
        if(initView != nullptr && initView->DidAnErrorStarting()){
            error=true; quit=true; errorMsg+="Error al inicializar cView:"+initView->geterrorMsg();}
    cController *initController = new cController;
        if(initController == nullptr){error=true; quit=true; errorMsg+="Error al crear cController \n";}

    
    if(!error)        //si no ocurrio un error 
    {
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
        
    if(bbcontroller != nullptr)
            bbcontroller->attachNetworkInterface(&networkInterface);
    else{ error=true; quit=true; errorMsg+="Error al crear BurgleBrosController \n";}

    networkEvent= new NetworkingEG;
    if(networkEvent == nullptr) {error=true; quit=true; errorMsg+="Error al crear NetworkEvent\n";}
    
    
    if(initController != nullptr)
    {
        while(!connect(bbcontroller) && !quit)
        {
            quit = initController->userQuit();
            if(gui.hayEvento())
                gui.parseEvento();
        }
        if(!quit )  //Si el usuario no salio 
        {
            delete p2Controller;
            delete p2Model;
            delete p2View;
            
            networkEvent->attachNetworkingInterface(&networkInterface);
            gui.attachEventGenerator(networkEvent);
            BurgleBrosModel *newModel = new BurgleBrosModel;
                if(newModel == nullptr){error=true; quit=true; errorMsg+="Error al crear BurgleBrosModel\n";}
            BurgleBrosView *newView= new BurgleBrosView(newModel);
                if(newView == nullptr){error=true; quit=true; errorMsg+="Error al crear BurgleBrosView\n";}
                if(newView != nullptr && newView->didAnErrorStarting()){error=true; quit=true; errorMsg+="Error al inciliazar BurgleBrosview:"+newView->getErrorMsg();}
            
            if(!error)    //y no ocurrio un error
            {
                newModel->attach(newView);
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
}

void
BurgleBrosWrapper::playGame() {

    string aux;
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
                if(controller->resetZMouse())
                    allegroEvent->resetZMouse();
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
        errorMsg+="\nError de networking:\n"+networkInterface.getErrorMsg();
        quit=true;
    }
    return retVal;
}

bool BurgleBrosWrapper::wasAnError() {
    return error;
}

void BurgleBrosWrapper::showError() {
    
  time_t rawtime;
  struct tm * timeinfo;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  
  string auxError="#Eda Burgle Bros:\t";
  auxError+=asctime (timeinfo);
  
  auxError+="\n" + errorMsg;
    
  std::ofstream archivo;
  archivo.open("Error/errores.txt",std::ios::app);
  
  archivo<<auxError + "\n";
  cout<<auxError;
  
  archivo.close();
}


BurgleBrosWrapper::BurgleBrosWrapper(const BurgleBrosWrapper& orig) {
}

BurgleBrosWrapper::~BurgleBrosWrapper() {
    
  if(networkEvent != nullptr) delete  networkEvent;                     
  if(allegroEvent != nullptr) delete  allegroEvent;
}

