import numpy as np
import pandas as pd
import os

n_events = 10000
output_dir = "./particlegen/input2/"

# Parámetros del detector en metros
detector_size = 6.0
height = 99.0

# Enumeración de tipos de partículas
particle_types = {
    0: 'proton'
    #1: 'neutron'
    #2: 'e-'
}

# 50 puntos intermedios + los 2 extremos = 52 energías en logspace de 20 MeV a 30 GeV
energies_MeV = np.logspace(np.log10(20.0), np.log10(30000.0), 52)

os.makedirs(output_dir, exist_ok=True)

for energy in energies_MeV:
    energy_int = int(round(energy))
    output_file = os.path.join(output_dir, f"input_particles_{energy_int}MeV_proton.csv")

    # Generación de tipos de partículas
    particle_ids = np.random.randint(0, 1, n_events)
    particle_names = [particle_types[pid] for pid in particle_ids]

    # Generación de datos
    data = {
        'particle_id': particle_ids,  # 0=neutron, 1=proton, 2=electron
        'particle': particle_names,
        'energy_MeV': np.random.uniform(energy, energy, n_events),  # Energía fija por CSV
        'pos_x_m': np.random.uniform(-detector_size/2, detector_size/2, n_events),
        'pos_y_m': [height] * n_events,  # Altura fija
        'pos_z_m': np.random.uniform(-detector_size/2, detector_size/2, n_events),
        'dir_x': [0.0] * n_events,  # Dirección perpendicular (hacia abajo)
        'dir_y': [-1.0] * n_events,
        'dir_z': [0.0] * n_events
    }

    df = pd.DataFrame(data)
    df.to_csv(output_file, index=False)
    print(f"Generado: {output_file}  ({energy:.4f} MeV)")

print(f"\nTotal de archivos generados: {len(energies_MeV)}")