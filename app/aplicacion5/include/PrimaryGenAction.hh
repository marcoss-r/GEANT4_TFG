#ifndef PRIMARYGENACTION_HH
#define PRIMARYGENACTION_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include <string>
/* Vector: contenedor dinámico de la biblioteca estándar de C++ */
#include <vector>


/* Estructura para almacenar los datos de cada partícula */
struct ParticleData {
  std::string particleName;
  double energy;
  double pos_x, pos_y, pos_z;
  double dir_x, dir_y, dir_z;
};

/* Definición de la clase PrimaryGenAction que hereda de
G4VUserPrimaryGeneratorAction */
class PrimaryGenAction : public G4VUserPrimaryGeneratorAction {
  public:

    /* Constructor y destructor de la clase */
    PrimaryGenAction();
    ~PrimaryGenAction() override;

    /* Método público que genera las partículas en el evento */
    void GeneratePrimaries(G4Event* anEvent) override;

  private:

  /* Clase de Geant4 que se encarga de generar partículas */
  G4ParticleGun* fparticleGun;
  /* Vector que guarda la estructura definida llamado fParticleList */
  static std::vector<ParticleData> fParticleList;
  /* Variable para asegurarnos de que los datos se cargan solo una
  vez con MT */
  static G4bool fIsLoaded;

  void LoadParticlesFromCSV(const std::string& filename);
  };

  #endif
