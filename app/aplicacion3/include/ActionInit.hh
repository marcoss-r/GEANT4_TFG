#ifndef ACTIONINIT_HH
#define ACTIONINIT_HH


#include "G4VUserActionInitialization.hh"

/* Definición de una clase que utiliza herencia pública de una clase 
G4VUserActionInitialization */
class ActionInit : public G4VUserActionInitialization{
    public:
    
      ActionInit();
      ~ActionInit() override;

      /* Sobrescritura  del método Build de la clase base para definir nuestras 
      acciones */
      void Build() const override;


};

#endif