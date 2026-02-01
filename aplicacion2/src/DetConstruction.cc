#include "DetConstruction.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
//Ignorar errores de geometría de GDML
#include "G4GDMLParser.hh"


namespace prueba{

G4VPhysicalVolume* DetConstruction::Construct() {

    // -------------------------------
    // 1. Definir materiales
    // -------------------------------
    // Aire para el World (material NIST)
    G4Material* air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

    // Plomo
    G4Element* elPb = new G4Element("Lead","Pb", 82., 207.2*g/mole);
    G4Material* Pb = new G4Material("Lead", 11.34*g/cm3, 1);
    Pb->AddElement(elPb, 1);

    // Hormigón (usado en el edificio exportado desde FreeCAD)
    G4Material* concrete = G4NistManager::Instance()->FindOrBuildMaterial("G4_CONCRETE");

    // -------------------------------
    // 2. Leer GDML exportado desde FreeCAD
    // -------------------------------
    G4GDMLParser parser;
    parser.Read("../geometry/v4_15pinzas-worldVOL.gdml"); // ruta relativa dentro del proyecto

    // -------------------------------
    // 3. Obtener World
    // -------------------------------
    G4VPhysicalVolume* world = parser.GetWorldVolume();

    return world;


    }

}