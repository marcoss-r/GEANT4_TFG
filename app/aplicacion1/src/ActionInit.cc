#include "ActionInit.hh"
#include "PrimaryGenAction.hh"
#include "SteppingAction.hh"

namespace prueba {

/*Desde este archivo comienza la inicialización de la acción en la que se definen las acciones del
usuario, la generación primaria y el stepping*/
void ActionInit::Build() const{
    //Build user actions
    SetUserAction(new PrimaryGenAction());
    SetUserAction(new SteppingAction());
};
}