#include "RunAction.hh"

/* Método que se ejecuta al final de la ejecución para obtener el número
de eventos procesados */
void RunAction::EndOfRunAction(const G4Run* run){
    nEvents = run->GetNumberOfEventToBeProcessed();
}

/* Devuelve el número de eventos procesados en la ejecución */
G4int RunAction::GetNumberOfEvents() const {
    return nEvents;
}
