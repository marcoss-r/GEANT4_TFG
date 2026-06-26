#include "G4ios.hh"
#include "G4RunManagerFactory.hh"
#include "ActionInit.hh"
#include "G4UImanager.hh"
#include "RunAction.hh"
#include "G4Types.hh"
#include "DetConstruction.hh"
#include "PrimaryGenAction.hh"

/* Lista de físicas incluida en esta aplicación. Se puede crear una propia */
#include "QGSP_BIC_HP.hh"


/* Archivos para soportar la visualización */
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

/* Archivos para contar tiempo, escribir en un archivo y formatear la salida */
#include <chrono>      
#include <fstream>      
#include <iomanip>
#include <filesystem>


int main(int argc, char** argv){

    /* Toma de tiempo inicial */
    auto inicio = std::chrono::high_resolution_clock::now();

    /* Número de argumentos para la interfaz de usuario */
    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

    /* Crear instancia de run manager */
    auto runManager = G4RunManagerFactory::CreateRunManager();
    
    /* Run manager debe definir la lista de físicas, construcción del mundo e 
    inicialización de la acción */

    /* Lista de físicas*/
    runManager->SetUserInitialization(new QGSP_BIC_HP());

    /* Construcción del mundo */
    runManager->SetUserInitialization(new DetConstruction());

    /* Inicialización de la acción  (Incluye generación de partículas) */
    runManager->SetUserInitialization(new ActionInit());

    /* Inicialización de la visualización */
    auto visManager = new G4VisExecutive(argc, argv);

    /* Constructors can also take optional arguments:
     - a graphics system of choice, eg. "OGL"
     - and a verbosity argument - see /vis/verbose guidance.
    auto visManager = new G4VisExecutive(argc, argv, "OGL", "Quiet");
    auto visManager = new G4VisExecutive("Quiet"); */
    visManager->Initialize();

    /* Obtención del puntero a la interfaz de usuario */
    auto UImanager = G4UImanager::GetUIpointer();

    /* Procesar archivo de macro o iniciar sesión de UI */
    if (!ui) {
        /* Ejecutar macro en modo batch */
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);

        /* Obtener el número de eventos simulados */
        G4int nEvents = runAction->GetNumberOfEvents();

        /* Toma de tiempo final y cálculo de la duración de la ejecución */
        auto fin = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duracion = fin - inicio;

        /* Instancia  auxiliar de la clase PrimaryGenAction  para obtener el 
        tipo de partícula y energía */
        PrimaryGenAction tempGen;
        G4String particleName = tempGen.GetParticleName();
        G4double energyMeV = tempGen.GetParticleEnergy() / CLHEP::MeV;

        /* Definición del nombre del archivo de resultados */
        const std::string filename = "../resultados/tiempoejecucion_v2.csv";

        /* Comprobamos si el archivo existe en la carpeta de resultados */
        bool existe = std::filesystem::exists(filename);

        /* Abrimos el archivo en modo append */
        std::ofstream file(filename, std::ios::app);

        /* Si el archivo no existe, se añade una cabecera */
        if (!existe) {
            file << "Tiempo (s),Partícula,Energía (MeV),Número de Partículas\n";
        }

        /* Escribimos los resultados en el archivo (número de eventos, 
        partícula, energía) */
        file << duracion.count() << "," << particleName << "," << energyMeV << ","<< nEvents <<"\n";
        file.close();
    }
    else {
        /* Iniciar sesión de UI en modo interactivo */
        UImanager->ApplyCommand("/control/execute ../mac_files/init_vis.mac");
        ui->SessionStart();
        delete ui;
    }

    /* Eliminar los objetos de visualización y run manager */
    delete visManager;
    delete runManager;
    

    /* Fin de la simulación */
    return 0;
}