#include "G4OpticalPhysics.hh"
#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "FTFP_BERT.hh"

#include "LGSimDetectorConstruction.hh"
#include "LGSimPrimaryGeneratorAction.hh"
#include "LGSimRunAction.hh"

int main(int argc, char** argv)
{   
    auto runManager = new G4RunManager();
    
    // physics list
    auto physicsList = new FTFP_BERT();
    physicsList->RegisterPhysics(new G4OpticalPhysics());
    runManager->SetUserInitialization(physicsList);
    
    auto runAction = new LGSimRunAction();
    runManager->SetUserInitialization(new LGSimDetectorConstruction(runAction));
    runManager->SetUserAction(new LGSimPrimaryGeneratorAction());
    runManager->SetUserAction(runAction);
    runManager->Initialize();
    
    auto visExecutive = new G4VisExecutive();
    visExecutive->Initialize();
    
    auto uiManager = G4UImanager::GetUIpointer();
    
    auto uiExecutive = new G4UIExecutive(argc, argv);
    uiManager->ExecuteMacroFile("macro/LGSimDefault.mac");
    uiExecutive->SessionStart();
    
    delete uiExecutive;
    delete visExecutive;
    delete runManager;
}