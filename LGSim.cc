#include "G4OpticalPhysics.hh"
#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "FTFP_BERT.hh"

#include "LGSimDetectorConstruction.hh"
#include "LGSimActionInitialization.hh"

int main(int argc, char** argv)
{
    // physics list
    auto LGSimPhysicsList = new FTFP_BERT();
    LGSimPhysicsList->RegisterPhysics(new G4OpticalPhysics());
    
    auto runManager = new G4RunManager();
    runManager->SetUserInitialization(new LGSimDetectorConstruction());
    runManager->SetUserInitialization(LGSimPhysicsList);
    runManager->SetUserInitialization(new LGSimActionInitialization());
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