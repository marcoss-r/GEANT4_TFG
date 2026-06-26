#include "G4ios.hh"
#include "G4RunManagerFactory.hh"
#include "ActionInit.hh"
#include "G4UImanager.hh"
#include "DetConstruction.hh"

/* Lista de físicas incluida en esta aplicación. Se puede crear una propia */
#include "QGSP_BERT.hh"


/* Archivos para soportar la visualización */
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

int main(int argc, char** argv){

    /* Número de argumentos para la interfaz de usuario */
    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

    /* Crear instancia de run manager */
    auto runManager = G4RunManagerFactory::CreateRunManager();
    
    /* Inicio de la simulación (Hola mundo por consola )*/
    G4cout << "Hello World" << G4endl;

    /* Run manager debe definir la lista de físicas, construcción del mundo e 
    inicialización de la acción */

    /* Lista de físicas*/
    runManager->SetUserInitialization(new QGSP_BERT());

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