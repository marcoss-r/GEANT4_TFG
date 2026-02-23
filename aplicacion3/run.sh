#!/bin/bash

# Carpetas
INPUT_DIR="../particlegen/input"
OUTPUT_DIR="../particlegen/output"
SRC_DIR="./src"
BUILD_DIR="build"

# Archivos a modificar
PRIMARY_FILE="$SRC_DIR/PrimaryGenAction.cc"
DETECTOR_FILE="$SRC_DIR/SensitiveDetector.cc"

# Obtener todos los CSVs de input
for INPUT_FILE in "$INPUT_DIR"/*.csv; do
    
    # Extraer nombre base (ej: input_particles_20MeV_neutron.csv)
    BASENAME=$(basename "$INPUT_FILE")
    
    # Generar nombre de salida (ej: output_particles_20MeV.csv)
    OUTPUT_NAME="${BASENAME/input/output}"
    OUTPUT_FILE="$OUTPUT_DIR/$OUTPUT_NAME"
    
    echo "Procesando: $BASENAME"
    
    # Modificar PrimaryGenAction.cc
    sed -i "s|LoadParticlesFromCSV(\".*\");|LoadParticlesFromCSV(\"$INPUT_FILE\");|g" "$PRIMARY_FILE"
    
    # Modificar SensitiveDetector.cc
    sed -i "s|fOutputFile.open(\".*\", std::ios::app);|fOutputFile.open(\"$OUTPUT_FILE\", std::ios::app);|g" "$DETECTOR_FILE"
    
    # Limpiar y compilar
    rm -rf "$BUILD_DIR"
    mkdir "$BUILD_DIR"
    cd "$BUILD_DIR"
    
    cmake .. > /dev/null 2>&1
    make -j4 > /dev/null 2>&1
    
    # Ejecutar
    ./aplicacion3 ./mac_files/run_batch.mac > /dev/null 2>&1
    
    cd ..
    
    echo "Completado: $OUTPUT_NAME"
    echo ""
    
done

echo "Todas las simulaciones terminadas"