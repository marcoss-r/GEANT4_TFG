#include "ActionInit.hh"
#include "PrimaryGenAction.hh"
#include "SteppingAction.hh"

/* Desde este archivo comienza la inicialización de la acción en la que se 
definen las acciones del usuario, la generación de partículas y el stepping 
(acciones a ejecutar por cada step) si procede */
void ActionInit::Build() const{
    
    /* Construir acciones del usuario */
    SetUserAction(new PrimaryGenAction());
    SetUserAction(new SteppingAction());
};