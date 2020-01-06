#include <iostream>
#include <fstream>

#include "LGSimRunAction.hh"

LGSimRunAction::LGSimRunAction() {}
LGSimRunAction::~LGSimRunAction() {}

void LGSimRunAction::BeginOfRunAction(const G4Run*)
{
    txt.open("charge_data.txt");
}

void LGSimRunAction::EndOfRunAction(const G4Run*)
{
    txt.close();
}

inline void LGSimRunAction::Fill(G4double q)
{
    txt << q << "\n";
}