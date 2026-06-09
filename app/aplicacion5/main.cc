#include "G4ios.hh"
#include "G4RunManagerFactory.hh"
#include "ActionInit.hh"
#include "G4UImanager.hh"
//Physics list incluida en este ejemplo. Buscar listas que se ajusten al desarrollo que se hace
//o desarrollar una propia
#include "QGSP_BERT.hh"
#include "DetConstruction.hh"

//Para visualizar añado estos includes
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

int main(int argc, char** argv){

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
    
    G4cout << "Hello World" << G4endl;

    /*En ese run manager, debemos definir la physics list, la construcción del detector y
    la inicialización de la acción*/
    //Physics list
    runManager->SetUserInitialization(new QGSP_BERT());

    //Detector construction
    runManager->SetUserInitialization(new DetConstruction());

    //Action initialization w/primary generator action
    runManager->SetUserInitialization(new ActionInit());

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