#include <iostream>
#include <fstream>

#include "LGSimRunAction.hh"

LGSimRunAction::LGSimRunAction() {}
LGSimRunAction::~LGSimRunAction() {}

void LGSimRunAction::BeginOfRunAction(const G4Run*)
{
    txt.open("charge_data.txt", std::ofstream::out | std::ofstream::app);
}

void LGSimRunAction::EndOfRunAction(const G4Run*)
{
    txt.close();
}

void LGSimRunAction::Fill(G4double q)
{
    txt << q << "\n";
}