#include <iostream>
#include <fstream>
#include <vector>

#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

#include "CRYGenerator.h"
#include "CRYParticle.h"
#include "CRYSetup.h"

#include "LGSimPrimaryGeneratorAction.hh"

LGSimPrimaryGeneratorAction::LGSimPrimaryGeneratorAction()
: fParticleGun(nullptr), fParticleTable(nullptr), fCRYGenerator(nullptr)
{
    fParticleGun = new G4ParticleGun();
    fParticleTable = G4ParticleTable::GetParticleTable();
    
    std::ifstream inputFile;
    inputFile.open("CRY.setup", std::ios::in);
    char buffer[1000];

    if(inputFile.fail())
        G4cout << "Failed to open input file " << "CRY.setup" << G4endl;
    else{
        std::string setupString("");
        while(!inputFile.getline(buffer,1000).eof()) {
            setupString.append(buffer);
            setupString.append(" ");
        }

    CRYSetup* crySetup = new CRYSetup(setupString, "/Users/seungho/Development/CRY/data");
    fCRYGenerator = new CRYGenerator(crySetup);
    }
}

LGSimPrimaryGeneratorAction::~LGSimPrimaryGeneratorAction()
{
    delete fParticleGun;
}

void LGSimPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
        std::vector<CRYParticle*>* evVector = new std::vector<CRYParticle*>;
        evVector->clear();
        fCRYGenerator->genEvent(evVector);
        
        G4String particleName;
        
        for(unsigned j=0; j < evVector->size(); j++){
            particleName = CRYUtils::partName((*evVector)[j]->id());
            
            //....debug output  
            G4cout << "  "          << particleName << " "
                 << "charge = "      << (*evVector)[j]->charge() << " "
                 << "energy (MeV) = " << (*evVector)[j]->ke() * MeV << " "
                 << "pos (m) "
                 << G4ThreeVector((*evVector)[j]->x(), (*evVector)[j]->y(), (*evVector)[j]->z())
                 << " " << "direction cosines "
                 << G4ThreeVector((*evVector)[j]->u(), (*evVector)[j]->v(), (*evVector)[j]->w())
                 << " " << G4endl;
        
            fParticleGun->SetParticleDefinition(fParticleTable->FindParticle((*evVector)[j]->PDGid()));
            fParticleGun->SetParticleEnergy((*evVector)[j]->ke()*MeV);
            fParticleGun->SetParticlePosition(G4ThreeVector((*evVector)[j]->x()*m, (*evVector)[j]->y()*m, 0.3*m));
            fParticleGun->SetParticleMomentumDirection(G4ThreeVector((*evVector)[j]->u(), (*evVector)[j]->v(), (*evVector)[j]->w()));
            fParticleGun->SetParticleTime((*evVector)[j]->t());
            fParticleGun->GeneratePrimaryVertex(anEvent);
            delete (*evVector)[j];
            G4cout << "Time simulated: " << fCRYGenerator->timeSimulated() << " sec" << G4endl;
        }
    fParticleGun->GeneratePrimaryVertex(anEvent);
}