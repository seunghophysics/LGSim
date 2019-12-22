#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

#include "LGSimDetectorConstruction.hh"

LGSimDetectorConstruction::LGSimDetectorConstruction() {}
LGSimDetectorConstruction::~LGSimDetectorConstruction() {}

G4VPhysicalVolume* LGSimDetectorConstruction::Construct()
{   
    // Optical properties
    G4double photonEnergy[] = {1.8*eV, 3.1*eV};
    G4double refIndex[] = {1.61, 1.65};
    G4MaterialPropertiesTable* lgMPT = new G4MaterialPropertiesTable();
    lgMPT->AddProperty("RINDEX", photonEnergy, refIndex, 2)->SetSpline(true);
    
    // Materials
    G4NistManager* nistManager = G4NistManager::Instance();
    G4Material* vacuum = nistManager->FindOrBuildMaterial("G4_Galactic");
    G4Material* leadglass = nistManager->FindOrBuildMaterial("G4_GLASS_LEAD");
    leadglass->SetMaterialPropertiesTable(lgMPT);
    
    // World
    G4Box* worldSolid = new G4Box("WorldSolid", 1.*m, 1.*m, 1.*m);
    G4LogicalVolume* worldLV = new G4LogicalVolume(worldSolid, vacuum, "WorldLV");
    G4VPhysicalVolume* worldPV = new G4PVPlacement(G4Transform3D(), "WorldPV", worldLV, 0, false, 0);
    
    // Lead-glass
    G4Box *lgSolid = new G4Box("LGSolid", 15.*cm, 5.*cm, 5.*cm);
    G4LogicalVolume* lgLV = new G4LogicalVolume(lgSolid, leadglass, "LGLV");
    new G4PVPlacement(G4Transform3D(), "LGPV", lgLV, worldPV, false, 0);
    
    // Visualization settings
    worldLV->SetVisAttributes(G4VisAttributes::Invisible);
    
    return worldPV;
}