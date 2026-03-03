#include "PrimaryGenAction.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
//Lectura de archivos
#include <fstream>
//Líneas de CSV
#include <sstream>
//Para mensajes de error
#include <iostream>
//Inclusión de mutex para MT
#include "G4AutoLock.hh"


/*Aquí se define la generación primaria, primero, con el constructor, definimos la "pistola" que
lanzará nuestras partículas. Primero creamos la pistola, definimos qué partícula usamos, definimos
el vector velocidad que llevarán las partículas, su energía y por último la posición inicial */

//Definición de variables static
std::vector<ParticleData> PrimaryGenAction::fParticleList;
G4bool PrimaryGenAction::fIsLoaded = false;

static G4Mutex csvMutex = G4MUTEX_INITIALIZER;

PrimaryGenAction::PrimaryGenAction(){
    //Creación de la pistola de partículas
    fparticleGun = new G4ParticleGun(1);
    
    G4AutoLock lock(&csvMutex);
    if(!fIsLoaded){

        //Carga de partículas desde el archivo CSV
        LoadParticlesFromCSV("/home/geant/Desktop/Marcos_TFG_GEANT/git/GEANT4_TFG/aplicacion4/../particlegen/input/input_particles_971MeV_proton.csv");
        fIsLoaded = true;
        //Debugging
        G4cout << "Loaded " << fParticleList.size() << " particles from CSV" << G4endl;
    }
    
    
};

PrimaryGenAction::~PrimaryGenAction(){
    delete fparticleGun;
};

void PrimaryGenAction::LoadParticlesFromCSV(const std::string& filename){

    //Apertura del archivo CSV
    std::ifstream file(filename);

    if(!file.is_open()){
        
        G4cerr << "ERROR: Cannot open " << filename << G4endl;      
        G4cerr << "Using default particle generation instead." << G4endl;
        return;

    }

    //Variable para guardar cada línea
    std::string line;

    //Salta la línea del encabezado
    std::getline(file, line); 

    while(std::getline(file, line)){

        std::stringstream ss(line);        
        std::string temp_variable;
        ParticleData particle;

        //Campo 1: particle_id (lo leemos pero no lo usamos)
        std::getline(ss, temp_variable, ',');
        
        //Campo 2: particle name
        std::getline(ss, particle.particleName, ',');
        
        // Campo 3: energy_MeV
        std::getline(ss, temp_variable, ',');
        particle.energy = std::stod(temp_variable);

        // Campo 4: pos_x_m
        std::getline(ss, temp_variable, ',');
        particle.pos_x = std::stod(temp_variable);
        
        // Campo 5: pos_y_m
        std::getline(ss, temp_variable, ',');
        particle.pos_y = std::stod(temp_variable);
        
        // Campo 6: pos_z_m
        std::getline(ss, temp_variable, ',');
        particle.pos_z = std::stod(temp_variable);
        
        // Campo 7: dir_x
        std::getline(ss, temp_variable, ',');
        particle.dir_x = std::stod(temp_variable);
        
        // Campo 8: dir_y
        std::getline(ss, temp_variable, ',');
        particle.dir_y = std::stod(temp_variable);
        
        // Campo 9: dir_z
        std::getline(ss, temp_variable, ',');
        particle.dir_z = std::stod(temp_variable);
        
        //Añade la partícula al vector
        fParticleList.push_back(particle);
    
    }

    file.close();
};


/*Esto es un método de la clase que es lo que genera las partículas en sí en el evento*/

void PrimaryGenAction::GeneratePrimaries(G4Event* anEvent){
    
    //Verificar si hay partículas cargadas
    if(fParticleList.empty()){

        //Generación de partícula por defecto

        G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
        
        fparticleGun->SetParticleDefinition(particle);
        fparticleGun->SetParticleEnergy(5*MeV);
        fparticleGun->SetParticlePosition(G4ThreeVector(0, 5*m, 0));
        fparticleGun->SetParticleMomentumDirection(G4ThreeVector(0, -1, 0));
        fparticleGun->GeneratePrimaryVertex(anEvent);
        return;
    }
    
    //Obtener la partícula actual
    G4int eventID = anEvent->GetEventID();

    size_t index = eventID % fParticleList.size();

    ParticleData& particle = fParticleList[index];
    
    //Buscar la definición de la partícula en GEANT4
    G4ParticleDefinition* particleDef = G4ParticleTable::GetParticleTable()->FindParticle(particle.particleName);

    //Verificar si la partícula existe y si no, usar neutrón por defecto
    if(!particleDef){

        G4cerr << "WARNING: Unknown particle '" << particle.particleName 
               << "', using neutron instead" << G4endl;
        particleDef = G4ParticleTable::GetParticleTable()->FindParticle("neutron");

    }
    
    //Configurar la pistola con los datos de la partícula

    //Partícula
    fparticleGun->SetParticleDefinition(particleDef);

    //Energía
    fparticleGun->SetParticleEnergy(particle.energy * MeV);

    //Posición
    fparticleGun->SetParticlePosition(G4ThreeVector(particle.pos_x * m, particle.pos_y * m, particle.pos_z * m));
    
    //Dirección
    fparticleGun->SetParticleMomentumDirection(G4ThreeVector(particle.dir_x, particle.dir_y, particle.dir_z));
    
    G4cout << "═══════════════════════════════════════" << G4endl;
    G4cout << "EVENT " << eventID << G4endl;
    G4cout << "  Particle: " << particle.particleName << G4endl;
    G4cout << "  Energy: " << particle.energy << " MeV" << G4endl;
    G4cout << "  Position: (" << particle.pos_x << ", " 
           << particle.pos_y << ", " << particle.pos_z << ") m" << G4endl;
    G4cout << "  Direction: (" << particle.dir_x << ", " 
           << particle.dir_y << ", " << particle.dir_z << ")" << G4endl;
    G4cout << "═══════════════════════════════════════" << G4endl;


    //Generar el vértice primario
    fparticleGun->GeneratePrimaryVertex(anEvent);
    
}
