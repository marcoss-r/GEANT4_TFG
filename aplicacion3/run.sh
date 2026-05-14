#!/bin/bash

# =========================
# Directorio del proyecto
# =========================
PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"

# =========================
# Carpetas
# =========================
INPUT_DIR="$PROJECT_DIR/../particlegen/input2"
OUTPUT_DIR="$PROJECT_DIR/../processed_output3/aplicaion3"

LOCAL_INPUT_DIR="$PROJECT_DIR/input"
LOCAL_OUTPUT_DIR="$PROJECT_DIR/output"

BUILD_DIR="$PROJECT_DIR/build"

# =========================
# Crear carpetas necesarias
# =========================
mkdir -p "$OUTPUT_DIR"
mkdir -p "$LOCAL_INPUT_DIR"
mkdir -p "$LOCAL_OUTPUT_DIR"

# =========================
# Compilar SOLO UNA VEZ
# =========================
echo "Compilando proyecto..."

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1

cmake .. || exit 1
make -j4 || exit 1

echo "Compilación completada"
echo ""

# =========================
# Procesar CSVs
# =========================
for INPUT_FILE in "$INPUT_DIR"/*.csv; do

    BASENAME=$(basename "$INPUT_FILE")

    echo "Procesando: $BASENAME"

    # ---------------------------------
    # Copiar como input.csv
    # ---------------------------------
    cp "$INPUT_FILE" "$LOCAL_INPUT_DIR/input.csv"

    # ---------------------------------
    # Ejecutar simulación
    # ---------------------------------
    echo "  Ejecutando simulación..."

    ./aplicacion3 ../mac_files/run_batch.mac

    # ---------------------------------
    # Nombre final del output
    # input_xxx.csv -> output_xxx.csv
    # ---------------------------------
    OUTPUT_NAME="${BASENAME/input/output}"

    # ---------------------------------
    # Mover output generado
    # ---------------------------------
    if [ -f "$LOCAL_OUTPUT_DIR/output.csv" ]; then

        mv "$LOCAL_OUTPUT_DIR/output.csv" \
           "$OUTPUT_DIR/$OUTPUT_NAME"

        echo "  Output guardado en:"
        echo "  $OUTPUT_DIR/$OUTPUT_NAME"

    else
        echo "  ERROR: No se generó output.csv"
    fi

    echo ""

done

echo "Todas las simulaciones terminadas"
echo "Archivos generados en:"
echo "$OUTPUT_DIR"

ls -lh "$OUTPUT_DIR"