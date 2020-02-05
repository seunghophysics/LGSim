#ifndef LGSimDetectorConstruction_HH
#define LGSimDetectorConstruction_HH 1

#include "G4VUserDetectorConstruction.hh"

class LGSimRunAction;

class LGSimDetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        LGSimDetectorConstruction();
        ~LGSimDetectorConstruction();
        
        G4VPhysicalVolume* Construct();
        void ConstructSDandField();
};

#endif