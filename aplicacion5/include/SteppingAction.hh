#ifndef STEPPINGACTION_HH
#define STEPPINGACTION_HH
#include "G4UserSteppingAction.hh"

    class SteppingAction : public G4UserSteppingAction{
    public:
        void UserSteppingAction(const G4Step*) override;
    };
#endif
