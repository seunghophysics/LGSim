#include "LGSimAnalysis.hh"
#include "LGSimRunAction.hh"
#include "LGSimLGSD.hh"
#include "LGSimLGSDMessenger.hh"

LGSimLGSD::LGSimLGSD(G4String name) 
:G4VSensitiveDetector(name), 
pathLength(0.), Edep(0.), initialKE(0.), initialKEChecked(false), fVerbosity(0)
{
    fMessenger = new LGSimLGSDMessenger(this);
}

LGSimLGSD::~LGSimLGSD() {} //delete fMessenger; }

void LGSimLGSD::Initialize(G4HCofThisEvent*)
{
    pathLength = 0.;
    Edep = 0.;
    initialKE = 0.;
    initialKEChecked = false;
}

G4bool LGSimLGSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{   
    auto aTrack = aStep->GetTrack();
    auto aParticle = aTrack->GetDynamicParticle()->GetDefinition()->GetParticleName();
    
    if(aParticle == "mu-" || aParticle == "mu+"){
        if(!initialKEChecked){
            initialKE = aTrack->GetKineticEnergy();
            initialKEChecked = true;
        }
        pathLength += aStep->GetStepLength();
        Edep += aStep->GetTotalEnergyDeposit();
    }
    
    return true;
}

void LGSimLGSD::EndOfEvent(G4HCofThisEvent*)
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    if(initialKEChecked){
        analysisManager->FillNtupleDColumn(1, 1, initialKE);
        analysisManager->FillNtupleDColumn(1, 2, pathLength);
        analysisManager->FillNtupleDColumn(1, 3, Edep);
        analysisManager->AddNtupleRow(1);
        
        if(fVerbosity){
            G4cout << "Initial KE: " << initialKE / 1e3 << " GeV\n" 
                    << "Path length: " << pathLength / 10. << " cm\n" 
                    << "E_dep: " << Edep << " MeV" << G4endl; 
        }
    }
}