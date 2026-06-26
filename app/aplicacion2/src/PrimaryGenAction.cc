#include "PrimaryGenAction.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"


/* Aquí se define la generación primaria: con el constructor definimos la
"pistola" que lanzará nuestras partículas. Creamos la pistola, definimos
qué partícula usamos, el vector velocidad, su energía y la posición inicial */

PrimaryGenAction::PrimaryGenAction(){
    fparticleGun = new G4ParticleGun();

    fparticleName="neutron";
    fparticleEnergy=20000*MeV;

    fparticleGun->SetParticleDefinition(
            G4ParticleTable::GetParticleTable()->FindParticle(fparticleName));
    fparticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, -1));
    fparticleGun->SetParticleEnergy(fparticleEnergy);
    fparticleGun->SetParticlePosition(G4ThreeVector(0,0,19.9*m));
}

/* Método que genera las partículas en sí en el evento */
void PrimaryGenAction::GeneratePrimaries(G4Event* anEvent){
    /* Mensaje para debug */
    G4cout << "Pew Pew" << G4endl;
    fparticleGun->GeneratePrimaryVertex(anEvent);
}

G4String PrimaryGenAction::GetParticleName() const{
    return fparticleName;
}

G4double PrimaryGenAction::GetParticleEnergy() const{
    return fparticleEnergy;
}
