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


G4VPhysicalVolume* DetConstruction::Construct() {

    // -------------------------------
    // 1. Definir materiales (si necesitas adicionales)
    // -------------------------------
    // El GDML ya incluye G4_AIR, G4_CONCRETE, G4_Al
    // Solo añade si necesitas otros materiales personalizados
    
    // -------------------------------
    // 2. Leer GDML exportado desde FreeCAD
    // -------------------------------
    G4GDMLParser parser;
    parser.Read("../geometry/v4_15pinzas-worldVOL.gdml");
    
    G4cout << "GDML loaded: ../geometries/casav1-worldVOL.gdml" << G4endl;
    
    // -------------------------------
    // 3. Obtener World
    // -------------------------------
    G4VPhysicalVolume* world = parser.GetWorldVolume();
    
    return world;
}



void DetConstruction::ConstructSDandField(){
    
    // -------------------------------
    // 1. Crear el Sensitive Detector
    // -------------------------------
    SensitiveDetector* sensitiveDetector = new SensitiveDetector("SensitiveDetector");
    
    // -------------------------------
    // 2. Registrarlo en el SDManager
    // -------------------------------
    G4SDManager::GetSDMpointer()->AddNewDetector(sensitiveDetector);
    
    // -------------------------------
    // 3. Buscar el volumen lógico del GDML
    // -------------------------------
    // El nombre del volumen lógico en tu GDML es "physDetector"
    G4LogicalVolumeStore* lvStore = G4LogicalVolumeStore::GetInstance();
    G4LogicalVolume* detectorLV = lvStore->GetVolume("physDetector");
    //                                                 ^^^^^^^^^^^^
    //                                                 Nombre del <volume> en el GDML
    
    // -------------------------------
    // 4. Verificar que se encontró y asignar
    // -------------------------------
    if (detectorLV) {
        detectorLV->SetSensitiveDetector(sensitiveDetector);
        G4cout << "Sensitive Detector assigned to 'physDetector'" << G4endl;
    } else {
        G4cerr << "ERROR: Logical volume 'physDetector' not found in GDML!" << G4endl;
    }
}

