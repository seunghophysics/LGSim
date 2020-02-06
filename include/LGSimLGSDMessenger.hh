#ifndef LGSimLGSDMessenger_HH
#define LGSimLGSDMessenger_HH 1

#include "G4UImessenger.hh"

class LGSimLGSD;
class G4UIcmdWithAnInteger;

class LGSimLGSDMessenger : public G4UImessenger
{
    public:
        LGSimLGSDMessenger(LGSimLGSD* lgSD);
        ~LGSimLGSDMessenger();
        
        void SetNewValue(G4UIcommand* cmd, G4String newValue);
        
    private:
        LGSimLGSD* fLGSD;
        G4UIdirectory* fDir;
        G4UIcmdWithAnInteger* fCmdVerbose;
};

#endif