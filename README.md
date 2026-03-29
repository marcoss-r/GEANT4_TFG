# GEANT4_TFG
Repositorio que contiene las distintas aplicacines de GEANT4 elaboradas para el TFG de FIE que simula un detector de neutrones.
## Aplicación 1
La primera aplicación contiene un código sencillo partiendo del tutorial con una geometría sencilla. No contiene actualmente ninguna acción de conteo elaborada, solamente un "stepping action" que marca cuando una partícula está en contacto con el detector.
## Aplicación 2
Esta segunda aplicación partió de la primera y consigue importar una geometría compleja partiendo desde FreeCAD y GDML. Además, se incluye una sección que crea un archivo .csv conteniendo el tiempo de ejecución cuando se ejecuta junto a otro archivo macro.
## Aplicación 3
Esta aplicación parte de la primera. La idea es hacer un plano de detección que proporcione medidas simples de las partículas simuladas como energía, momento, tipo y ángulo de incidencia en el plano con una geometría sencilla y una capa de aire por encima.
## Aplicación 4
La cuarta aplicación es una copia de la tercera. Contiene la misma funcionalidad de recibir como entrada un archivo .csv y proporcionar como salida otro archivo .csv con diversos datos de las partículas que llegan al plano de detección. El cambio que aparece en esta aplicación es la presencia de un tejado de hormigón que bloquea partículas incidentes.
## Aplicación 5
La quinta aplicación es una copia de la cuarta. Contiene la misma funcionalidad de recibir como entrada un archivo .csv y proporcionar como salida otro archivo .csv con diversos datos de las partículas que llegan al plano de detección. El cambio que aparece en esta aplicación es la presencia de unas paredes de hormigón que bloquean las partículas incidentes.
