#ifndef LGSimLGSD_HH
#define LGSimLGSD_HH 1

#include "G4VSensitiveDetector.hh"

class LGSimRunAction;

class LGSimLGSD : public G4VSensitiveDetector
{
    public:
        LGSimLGSD(G4String name);
        ~LGSimLGSD();
        
        void Initialize(G4HCofThisEvent*);
        G4bool ProcessHits(G4Step* aStep, G4TouchableHistory*);
        void EndOfEvent(G4HCofThisEvent*);
    
    private:
        G4double pathLength;
        G4double Edep;
        G4double initialKE;
        G4bool initialKEChecked;
};

#endif