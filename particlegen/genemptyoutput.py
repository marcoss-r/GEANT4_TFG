###############################################################################
# Script para generar archivos CSV vacíos con el mismo formato que
# particlegen.py, pero sin datos.
###############################################################################


import os

carpeta_origen = "./input"
carpeta_destino = "./output"

os.makedirs(carpeta_destino, exist_ok=True)

prefijo_entrada = "input_particles"
prefijo_salida = "output_particles"

for archivo in os.listdir(carpeta_origen):
    if archivo.endswith(".csv") and archivo.startswith(prefijo_entrada):

        # Parte que viene después del prefijo
        resto_nombre = archivo[len(prefijo_entrada):]

        # Nuevo nombre manteniendo el resto
        nuevo_nombre = prefijo_salida + resto_nombre

        ruta_destino = os.path.join(carpeta_destino, nuevo_nombre)

        # Crear archivo vacío en destino
        with open(ruta_destino, "w", encoding="utf-8"):
            pass

        print(f"{archivo} -> {nuevo_nombre}")

print("Proceso terminado.")