#ifndef SENSITIVEDETECTOR_HH
#define SENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
//Colección de hits del evento actual
#include "G4HCofThisEvent.hh"
#include "G4ThreeVector.hh"
#include <fstream>
#include <map>

/** Estructura para almacenar datos de cada partícula (ya sea primaria o 
 * secundaria) que se almacenará en el mapa */
struct ParticleData {
    G4int eventID;    /** ID del evento (partícula primaria) */
    G4int trackID;    /** ID del track (partícula concreta, primaria o 
                       *  secundaria) */
    G4int parentID;   /** ID del track padre (0 para primarias, TrackID
                        *  de la partícula madre para secundarias) */
    G4String particleName;
    G4double initialEnergy;       /** MeV, en el primer step del track */
    G4ThreeVector entryPosition;     /** m */
    G4ThreeVector entryMomentum;     /** dirección unitaria */
    G4double totalEnergyDeposit;  /** MeV acumulados */
    G4double globalTime;           /** s, tiempo absoluto de llegada */
    G4double deltaTime;            /** s, tiempo relativo al track padre */

    /** Constructor por defecto para inicializar la estructura */
    ParticleData()
        : eventID(-1), trackID(-1), parentID(-1),
          initialEnergy(0.), totalEnergyDeposit(0.), globalTime(0.), 
          deltaTime(0.) {}
};

class SensitiveDetector : public G4VSensitiveDetector
{
public:
    explicit SensitiveDetector(const G4String& name);
    ~SensitiveDetector() override;

    /** Interfaz G4VSensitiveDetector */
    void   Initialize(G4HCofThisEvent* hce) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    void   EndOfEvent(G4HCofThisEvent* hce) override;

private:
    /** Mapa que guarda los datos de la estructura ParticleData para cada
     *  trackID que ha interactuado con el detector. */
    std::map<G4int, ParticleData> fParticleMap;

    /** Recurso compartido entre hilos para la inicialización del archivo CSV */
    static std::ofstream fOutputFile;
    static G4bool fFileInitialized;

    /** Recursos para medir el tiempo */
    G4double fFirstTrackTime;  /** Tiempo del primer track registrado en el 
                                *  evento */
    G4bool fFirstTrackTimeSet; /** Flag para saber si se ha registrado el
                                *  tiempo del primer track */

    G4int fCurrentEventID; /** ID del evento actual */
                               
    /** Declaración del método para escribir una fila en el CSV */
    void WriteRow(const ParticleData& pd);
};

#endif // SENSITIVEDETECTOR_HH