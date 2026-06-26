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
#include "G4GDMLParser.hh"
#include "G4LogicalVolumeStore.hh"

DetConstruction::DetConstruction(){
}

DetConstruction::~DetConstruction(){
}


/* Método para construir el mundo de simulación a partir de una geometría
GDML exportada desde FreeCAD. El propio GDML ya incluye los materiales
G4_AIR, G4_CONCRETE y G4_Al */
G4VPhysicalVolume* DetConstruction::Construct() {

    /* Lectura del archivo GDML exportado desde FreeCAD */
    G4GDMLParser parser;
    parser.Read("../geometries/casav1-worldVOL.gdml");

    G4cout << "GDML loaded: ../geometries/casav1-worldVOL.gdml" << G4endl;

    /* Obtención del volumen mundo */
    G4VPhysicalVolume* world = parser.GetWorldVolume();

    return world;
}


/* Método para registrar y asignar el detector sensible */
void DetConstruction::ConstructSDandField(){

    /* Creación del detector sensible */
    SensitiveDetector* sensitiveDetector =
            new SensitiveDetector("SensitiveDetector");

    /* Registro en el SDManager */
    G4SDManager::GetSDMpointer()->AddNewDetector(sensitiveDetector);

    /* Búsqueda del volumen lógico definido en el GDML */
    G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
    G4LogicalVolume* detectorLV = lvStore->GetVolume("physDetector");

    /* Verificación y asignación del detector sensible */
    if (detectorLV) {
        detectorLV->SetSensitiveDetector(sensitiveDetector);
        G4cout << "Sensitive Detector assigned to 'physDetector'"
               << G4endl;
    } else {
        G4cerr << "ERROR: Logical volume 'physDetector' not found!"
               << G4endl;
    }
}
