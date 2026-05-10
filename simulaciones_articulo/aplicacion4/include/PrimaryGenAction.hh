#ifndef PRIMARYGENACTION_HH  // ← FALTABA ESTO
#define PRIMARYGENACTION_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include <string>
//Vector es un contenedor dinámico de la biblioteca estándar de C++ (crece y se encoge)
#include <vector>


//Estructura para almacenar datos de la partícula
struct ParticleData {
  std::string particleName;
  double energy;
  double pos_x, pos_y, pos_z;
  double dir_x, dir_y, dir_z;
};

//Definción de la clase PrimaryGenAction que hereda de G4VUserPrimaryGeneratorAction
class PrimaryGenAction : public G4VUserPrimaryGeneratorAction {
  public:

    //Constructor y destructor de la clase. Evitamos virtual y override para evitar redundancia
    PrimaryGenAction();
    ~PrimaryGenAction() override;

    //Método público (necesita ser llamado desde fuera) que genera las partículas en el evento
    void GeneratePrimaries(G4Event* anEvent) override;

  private:

  //Clase de Geant4 que se encarga de generar partículas
  G4ParticleGun* fparticleGun;
  //Vector que guarda la estructura definida llamado fParticleList
  static std::vector<ParticleData> fParticleList;
  //Variable para asegurarnos de que los datos se cargan solo una vez con MT
  static G4bool fIsLoaded; 
    
  void LoadParticlesFromCSV(const std::string& filename);
  };

  #endif



