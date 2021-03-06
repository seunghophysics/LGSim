#include <fstream>
#include <string>

#include "G4SystemOfUnits.hh"

#include "LGSimAnalysis.hh"
#include "LGSimRunAction.hh"
#include "LGSimPMT.hh"
#include "LGSimPMTMessenger.hh"

LGSimPMT::LGSimPMT(G4String name) 
:G4VSensitiveDetector(name), fMessenger(nullptr), qe_nhits(0.), p2c(3.8e-13), fileName("signal.csv")
{
    fMessenger = new LGSimPMTMessenger(this);
    collectionName.insert("LGSimPMTHitCollection");
}

LGSimPMT::~LGSimPMT() {}

G4double LGSimPMT::GetQE(G4double energy)
{
    G4double qetable[] = {0., 0.04, 0.2, 0.22, 0.3, 0.3, 0.15, 0.03, 0.};
    G4double eVtable[] = {1.77*eV, 2.07*eV, 2.48*eV, 2.76*eV, 3.10*eV, 3.54*eV, 4.13*eV, 4.43*eV, 4.77*eV};
    
    G4int tableSize = 9;
    
    G4int i = -1;
    while(energy >= eVtable[i+1]){
        i++;
    }
    
    if(i < 0 || i >= tableSize - 1)
        return 0.;
    else
        return (qetable[i+1] - qetable[i]) * (energy - eVtable[i]) 
                / (eVtable[i+1] - eVtable[i]) + qetable[i];
    
}

void LGSimPMT::Initialize(G4HCofThisEvent* HCE)
{
    hitCollection = new LGSimPMTHitCollection(GetName(), collectionName[0]);
    
    static G4int HCID = -1;
    if(HCID < 0) HCID = GetCollectionID(0);
    HCE->AddHitsCollection(HCID, hitCollection);
    
    qe_nhits = 0;
    p2c = 8.4e-13;                 // photon-to-charge ratio (gain)
}

G4bool LGSimPMT::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{   
    auto aTrack = aStep->GetTrack();
    auto aParticle = aTrack->GetDynamicParticle()->GetDefinition()->GetParticleName();
    
    G4double eqCharge = 0.;
    G4double qe = 0.;
    
    if(aParticle == "opticalphoton" && aStep->IsFirstStepInVolume()){
        qe = GetQE(aTrack->GetKineticEnergy());
        qe_nhits += qe;
        eqCharge = p2c * qe;
        
        LGSimPMTHit* aHit = new LGSimPMTHit();
        aHit->SetEqCharge(eqCharge);
        aHit->SetHitTime(aTrack->GetGlobalTime());
        if(eqCharge) hitCollection->insert(aHit);
    }
    
    return true;
}

void LGSimPMT::EndOfEvent(G4HCofThisEvent*)
{
    static G4int eventID = 1;
    
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    G4double q_dep = qe_nhits * p2c;      // deposited charge
    
    if(q_dep > 0){
        analysisManager->FillNtupleDColumn(2, 1, q_dep);
        analysisManager->AddNtupleRow(2);
        //G4cout << "Hit: " << nhits << " photons" << G4endl;
        //G4cout << "Deposited charge: " << q_dep * 1e12 << " pC" << G4endl;
    }
    
    //G4int threadID = G4Threading::G4GetThreadId();
    
    std::ofstream signalCSV;
    //std::string fileName("signal_" + std::to_string(threadID) + ".csv");
    //signalCSV.open(fileName.c_str(), std::ios_base::app);
    signalCSV.open(fileName.data(), std::ios_base::app);
    
    for(G4int i=0; i<(G4int)hitCollection->entries(); i++){
        signalCSV << eventID << "," 
                    << (*hitCollection)[i]->GetHitTime() << "," 
                    << (*hitCollection)[i]->GetEqCharge() << std::endl;
    }
    
    eventID++;
}