#ifndef LGSimSensitiveDetector_HH
#define LGSimSensitiveDetector_HH 1

#include "G4VSensitiveDetector.hh"

class LGSimRunAction;

class LGSimSensitiveDetector : public G4VSensitiveDetector
{
    public:
        LGSimSensitiveDetector(LGSimRunAction* runAction, G4String name);
        ~LGSimSensitiveDetector();
        
        void Initialize(G4HCofThisEvent*);
        G4bool ProcessHits(G4Step*, G4TouchableHistory*);
        void EndOfEvent(G4HCofThisEvent*);
    
    private:
        G4double q;
        LGSimRunAction* fRunAction;
};

#endif