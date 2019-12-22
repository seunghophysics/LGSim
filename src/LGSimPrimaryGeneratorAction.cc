#include "LGSimPrimaryGeneratorAction.hh"

LGSimPrimaryGeneratorAction::LGSimPrimaryGeneratorAction()
: fParticleGun(nullptr)
{
    fParticleGun = new G4ParticleGun();
}

LGSimPrimaryGeneratorAction::~LGSimPrimaryGeneratorAction()
{
    delete fParticleGun;
}

void LGSimPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    fParticleGun->GeneratePrimaryVertex(anEvent);
}