#ifndef LGSimPMT_HH
#define LGSimPMT_HH 1

#include "G4VSensitiveDetector.hh"

#include "LGSimPMTHit.hh"

class LGSimPMTMessenger;
class LGSimRunAction;

class LGSimPMT : public G4VSensitiveDetector
{
    public:
        LGSimPMT(G4String name);
        ~LGSimPMT();
        
        G4double GetQE(G4double energy);
        
        inline void SetFileName(G4String name) { fileName = name; }
        inline const char* GetFileName() { return fileName.data(); }
        
        void Initialize(G4HCofThisEvent*);
        G4bool ProcessHits(G4Step* aStep, G4TouchableHistory*);
        void EndOfEvent(G4HCofThisEvent*);
        
    private:
        LGSimPMTHitCollection* hitCollection;
        LGSimPMTMessenger* fMessenger;
        G4double qe_nhits;
        G4double p2c;
        G4String fileName;
};

#endif