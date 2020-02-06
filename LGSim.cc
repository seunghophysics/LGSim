#include "G4MTRunManager.hh"

#include "G4OpticalPhysics.hh"
#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "FTFP_BERT.hh"

#include "LGSimActionInitialization.hh"
#include "LGSimDetectorConstruction.hh"
#include "LGSimPrimaryGeneratorAction.hh"
#include "LGSimRunAction.hh"

int main(int argc, char** argv)
{   
    auto runManager = new G4MTRunManager();
    G4int nThreads = G4Threading::G4GetNumberOfCores();
    runManager->SetNumberOfThreads(nThreads);
    
    //auto runManager = new G4RunManager();
    
    // physics list
    auto physicsList = new FTFP_BERT();
    physicsList->RegisterPhysics(new G4OpticalPhysics());
    runManager->SetUserInitialization(physicsList);
    
    auto runAction = new LGSimRunAction();
    runManager->SetUserInitialization(new LGSimDetectorConstruction());
    runManager->SetUserInitialization(new LGSimActionInitialization(runAction));
    runManager->Initialize();
    
    //auto visExecutive = new G4VisExecutive();
    //visExecutive->Initialize();
    
    auto uiManager = G4UImanager::GetUIpointer();
    
    auto uiExecutive = new G4UIExecutive(argc, argv, "tcsh");
    uiManager->ExecuteMacroFile("macro/LGSimDefault.mac");
    uiExecutive->SessionStart();
    
    delete uiExecutive;
    //delete visExecutive;
    delete runManager;
}