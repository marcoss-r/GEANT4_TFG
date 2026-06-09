#include "RunAction.hh"

namespace prueba{

    void RunAction::EndOfRunAction(const G4Run* run){
        nEvents = run->GetNumberOfEventToBeProcessed();
    }

    G4int RunAction::GetNumberOfEvents() const {
        return nEvents;
    }
}