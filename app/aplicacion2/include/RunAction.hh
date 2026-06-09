#include "G4UserRunAction.hh"
#include "G4Run.hh"

namespace prueba{
    class RunAction : public G4UserRunAction{
        public:
            //Constructor de la clase que se queda por defecto
            RunAction()=default;
            void EndOfRunAction(const G4Run* run) override;
            G4int GetNumberOfEvents() const ;
        private:
            G4int nEvents=0;
    };
}