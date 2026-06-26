#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

class PrimaryGenAction : public G4VUserPrimaryGeneratorAction {
    public:
        PrimaryGenAction();
        void GeneratePrimaries(G4Event* anEvent) override;

        /* Métodos para obtener el nombre y la energía de la partícula 
        generada */
        G4String GetParticleName() const;
        G4double GetParticleEnergy() const;

    private:

        /* Atributos para almacenar la información de la partícula generada */
        G4ParticleGun* fparticleGun;
        G4String fparticleName;
        G4double fparticleEnergy;
};
