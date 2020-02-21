#ifndef LGSimPrimaryGeneratorMessenger_HH
#define LGSimPrimaryGeneratorMessenger_HH 1

#include "G4UImessenger.hh"

class LGSimPrimaryGeneratorAction;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;

class LGSimPrimaryGeneratorMessenger : public G4UImessenger
{
    public:
        LGSimPrimaryGeneratorMessenger(LGSimPrimaryGeneratorAction* PGA);
        ~LGSimPrimaryGeneratorMessenger();
        
        void SetNewValue(G4UIcommand* cmd, G4String newValue);
        
    private:
        LGSimPrimaryGeneratorAction* fPGA;
        G4UIdirectory* fDir;
        G4UIcmdWithAString* fCmdTurnOnOff;
        G4UIcmdWithAnInteger* fCmdVerbose;
        G4UIcmdWithADoubleAndUnit* fCmdScintZ;
};

#endif