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
#include "G4Trap.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "LGSimDetectorConstruction.hh"
#include "LGSimLGSD.hh"
#include "LGSimPMT.hh"

LGSimDetectorConstruction::LGSimDetectorConstruction() {}

LGSimDetectorConstruction::~LGSimDetectorConstruction() {}

G4VPhysicalVolume* LGSimDetectorConstruction::Construct()
{    
    //----------------------------------------------------------------------------------
    // ** Materials **
    //----------------------------------------------------------------------------------
    //
    // Air, Aluminum
    G4NistManager* nistManager = G4NistManager::Instance();
    G4Material* air = nistManager->FindOrBuildMaterial("G4_AIR");
    G4Material* al = nistManager->FindOrBuildMaterial("G4_Al");
    //G4Material* scint = nistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    //G4Material* leadglass = nistManager->FindOrBuildMaterial("G4_GLASS_LEAD");
    //
    // SiO2, PbO, K2O, Na2O for SF6W
    G4Material* siliconDioxide = nistManager->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
    G4Material* leadOxide = nistManager->FindOrBuildMaterial("G4_LEAD_OXIDE");
    G4Material* potassiumOxide = nistManager->FindOrBuildMaterial("G4_POTASSIUM_OXIDE");
    G4Material* sodiumMonoxide = nistManager->FindOrBuildMaterial("G4_SODIUM_MONOXIDE");
    //
    // Sb203
    G4Element* oxygen = new G4Element("Oxygen", "O", 8, 16.0 * g/mole);
    G4Element* antimony = new G4Element("Antimony", "Sb", 51, 121.76 * g/mole);
    G4Material* antimonyTrioxideMaterial= new G4Material("Sb2O3", 5.2*g/cm3, 2); 
    antimonyTrioxideMaterial->AddElement(antimony, 2);
    antimonyTrioxideMaterial->AddElement(oxygen, 3);
    //
    // Lead-glass (SF6W)
    G4Material* leadglass = new G4Material("leadglass", 5.2*g/cm3, 5);
    leadglass->AddMaterial(siliconDioxide, 27.3*perCent);
    leadglass->AddMaterial(leadOxide, 70.9*perCent);
    leadglass->AddMaterial(potassiumOxide, 0.9*perCent);
    leadglass->AddMaterial(sodiumMonoxide, 0.6*perCent);
    leadglass->AddMaterial(antimonyTrioxideMaterial, 0.3*perCent);
    
  
    //----------------------------------------------------------------------------------
    // ** Optical Properties **
    //----------------------------------------------------------------------------------
    //
    // SF6W optical properties
    G4double refEnergy[] = {1.24*eV, 1.27*eV, 1.29*eV, 1.32*eV, 1.35*eV, 
                            1.38*eV, 1.41*eV, 1.44*eV, 1.48*eV, 1.51*eV, 
                            1.55*eV, 1.59*eV, 1.63*eV, 1.68*eV, 1.72*eV, 
                            1.77*eV, 1.82*eV, 1.88*eV, 1.94*eV, 2.0*eV, 
                            2.07*eV, 2.14*eV, 2.21*eV, 2.3*eV, 2.38*eV, 
                            2.48*eV, 2.58*eV, 2.7*eV, 2.82*eV, 2.95*eV, 
                            3.1*eV, 3.26*eV, 3.44*eV, 3.65*eV, 3.87*eV,
                            4.13*eV, 4.43*eV, 4.77*eV, 5.17*eV, 5.64*eV, 6.2*eV};
    G4double refIndex[] = {1.77568, 1.77635, 1.77705, 1.77779, 1.77858, 
                            1.77941, 1.78029, 1.78123, 1.78223, 1.7833, 
                            1.78445, 1.78569, 1.78703, 1.78847, 1.79004, 
                            1.79176, 1.79363, 1.79569, 1.79796, 1.80048, 
                            1.80328, 1.80641, 1.80993, 1.81391, 1.81844, 
                            1.82365, 1.82968, 1.83673, 1.84508, 1.85511, 
                            1.86739, 1.88274, 1.9025, 1.92879, 1.96527,
                            2.01844, 2.10029, 2.23367, 2.4627, 2.87238, 3.62362};           
    const G4int refNbins = sizeof(refEnergy) / sizeof(G4double);
    assert(sizeof(refEnergy) == sizeof(refIndex));

    G4double absEnergy[] = {0.62*eV, 0.83*eV, 1.17*eV, 1.55*eV, 1.77*eV, 
                            1.91*eV, 2.07*eV, 2.25*eV, 2.48*eV, 2.58*eV, 
                            2.7*eV, 2.82*eV, 2.95*eV, 3.1*eV, 3.18*eV, 
                            3.26*eV, 3.35*eV, 3.44*eV};
    G4double absLength[] = {79.32*cm, 141.01*cm, 188.99*cm, 572.76*cm, 572.76*cm, 
                            284.94*cm, 284.94*cm, 284.94*cm, 188.99*cm, 188.99*cm, 
                            112.23*cm, 93.03*cm, 54.64*cm, 25.80*cm, 14.93*cm, 
                            7.41*cm, 3.14*cm, 1.04*cm};
    const G4int absNbins = sizeof(absEnergy) / sizeof(G4double);
    assert(sizeof(absEnergy) == sizeof(absLength));
    
    G4MaterialPropertiesTable* lgMPT = new G4MaterialPropertiesTable();
    lgMPT->AddProperty("RINDEX", refEnergy, refIndex, refNbins)->SetSpline(true);
    lgMPT->AddProperty("ABSLENGTH", absEnergy, absLength, absNbins)->SetSpline(true);
    leadglass->SetMaterialPropertiesTable(lgMPT);
    //
    // Lead-glass mirror surface
    G4OpticalSurface* lgMirrorSurface = new G4OpticalSurface("LGMirrorSurface");
    lgMirrorSurface->SetType(dielectric_LUT);
    lgMirrorSurface->SetModel(LUT);
    lgMirrorSurface->SetFinish(polishedlumirrorair);
    //
    // Lead-glass taped surface
    //G4OpticalSurface* lgTapedSurface = new G4OpticalSurface("LGTapedSurface");
    //lgTapedSurface->SetType(dielectric_LUT);
    //lgTapedSurface->SetModel(LUT);
    //lgTapedSurface->SetFinish(groundbackpainted);
    
    
    //----------------------------------------------------------------------------------
    // ** Volumes **
    //----------------------------------------------------------------------------------
    //
    // World
    G4Box* worldSolid = new G4Box("WorldSolid", 1.*m, 1.*m, 1.*m);
    G4LogicalVolume* worldLV = new G4LogicalVolume(worldSolid, air, "WorldLV");
    G4VPhysicalVolume* worldPV = new G4PVPlacement(G4Transform3D(), "WorldPV", worldLV, 0, false, 0);
    //
    //　Al side cover
    G4Trap* alSideSolid = new G4Trap("AlSideSolid", 122.4*mm, 340.2*mm, 135.4*mm, 113.4*mm);
    G4LogicalVolume* alSideLV = new G4LogicalVolume(alSideSolid, al, "AlSideLV");
    G4RotationMatrix rotAlSide; rotAlSide.rotateX(270.*deg); G4ThreeVector vAlSide(0, 0, -.1*mm);
    G4VPhysicalVolume* alSidePV = new G4PVPlacement(G4Transform3D(rotAlSide, vAlSide), "AlSidePV", alSideLV, worldPV, false, 0);
    //                                                                                                                                                                      
    // Lead-glass block
    G4Trap* lgBoxSolid = new G4Trap("LGBoxSolid", 122.*mm, 340.*mm, 135.*mm, 113.*mm);
    G4LogicalVolume* lgBoxLV = new G4LogicalVolume(lgBoxSolid, leadglass, "LGBoxLV");
    G4RotationMatrix rotLGBox; rotLGBox.rotateX(0/*270.*deg*/); G4ThreeVector vLGBox(0, -.1*mm, 0);
    G4VPhysicalVolume* lgBoxPV = new G4PVPlacement(G4Transform3D(rotLGBox, vLGBox), "LGBoxPV", lgBoxLV, alSidePV, false, 0);
    //
    // light guide
    G4Tubs *lgTubeSolid = new G4Tubs("LGTubeSolid", 0, 38.*mm, 15.*mm, 0, 360.*deg);
    G4LogicalVolume* lgTubeLV = new G4LogicalVolume(lgTubeSolid, leadglass, "LGTubeLV");
    G4RotationMatrix rotLGTube; rotLGTube.rotateY(0.*deg); G4ThreeVector vLGTube(5.5*mm, 0, 185.*mm);
    new G4PVPlacement(G4Transform3D(rotLGTube, vLGTube), "LGTubePV", lgTubeLV, worldPV, false, 0);
    //
    // Al top cover
    G4Box* alBoxSolid = new G4Box("AlBoxSolid", 67.5*mm, 61.*mm, 0.1*mm);
    G4Tubs* alTubeSolid = new G4Tubs("AlTubeSolid", 0, 38.*mm, 0.1*mm, 0, 360.*deg);
    G4VSolid* alPlateSolid = new G4SubtractionSolid("AlPlateSolid", alBoxSolid, alTubeSolid, G4Transform3D());
    G4LogicalVolume* alPlateLV = new G4LogicalVolume(alPlateSolid, al, "AlPlateLV");
    G4RotationMatrix rotAlPlate; rotAlPlate.rotateY(0.*deg); G4ThreeVector vAlPlate(5.5*mm, 0, 170.1*mm);
    /*G4VPhysicalVolume* alPlatePV = */new G4PVPlacement(G4Transform3D(rotAlPlate, vAlPlate), "AlPlatePV", alPlateLV, worldPV, false, 0);
    //
    // PMT Photocathode
    G4Tubs *cathodeSolid= new G4Tubs("CathodeSolid", 0, 38.*mm, 1.*mm, 0, 360.*deg);
    G4LogicalVolume* cathodeLV = new G4LogicalVolume(cathodeSolid, leadglass, "CathodeLV");
    G4RotationMatrix rotCathode; rotCathode.rotateY(0.*deg); G4ThreeVector vCathode(5.5*mm, 0, 201.*mm);
    new G4PVPlacement(G4Transform3D(rotCathode, vCathode), "CathodePV", cathodeLV, worldPV, false, 0);
    //
    // Optical border surfaces
    new G4LogicalBorderSurface("LGMirrorBorderSurface", lgBoxPV, alSidePV, lgMirrorSurface);
    //new G4LogicalBorderSurface("LGTapedBorderSurface", lgBoxPV, alPlatePV, lgTapedSurface);
    
    // Visualization settings
    worldLV->SetVisAttributes(G4VisAttributes::Invisible);
    
    return worldPV;
}

void LGSimDetectorConstruction::ConstructSDandField()
{
    LGSimLGSD* lgSD = new LGSimLGSD("LGSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(lgSD);
    SetSensitiveDetector("LGBoxLV", lgSD);
    SetSensitiveDetector("LGTubeLV", lgSD);
    
    LGSimPMT* pmtSD = new LGSimPMT("PMTSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(pmtSD);
    SetSensitiveDetector("CathodeLV", pmtSD);
}