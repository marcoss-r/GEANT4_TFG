import os
import glob
import re
import numpy as np
import matplotlib.pyplot as plt

PROTON_DIR = "output_proton"
NEUTRON_DIR = "output_neutron"
OUTPUT_DIR = "output_plots"
N_LAUNCHED = 10000

E_MIN = 20       # MeV
E_MAX = 30000    # MeV


def extract_energy(filename):
    """
    Extrae energía en MeV a partir nombre del archivo.
    """
    match = re.search(r'(\d+\.?\d*)MeV', filename)
    if not match:
        raise ValueError(f"No se pudo extraer energía de {filename}")
    return float(match.group(1))


def compute_fractions(directory):
    """
    1) Extrae energía del nombre
    2) Ordena físicamente por energía
    3) Devuelve fracciones en ese orden
    """
    files = glob.glob(os.path.join(directory, "*.csv"))
    
    if len(files) == 0:
        raise ValueError(f"No se encontraron archivos en {directory}")
    
    data = []
    
    for f in files:
        energy = extract_energy(os.path.basename(f))
        data.append((energy, f))
    
    # Orden creciente de energías
    data.sort(key=lambda x: x[0])
    
    fractions = []
    
    for energy, f in data:
        with open(f, "r") as file:
            n_lines = sum(1 for _ in file) - 1
        
        fractions.append(n_lines / N_LAUNCHED)
    
    return np.array(fractions)


# Calcular fracciones
proton_frac = compute_fractions(PROTON_DIR)
neutron_frac = compute_fractions(NEUTRON_DIR)

# Verificación de número de elementos
if len(proton_frac) != len(neutron_frac):
    print("Distinto número de energías entre protones y neutrones")

n_points = min(len(proton_frac), len(neutron_frac))

# Logspace preciso para representar en la gráfica
energies_log = np.logspace(np.log10(E_MIN),
                           np.log10(E_MAX),
                           n_points)

# Plot
plt.figure(figsize=(8,6))

plt.scatter(energies_log, proton_frac[:n_points], label="Protones")
plt.scatter(energies_log, neutron_frac[:n_points], label="Neutrones")

plt.xscale("log")

plt.xlabel("Energía [MeV]")
plt.ylabel("Fracción de partículas en detector")
plt.title(" Fracción de partículas detectadas en función de la Energía")

plt.grid(True, which="both", ls="--", alpha=0.5)
plt.legend()

plt.tight_layout()
output_filename = os.path.join(OUTPUT_DIR, f"plot_fractions.png")
plt.savefig(output_filename, dpi=300)
plt.show()

