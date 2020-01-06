#include "LGSimRunAction.hh"
#include "LGSimSensitiveDetector.hh"

LGSimSensitiveDetector::LGSimSensitiveDetector(LGSimRunAction* runAction, G4String name) 
:G4VSensitiveDetector(name), q(0.), fRunAction(runAction) {}

LGSimSensitiveDetector::~LGSimSensitiveDetector() {}

void LGSimSensitiveDetector::Initialize(G4HCofThisEvent*)
{
    q = 0.;
}

G4bool LGSimSensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{   
    auto particle = aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName();
    
    if(particle == "opticalphoton")
        q += 2.e-12;
    
    return true;
}

void LGSimSensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    fRunAction->Fill(q);
}