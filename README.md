# GEANT4_TFG
Repositorio que contiene las distintas aplicacines de GEANT4 elaboradas para el TFG de FIE que simula un detector de neutrones.

## Aplicaciones
A continuación se muestra un listado de las distintas aplicaciones creadas: 
### Aplicación 1
La primera aplicación contiene un código sencillo partiendo del tutorial con una geometría sencilla. No contiene actualmente ninguna acción de conteo elaborada, solamente un "stepping action" que marca cuando una partícula está en contacto con el detector.
### Aplicación 2
Esta segunda aplicación partió de la primera y consigue importar una geometría compleja partiendo desde FreeCAD y GDML. Además, se incluye una sección que crea un archivo .csv conteniendo el tiempo de ejecución cuando se ejecuta junto a otro archivo macro.
### Aplicación 3
Esta aplicación parte de la primera. La idea es hacer un plano de detección que proporcione medidas simples de las partículas simuladas como energía, momento, tipo y ángulo de incidencia en el plano con una geometría sencilla y una capa de aire por encima.
### Aplicación 4
La cuarta aplicación es una copia de la tercera. Contiene la misma funcionalidad de recibir como entrada un archivo .csv y proporcionar como salida otro archivo .csv con diversos datos de las partículas que llegan al plano de detección. El cambio que aparece en esta aplicación es la presencia de un tejado de hormigón que bloquea partículas incidentes.
### Aplicación 5
La quinta aplicación es una copia de la cuarta. Contiene la misma funcionalidad de recibir como entrada un archivo .csv y proporcionar como salida otro archivo .csv con diversos datos de las partículas que llegan al plano de detección. El cambio que aparece en esta aplicación es la presencia de unas paredes de hormigón que bloquean las partículas incidentes a la vez que un suelo.
### Aplicación &
Cuenta con la misma funcionalidad que las aplicaciones anteriores. La única modificación es la geometría. Esta geometría es importada desde FreeCAD empleando GDML y el parser de GEANT4.

## Generación de partículas
Como se puede apreciar en la descripción de las aplicaciones, algunas reciben como entrada un archivo csv con datos de partículas que actuarán como partículas primarias en la simulación. Estos archivos son generados automáticamente por un archivo de python llamado "particlegen". Este archivo tiene la capacidad de generar partículas de manera aleatoria en un volumen determinado. Actualmente y para cumplir las necesidades del trabajo, la altura está marcada como fija al igual que la dirección de movimiento. En cuanto a la energía, se genera un espaciado logarítmico y se genera un número especificado de partículas por cada punto del espaciado. El tipo de partícula también está marcado como fijo y se va cambiando según las necesidades de las simulaciones.

## Uso de las aplicaciones para realizar simulaciones
A continuación, se muestra una descripción de las principales funcionalidades de las distintas aplicaciones:

### Construcción del detector
La construcción del detector es variada en función de la aplicación. Los dos archivos que intervienen en la creación de geometrías, que no solo en la del detector, son "DetConstruction.cc" y "SensitiveDetector.cc". En el primero de ellos se detalla la construcción del mundo de simulación y el resto de geometrías. Hay dos maneras de realizar esta descripción, una es mediante una geometría exportada en formato GDML desde FreeCAD, y otra es hacerlo manualmente con las bibliotecas de GEANT4. Solamente se utiliza la primera opción en las aplicaciones 2 y 6. En el segundo archivo, "SensitiveDetector.cc", se realiza y genera la salida en un archivo csv. Esta generación se hace en una ruta determinada en la que hay que especificar el nombre del archivo de salida.

### Generación de partículas primarias.
En las aplicaciones 1 y 2, la simulación es realmente sencilla y la generación de partículas primarias está marcada únicamente por el archivo "PrimaryGenAction.cc". Este archivo contiene las características de las partículas primarias. Este no es el caso para el resto de aplicaciones pues en estas, PrimaryGenAction obtiene su input de una ruta relativa marcada en el código. Esta ruta debe ser cambiada para ajustar la entrada deseada a la simulación.

### Stepping Action
Esta función del código es obsoleta en cualquier aplicación a partir de la número 2. Se mantiene en el resto de aplicaciones pero la intención sería eliminarla. Actualmente, la funcionalidad implementada saca por consola el mensaje "Hit detector" cuando un step de una partícula se encuentra en el interior de la geometría marcada como detector.

## Procesamiento de la salida
Para el procesamiento de los datos una vez realizadas las simulaciones, se usan los archivos de pyhton incluidos en la carpeta "processed_output". Se pueden destacar "cleanheaders.py", que descarta cualquier cabecera de archivo previa a una posterior al igual que sus datos asociados en caso de un error en la ejecución de simulaciones. "compare_counts.py" compara los datos de salida de dos aplicaciones distintas.

## Manual de instalación de GEANT4 para Linux