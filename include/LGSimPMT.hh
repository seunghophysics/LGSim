#ifndef LGSimPMT_HH
#define LGSimPMT_HH 1

#include "G4VSensitiveDetector.hh"

class LGSimRunAction;

class LGSimPMT : public G4VSensitiveDetector
{
    public:
        LGSimPMT(G4String name);
        ~LGSimPMT();
        
        void Initialize(G4HCofThisEvent*);
        G4bool ProcessHits(G4Step* aStep, G4TouchableHistory*);
        void EndOfEvent(G4HCofThisEvent*);
    
    private:
        G4int nhits;
        G4double qe;                     // quantum efficiency of photocathode
        G4double p2c;
};

#endif