//Instrucciones del compilador para que solo se incluya la cabecera una vez
#pragma once

#include "G4VUserActionInitialization.hh"

//Forma de agrupar identificadores
namespace prueba{

//Definimos una clase que utiliza herencia pública de una clase G4VUserAction...
class ActionInit : public G4VUserActionInitialization{
    public:
      void Build() const override;


};
}