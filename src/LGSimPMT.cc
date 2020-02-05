#include "LGSimAnalysis.hh"
#include "LGSimRunAction.hh"
#include "LGSimPMT.hh"

LGSimPMT::LGSimPMT(G4String name) 
:G4VSensitiveDetector(name), nhits(0) {}

LGSimPMT::~LGSimPMT() {}

void LGSimPMT::Initialize(G4HCofThisEvent*)
{
    nhits = 0;
}

G4bool LGSimPMT::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{   
    auto aTrack = aStep->GetTrack();
    auto aParticle = aTrack->GetDynamicParticle()->GetDefinition()->GetParticleName();
    
    if(aParticle == "opticalphoton" && aStep->IsFirstStepInVolume()){
        nhits++;
    }
    
    return true;
}

void LGSimPMT::EndOfEvent(G4HCofThisEvent*)
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    G4double qe = 0.3;                      // quantum efficiency of photocathode
    G4double p2c = 3.8e-13;                 // photon-to-charge ratio (gain)
    G4double q_dep = nhits * p2c * qe;      // deposited charge
    
    if(nhits > 0){
        analysisManager->FillNtupleDColumn(2, 1, q_dep);
        analysisManager->AddNtupleRow(2);
        //G4cout << "Hit: " << nhits << " photons" << G4endl;
        //G4cout << "Deposited charge: " << q_dep * 1e12 << " pC" << G4endl;
    }      
}