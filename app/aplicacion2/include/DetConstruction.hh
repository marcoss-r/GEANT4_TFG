#include "G4VUserDetectorConstruction.hh"

namespace prueba{
    class DetConstruction : public G4VUserDetectorConstruction{
    public:
        G4VPhysicalVolume* Construct();
    };
}