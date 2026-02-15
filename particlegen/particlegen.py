import numpy as np
import pandas as pd

n_events = 10000 
output_file = "./particlegen/input/input_particles_30000MeV_neutron.csv"

# Parámetros del detector en metros
detector_size = 6.0
height = 99.0 

# Enumeración de tipos de partículas
particle_types = {
    0: 'neutron'
    #1: 'proton'
    #2: 'e-'
}

# Generación de tipos de partículas
particle_ids = np.random.randint(0, 1, n_events)
particle_names = [particle_types[pid] for pid in particle_ids]

# Generación de datos
data = {
    'particle_id': particle_ids,  # 0=neutron, 1=proton, 2=electron
    'particle': particle_names,
    'energy_MeV': np.random.uniform(30000.0, 30000.0, n_events),  # Energía entre 20 MeV y 30 GeV
    'pos_x_m': np.random.uniform(-detector_size/2, detector_size/2, n_events),
    'pos_y_m': [height] * n_events,  # Altura fija
    'pos_z_m': np.random.uniform(-detector_size/2, detector_size/2, n_events),
    'dir_x': [0.0] * n_events,  # Dirección perpendicular (hacia abajo)
    'dir_y': [-1.0] * n_events,
    'dir_z': [0.0] * n_events
}

df = pd.DataFrame(data)

# Guardar CSV
df.to_csv(output_file, index=False)