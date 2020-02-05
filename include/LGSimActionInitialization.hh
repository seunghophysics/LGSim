#ifndef LGSimActionInitialization_HH
#define LGSimActionInitialization_HH 1

#include "G4VUserActionInitialization.hh"

class LGSimRunAction;

class LGSimActionInitialization : public G4VUserActionInitialization
{
    public:
        LGSimActionInitialization(LGSimRunAction* runAction);
        ~LGSimActionInitialization();
        
        virtual void Build() const;
        void BuildForMaster() const;
        
    private:
        LGSimRunAction* fRunAction;
};

#endif