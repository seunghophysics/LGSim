#ifndef LGSimActionInitialization_HH
#define LGSimActionInitialization_HH 1

#include "G4VUserActionInitialization.hh"

class LGSimActionInitialization : public G4VUserActionInitialization
{
    public:
        LGSimActionInitialization();
        ~LGSimActionInitialization();
        
        void Build() const;
};

#endif