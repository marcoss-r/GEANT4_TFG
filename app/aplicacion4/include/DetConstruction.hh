#ifndef DETCONSTRUCTION_HH
#define DETCONSTRUCTION_HH
#include "G4VUserDetectorConstruction.hh"


class DetConstruction : public G4VUserDetectorConstruction{
    public:

        DetConstruction();
        ~DetConstruction() override;    

        G4VPhysicalVolume* Construct() override;

        void ConstructSDandField() override;
    };

#endif