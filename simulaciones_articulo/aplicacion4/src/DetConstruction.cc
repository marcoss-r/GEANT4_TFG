#include "DetConstruction.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "SensitiveDetector.hh"
#include "G4SDManager.hh"

DetConstruction::DetConstruction(){
}

DetConstruction::~DetConstruction(){
}

G4VPhysicalVolume* DetConstruction::Construct(){

    //solid then logic then phys

    //Volumen sólido
    G4Box *solidWorld = new G4Box("solidWorld",10 * m, 100 * m, 10 * m);

    //Obtenemos el material de nuestro mundo, en este caso aire
    //Vacío=G4_Galactic
    G4Material *air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

    //Construimos el volumen lógico a partir de nuestro material y el volumn sólido
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld,air,"logicWorld");

    //Construimos el volumen físico, 
    /*
    El primero de los campos es la rotación
    El segundo de los campos es la posición
    Volumen lógico
    Nombre
    Madre (en este caso no tiene madre porque es la madre y el mundo es la madre de todo)
    Many?
    Número de copias
    */
    G4VPhysicalVolume *physWorld = new G4PVPlacement(nullptr,G4ThreeVector(),logicWorld,
        "physWorld",nullptr,false,0);

    //add a aluminum detector
    G4Material *vacuum =G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

    G4Box *solidDetector = new G4Box("solidDetector", 75 * cm, 0.01 * cm,  35 * cm);

    G4LogicalVolume *logicDetector = new G4LogicalVolume(solidDetector,vacuum,"logicDetector");

    //Por algún motivo extraño no hay que definir una variable con el volumen físico

    new G4PVPlacement(nullptr,G4ThreeVector(0,-92.1*m,0*m),logicDetector,"physDetector",logicWorld,false,0);


    G4Material *hormigon =G4NistManager::Instance()->FindOrBuildMaterial("G4_CONCRETE");

    G4Box *solidDetector2 = new G4Box("solidDetector2", 4 * m, 15 * cm,  4 * m);

    G4LogicalVolume *logicDetector2 = new G4LogicalVolume(solidDetector2,hormigon,"logicDetector2");

    //Por algún motivo extraño no hay que definir una variable con el volumen físico

    new G4PVPlacement(nullptr,G4ThreeVector(0,-88.85*m,0*m),logicDetector2,"physDetector2",logicWorld,false,0);
    

    return physWorld;



}

void DetConstruction::ConstructSDandField(){
    
    //Creación de sensitive detector
    SensitiveDetector* sensitiveDetector = new SensitiveDetector("SensitiveDetector");
    
    // Registro en el manager
    G4SDManager::GetSDMpointer()->AddNewDetector(sensitiveDetector);
    
    //Asignación del sensitive detector a los volúmenes lógicos correspondientes
    SetSensitiveDetector("logicDetector", sensitiveDetector);

    //Debug
    G4cout << "Sensitive Detector registered and assigned" << G4endl;
}

