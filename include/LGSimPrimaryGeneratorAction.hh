#ifndef LGSimPrimaryGeneratorAction_HH
#define LGSimPrimaryGeneratorAction_HH 1

#include "G4ParticleGun.hh"
#include "G4Types.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class LGSimPrimaryGeneratorMessenger;
class CRYGenerator;
class LGSimPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
    public:
        LGSimPrimaryGeneratorAction();
        ~LGSimPrimaryGeneratorAction();
        
        void GeneratePrimaries(G4Event*);
        G4bool HitsScintBars(G4ThreeVector pos, G4ThreeVector dir);
        
        inline G4bool GetCRY_STATUS() { return bCRY_STATUS; }
        inline void SetCRY_STATUS(G4bool b) { bCRY_STATUS = b; }
        
        inline G4int GetCRY_Verbose() { return fCRYVerbosity; }
        inline void SetCRY_Verbose(G4int i) { fCRYVerbosity = i; }
        
        inline G4double GetScintZ() { return scintZ; }
        inline void SetScintZ(G4double d) { scintZ = d; }
        
    private:
        G4ParticleGun* fParticleGun;
        G4ParticleTable* fParticleTable;
        
        CRYGenerator* fCRYGenerator;
        G4bool bCRY_STATUS;
        G4int fCRYVerbosity;
        G4double scintZ;
        
        LGSimPrimaryGeneratorMessenger* fMessenger;
};


#endif