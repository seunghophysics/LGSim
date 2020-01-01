#include "G4Box.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4OpticalSurface.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
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
    G4Material* vacuum = nistManager->FindOrBuildMaterial("G4_AIR");
    G4Material* leadglass = nistManager->FindOrBuildMaterial("G4_GLASS_LEAD");
    leadglass->SetMaterialPropertiesTable(lgMPT);
    
    // World
    G4Box* worldSolid = new G4Box("WorldSolid", 1.*m, 1.*m, 1.*m);
    G4LogicalVolume* worldLV = new G4LogicalVolume(worldSolid, vacuum, "WorldLV");
    G4VPhysicalVolume* worldPV = new G4PVPlacement(G4Transform3D(), "WorldPV", worldLV, 0, false, 0);
    
    // Lead-glass
    G4Box *lgBoxSolid = new G4Box("LGBoxSolid", 15.*cm, 5.*cm, 5.*cm);
    G4LogicalVolume* lgBoxLV = new G4LogicalVolume(lgBoxSolid, leadglass, "LGBoxLV");
    G4VPhysicalVolume* lgBoxPV = new G4PVPlacement(G4Transform3D(), "LGBoxPV", lgBoxLV, worldPV, false, 0);
    
    G4Tubs *lgTubeSolid = new G4Tubs("LGTubeSolid", 0, 3.5*cm, 1.5*cm, 0, 360.*deg);
    G4LogicalVolume* lgTubeLV = new G4LogicalVolume(lgTubeSolid, leadglass, "LGTubeLV");
    G4RotationMatrix rotLGTube; rotLGTube.rotateY(90.*deg); G4ThreeVector vLGTube(16.5*cm, 0, 0);
    new G4PVPlacement(G4Transform3D(rotLGTube, vLGTube), "LGTubePV", lgTubeLV, worldPV, false, 0, true);
    
    G4OpticalSurface* lgSurface = new G4OpticalSurface("LGSurface");
    new G4LogicalBorderSurface("LGBorderSurface", lgBoxPV, worldPV, lgSurface);
    lgSurface->SetType(dielectric_LUT);
    lgSurface->SetModel(LUT);
    lgSurface->SetFinish(polishedlumirrorair);
    
    // Visualization settings
    worldLV->SetVisAttributes(G4VisAttributes::Invisible);
    
    return worldPV;
}