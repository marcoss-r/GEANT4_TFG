##############################################################################
# Script para generar archivos CSV con datos de partículas que serán empleadas
# como entrada para las simulaciones de Geant4.
##############################################################################


import numpy as np
import pandas as pd
import os

# Número de eventos a generar por cada energía
n_events = 10000

# Directorio de salida para los archivos CSV
output_dir = "./particlegen/input_article/"

# Parámetros de la simulación (metros)
generation_area_size = 8.0
height = 99.0

# Enumeración de tipos de partículas
particle_types = {
    0: 'mu+'
    #1: 'proton'
    #2: 'e-'
    #3: 'mu-'
    #4: 'neutron'
}

# 50 puntos de espaciado logarítmico desde 20 MeV a 4 GeV más los dos extremos
energies_MeV = np.logspace(np.log10(20.0), np.log10(4000.0), 52)

os.makedirs(output_dir, exist_ok=True)

# Bucle principal para generar los archivos CSV
for energy in energies_MeV:
    energy_int = int(round(energy))
    output_file = os.path.join(output_dir, f"input_particles_{energy_int}MeV_mu+.csv")

    # Generación de tipos de partículas
    particle_ids = np.random.randint(0, 1, n_events)
    particle_names = [particle_types[pid] for pid in particle_ids]

    # Generación de datos
    data = {
        'particle_id': particle_ids,
        'particle': particle_names,
        'energy_MeV': np.random.uniform(energy, energy, n_events),  # Energía fija por CSV
        'pos_x_m': np.random.uniform(-generation_area_size/2, generation_area_size/2, n_events),
        'pos_y_m': [height] * n_events,  # Altura fija
        'pos_z_m': np.random.uniform(-generation_area_size/2, generation_area_size/2, n_events),
        # Dirección perpendicular (hacia abajo)
        'dir_x': [0.0] * n_events,  
        'dir_y': [-1.0] * n_events,
        'dir_z': [0.0] * n_events
    }

    df = pd.DataFrame(data)
    df.to_csv(output_file, index=False)
    print(f"Generado: {output_file}  ({energy:.4f} MeV)")

print(f"\nTotal de archivos generados: {len(energies_MeV)}")