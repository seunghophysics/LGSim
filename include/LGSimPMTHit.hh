#ifndef LGSimPMTHit_HH
#define LGSimPMTHit_HH 1

#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4VHit.hh"

class LGSimPMTHit : public G4VHit
{
    public:
        LGSimPMTHit();
        ~LGSimPMTHit();
        
        //inline void* operator new(size_t);
        //inline void operator delete(void* aHit);
        
        inline void SetHitTime(G4double t) { fHitTime = t; }
        inline G4double GetHitTime() const { return fHitTime; }
        
        inline void SetEqCharge(G4double q) { fEqCharge = q; }
        inline G4double GetEqCharge() const { return fEqCharge; }
        
        void Print();
        
    private:
        G4double fEqCharge;
        G4double fHitTime;
};

typedef G4THitsCollection<LGSimPMTHit> LGSimPMTHitCollection;
extern G4ThreadLocal G4Allocator<LGSimPMTHit> LGSimPMTHitAllocator;

/*
inline void* LGSimPMTHit::operator new(size_t)
{
    void* aHit;
    aHit = (void*) LGSimPMTHitAllocator.MallocSingle();
    return aHit;
}

inline void LGSimPMTHit::operator delete(void* aHit)
{
    LGSimPMTHitAllocator.FreeSingle((LGSimPMTHit*) aHit);
}
*/

#endif