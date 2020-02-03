#ifndef LGSimPrimaryGeneratorAction_HH
#define LGSimPrimaryGeneratorAction_HH 1

#include "G4ParticleGun.hh"
#include "G4Types.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class CRYGenerator;

G4bool HitsScintillators(G4ThreeVector pos, G4ThreeVector dir);
class LGSimPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
    public:
        LGSimPrimaryGeneratorAction();
        ~LGSimPrimaryGeneratorAction();
        
        void GeneratePrimaries(G4Event*);
    
    private:
        G4ParticleGun* fParticleGun;
        G4ParticleTable* fParticleTable;
        CRYGenerator* fCRYGenerator;
};


#endif