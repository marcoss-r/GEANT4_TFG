#include "DetConstruction.hh"
#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4GDMLParser.hh"


/* Método para construir el mundo de simulación a partir de una geometría
GDML exportada desde FreeCAD */
G4VPhysicalVolume* DetConstruction::Construct() {

    /* Definición de materiales */

    /* Aire para el world (material NIST) */
    G4Material* air =
            G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

    /* Plomo */
    G4Element* elPb = new G4Element("Lead","Pb", 82., 207.2*g/mole);
    G4Material* Pb = new G4Material("Lead", 11.34*g/cm3, 1);
    Pb->AddElement(elPb, 1);

    /* Hormigón (usado en el edificio exportado desde FreeCAD) */
    G4Material* concrete =
            G4NistManager::Instance()->FindOrBuildMaterial("G4_CONCRETE");

    /* Lectura del archivo GDML exportado desde FreeCAD */
    G4GDMLParser parser;
    parser.Read("../geometry/v4_15pinzas-worldVOL.gdml");

    /* Obtención del volumen mundo */
    G4VPhysicalVolume* world = parser.GetWorldVolume();

    return world;
}
