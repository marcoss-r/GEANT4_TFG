#include "SensitiveDetector.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include <iomanip>

//Constructor
SensitiveDetector::SensitiveDetector(const G4String& name) : G4VSensitiveDetector(name), fHasEntry(false),
      fInitialEnergy(0), fTotalEnergyDeposit(0), fEventID(-1){

    //Abrir archivo CSV en modo agregar
    fOutputFile.open("detector_hits.csv", std::ios::app);
    
    //Si está vacío, escribir encabezado
    fOutputFile.seekp(0, std::ios::end);
    if(fOutputFile.tellp() == 0){
        fOutputFile << "EventID,ParticleName,Energy_MeV,PosX_m,PosY_m,PosZ_m,"
                    << "Theta_deg,Phi_deg,MomX,MomY,MomZ\n";
    }
}

//Destructor
SensitiveDetector::~SensitiveDetector(){
    if(fOutputFile.is_open()){
        fOutputFile.close();
    }
}

//Método de inicialización llamado al inicio de cada evento
void SensitiveDetector::Initialize(G4HCofThisEvent* hce){

    fHasEntry = false;
    fTotalEnergyDeposit = 0;
    
    fEventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
}

//Procesamiento de hits
G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* history)
{   

    G4Track* track = step->GetTrack();
    
    //Registro de la entrada al detector
    if(!fHasEntry && step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary){
        
        fHasEntry = true;
        
        //Guardar información de entrada
        fParticleName = track->GetDefinition()->GetParticleName();
        fInitialEnergy = step->GetPreStepPoint()->GetKineticEnergy() / MeV;
        fEntryPosition = step->GetPreStepPoint()->GetPosition();
        fEntryMomentum = step->GetPreStepPoint()->GetMomentumDirection();
        
        G4cout << "Particle entered: " << fParticleName 
               << " with E=" << fInitialEnergy << " MeV" << G4endl;
    }
    
    //Cálculo de energía total depositada
    G4double energyDeposit = step->GetTotalEnergyDeposit() / MeV;

    //Solo guardamos energía depositada si es mayor que cero para evitar errores
    if(energyDeposit > 0){
        fTotalEnergyDeposit += energyDeposit;
    }
    
    return true;
}

//Finalización del evento con escritura de datos al CSV
void SensitiveDetector::EndOfEvent(G4HCofThisEvent* hce)
{
    if(!fHasEntry){
        return;
    }
    
    //Ángulos en grados
    G4double theta = fEntryMomentum.theta() / deg;
    G4double phi = fEntryMomentum.phi() / deg;
    
    //Posición (m)
    G4double x = fEntryPosition.x() / m;
    G4double y = fEntryPosition.y() / m;
    G4double z = fEntryPosition.z() / m;
    
    //Escritura CSV
    fOutputFile << fEventID << ","
                << fParticleName << ","
                << std::fixed << std::setprecision(6)
                << fInitialEnergy << ","
                << fTotalEnergyDeposit << ","
                << x << "," << y << "," << z << ","
                << theta << "," << phi << ","
                << fEntryMomentum.x() << "," 
                << fEntryMomentum.y() << "," 
                << fEntryMomentum.z()
                << "\n";
    
    //Debug
    G4cout << "Event " << fEventID << " finished: "
           << fParticleName << " deposited " 
           << fTotalEnergyDeposit << " MeV total" << G4endl;
    
    //Forzamos el volcado en el archivo
    fOutputFile.flush();
}
