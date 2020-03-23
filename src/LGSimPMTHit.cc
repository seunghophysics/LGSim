#include "LGSimPMTHit.hh"

G4ThreadLocal G4Allocator<LGSimPMTHit> LGSimPMTHitAllocator;

LGSimPMTHit::LGSimPMTHit() {}
LGSimPMTHit::~LGSimPMTHit() {}

void LGSimPMTHit::Print()
{
    G4cout << "=======================================" << G4endl;
    G4cout << "Time: " << fHitTime << " sec" << G4endl;
    G4cout << "EqCharge: " << fEqCharge * 1e12 << " pC" << G4endl;
    G4cout << "=======================================" << G4endl;
 }