# GEANT4_TFG
Repositorio que contiene las distintas aplicaciones de GEANT4 elaboradas para el TFG de Física e Instrumentación espacial que simula un detector de neutrones con métodos Monte Carlo, concretamente usando la librería GEANT4. Contiene además las entradas de las simulaciones así como imágenes de GEANT4, archivos para el procesamiento de datos y gráficas obtenidas mediante los mimsmos.

## Aplicaciones
A continuación se muestra un listado de las distintas aplicaciones creadas: 
### Aplicación 1
La primera aplicación contiene un código sencillo partiendo del manual oficial de G4 con una geometría sencilla. No contiene actualmente ninguna acción de conteo elaborada, solamente un "stepping action" que marca cuando una partícula está en contacto con el detector. Asimismo, tampoco contiene una generación de partículas compleja.
### Aplicación 2
Esta segunda aplicación parte de la primera y consigue importar una geometría compleja partiendo desde FreeCAD y GDML. Concretamente, el mundo de simulación lo componen las pinzas de plomo que actúan de productor en un monitor de neutrones siguiendo el estándar NM64. Sus medidas están basadas en las del detector CaLMa. Además, se incluye una sección que crea un archivo .csv conteniendo el tiempo de ejecución cuando se ejecuta con un archivo macro como argumento.
### Aplicación 3
Esta aplicación parte de la primera. Contiene un plano de detección que proporciona medidas de las partículas incidentes. Entre estas medidas se encuentran energía, momento, tipo, ángulo de incidencia, tiempo relativo a la primera partícula en interactuar con el detector de todo el evento y otras más. El plano tiene una geometría sencilla y una capa de aire por encima de 200 metros que busca modelar la capa atmosférica más cercana al detector. Para la generación de partículas, se obtienen los datos de las mismas a partir de un archivo csv. Se emplea multihilo y, por tanto, mecanismos de exclusión mutua para evitar conflictos entre los hilos de ejecución. Esta aplicación se ha empleado como referencia en el TFG.
### Aplicación 4
La cuarta aplicación es una copia de la tercera. Contiene la misma funcionalidad de recibir como entrada un archivo .csv y proporcionar como salida otro archivo .csv con diversos datos de las partículas que llegan al plano de detección. El cambio que aparece en esta aplicación es la presencia de un tejado de hormigón que atenua las partículas incidentes. Esta aplicación es la llamada S1 en el TFG.
### Aplicación 5
La quinta aplicación es una copia de la cuarta. Contiene la misma funcionalidad de recibir como entrada un archivo .csv y proporcionar como salida otro archivo .csv con diversos datos de las partículas que llegan al plano de detección. El cambio que aparece en esta aplicación es la adición de unas paredes laterales y suelo, también d ehormigón, que pueden influir en los conteos. Esta aplicación es la llamada S2 en el TFG.
### Aplicación 6
Cuenta con la misma funcionalidad que las aplicaciones anteriores. La única modificación es la geometría. Esta geometría es importada desde FreeCAD empleando GDML y el parser de GEANT4. Esta aplicación es la llamada S3 en el TFG.

## Generación de partículas
Como se puede apreciar en la descripción de las aplicaciones, algunas reciben como entrada un archivo csv con datos de partículas que actuarán como partículas primarias en la simulación. Estos archivos son generados automáticamente por un archivo de python llamado "particlegen". Este archivo tiene la capacidad de generar partículas de manera aleatoria en un volumen determinado. Actualmente y para cumplir las necesidades del trabajo, la altura está marcada como fija al igual que la dirección de movimiento. En cuanto a la energía, se genera un espaciado logarítmico y se genera un número especificado de partículas por cada punto del espaciado. El tipo de partícula también está marcado como fijo y varía en función de las necesidades de las simulaciones.

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


## Manual de instalación de GEANT4 en Linux


## Archivos previos para descargar

### CMake

Para poder instalar GEANT4, necesitamos tener una versión reciente de CMake para poder realizar la compilación del programa.

**1. Instalación por defecto**

Primero probamos con la opción por defecto de Linux para instalar paquetes. Además, instalaremos la interfaz para mayor facilidad con la instalación:

```bash
sudo apt install cmake cmake-curses-gui
```

Normalmente, CMake se encuentra instalado en la gran mayoría de ordenadores y no suele ser necesario instalarlo. Si hemos instalado una versión posterior a la 3.16, no necesitamos más de esta sección.

Se puede comprobar la versión con:

```bash
cmake --version
```

**2. Actualización de apt**

En el caso de no haber podido instalar una versión posterior de CMake, podemos actualizar apt por si tenemos una versión previa en la que no se encuentre CMake >= 3.16:

```bash
sudo apt update
sudo apt upgrade cmake
```

Puede suceder que esto no sirva si tenemos instalada una versión de Ubuntu muy antigua.

**3. Instalación manual**

En el caso, de nuevo, de no tener todavía una versión de CMake válida, debemos obtenerla de manera externa. A continuación se muestra el ejemplo para instalar la versión 3.16.9:

```bash
wget https://github.com/Kitware/CMake/releases/download/v3.16.9/cmake-3.16.9-Linux-x86_64.tar.gz
tar -xzvf cmake-3.16.9-Linux-x86_64.tar.gz
sudo mv cmake-3.16.9-Linux-x86_64 /opt/cmake-3.16.9
export PATH=/opt/cmake-3.16.9/bin:$PATH
```

Esto finaliza la descarga de CMake con una versión adecuada para su uso con GEANT4. Lo utilizaremos tanto para compilar GEANT4 como los proyectos que hagamos.


### XERCES-C para el uso de GDML

Xerces-C es una librería necesaria para poder usar archivos GDML con GEANT4, lo cual es muy útil si queremos utilizar geometrías complejas. Su instalación es fácil y, dependiendo de la versión de Ubuntu, usaremos un comando u otro:

