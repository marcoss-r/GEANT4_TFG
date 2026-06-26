#include "G4VUserDetectorConstruction.hh"

class DetConstruction : public G4VUserDetectorConstruction{
    public:
        G4VPhysicalVolume* Construct();
};