#include "ActionInit.hh"
#include "PrimaryGenAction.hh"

/*Desde este archivo comienza la inicialización de la acción en la que se definen las acciones del
usuario, la generación primaria y el stepping*/

ActionInit::ActionInit(){
}

ActionInit::~ActionInit(){
}

void ActionInit::Build() const{
    //Build user actions
    SetUserAction(new PrimaryGenAction());
};
