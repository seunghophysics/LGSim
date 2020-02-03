#ifndef LGSimSensitiveDetector_HH
#define LGSimSensitiveDetector_HH 1

#include "G4VSensitiveDetector.hh"

class LGSimRunAction;

class LGSimPMT : public G4VSensitiveDetector
{
    public:
        LGSimPMT(LGSimRunAction* runAction, G4String name);
        ~LGSimPMT();
        
        void Initialize(G4HCofThisEvent*);
        G4bool ProcessHits(G4Step* aStep, G4TouchableHistory*);
        void EndOfEvent(G4HCofThisEvent*);
    
    private:
        G4int nhits;
        LGSimRunAction* fRunAction;
};

#endif