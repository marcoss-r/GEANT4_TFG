#ifndef SENSITIVEDETECTOR_HH
#define SENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
//Colección de hits del evento actual
#include "G4HCofThisEvent.hh"
#include <fstream>

class SensitiveDetector : public G4VSensitiveDetector
{
public:
    //Constructor y destructor
    SensitiveDetector(const G4String& name);
    ~SensitiveDetector() override;
    
    //Métodos de la clase
    void Initialize(G4HCofThisEvent* hce) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    void EndOfEvent(G4HCofThisEvent* hce) override;

private:

    //Variables para almacenar información del hit
    
    G4bool fHasEntry;                 
    G4String fParticleName;           
    G4double fInitialEnergy;          
    G4double fTotalEnergyDeposit;    
    G4ThreeVector fEntryPosition;     
    G4ThreeVector fEntryMomentum;     
    G4int fEventID;                   

    //Variables estáticas para manejo de archivo CSV y MT
    static std::ofstream fOutputFile;
    static G4bool fFileInitialized;
};

#endif