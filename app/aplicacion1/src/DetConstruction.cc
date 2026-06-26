#include "DetConstruction.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"


/* Método para construir el mundo de simulación:
Primero debe definirse un volumen sólido, luego el volumen lógico y 
finalmente el volumen físico */
G4VPhysicalVolume* DetConstruction::Construct(){

    /* Volumen sólido del mundo (dimensiones) */
    G4Box *solidWorld = new G4Box("solidWorld",5 * m, 5 * m, 5 * m);

    /* Obtención del material del mundo (aire) */
    G4Material *vacuum = 
            G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

    /* Construcción del volumen lógico a partir del volumen sólido y el 
    material */
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, vacuum, 
            "logicWorld");

    
    /* Construcción del volumen físico (rotación, posición, volumen lógico, 
    nombre, madre, many, copy number)*/
    G4VPhysicalVolume *physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), 
            logicWorld, "physWorld", nullptr, false, 0);

    /* Adición de un detector de aluminio */
    G4Material *aluminum =
            G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
    
    /* Construcción del volumen sólido del detector */
    G4Box *solidDetector = new G4Box("solidDetector",3 * m, 10 * cm,  3 * m);
    
    /* Construcción del volumen lógico del detector */
    G4LogicalVolume *logicDetector = new G4LogicalVolume(solidDetector, 
            aluminum, "logicDetector");

    /* Construcción del volumen físico del detector (no es necesaria una 
    variable porque se adhiere a la madre) */
    new G4PVPlacement(nullptr, G4ThreeVector(0, -2.3*m, 0*m), logicDetector,
            "physDetector", logicWorld, false, 0);


    /* Adición de un material centelleador */
    G4Material *centelleador =
            G4NistManager::Instance()->FindOrBuildMaterial(
            "G4_PLASTIC_SC_VINYLTOLUENE");
    
    /* Construcción del volumen sólido del centelleador */
    G4Box *solidDetector2 = new G4Box("solidDetector2",3 * m, 10 * cm,  3 * m);

    /* Construcción del volumen lógico del centelleador */
    G4LogicalVolume *logicDetector2 = new G4LogicalVolume(solidDetector2, 
            centelleador, "logicDetector2");

    /* Construcción del volumen físico del centelleador */
    new G4PVPlacement(nullptr, G4ThreeVector(0, -1.8*m, 0*m), logicDetector2,
            "physDetector2", logicWorld, false, 0);


    return physWorld;



}
