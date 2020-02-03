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

G4bool HitsScintBars(G4ThreeVector pos, G4ThreeVector dir)
{
    G4double t_top = (5*cm - pos.x()) / dir.x();
    G4double t_bottom = (-5*cm - pos.x()) / dir.x();
    
    G4double y_top = pos.y() + dir.y() * t_top;
    G4double z_top = pos.z() + dir.z() * t_top;
    G4double y_bottom = pos.y() + dir.y() * t_bottom;
    G4double z_bottom = pos.z() + dir.z() * t_bottom;
    
    G4cout << "y_top: " << y_top / cm << " z_top: " << z_top / cm << G4endl;
    G4cout << "y_bottom: " << y_bottom / cm << " z_bottom: " << z_bottom / cm << G4endl;
    
    G4bool hitsTopBar = (-30*cm < y_top && y_top < 5*cm && -5*cm < z_top && z_top < 5*cm);
    G4bool hitsBottomBar = (-5*cm < y_bottom && y_bottom < 30*cm && -5*cm < z_bottom && z_bottom < 5*cm);
    
    G4cout << "hitsTopBar: " << hitsTopBar << G4endl;
    G4cout << "hitsBottomBar: " << hitsBottomBar << G4endl;
    
    return hitsTopBar && hitsBottomBar;
}

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
            
            G4ThreeVector pos = G4ThreeVector(10*cm, (*evVector)[j]->x()*m, (*evVector)[j]->y()*m);
            G4ThreeVector dir = G4ThreeVector((*evVector)[j]->w(), (*evVector)[j]->u(), (*evVector)[j]->v());
            
            fParticleGun->SetParticlePosition(pos);
            fParticleGun->SetParticleMomentumDirection(dir);
            fParticleGun->SetParticleTime((*evVector)[j]->t());
            
            if(HitsScintBars(pos, dir)){
            fParticleGun->SetParticleDefinition(fParticleTable->FindParticle((*evVector)[j]->PDGid()));
            fParticleGun->SetParticleEnergy((*evVector)[j]->ke()*MeV);
            fParticleGun->GeneratePrimaryVertex(anEvent);
            G4cout << "Triggered! Time simulated: " << fCRYGenerator->timeSimulated() << " sec" << G4endl;
            }
            else{
            fParticleGun->SetParticleDefinition(fParticleTable->FindParticle("geantino"));
            fParticleGun->SetParticleEnergy(1*keV);
            fParticleGun->GeneratePrimaryVertex(anEvent);
            }
            delete (*evVector)[j];
        }
}