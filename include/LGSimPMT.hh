#ifndef LGSimPMT_HH
#define LGSimPMT_HH 1

#include "G4VSensitiveDetector.hh"

class LGSimRunAction;

class LGSimPMT : public G4VSensitiveDetector
{
    public:
        LGSimPMT(G4String name);
        ~LGSimPMT();
        
        G4double GetQE(G4double energy);
        
        void Initialize(G4HCofThisEvent*);
        G4bool ProcessHits(G4Step* aStep, G4TouchableHistory*);
        void EndOfEvent(G4HCofThisEvent*);
        
    private:
        G4double qe_nhits;
        G4double p2c;
};

#endif