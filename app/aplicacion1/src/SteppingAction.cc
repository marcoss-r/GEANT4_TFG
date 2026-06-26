#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4ios.hh"

/* Esta clase sirve para definir un método que se ejecuta en cada paso de la 
simulación, en concreto en el interior de un volumen físico */   
void SteppingAction::UserSteppingAction(const G4Step *aStep ){
    if (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == 
            "physDetector"){
        G4cout << "Hit detector" << G4endl;
    }
}
