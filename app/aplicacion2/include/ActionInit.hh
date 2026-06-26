/* Instrucciones del compilador para que solo se incluya la cabecera una vez 
(guardas de C++) */
#pragma once
#include "G4VUserActionInitialization.hh"


/* Definición de una clase que utiliza herencia pública de una clase 
G4VUserActionInitialization */
class ActionInit : public G4VUserActionInitialization{
    public:
      void Build() const override;


};