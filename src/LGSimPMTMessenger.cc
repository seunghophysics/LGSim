#include "G4UIcmdWithAString.hh"

#include "LGSimPMT.hh"
#include "LGSimPMTMessenger.hh"

LGSimPMTMessenger::LGSimPMTMessenger(LGSimPMT* lgPMT)
: fPMT(lgPMT)
{
    fDir = new G4UIdirectory("/PMT/");
    fDir->SetGuidance("PMT control");
    
    fCmdFileName = new G4UIcmdWithAString("/PMT/saveas", this);
    fCmdFileName->SetGuidance("Set save file name.");
}

LGSimPMTMessenger::~LGSimPMTMessenger()
{
    delete fPMT;
    delete fDir;
    delete fCmdFileName;
}

void LGSimPMTMessenger::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
    if(cmd == fCmdFileName) fPMT->SetFileName(newValue);
}