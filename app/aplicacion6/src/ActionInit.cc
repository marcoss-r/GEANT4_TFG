#include "ActionInit.hh"
#include "PrimaryGenAction.hh"

/* Desde este archivo comienza la inicialización de la acción en la que se 
definen las acciones del usuario, la generación de partículas y el stepping 
(acciones a ejecutar por cada step) si procede */

/* Constructor */
ActionInit::ActionInit(){
}

/* Destructor */
ActionInit::~ActionInit(){
}

void ActionInit::Build() const{
    
    /* Construir acciones del usuario */
    SetUserAction(new PrimaryGenAction());
};