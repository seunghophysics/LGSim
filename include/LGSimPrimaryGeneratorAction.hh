#ifndef LGSimPrimaryGeneratorAction_HH
#define LGSimPrimaryGeneratorAction_HH 1

#include "G4ParticleGun.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class LGSimPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
    public:
        LGSimPrimaryGeneratorAction();
        ~LGSimPrimaryGeneratorAction();
        
        void GeneratePrimaries(G4Event*);
    
    private:
        G4ParticleGun* fParticleGun;
};

#endif