```bash
sudo apt install xerces-c-dev
# o bien:
sudo apt install libxerces-c-dev
```


### QT

Si queremos visualizar nuestras simulaciones realizadas con GEANT4, necesitamos instalar un visualizador. En este caso utilizaremos QT, del que necesitamos una versión posterior a la 5.9:

```bash
sudo apt install qt5-default qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools
qmake --version
```


### OpenGL

Para poder usar la visualización con QT, necesitamos tener instalado OpenGL. Podemos comprobar si ya lo tenemos, que es lo más habitual, con:

```bash
ls /usr/include/GL/
```

En caso de no tenerlo instalado:

```bash
sudo apt install libglu1-mesa-dev freeglut3-dev mesa-common-dev
```


### Paquetes extra necesarios

Además de todo lo mencionado previamente, para la instalación de GEANT4 necesitamos instalar ciertos paquetes adicionales como compiladores y demás:

```bash
sudo apt install g++ gcc binutils libx11-dev libxpm-dev \
libxft-dev libxext-dev libglew-dev libjpeg-dev libpng-dev libtiff-dev \
libgif-dev libxml2-dev libssl-dev libfftw3-dev libqt5core5a
```

Deberíamos tener la mayoría de paquetes ya instalados; algunos dan problemas debido a cambios de nombre entre versiones de Ubuntu. Podemos comprobar si los tenemos con:

```bash
dpkg -l | grep libxft
dpkg -l | grep gif
dpkg -l | grep libxext
dpkg -l | grep libxml2-dev
```

Una vez comprobamos que ya están, los borramos del comando original y volvemos a ejecutar.

Este paquete acostumbra a estar instalado, pero es posible que no lo esté y dará problemas en la compilación:

```bash
sudo apt install libxmu-dev
```


## Instalación de GEANT4

### Descarga del paquete

En esta sección ya tenemos todos los paquetes y programas necesarios para poder instalar GEANT4.

**1.** Creamos un directorio que llamaremos `GEANT4` y nos metemos en él:

```bash
mkdir GEANT4
cd GEANT4/
```

**2.** Descargamos el paquete:

```bash
wget https://gitlab.cern.ch/geant4/geant4/-/archive/v11.3.2/geant4-v11.3.2.tar.gz
```

**3.** Descomprimimos el archivo recién descargado:

```bash
tar xzfv geant4-v11.3.2.tar.gz
```


### Compilación con CMake

Partimos del archivo descomprimido para obtener una versión de GEANT4 con la que poder crear nuestros proyectos.

**1.** Creamos un directorio de build y nos metemos dentro:

```bash
mkdir geant4-v11.3.2-build
cd geant4-v11.3.2-build/
```

**2.** Abrimos la interfaz gráfica de CMake:

```bash
ccmake ../geant4-v11.3.2
```

**3.** Configuramos el archivo presionando `c`.

**4.** Cuando ya se haya configurado, nos aparecerán distintas opciones. Debemos modificar los siguientes parámetros:

- CMAKE_INSTALL_PREFIX: pondremos la ruta de nuestra carpeta de GEANT4 añadiendo un directorio de install: geant4-v11.3.2-install
- GEANT4_INSTALL_DATA: lo marcamos a ON
- GEANT4_USE_GDML: si vamos a usar GDML para las geometrías de nuestros proyectos, lo marcamos a ON
- GEANT4_USE_OPENGL_X11: lo marcamos a ON
- GEANT4_USE_QT: lo marcamos a ON
- GEANT4_USE_RAYTRACER_X11: lo marcamos a ON
- CMAKE_BUILD_TYPE: lo pondremos en RelWithDebInfo por si acaso quisiésemos hacer una depuración de nuestro proyecto al obtener errores

**5.** Configuramos la instalación presionando `c`.

**6.** Generamos presionando `g`.


### Instalación

Partimos de los archivos de la carpeta build mediante CMake junto con el archivo `Makefile` que habrá aparecido en el directorio de build.

**1.** Compilamos los archivos:

```bash
make -j($(nproc))
```

Esto compilará los archivos siguiendo la configuración marcada y utilizará todos los núcleos disponibles del ordenador para reducir el tiempo de compilación.

**2.** Instalamos GEANT4 una vez compilados todos los archivos:

```bash
make install
```


### Pasos post-instalación

Pese a que ya hayamos instalado GEANT4 en nuestro ordenador, nos falta un paso crucial para poder compilar y ejecutar nuestros proyectos.

**1.** Nos metemos en el directorio de instalación siguiendo la ruta específica y copiamos la ruta absoluta:

```bash
cd geant4-v11.3.2-install/share/Geant4/geant4make
```

**2.** Modificamos el archivo `.bashrc`:

```bash
vim /home/*nombre_de_usuario*/.bashrc
```

**3.** Al final del archivo escribimos lo siguiente:

```bash
alias geant4make='source /home/geant/GEANT4_installation/GEANT4/geant4-v11.3.2-install/share/Geant4/geant4make/geant4make.sh'
```

Ahora, cada vez que queramos activar las variables de entorno necesarias para la ejecución de GEANT4, simplemente escribimos `geant4make` en la consola.

Nota: debemos cerrar la consola actual para que se actualice el cambio en .bashrc.


## Compilación de un proyecto

En esta sección se muestra cómo compilar un proyecto ya realizado.

**1.** Nos metemos en la carpeta del proyecto y creamos un directorio de build:

```bash
mkdir build
cd build
```

**2.** Usamos CMake directamente sin la interfaz, compilamos y generamos el ejecutable:

```bash
cmake ..
make
```

**3.** Para ejecutar el archivo:

```bash
./example
```