#include "LGSimRunAction.hh"
#include "LGSimSensitiveDetector.hh"

LGSimSensitiveDetector::LGSimSensitiveDetector(LGSimRunAction* runAction, G4String name) 
:G4VSensitiveDetector(name), nhits(0), fRunAction(runAction) {}

LGSimSensitiveDetector::~LGSimSensitiveDetector() {}

void LGSimSensitiveDetector::Initialize(G4HCofThisEvent*)
{
    nhits = 0;
}

G4bool LGSimSensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{   
    auto aTrack = aStep->GetTrack();
    auto aParticle = aTrack->GetDynamicParticle()->GetDefinition()->GetParticleName();
    
    if(aParticle == "opticalphoton"){
        nhits++;
    }
    
    return true;
}

void LGSimSensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    G4double qe = 0.3;                      // quantum efficiency of photocathode
    G4double p2c = 3.8e-13;                 // photon-to-charge ratio (gain)
    G4double q_dep = nhits * p2c * qe;      // deposited charge
    
    if(nhits > 0){
        fRunAction->Fill(q_dep);
        G4cout << "Hit: " << nhits << " photons" << G4endl;
        G4cout << "Deposited charge: " << q_dep * 1e12 << " pC" << G4endl;
    }
    else
        G4cout << "No hit detected." << G4endl;
        
}