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
#include "LGSimPrimaryGeneratorMessenger.hh"

G4bool LGSimPrimaryGeneratorAction::HitsScintBars(G4ThreeVector pos, G4ThreeVector dir)
{
    G4double t_top = (30.3*cm - pos.y()) / dir.y();
    G4double t_bottom = (-12.7*cm - pos.y()) / dir.y();
    
    G4double x_top = pos.x() + dir.x() * t_top;
    G4double z_top = pos.z() + dir.z() * t_top;
    G4double x_bottom = pos.x() + dir.x() * t_bottom;
    G4double z_bottom = pos.z() + dir.z() * t_bottom;
    
    //G4cout << "x_top: " << x_top / cm << " z_top: " << z_top / cm << G4endl;
    //G4cout << "x_bottom: " << x_bottom / cm << " z_bottom: " << z_bottom / cm << G4endl;
    
    G4bool hitsTopBar = (-23.8*cm < x_top && x_top < 6.2*cm && -7.5*cm + scintZ < z_top && z_top < 7.5*cm + scintZ);
    G4bool hitsBottomBar = (-23.8*cm < x_bottom && x_bottom < 6.2*cm && -7.5*cm + scintZ < z_bottom && z_bottom < 7.5*cm + scintZ);
    
    //G4cout << "hitsTopBar: " << hitsTopBar << G4endl;
    //G4cout << "hitsBottomBar: " << hitsBottomBar << G4endl;
    
    return hitsTopBar && hitsBottomBar;
}

LGSimPrimaryGeneratorAction::LGSimPrimaryGeneratorAction()
: fParticleGun(nullptr), fParticleTable(nullptr), 
fCRYGenerator(nullptr), bCRY_STATUS(true), fCRYVerbosity(0), scintZ(0.), fMessenger(nullptr)
{
    fParticleGun = new G4ParticleGun();
    fParticleTable = G4ParticleTable::GetParticleTable();
    
    fMessenger = new LGSimPrimaryGeneratorMessenger(this);
    
    const char* crysetupfile = "CRY.setup";
    std::ifstream inputFile;
    inputFile.open(crysetupfile, std::ios::in);
    char buffer[1000];
    
    if(inputFile.fail())
        G4cout << "Failed to open input file " << crysetupfile << G4endl;
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
    //delete fParticleGun;
    //delete fParticleTable;
    //delete fCRYGenerator;
    //delete fMessenger;
}

void LGSimPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    if(bCRY_STATUS){
        std::vector<CRYParticle*>* evVector = new std::vector<CRYParticle*>;
        evVector->clear();
        fCRYGenerator->genEvent(evVector);
        
        G4String particleName;
        
        for(unsigned j=0; j < evVector->size(); j++){
            particleName = CRYUtils::partName((*evVector)[j]->id());
            
            //....debug output  
            //G4cout << "  "          << particleName << " "
            //    << "charge = "      << (*evVector)[j]->charge() << " "
            //     << "energy (MeV) = " << (*evVector)[j]->ke() * MeV << " "
            //     << "pos (mm) "
            //     << G4ThreeVector((*evVector)[j]->x()*m, 10.*cm, (*evVector)[j]->y()*m)
            //     << " " << "direction cosines "
            //    << G4ThreeVector((*evVector)[j]->u(), (*evVector)[j]->w(), (*evVector)[j]->v())
            //     << " " << G4endl;
            
            G4ThreeVector pos = G4ThreeVector((*evVector)[j]->x()*m, 10.*cm, (*evVector)[j]->y()*m);
            G4ThreeVector dir = G4ThreeVector((*evVector)[j]->u(), (*evVector)[j]->w(), (*evVector)[j]->v());
            
            fParticleGun->SetParticlePosition(pos);
            fParticleGun->SetParticleMomentumDirection(dir);
            fParticleGun->SetParticleTime((*evVector)[j]->t());
            
            if(HitsScintBars(pos, dir)){
            fParticleGun->SetParticleDefinition(fParticleTable->FindParticle((*evVector)[j]->PDGid()));
            fParticleGun->SetParticleEnergy((*evVector)[j]->ke()*MeV);
            fParticleGun->GeneratePrimaryVertex(anEvent);
            if(fCRYVerbosity)
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
    
    fParticleGun->GeneratePrimaryVertex(anEvent);
    G4ThreeVector gunpos = fParticleGun->GetParticlePosition();
    G4ThreeVector gundir = fParticleGun->GetParticleMomentumDirection();
    if(HitsScintBars(gunpos, gundir)){
        if(fCRYVerbosity){
            G4cout << "Coincidence Hit!" << G4endl;
        }
    }
}