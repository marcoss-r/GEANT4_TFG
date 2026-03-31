import os
import glob
import re
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# ========== CONFIGURACIÓN ==========
REFERENCE_DIR_PROTON = "aplicacion4/output_proton"
REFERENCE_DIR_NEUTRON = "aplicacion4/output_neutron"

COVERED_DIR_PROTON = "aplicacion5/output_proton"
COVERED_DIR_NEUTRON = "aplicacion5/output_neutron"

OUTPUT_DIR = "aplicacion5/output_plots"

E_MIN = 20       # MeV
E_MAX = 30000    # MeV

# Columnas del CSV
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

# ========== FUNCIONES ==========
def extract_energy(filename):
    """Extrae energía en MeV del nombre del archivo."""
    match = re.search(r'(\d+\.?\d*)MeV', filename)
    if match:
        return float(match.group(1))
    
    match = re.search(r'(\d+\.?\d*)GeV', filename)
    if match:
        return float(match.group(1)) * 1000
    
    raise ValueError(f"No se pudo extraer energía de {filename}")


def compute_counts_proton_neutron(directory, energy_threshold=20):
    """
    Cuenta SOLO protones y neutrones con energía depositada >= threshold.
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
        
        # Filtrar: solo proton/neutron Y energía >= 20 MeV
        df_filtered = df[
            (df['ParticleName'].isin(['proton', 'neutron'])) & 
            (df['DepositedEnergy_MeV'] >= energy_threshold)
        ]
        
        counts.append(len(df_filtered))
    
    return np.array(counts)


# ========== PROCESAMIENTO ==========
print("Calculando conteos de referencia...")
print("  Filtrando: solo proton/neutron con E ≥ 20 MeV")
ref_proton = compute_counts_proton_neutron(REFERENCE_DIR_PROTON, energy_threshold=20)
ref_neutron = compute_counts_proton_neutron(REFERENCE_DIR_NEUTRON, energy_threshold=20)

print("\nCalculando conteos con cobertura...")
print("  Filtrando: solo proton/neutron con E ≥ 20 MeV")
covered_proton = compute_counts_proton_neutron(COVERED_DIR_PROTON, energy_threshold=20)
covered_neutron = compute_counts_proton_neutron(COVERED_DIR_NEUTRON, energy_threshold=20)

# Verificar longitudes
n_points = min(len(ref_proton), len(covered_proton), len(ref_neutron), len(covered_neutron))

# Generar logspace
energies_log = np.logspace(np.log10(E_MIN), np.log10(E_MAX), n_points)

# ========== CALCULAR PROPORCIONES ==========
ratio_proton = np.divide(covered_proton[:n_points], ref_proton[:n_points], 
                         out=np.zeros_like(covered_proton[:n_points], dtype=float), 
                         where=ref_proton[:n_points]!=0)

ratio_neutron = np.divide(covered_neutron[:n_points], ref_neutron[:n_points], 
                          out=np.zeros_like(covered_neutron[:n_points], dtype=float), 
                          where=ref_neutron[:n_points]!=0)

# ========== GRÁFICA: Proporción de detección ==========
plt.figure(figsize=(10, 6))

plt.scatter(energies_log, ratio_proton, label="Haz de protones", alpha=0.7, s=50)
plt.scatter(energies_log, ratio_neutron, label="Haz de neutrones", alpha=0.7, s=50)

plt.xscale("log")

plt.xlabel("Energía del haz [MeV]", fontsize=12)
plt.ylabel("Proporción de detección (p/n, E ≥ 20 MeV)", fontsize=12)
plt.title("Efecto de la cobertura: detección de protones y neutrones", fontsize=14)

# Línea horizontal en y=1
plt.axhline(y=1.0, color='gray', linestyle='--', linewidth=1, alpha=0.5, label='Sin atenuación')

plt.grid(True, which="both", ls="--", alpha=0.3)
plt.legend(fontsize=11)

plt.tight_layout()

# Guardar
os.makedirs(OUTPUT_DIR, exist_ok=True)
output_file = os.path.join(OUTPUT_DIR, "plot_attenuation_ratio_proton_neutron_E20MeV.png")
plt.savefig(output_file, dpi=300)
print(f"\n✓ Gráfica guardada: {output_file}")
plt.show()

# ========== ESTADÍSTICAS ==========
print("\n" + "="*60)
print("ESTADÍSTICAS DE ATENUACIÓN (solo p/n con E ≥ 20 MeV)")
print("="*60)
print(f"\nHaz de PROTONES:")
print(f"  Atenuación media: {(1 - ratio_proton.mean())*100:.1f}%")
print(f"  Rango: {ratio_proton.min():.3f} - {ratio_proton.max():.3f}")

print(f"\nHaz de NEUTRONES:")
print(f"  Atenuación media: {(1 - ratio_neutron.mean())*100:.1f}%")
print(f"  Rango: {ratio_neutron.min():.3f} - {ratio_neutron.max():.3f}")

print("\n✅ Procesamiento completado")