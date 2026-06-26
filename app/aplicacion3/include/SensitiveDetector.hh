#ifndef SENSITIVEDETECTOR_HH
#define SENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
/* Colección de hits del evento actual */
#include "G4HCofThisEvent.hh"
#include "G4ThreeVector.hh"
#include <fstream>
#include <vector>

class SensitiveDetector : public G4VSensitiveDetector
{
public:
    explicit SensitiveDetector(const G4String& name);
    ~SensitiveDetector() override;

    /* Interfaz G4VSensitiveDetector */
    void   Initialize(G4HCofThisEvent* hce) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    void   EndOfEvent(G4HCofThisEvent* hce) override;

private:

    /* Recurso compartido entre hilos para la inicialización del CSV */
    static std::ofstream fOutputFile;
    static G4bool fFileInitialized;

    /* Recursos para medir el tiempo */
    G4double fFirstTrackTime;
    /* Flag para saber si se ha registrado el tiempo del primer track */
    G4bool fFirstTrackTimeSet;

    /* Id del evento actual */
    G4int fEventID;
};

#endif // SENSITIVEDETECTOR_HH
