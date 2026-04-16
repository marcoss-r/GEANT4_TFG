#include "PrimaryGenAction.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"

namespace prueba{

/*Aquí se define la generación primaria, primero, con el constructor, definimos la "pistola" que
lanzará nuestras partículas. Primero creamos la pistola, definimos qué partícula usamos, definimos
el vector velocidad que llevarán las partículas, su energía y por último la posición inicial */

PrimaryGenAction::PrimaryGenAction(){
    fparticleGun = new G4ParticleGun();

    fparticleName="neutron";
    fparticleEnergy=20000*MeV;

    fparticleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle(fparticleName));
    fparticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, -1));
    fparticleGun->SetParticleEnergy(fparticleEnergy);
    fparticleGun->SetParticlePosition(G4ThreeVector(0,0,19.9*m));
}

/*Esto es un método de la clase que es lo que genera las partículas en sí en el evento*/

void PrimaryGenAction::GeneratePrimaries(G4Event* anEvent){
    G4cout << "Pew Pew" << G4endl;
    fparticleGun->GeneratePrimaryVertex(anEvent);
    //Cada vez que se genere una partícula (evento), saldrá por consola pew pew
}

G4String PrimaryGenAction::GetParticleName() const{
    return fparticleName;
}

G4double PrimaryGenAction::GetParticleEnergy() const{
    return fparticleEnergy;
}


}
