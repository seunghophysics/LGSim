#include "G4UIcmdWithAString.hh"

#include "LGSimPrimaryGeneratorAction.hh"
#include "LGSimPrimaryGeneratorMessenger.hh"

LGSimPrimaryGeneratorMessenger::LGSimPrimaryGeneratorMessenger(LGSimPrimaryGeneratorAction* PGA)
: fPGA(PGA)
{
    fDir = new G4UIdirectory("/CRY/");
    fDir->SetGuidance("CRY on/off control");
    
    fCmd = new G4UIcmdWithAString("/CRY/turn", this);
    fCmd->SetGuidance("Turn on/off CRY.");
    fCmd->SetCandidates("on off");
}

LGSimPrimaryGeneratorMessenger::~LGSimPrimaryGeneratorMessenger()
{
    delete fDir;
    delete fCmd;
}

void LGSimPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
    if(cmd == fCmd){
        if(newValue == "on")
            fPGA->SetCRY_STATUS(1);
        else if(newValue == "off")
            fPGA->SetCRY_STATUS(0);
    }
}
