#ifndef LGSimPMTMessenger_HH
#define LGSimPMTMessenger_HH 1

#include "G4UImessenger.hh"

class LGSimPMT;
class G4UIcmdWithAString;

class LGSimPMTMessenger : public G4UImessenger
{
    public:
        LGSimPMTMessenger(LGSimPMT* lgPMT);
        ~LGSimPMTMessenger();
        
        void SetNewValue(G4UIcommand* cmd, G4String newValue);
        
    private:
        LGSimPMT* fPMT;
        G4UIdirectory* fDir;
        G4UIcmdWithAString* fCmdFileName;
};

#endif