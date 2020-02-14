#include "LGSimActionInitialization.hh"
#include "LGSimPrimaryGeneratorAction.hh"
#include "LGSimRunAction.hh"

LGSimActionInitialization::LGSimActionInitialization()
: G4VUserActionInitialization()
{}

LGSimActionInitialization::~LGSimActionInitialization() {}

void LGSimActionInitialization::Build() const
{
    SetUserAction(new LGSimRunAction());
    SetUserAction(new LGSimPrimaryGeneratorAction());
}

void LGSimActionInitialization::BuildForMaster() const
{
    SetUserAction(new LGSimRunAction());
}