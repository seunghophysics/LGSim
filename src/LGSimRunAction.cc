#include <iostream>
#include <fstream>

#include "G4AutoLock.hh"
#include "globals.hh"

#include "LGSimAnalysis.hh"
#include "LGSimRunAction.hh"

LGSimRunAction::LGSimRunAction() {}
LGSimRunAction::~LGSimRunAction() { delete G4AnalysisManager::Instance(); }

void LGSimRunAction::BeginOfRunAction(const G4Run*)
{   
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile("data");
    
    if(analysisManager->GetFirstNtupleId() != 1)
        analysisManager->SetFirstNtupleId(1);
    if(analysisManager->GetFirstNtupleColumnId() != 1)
        analysisManager->SetFirstNtupleColumnId(1);
    
    analysisManager->CreateNtuple("lg", "kinematics");
    analysisManager->CreateNtupleDColumn("initial_KE");
    analysisManager->CreateNtupleDColumn("path_length");
    analysisManager->CreateNtupleDColumn("e_dep");
    analysisManager->FinishNtuple();
    
    analysisManager->CreateNtuple("pmt", "charge");
    analysisManager->CreateNtupleDColumn("charge");
    analysisManager->FinishNtuple();
}

void LGSimRunAction::EndOfRunAction(const G4Run*)
{   
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}