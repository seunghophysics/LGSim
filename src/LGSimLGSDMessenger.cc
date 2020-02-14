#include "G4UIcmdWithAnInteger.hh"

#include "LGSimLGSD.hh"
#include "LGSimLGSDMessenger.hh"

LGSimLGSDMessenger::LGSimLGSDMessenger(LGSimLGSD* lgSD)
: fLGSD(lgSD)
{
    fLGSD = lgSD;
    
    fDir = new G4UIdirectory("/LGSD/");
    fDir->SetGuidance("LGSD control");
    
    fCmdVerbose = new G4UIcmdWithAnInteger("/LGSD/verbose", this);
    fCmdVerbose->SetGuidance("Set LGSD verbosity.");
}

LGSimLGSDMessenger::~LGSimLGSDMessenger()
{
    delete fLGSD;
    delete fDir;
    delete fCmdVerbose;
}

void LGSimLGSDMessenger::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
    if(cmd == fCmdVerbose){
        if(newValue == "1")
            fLGSD->SetVerbose(1);
        else if(newValue == "0")
            fLGSD->SetVerbose(0);
    }
}
