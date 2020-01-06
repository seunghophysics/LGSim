#ifndef LGSimRunAction_HH
#define LGSimRunAction_HH 1

#include <fstream>

#include "G4UserRunAction.hh"

class LGSimRunAction : public G4UserRunAction
{
    public:
        LGSimRunAction();
        ~LGSimRunAction();
        
        void BeginOfRunAction(const G4Run*);
        void EndOfRunAction(const G4Run*);
        
        void Fill(G4double q);
    
    private:
        std::ofstream txt;
};

#endif