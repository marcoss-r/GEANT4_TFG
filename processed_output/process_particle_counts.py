################################################################################
# Script que procesa el número de partículas detectadas por cada archivo de 
# salida csv de Geant4, generando gráficas para cada partícula detectada en
# función de la energía del haz incidente.
################################################################################

import os
import glob
import re
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from collections import defaultdict

# Configuración básica
N_LAUNCHED = 10000
E_MIN = 20       # MeV
E_MAX = 30000    # MeV

# Directorios
PARTICLE_TYPES = {
    'neutron': {
        'input': 'output_neutron',
        'output': 'output_plots/neutrons'
    },
    'proton': {
        'input': 'output_proton',
        'output': 'output_plots/protons'
    }
}

# Columnas de CSVs
COLUMN_NAMES = [
    'EventID',
    'ParticleName',
    'InitialEnergy_MeV',
    'DepositedEnergy_MeV',
    'PosX_m',
    'PosY_m',
    'PosZ_m',
    'Theta_deg',
    'Phi_deg',
    'MomX',
    'MomY',
    'MomZ'
]

# Definición de funciones
def extract_energy(filename):
    """Extrae energía en MeV a partir del nombre del archivo."""
    match = re.search(r'(\d+\.?\d*)MeV', filename)
    if match:
        return float(match.group(1))
    
    match = re.search(r'(\d+\.?\d*)GeV', filename)
    if match:
        return float(match.group(1)) * 1000
    
    raise ValueError(f"No se pudo extraer la energía de {filename}")


def process_directory(input_dir, output_dir, particle_type):
    """Procesa una carpeta entera y genera las gráficas."""
    
    print(f"\n{'='*60}")
    print(f"Procesando: {particle_type.upper()}")
    print(f"{'='*60}")
    
    # Crear carpeta de salida si no existe
    os.makedirs(output_dir, exist_ok=True)
    
    # Búsqueda de archivos csv en la carpeta indicada
    files = glob.glob(os.path.join(input_dir, "*.csv"))
    
    if len(files) == 0:
        print(f"No se encontraron archivos en {input_dir}")
        return
    
    # Extraer energía y ordenar en función de esta
    data = []
    for f in files:
        energy = extract_energy(os.path.basename(f))
        data.append((energy, f))
    
    data.sort(key=lambda x: x[0])
    
    print(f"Archivos encontrados: {len(data)}")
    
    # Obtener todas las partículas únicas detectadas
    all_particles = set()
    for _, filepath in data:
        df = pd.read_csv(filepath, names=COLUMN_NAMES, skiprows=1)
        all_particles.update(df['ParticleName'].unique())
    
    print(f"Tipos de partículas: {sorted(all_particles)}")
    
    # Inicializar contadores
    particle_counts = {particle: [] for particle in all_particles}
    
    # Contar partículas en cada archivo
    for energy, filepath in data:
        df = pd.read_csv(filepath, names=COLUMN_NAMES, skiprows=1)
        
        counts = df['ParticleName'].value_counts().to_dict()
        
        for particle in all_particles:
            particle_counts[particle].append(counts.get(particle, 0))
    
    # Generar logspace preciso para representación
    n_points = len(data)
    energies_log = np.logspace(np.log10(E_MIN), np.log10(E_MAX), n_points)
    
    # Generar gráficas
    for particle_name, counts in particle_counts.items():
        
        counts_array = np.array(counts)
        
        plt.figure(figsize=(10, 6))
        
        plt.scatter(energies_log, counts_array, alpha=0.7, s=50)
        
        plt.xscale("log")
        
        plt.xlabel("Energía del haz incidente [MeV]", fontsize=12)
        plt.ylabel(f"Número de {particle_name} detectados", fontsize=12)
        plt.title(f"Haz de {particle_type}: Conteo de {particle_name} vs Energía", fontsize=14)
        
        plt.grid(True, which="both", ls="--", alpha=0.3)
        
        total_count = int(counts_array.sum())
        plt.text(0.02, 0.98, 
                 f'Total: {total_count}\nHaz: {particle_type}',
                 transform=plt.gca().transAxes,
                 verticalalignment='top',
                 bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.5))
        
        plt.tight_layout()
        
        output_filename = os.path.join(output_dir, f"plot_{particle_name}.png")
        plt.savefig(output_filename, dpi=300)
        print(f"  ✓ {particle_name}: {total_count} detectados")
        
        plt.close()
    
    print(f"Gráficas guardadas en: {output_dir}")


# Función principal
print("Iniciando procesamiento de datos...")

for particle_type, dirs in PARTICLE_TYPES.items():
    process_directory(dirs['input'], dirs['output'], particle_type)

print(f"\n{'='*60}")
print("PROCESAMIENTO COMPLETADO")
print(f"{'='*60}")
print(f"Gráficas guardadas en:")
for particle_type, dirs in PARTICLE_TYPES.items():
    print(f"  - {dirs['output']}/")
