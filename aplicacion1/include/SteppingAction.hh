#include "G4UserSteppingAction.hh"

namespace prueba{
    class SteppingAction : public G4UserSteppingAction{
    public:
        void UserSteppingAction(const G4Step*) override;
    };
}