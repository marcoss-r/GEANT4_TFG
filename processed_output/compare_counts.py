###############################################################################
# Script para comparar los conteos de protones y neutrones con energías 
# superiores a 20 MeV entre dos conjuntos de archivos CSV, donde el primero
# actúa como referencia para calcular una proporción.
#################################################################################


import os
import glob
import re
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# ========== CONFIGURACIÓN ==========
REFERENCE_DIR_PROTON  = "aplicacion5/output_proton"
REFERENCE_DIR_NEUTRON = "aplicacion5/output_neutron"

COVERED_DIR_PROTON    = "aplicacion6/output_proton"
COVERED_DIR_NEUTRON   = "aplicacion6/output_neutron"

OUTPUT_DIR = "aplicacion6/output_plots"

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
    raise ValueError(f"No se pudo extraer energía de: {filename}")


def compute_counts_by_energy(directory, energy_threshold=20):
    """
    Devuelve un dict {energia_MeV: conteo} con solo protones/neutrones
    con energía depositada >= threshold.
    La clave es la energía real extraída del nombre del archivo.
    """
    files = glob.glob(os.path.join(directory, "*.csv"))
    if not files:
        raise ValueError(f"No se encontraron archivos CSV en: {directory}")

    counts = {}
    for filepath in files:
        energy = extract_energy(os.path.basename(filepath))

        df = pd.read_csv(filepath, names=COLUMN_NAMES, skiprows=1)

        # Convertir a numérico por si hay headers duplicados residuales
        df['DepositedEnergy_MeV'] = pd.to_numeric(df['DepositedEnergy_MeV'], errors='coerce')

        df_filtered = df[
            (df['ParticleName'].isin(['proton', 'neutron'])) &
            (df['DepositedEnergy_MeV'] >= energy_threshold)
        ]

        counts[energy] = len(df_filtered)

    return counts


def align_and_ratio(ref_counts: dict, cov_counts: dict):
    """
    Une los dos dicts por energía (solo las energías presentes en AMBOS),
    ordena por energía y calcula la proporción cubierto/referencia.
    Devuelve (energies, ratios, ref_array, cov_array).
    """
    common_energies = sorted(set(ref_counts.keys()) & set(cov_counts.keys()))

    missing_ref = set(cov_counts.keys()) - set(ref_counts.keys())
    missing_cov = set(ref_counts.keys()) - set(cov_counts.keys())
    if missing_ref:
        print(f"  ⚠ Energías en cubierto pero NO en referencia (ignoradas): {sorted(missing_ref)}")
    if missing_cov:
        print(f"  ⚠ Energías en referencia pero NO en cubierto (ignoradas): {sorted(missing_cov)}")

    ref_arr = np.array([ref_counts[e] for e in common_energies], dtype=float)
    cov_arr = np.array([cov_counts[e] for e in common_energies], dtype=float)

    ratios = np.divide(cov_arr, ref_arr,
                       out=np.zeros_like(cov_arr),
                       where=ref_arr != 0)

    return np.array(common_energies), ratios, ref_arr, cov_arr


# ========== PROCESAMIENTO ==========
print("Calculando conteos de referencia (aplicacion5)...")
ref_proton_counts  = compute_counts_by_energy(REFERENCE_DIR_PROTON)
ref_neutron_counts = compute_counts_by_energy(REFERENCE_DIR_NEUTRON)

print("Calculando conteos con cobertura (aplicacion6)...")
cov_proton_counts  = compute_counts_by_energy(COVERED_DIR_PROTON)
cov_neutron_counts = compute_counts_by_energy(COVERED_DIR_NEUTRON)

print("\nAlineando por energía real...")
energies_p, ratio_proton,  ref_p, cov_p = align_and_ratio(ref_proton_counts,  cov_proton_counts)
energies_n, ratio_neutron, ref_n, cov_n = align_and_ratio(ref_neutron_counts, cov_neutron_counts)

# ========== GRÁFICA ==========
plt.figure(figsize=(10, 6))

plt.scatter(energies_p, ratio_proton,  label="Haz de protones",  alpha=0.7, s=50)
plt.scatter(energies_n, ratio_neutron, label="Haz de neutrones", alpha=0.7, s=50)

plt.xscale("log")
plt.axhline(y=1.0, color='gray', linestyle='--', linewidth=1, alpha=0.5, label='Sin atenuación')

plt.xlabel("Energía del haz [MeV]", fontsize=12)
plt.ylabel("Proporción de detección (E ≥ 20 MeV)", fontsize=12)
plt.title("Proporción de conteos debido a la cobertura en función de la energía", fontsize=14)
plt.grid(True, which="both", ls="--", alpha=0.3)
plt.legend(fontsize=11)
plt.tight_layout()

os.makedirs(OUTPUT_DIR, exist_ok=True)
output_file = os.path.join(OUTPUT_DIR, "proportion_attenuation_5_to_6.png")
plt.savefig(output_file, dpi=300)
print(f"\n✓ Gráfica guardada: {output_file}")
plt.show()

# ========== ESTADÍSTICAS ==========
print("\n" + "="*60)
print("ESTADÍSTICAS DE ATENUACIÓN (solo p/n con E ≥ 20 MeV)")
print("="*60)

for label, energies, ratios, ref_arr, cov_arr in [
    ("PROTONES",  energies_p, ratio_proton,  ref_p, cov_p),
    ("NEUTRONES", energies_n, ratio_neutron, ref_n, cov_n),
]:
    print(f"\nHaz de {label}:")
    print(f"  Atenuación media : {(1 - ratios.mean())*100:.1f}%")
    print(f"  Rango proporción : {ratios.min():.4f} – {ratios.max():.4f}")

    # Avisar si hay proporciones anómalas
    anomalas = [(e, r, r_ref, r_cov)
                for e, r, r_ref, r_cov in zip(energies, ratios, ref_arr, cov_arr)
                if r > 2.0]
    if anomalas:
        print(f"  ⚠ Proporciones > 2 detectadas:")
        for e, r, r_ref, r_cov in anomalas:
            print(f"      E={e:.1f} MeV  ratio={r:.4f}  ref={int(r_ref)}  cov={int(r_cov)}")

# ========== TABLA COMPLETA ==========
print("\n" + "="*60)
print("PROPORCIONES POR ENERGÍA")
print("="*60)

for label, energies, ratios, ref_arr, cov_arr in [
    ("PROTONES",  energies_p, ratio_proton,  ref_p, cov_p),
    ("NEUTRONES", energies_n, ratio_neutron, ref_n, cov_n),
]:
    print(f"\nHaz de {label}:")
    print(f"  {'Energía (MeV)':>14}  {'Ref':>6}  {'Cub':>6}  {'Proporción':>10}")
    print(f"  {'-'*14}  {'-'*6}  {'-'*6}  {'-'*10}")
    for e, r, r_ref, r_cov in zip(energies, ratios, ref_arr, cov_arr):
        print(f"  {e:>14.3f}  {int(r_ref):>6}  {int(r_cov):>6}  {r:>10.6f}")

print("\n✅ Procesamiento completado")