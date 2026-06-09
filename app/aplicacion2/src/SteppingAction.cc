#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4ios.hh"

namespace prueba{

/*En GEANT4, las partículas obviamente no siguen un continuo, sino que van por unos ciertos pasos.
Con esta clase, podemos definir un método de la clase */    
    void SteppingAction::UserSteppingAction(const G4Step *aStep ){
        if (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "physDetector"){
           
        }
    }
}
