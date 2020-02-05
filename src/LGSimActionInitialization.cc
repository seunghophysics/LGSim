#include "LGSimActionInitialization.hh"
#include "LGSimPrimaryGeneratorAction.hh"
#include "LGSimRunAction.hh"

LGSimActionInitialization::LGSimActionInitialization(LGSimRunAction* runAction)
: G4VUserActionInitialization(), fRunAction(runAction) {}

LGSimActionInitialization::~LGSimActionInitialization() {}

void LGSimActionInitialization::Build() const
{
    SetUserAction(fRunAction);
    SetUserAction(new LGSimPrimaryGeneratorAction());
}

void LGSimActionInitialization::BuildForMaster() const
{
    SetUserAction(fRunAction);
}