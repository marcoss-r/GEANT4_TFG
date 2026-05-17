#include "SensitiveDetector.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include <iomanip>
#include "G4AutoLock.hh"


/* Definición de variables estáticas */
std::ofstream SensitiveDetector::fOutputFile;
G4bool SensitiveDetector::fFileInitialized = false;

/* Definición de mutex para manejo de archivo en MT */
static G4Mutex fileMutex = G4MUTEX_INITIALIZER;

/* Constructor */
SensitiveDetector::SensitiveDetector(const G4String& name)
    : G4VSensitiveDetector(name),
      fEventID(-1),
      fFirstTrackTime(0.){
    
    /* Bloqueo del semáforo */
    G4AutoLock lock(&fileMutex);
    if(!fFileInitialized){
        /* Abrir archivo CSV en modo agregar (dos strings se concatenan) */
        fOutputFile.open("/home/geant/Desktop/Marcos_TFG_GEANT/git/GEANT4_TFG/"
            "aplicacion6/output/output.csv", 
            std::ios::app);
        /* Escritura de cabecera */
        fOutputFile
            << "EventID,TrackID,ParentID,ParticleName,"
            << "Initial_Energy_MeV,"
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
    
    /* Obtenemos el ID del evento actual */
    fEventID =
        G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    /* Inicialización del tiempo del primer track */
    fFirstTrackTime = -1.; 
   
}

/* Procesamiento de hits */
G4bool SensitiveDetector::ProcessHits(G4Step* step, 
    G4TouchableHistory* /*history*/)
{
    /* Solo registramos el paso en la frontera de entrada */
    if(step->GetPreStepPoint()->GetStepStatus() != fGeomBoundary)
    {
        return false;
    }

    G4Track* track = step->GetTrack();
    G4StepPoint* prePoint = step->GetPreStepPoint();

    /* Tiempo global de este track */
    G4double globalTime = prePoint->GetGlobalTime();

    /* El primer track que entra define el tiempo de referencia */
    if(fFirstTrackTime < 0.)
    {
        fFirstTrackTime = globalTime;
    }

    G4double deltaTime = globalTime - fFirstTrackTime;

    /* Conversión de unidades */
    G4double x = prePoint->GetPosition().x() / m;
    G4double y = prePoint->GetPosition().y() / m;
    G4double z = prePoint->GetPosition().z() / m;
    G4double theta = prePoint->GetMomentumDirection().theta() / deg;
    G4double phi = prePoint->GetMomentumDirection().phi() / deg;

    /* Escritura CSV protegida por mutex para MT */
    G4AutoLock lock(&fileMutex);

    fOutputFile
        << fEventID << ","
        << track->GetTrackID() << ","
        << track->GetParentID() << ","
        << track->GetDefinition()->GetParticleName() << ","
        << std::fixed << std::setprecision(6)
        << prePoint->GetKineticEnergy() / MeV << ","
        << x << "," << y << "," << z << ","
        << theta << "," << phi << ","
        << prePoint->GetMomentumDirection().x() << ","
        << prePoint->GetMomentumDirection().y() << ","
        << prePoint->GetMomentumDirection().z() << ","
        << globalTime / ns << ","
        << deltaTime  / ns
        << "\n";

    return true;
}

/* Finalización del evento con escritura de datos al CSV */
void SensitiveDetector::EndOfEvent(G4HCofThisEvent* /*hce*/)
{

}