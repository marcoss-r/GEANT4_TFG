#include "G4ios.hh"
#include "G4RunManagerFactory.hh"
#include "ActionInit.hh"
#include "G4UImanager.hh"
#include "RunAction.hh"
#include "G4Types.hh"
//Physics list incluida en este ejemplo. Buscar listas que se ajusten al desarrollo que se hace
//o desarrollar una propia
#include "QGSP_BIC_HP.hh"
#include "DetConstruction.hh"

//Para visualizar añado estos includes
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

//Includes de C para contar tiempo, escribit en un archivo y formatear la salida
#include <chrono>      
#include <fstream>      
#include <iomanip>
#include <filesystem>

#include "PrimaryGenAction.hh"


using namespace prueba;

int main(int argc, char** argv){


    auto inicio = std::chrono::high_resolution_clock::now();

    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }
    // TODOS:

    //Get the g4 app compiling

    //Run manager
    //auto se usa en C++ para asignar automáticamente el tipo a una variable
    //Si le damos clic derecho y go to definition podemos ver lo que necesita la función

    //Una vez que hemos terminado con la inicialización de la visualización, creamos el run manager
    auto runManager = G4RunManagerFactory::CreateRunManager();
    
    /*En ese run manager, debemos definir la physics list, la construcción del detector y
    la inicialización de la acción*/
    //Physics list
    runManager->SetUserInitialization(new QGSP_BIC_HP());

    //Detector construction
    runManager->SetUserInitialization(new DetConstruction());

    //Action initialization w/primary generator action
    runManager->SetUserInitialization(new ActionInit());

    // Crear instancia de RunAction
    auto runAction = new RunAction();
    runManager->SetUserAction(runAction);

    // initialize G4 kernel (no para visualización)
    //runManager->Initialize();

    // Initialize visualization with the default graphics system
    auto visManager = new G4VisExecutive(argc, argv);
    // Constructors can also take optional arguments:
    // - a graphics system of choice, eg. "OGL"
    // - and a verbosity argument - see /vis/verbose guidance.
    // auto visManager = new G4VisExecutive(argc, argv, "OGL", "Quiet");
    // auto visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();

    // Get the pointer to the User Interface manager
    auto UImanager = G4UImanager::GetUIpointer();

    // Process macro or start UI session
    //
    if (!ui) {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);

        G4int nEvents = runAction->GetNumberOfEvents();

        auto fin = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duracion = fin - inicio;

        //Instancia de la clase PrimaryGenAction temporal para obtener el tipo de partícula y energía
        PrimaryGenAction tempGen;
        G4String particleName = tempGen.GetParticleName();
        G4double energyMeV = tempGen.GetParticleEnergy() / CLHEP::MeV;

        const std::string filename = "../resultados/tiempoejecucion_v2.csv";

        //Comprobamos si el archivo existe en la carpeta de resultados
        bool existe = std::filesystem::exists(filename);

        //Abrimos el archivo en modo append
        std::ofstream file(filename, std::ios::app);

        //Si el archivo no existe, se añade una cabecera
        if (!existe) {
            file << "Tiempo (s),Partícula,Energía (MeV),Número de Partículas\n";
        }

        file << duracion.count() << "," << particleName << "," << energyMeV << ","<< nEvents <<"\n";
        file.close();
    }
    else {
        // interactive mode
        UImanager->ApplyCommand("/control/execute ../mac_files/init_vis.mac");
        ui->SessionStart();
        delete ui;
    }

    

    //IMPORTANTE: Si estamos con visualización, se lanzan los comandos desde un mac_file

    // start a run
    //int numberOfEvent = 3;
    //runManager->BeamOn(numberOfEvent);

    // job termination
    delete visManager;
    delete runManager;
    

    //Stepping action
    return 0;
}