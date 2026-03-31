#!/bin/bash

# Obtener directorio absoluto del proyecto
PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"

# Carpetas (ABSOLUTAS)
INPUT_DIR="$PROJECT_DIR/../particlegen/input"
OUTPUT_DIR="$PROJECT_DIR/../particlegen/output"
SRC_DIR="$PROJECT_DIR/src"
BUILD_DIR="$PROJECT_DIR/build"

# Archivos a modificar
PRIMARY_FILE="$SRC_DIR/PrimaryGenAction.cc"
DETECTOR_FILE="$SRC_DIR/SensitiveDetector.cc"

# Crear carpeta output si no existe
mkdir -p "$OUTPUT_DIR"

# Obtener todos los CSVs de input
for INPUT_FILE in "$INPUT_DIR"/*.csv; do
    
    BASENAME=$(basename "$INPUT_FILE")
    
    # Generar nombre de salida
    OUTPUT_NAME="${BASENAME/input/output}"
    OUTPUT_FILE="$OUTPUT_DIR/$OUTPUT_NAME"
    
    echo "Procesando: $BASENAME"
    echo "  Input:  $INPUT_FILE"
    echo "  Output: $OUTPUT_FILE"
    
    # Modificar PrimaryGenAction.cc con ruta ABSOLUTA
    sed -i "s|LoadParticlesFromCSV(\".*\");|LoadParticlesFromCSV(\"$INPUT_FILE\");|g" "$PRIMARY_FILE"
    
    # Modificar SensitiveDetector.cc con ruta ABSOLUTA
    sed -i "s|fOutputFile.open(\".*\", std::ios::app);|fOutputFile.open(\"$OUTPUT_FILE\", std::ios::app);|g" "$DETECTOR_FILE"
    
    # Limpiar y compilar
    echo "  Compilando..."
    rm -rf "$BUILD_DIR"
    mkdir "$BUILD_DIR"
    cd "$BUILD_DIR"
    
    cmake .. > /dev/null
    make -j4 > /dev/null
    
    # Ejecutar
    echo "  Ejecutando..."
    ./aplicacion6 ../mac_files/run_batch.mac
    
    cd "$PROJECT_DIR"
    
    echo "  Completado"
    echo ""
    
done

echo "Todas las simulaciones terminadas"
echo "Archivos en: $OUTPUT_DIR"
ls -lh "$OUTPUT_DIR"
