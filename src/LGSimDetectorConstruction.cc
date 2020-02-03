#include "G4Box.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4OpticalSurface.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "LGSimDetectorConstruction.hh"
#include "LGSimPMT.hh"

LGSimDetectorConstruction::LGSimDetectorConstruction(LGSimRunAction* runAction)
: fRunAction(runAction) {}

LGSimDetectorConstruction::~LGSimDetectorConstruction() {}

G4VPhysicalVolume* LGSimDetectorConstruction::Construct()
{   
    // Optical properties
    G4double photonEnergy[] = {1.8*eV, 3.1*eV};
    G4double refIndex[] = {1.61, 1.65};
    G4double absLength[] = {250.*cm, 90.*cm};
    G4MaterialPropertiesTable* lgMPT = new G4MaterialPropertiesTable();
    lgMPT->AddProperty("RINDEX", photonEnergy, refIndex, 2)->SetSpline(true);
    lgMPT->AddProperty("ABSLENGTH", photonEnergy, absLength, 2)->SetSpline(true);
    
    // Materials
    G4NistManager* nistManager = G4NistManager::Instance();
    G4Material* air = nistManager->FindOrBuildMaterial("G4_AIR");
    G4Material* al = nistManager->FindOrBuildMaterial("G4_Al");
    G4Material* leadglass = nistManager->FindOrBuildMaterial("G4_GLASS_LEAD");
    leadglass->SetMaterialPropertiesTable(lgMPT);
    
    // World
    G4Box* worldSolid = new G4Box("WorldSolid", 1.*m, 1.*m, 1.*m);
    G4LogicalVolume* worldLV = new G4LogicalVolume(worldSolid, air, "WorldLV");
    G4VPhysicalVolume* worldPV = new G4PVPlacement(G4Transform3D(), "WorldPV", worldLV, 0, false, 0);
    
    // Lead-glass
    G4Box* lgBoxSolid = new G4Box("LGBoxSolid", 5.*cm, 5.*cm, 15.*cm);
    G4LogicalVolume* lgBoxLV = new G4LogicalVolume(lgBoxSolid, leadglass, "LGBoxLV");
    G4VPhysicalVolume* lgBoxPV = new G4PVPlacement(G4Transform3D(), "LGBoxPV", lgBoxLV, worldPV, false, 0);
    
    G4Tubs *lgTubeSolid = new G4Tubs("LGTubeSolid", 0, 3.5*cm, 1.5*cm, 0, 360.*deg);
    G4LogicalVolume* lgTubeLV = new G4LogicalVolume(lgTubeSolid, leadglass, "LGTubeLV");
    G4RotationMatrix rotLGTube; rotLGTube.rotateY(0.*deg); G4ThreeVector vLGTube(0, 0, 16.5*cm);
    new G4PVPlacement(G4Transform3D(rotLGTube, vLGTube), "LGTubePV", lgTubeLV, worldPV, false, 0, true);
    
    // Al plate
    G4Box* alBoxSolid = new G4Box("AlBoxSolid", 5.*cm, 5.*cm, 0.1*cm);
    G4Tubs* alTubeSolid = new G4Tubs("AlTubeSolid", 0, 3.5*cm, 0.1*cm, 0, 360.*deg);
    G4VSolid* alPlateSolid = new G4SubtractionSolid("AlPlateSolid", alBoxSolid, alTubeSolid, G4Transform3D());
    G4LogicalVolume* alPlateLV = new G4LogicalVolume(alPlateSolid, al, "AlPlateLV");
    G4RotationMatrix rotAlPlate; rotAlPlate.rotateY(0.*deg); G4ThreeVector vAlPlate(0, 0, 15.1*cm);
    G4VPhysicalVolume* alPlatePV = new G4PVPlacement(G4Transform3D(rotAlPlate, vAlPlate), "AlPlatePV", alPlateLV, worldPV, false, 0, true);
    
    // Lead-glass mirror surface
    G4OpticalSurface* lgMirrorSurface = new G4OpticalSurface("LGMirrorSurface");
    lgMirrorSurface->SetType(dielectric_LUT);
    lgMirrorSurface->SetModel(LUT);
    lgMirrorSurface->SetFinish(polishedlumirrorair);
    new G4LogicalBorderSurface("LGMirrorBorderSurface", lgBoxPV, worldPV, lgMirrorSurface);
    
    // Lead-glass taped surface
    G4OpticalSurface* lgTapedSurface = new G4OpticalSurface("LGTapedSurface");
    new G4LogicalBorderSurface("LGTapedBorderSurface", lgBoxPV, alPlatePV, lgTapedSurface);
    lgMirrorSurface->SetType(dielectric_LUT);
    lgMirrorSurface->SetModel(LUT);
    lgMirrorSurface->SetFinish(groundfrontpainted);
    
    // PMT Photocathode as SD
    G4Tubs *cathodeSolid= new G4Tubs("CathodeSolid", 0, 3.5*cm, 0.1*cm, 0, 360.*deg);
    G4LogicalVolume* cathodeLV = new G4LogicalVolume(cathodeSolid, leadglass, "CathodeLV");
    G4RotationMatrix rotCathode; rotCathode.rotateY(0.*deg); G4ThreeVector vCathode(0, 0, 18.1*cm);
    new G4PVPlacement(G4Transform3D(rotCathode, vCathode), "CathodePV", cathodeLV, worldPV, false, 0, true);
    
    LGSimPMT* aSD = new LGSimPMT(fRunAction, "LGSimSD");
    cathodeLV->SetSensitiveDetector(aSD);
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    sdManager->AddNewDetector(aSD);
    
    // Visualization settings
    worldLV->SetVisAttributes(G4VisAttributes::Invisible);
    
    return worldPV;
}