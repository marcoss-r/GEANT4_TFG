#include "DetConstruction.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"



G4VPhysicalVolume* DetConstruction::Construct(){

    //solid then logic then phys

    //Volumen sólido
    G4Box *solidWorld = new G4Box("solidWorld",5 * m, 5 * m, 5 * m);

    //Obtenemos el material de nuestro mundo, en este caso aire
    //Vacío=G4_Galactic
    G4Material *vacuum = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

    //Construimos el volumen lógico a partir de nuestro material y el volumn sólido
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld,vacuum,"logicWorld");

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
    G4Material *aluminum =G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");

    G4Box *solidDetector = new G4Box("solidDetector",3 * m, 10 * cm,  3 * m);

    G4LogicalVolume *logicDetector = new G4LogicalVolume(solidDetector,aluminum,"logicDetector");

    //Por algún motivo extraño no hay que definir una variable con el volumen físico

    new G4PVPlacement(nullptr,G4ThreeVector(0,-2.3*m,0*m),logicDetector,"physDetector",logicWorld,false,0);



    G4Material *centelleador =G4NistManager::Instance()->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

    G4Box *solidDetector2 = new G4Box("solidDetector2",3 * m, 10 * cm,  3 * m);

    G4LogicalVolume *logicDetector2 = new G4LogicalVolume(solidDetector2,centelleador,"logicDetector2");

    //Por algún motivo extraño no hay que definir una variable con el volumen físico

    new G4PVPlacement(nullptr,G4ThreeVector(0,-1.8*m,0*m),logicDetector2,"physDetector2",logicWorld,false,0);


    return physWorld;



}
