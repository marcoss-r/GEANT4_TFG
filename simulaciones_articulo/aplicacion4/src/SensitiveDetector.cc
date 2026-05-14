#include "SensitiveDetector.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include <iomanip>
#include "G4AutoLock.hh"


//Definición de variables estáticas
std::ofstream SensitiveDetector::fOutputFile;
G4bool SensitiveDetector::fFileInitialized = false;

//Definición de mutex para manejo de archivo en MT
static G4Mutex fileMutex = G4MUTEX_INITIALIZER;

//Constructor
SensitiveDetector::SensitiveDetector(const G4String& name)
    : G4VSensitiveDetector(name){
    
    /* Bloqueo del semáforo */
    G4AutoLock lock(&fileMutex);
    if(!fFileInitialized){
        /* Abrir archivo CSV en modo agregar (dos strings se concatenan) */
        fOutputFile.open("/home/geant/Desktop/Marcos_TFG_GEANT/git/GEANT4_TFG/"
            "simulaciones_articulo/aplicacion4/output/output.csv", 
            std::ios::app);
        /* Escritura de cabecera */
        fOutputFile
            << "EventID,TrackID,ParentID,ParticleName,"
            << "Initial_Energy_MeV,Total_Energy_Deposit_MeV,"
            << "PosX_m,PosY_m,PosZ_m,"
            << "Theta_deg,Phi_deg,"
            << "MomX,MomY,MomZ,"
            << "GlobalTime_s,DeltaTime_s\n";
        
        /* Archivo inicializado */
        fFileInitialized = true;
        G4cout << "CSV file initialized" << G4endl;
    }
}

/* Destructor */
SensitiveDetector::~SensitiveDetector(){

}

/* Método de inicialización llamado al inicio de cada evento */
void SensitiveDetector::Initialize(G4HCofThisEvent* /*hce*/){

    /* Limpieza del mapa de partículas */
    fParticleMap.clear();
    fFirstTrackTimeSet = false;
    fFirstTrackTime = 0.;
}

//Procesamiento de hits
G4bool SensitiveDetector::ProcessHits(G4Step* step, 
    G4TouchableHistory* /*history*/)
{
    /* Obtención del track actual y su ID */
    G4Track* track  = step->GetTrack();
    G4int    trkID  = track->GetTrackID();

    /* Comprobación por si el track ya ha interactuado con el detector */
    if (fParticleMap.find(trkID) == fParticleMap.end()) {

        /* Get global time*/
        G4double globalTime = step->GetPreStepPoint()->GetGlobalTime();

        /* Check for the first hit */
        if(!fFirstTrackTimeSet){
            fFirstTrackTime = globalTime;
            fFirstTrackTimeSet = true;
        }

        const G4int eventID =
            G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

        G4StepPoint* prePoint = step->GetPreStepPoint();

        ParticleData pd;
        pd.eventID = eventID;
        pd.trackID = trkID;
        pd.parentID = track->GetParentID();
        pd.particleName = track->GetDefinition()->GetParticleName();
        pd.initialEnergy = prePoint->GetKineticEnergy() / MeV;
        pd.entryPosition = prePoint->GetPosition();
        pd.entryMomentum = prePoint->GetMomentumDirection();
        pd.totalEnergyDeposit = 0.;
        pd.globalTime = globalTime;
        pd.deltaTime = globalTime - fFirstTrackTime;

        fParticleMap[trkID] = pd;

        G4cout << " Nueva partícula " << trkID
               << " (parent=" << pd.parentID << ") "
               << pd.particleName
               << " E=" << pd.initialEnergy << " MeV"
               << G4endl;
    }

    /* Acumulación de energía depositada */
    G4double edep = step->GetTotalEnergyDeposit() / MeV;
    if (edep > 0.)
        fParticleMap[trkID].totalEnergyDeposit += edep;

    return true;
}

/* Finalización del evento con escritura de datos al CSV */
void SensitiveDetector::EndOfEvent(G4HCofThisEvent* /*hce*/)
{
    if (fParticleMap.empty())
        return;

    G4AutoLock lock(&fileMutex);

    /* Escritura de cada partícula  con bucle */
    for (const auto& kv : fParticleMap) {
        WriteRow(kv.second);
    }

    fOutputFile.flush();

    G4cout << " Evento "
           << fParticleMap.begin()->second.eventID
           << ": " << fParticleMap.size() << " partículas contadas."
           << G4endl;
}

/* Escribe una línea CSV para el ParticleData dado (para cada partícula) */
void SensitiveDetector::WriteRow(const ParticleData& pd)
{
    // Conversión de unidades
    G4double x     = pd.entryPosition.x() / m;
    G4double y     = pd.entryPosition.y() / m;
    G4double z     = pd.entryPosition.z() / m;
    G4double theta = pd.entryMomentum.theta() / deg;
    G4double phi   = pd.entryMomentum.phi()   / deg;

    fOutputFile
        << pd.eventID      << ","
        << pd.trackID      << ","
        << pd.parentID     << ","
        << pd.particleName << ","
        << std::fixed << std::setprecision(6)
        << pd.initialEnergy          << ","
        << pd.totalEnergyDeposit     << ","
        << x     << "," << y     << "," << z   << ","
        << theta << "," << phi   << ","
        << pd.entryMomentum.x() << ","
        << pd.entryMomentum.y() << ","
        << pd.entryMomentum.z() << ","
        << pd.globalTime / ns << ","
        << pd.deltaTime / ns
        << "\n";
}
