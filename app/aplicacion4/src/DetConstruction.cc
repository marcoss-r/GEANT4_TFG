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

/* Método para construir el mundo de simulación:
Primero debe definirse un volumen sólido, luego el volumen lógico y
finalmente el volumen físico */
G4VPhysicalVolume* DetConstruction::Construct(){

    /* Volumen sólido del mundo (dimensiones) */
    G4Box *solidWorld = new G4Box("solidWorld",10 * m, 100 * m, 10 * m);

    /* Obtención del material del mundo (aire) */
    G4Material *air =
            G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

    /* Construcción del volumen lógico a partir del volumen sólido y el
    material */
    G4LogicalVolume *logicWorld =
            new G4LogicalVolume(solidWorld,air,"logicWorld");

    /* Construcción del volumen físico (rotación, posición, volumen lógico,
    nombre, madre, many, copy number) */
    G4VPhysicalVolume *physWorld = new G4PVPlacement(nullptr,
            G4ThreeVector(),logicWorld,"physWorld",nullptr,false,0);

    /* Adición de un detector de aluminio */
    G4Material *aluminum =
            G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");

    G4Box *solidDetector =
            new G4Box("solidDetector", 3 * m, 10 * cm,  3 * m);

    G4LogicalVolume *logicDetector =
            new G4LogicalVolume(solidDetector,aluminum,"logicDetector");

    /* Construcción del volumen físico del detector (no es necesaria una
    variable porque se adhiere a la madre) */
    new G4PVPlacement(nullptr,G4ThreeVector(0,-92.1*m,0*m),
            logicDetector,"physDetector",logicWorld,false,0);


    /* Adición de una losa de hormigón */
    G4Material *hormigon =
            G4NistManager::Instance()->FindOrBuildMaterial("G4_CONCRETE");

    G4Box *solidDetector2 =
            new G4Box("solidDetector2", 4 * m, 15 * cm,  4 * m);

    G4LogicalVolume *logicDetector2 =
            new G4LogicalVolume(solidDetector2,hormigon,"logicDetector2");

    /* Construcción del volumen físico de la losa (no es necesaria una
    variable porque se adhiere a la madre) */
    new G4PVPlacement(nullptr,G4ThreeVector(0,-88.85*m,0*m),
            logicDetector2,"physDetector2",logicWorld,false,0);


    return physWorld;



}

/* Método para registrar y asignar el detector sensible */
void DetConstruction::ConstructSDandField(){

    /* Creación del detector sensible */
    SensitiveDetector* sensitiveDetector =
            new SensitiveDetector("SensitiveDetector");

    /* Registro en el manager */
    G4SDManager::GetSDMpointer()->AddNewDetector(sensitiveDetector);

    /* Asignación del detector sensible al volumen lógico correspondiente */
    SetSensitiveDetector("logicDetector", sensitiveDetector);

    /* Mensaje de debug */
    G4cout << "Sensitive Detector registered and assigned" << G4endl;
}
