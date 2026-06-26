#include "PrimaryGenAction.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"

/*Aquí se define la generación primaria, primero, con el constructor, definimos la "pistola" que
lanzará nuestras partículas. Primero creamos la pistola, definimos qué partícula usamos, definimos
el vector velocidad que llevarán las partículas, su energía y por último la posición inicial */

PrimaryGenAction::PrimaryGenAction(){
    fparticleGun = new G4ParticleGun();
    fparticleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->FindParticle("e-"));
    fparticleGun->SetParticleMomentumDirection(G4ThreeVector(0, -1, 0));
    fparticleGun->SetParticleEnergy(200*MeV);
    fparticleGun->SetParticlePosition(G4ThreeVector(0,2.3*m,0));
}

/*Esto es un método de la clase que es lo que genera las partículas en sí en el evento*/

void PrimaryGenAction::GeneratePrimaries(G4Event* anEvent){
    G4cout << "Pew Pew" << G4endl;
    fparticleGun->GeneratePrimaryVertex(anEvent);
    //Cada vez que se genere una partícula (evento), saldrá por consola pew pew
}
