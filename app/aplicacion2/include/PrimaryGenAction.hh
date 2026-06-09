#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

namespace prueba{

    class PrimaryGenAction : public G4VUserPrimaryGeneratorAction {
        public:
          PrimaryGenAction();
          void GeneratePrimaries(G4Event* anEvent) override;

          //Funciones que obtienen el nombre de la partícula para ponerla en el archivo del tiempo
          G4String GetParticleName() const;
          G4double GetParticleEnergy() const;

        private:
          G4ParticleGun* fparticleGun;
          G4String fparticleName;
          G4double fparticleEnergy;
    };

}