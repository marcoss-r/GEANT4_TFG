###############################################################################
# Script que procesa el número de conteos de partículas por cada archivo de 
# salida csv de Geant4, eliminando partículas con energía inferor a 20 MeV.
# Genera dos gráficas:
# 1) Conteo total de partículas detectadas (E ≥ 20 MeV)
# 2) Conteo de protones y neutrones detectados (E ≥ 20 MeV)
###############################################################################

import os
import glob
import re
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

PROTON_DIR = "output_proton"
NEUTRON_DIR = "output_neutron"
OUTPUT_DIR = "../../../../GEANT4_TFG/output/processed_output3/aplicacion3/output_plots"
N_LAUNCHED = 10000

E_MIN = 20       # MeV
E_MAX = 30000    # MeV

# Columnas de los CSVs generados
COLUMN_NAMES = [
    'EventID',
    'TrackID',
    'ParentID',
    'ParticleName',
    'Initial_Energy_MeV',
    'PosX_m',
    'PosY_m',
    'PosZ_m',
    'Theta_deg',
    'Phi_deg',
    'MomX',
    'MomY',
    'MomZ',
    'GlobalTime_s',
    'DeltaTime_s'
]
# ========== FUNCIONES ==========

def extract_energy(filename):
    """Extrae energía en MeV a partir del nombre del archivo."""
    #Buscar patrón de MeV
    match = re.search(r'(\d+\.?\d*)MeV', filename)
    if match:
        return float(match.group(1))
    
    #Buscar patrón de GeV
    match = re.search(r'(\d+\.?\d*)GeV', filename)
    if match:
        return float(match.group(1)) * 1000
    
    raise ValueError(f"No se pudo extraer energía de {filename}")


def compute_counts_all_particles(directory, energy_threshold=20):
    """
    Cuenta todas las partículas detectadas con energía superior al umbral.
    """
    files = glob.glob(os.path.join(directory, "*.csv"))
    
    if len(files) == 0:
        raise ValueError(f"No se encontraron archivos en {directory}")
    
    data = []
    for f in files:
        energy = extract_energy(os.path.basename(f))
        data.append((energy, f))
    
    data.sort(key=lambda x: x[0])
    
    counts = []
    
    for energy, filepath in data:
        df = pd.read_csv(filepath, names=COLUMN_NAMES, skiprows=1)
        
        # Filtrar por energía inicial superior al umbral
        df_filtered = df[df['Initial_Energy_MeV'] >= energy_threshold]
        
        counts.append(len(df_filtered))
    
    return np.array(counts)


def compute_counts_specific_particles(directory, particle_types=['proton', 'neutron'], energy_threshold=20):
    """
    Cuenta solo protones y neutrones con energía superior al umbral.
    """
    files = glob.glob(os.path.join(directory, "*.csv"))
    
    if len(files) == 0:
        raise ValueError(f"No se encontraron archivos en {directory}")
    
    data = []
    for f in files:
        energy = extract_energy(os.path.basename(f))
        data.append((energy, f))
    
    data.sort(key=lambda x: x[0])
    
    counts = []
    
    for energy, filepath in data:
        df = pd.read_csv(filepath, names=COLUMN_NAMES, skiprows=1)
        
        # Filtrar por tipo de partícula y energía
        df_filtered = df[
            (df['ParticleName'].isin(particle_types)) & 
            (df['Initial_Energy_MeV'] >= energy_threshold)
        ]
        
        counts.append(len(df_filtered))
    
    return np.array(counts)


# ========== GRÁFICA 1: Todas las partículas (E >= 20 MeV) ==========
print("Calculando conteos de todas las partículas...")

proton_all = compute_counts_all_particles(PROTON_DIR, energy_threshold=20)
neutron_all = compute_counts_all_particles(NEUTRON_DIR, energy_threshold=20)

n_points = min(len(proton_all), len(neutron_all))
energies_log = np.logspace(np.log10(E_MIN), np.log10(E_MAX), n_points)

plt.figure(figsize=(10, 6))

plt.scatter(energies_log, proton_all[:n_points], label="Haz de protones", alpha=0.7, s=50)
plt.scatter(energies_log, neutron_all[:n_points], label="Haz de neutrones", alpha=0.7, s=50)

plt.xscale("log")

plt.xlabel("Energía del haz [MeV]", fontsize=16)
plt.ylabel("Número de partículas detectadas (E ≥ 20 MeV)", fontsize=16)
plt.title("Conteo total de partículas detectadas en función de la energía del haz", fontsize=18)

plt.grid(True, which="both", ls="--", alpha=0.3)
plt.legend(fontsize=15)

plt.tight_layout()
os.makedirs(OUTPUT_DIR, exist_ok=True)
output_file1 = os.path.join(OUTPUT_DIR, "plot_all_counts_aboveTH.png")
plt.savefig(output_file1, dpi=300)
print(f"✓ Gráfica 1 guardada: {output_file1}")
plt.show()


# ========== GRÁFICA 2: Solo protones y neutrones (E >= 20 MeV) ==========
print("\nCalculando conteos de protones y neutrones...")

proton_specific = compute_counts_specific_particles(PROTON_DIR, 
                                                     particle_types=['proton', 'neutron'], 
                                                     energy_threshold=20)
neutron_specific = compute_counts_specific_particles(NEUTRON_DIR, 
                                                      particle_types=['proton', 'neutron'], 
                                                      energy_threshold=20)

plt.figure(figsize=(10, 6))

plt.scatter(energies_log, proton_specific[:n_points], label="Haz de protones", alpha=0.7, s=50)
plt.scatter(energies_log, neutron_specific[:n_points], label="Haz de neutrones", alpha=0.7, s=50)

plt.xscale("log")

plt.xlabel("Energía del haz [MeV]", fontsize=16)
plt.ylabel("Número de p/n detectados (E ≥ 20 MeV)", fontsize=16)
plt.title("Conteo de protones y neutrones detectados en función de la energía del haz", fontsize=18)

plt.grid(True, which="both", ls="--", alpha=0.3)
plt.legend(fontsize=15)

plt.tight_layout()
output_file2 = os.path.join(OUTPUT_DIR, "plot_proton_neutron_E20MeV.png")
plt.savefig(output_file2, dpi=300)
print(f"✓ Gráfica 2 guardada: {output_file2}")
plt.show()

print("\n✅ Procesamiento completado")

