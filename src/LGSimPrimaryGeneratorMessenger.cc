#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"

#include "LGSimPrimaryGeneratorAction.hh"
#include "LGSimPrimaryGeneratorMessenger.hh"

LGSimPrimaryGeneratorMessenger::LGSimPrimaryGeneratorMessenger(LGSimPrimaryGeneratorAction* PGA)
: fPGA(PGA)
{
    fDir = new G4UIdirectory("/CRY/");
    fDir->SetGuidance("CRY control");
    
    fCmdTurnOnOff = new G4UIcmdWithAString("/CRY/turn", this);
    fCmdTurnOnOff->SetGuidance("Turn on/off CRY.");
    fCmdTurnOnOff->SetCandidates("on off");
    
    fCmdVerbose = new G4UIcmdWithAnInteger("/CRY/verbose", this);
    fCmdVerbose->SetGuidance("Set CRY verbosity.");
    
    fCmdScintZ = new G4UIcmdWithADoubleAndUnit("/CRY/scintZfromCenter", this);
    fCmdScintZ->SetGuidance("Set scintillator Z position from the center.");
    fCmdScintZ->SetDefaultUnit("cm");
}

LGSimPrimaryGeneratorMessenger::~LGSimPrimaryGeneratorMessenger()
{
    delete fPGA;
    delete fDir;
    delete fCmdTurnOnOff;
    delete fCmdVerbose;
    delete fCmdScintZ;
}

void LGSimPrimaryGeneratorMessenger::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
    if(cmd == fCmdTurnOnOff){
        if(newValue == "on")
            fPGA->SetCRY_STATUS(1);
        else if(newValue == "off")
            fPGA->SetCRY_STATUS(0);
    }
    
    if(cmd == fCmdVerbose){
        if(newValue == "1")
            fPGA->SetCRY_Verbose(1);
        else if(newValue == "0")
            fPGA->SetCRY_Verbose(0);
    }
    
    if(cmd == fCmdScintZ){
        fPGA->SetScintZ(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue));
    }
}
