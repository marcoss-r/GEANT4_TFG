#ifndef ACTIONINIT_HH
#define ACTIONINIT_HH

//Instrucciones del compilador para que solo se incluya la cabecera una vez
#pragma once

#include "G4VUserActionInitialization.hh"

//Definimos una clase que utiliza herencia pública de una clase G4VUserAction...
class ActionInit : public G4VUserActionInitialization{
    public:
    
      ActionInit();
      ~ActionInit() override;

      //Sobrescribimos el método Build de la clase base para definir nuestras acciones
      void Build() const override;


};

#endif