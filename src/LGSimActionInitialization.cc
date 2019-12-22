#include "LGSimActionInitialization.hh"
#include "LGSimPrimaryGeneratorAction.hh"

LGSimActionInitialization::LGSimActionInitialization() {}
LGSimActionInitialization::~LGSimActionInitialization() {}

void LGSimActionInitialization::Build() const
{
    SetUserAction(new LGSimPrimaryGeneratorAction());
}
