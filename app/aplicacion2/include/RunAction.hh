#include "G4UserRunAction.hh"
#include "G4Run.hh"

class RunAction : public G4UserRunAction{
    public:
            
        /* Constructor de la clase */
        RunAction()=default;

        /* Métodos para manejar el final de la ejecución */
        void EndOfRunAction(const G4Run* run) override;
        G4int GetNumberOfEvents() const ;

    private:
        /* Atributo para almacenar el número de eventos */
        G4int nEvents=0;
};