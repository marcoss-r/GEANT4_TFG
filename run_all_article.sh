
cd ./aplicacion3 || exit 1
echo "Ejecutando simulación de referencia..."
./run.sh
echo "Ejecución de simulación de referencia completada"

# Change directory to article simulation with roof
cd ../aplicacion4 || exit 1

THICKNESSES=("10" "15" "50" "80")

for THICKNESS in "${THICKNESSES[@]}"; do

    echo "Configurando simulación con techo: output_${THICKNESS}cm"


    # Edit DetConstruction.cc para cambiar el grosor del techo
    DET_FILE="src/DetConstruction.cc"
    sed -i "s/G4Box \*solidDetector2 = new G4Box(\"solidDetector2\", 4 \* m, [0-9]\+ \* cm,  4 \* m);/G4Box *solidDetector2 = new G4Box(\"solidDetector2\", 4 * m, ${THICKNESS} * cm,  4 * m);/" "$DET_FILE"
    echo "  DetConstruction actualizado: ${THICKNESS} * cm"

    # 2. Editar OUTPUT_DIR en run.sh
    sed -i "s|OUTPUT_DIR=\"\$PROJECT_DIR/../../../simulationdata/output_article/simulation_4/.*\"|OUTPUT_DIR=\"\$PROJECT_DIR/../../../simulationdata/output_article/simulation_4/output_${THICKNESS}cm\"|" run.sh
    echo "  run.sh actualizado: output_${THICKNESS}cm"

    # 3. Ejecutar
    echo "Ejecutando simulación con techo (${THICKNESS} cm)..."
    ./run.sh
    echo "Ejecución con techo ${THICKNESS} cm completada"

done