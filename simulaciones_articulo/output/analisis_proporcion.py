import os
import glob
import re
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# ========== CONFIGURACIÓN ==========
REFERENCE_DIR_PROTON  = "output_ref/output_proton"
REFERENCE_DIR_NEUTRON = "output_ref/output_neutron"
REFERENCE_DIR_MUON_P  = "output_ref/output_muon+"
REFERENCE_DIR_MUON_M  = "output_ref/output_muon-"

COVERED_DIR_PROTON    = "output_80cm/output_proton"
COVERED_DIR_NEUTRON   = "output_80cm/output_neutron"
COVERED_DIR_MUON_P    = "output_80cm/output_muon+"
COVERED_DIR_MUON_M    = "output_80cm/output_muon-"

OUTPUT_DIR = "output_10cm/output_plots"

# ── Umbral de energía ─────────────────────────────────────
USE_ENERGY_THRESHOLD = False
ENERGY_THRESHOLD_MEV = 20
# ──────────────────────────────────────────────────────────

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

# =========================================================
# DEFINICIÓN DE HACES
# =========================================================

BEAMS = [
    {
        "label":   "Haz protones",
        "ref_dir": REFERENCE_DIR_PROTON,
        "cov_dir": COVERED_DIR_PROTON,
        "marker":  "o",
    },
    {
        "label":   "Haz neutrones",
        "ref_dir": REFERENCE_DIR_NEUTRON,
        "cov_dir": COVERED_DIR_NEUTRON,
        "marker":  "s",
    },
    {
        "label":   "Haz muones+",
        "ref_dir": REFERENCE_DIR_MUON_P,
        "cov_dir": COVERED_DIR_MUON_P,
        "marker":  "^",
    },
    {
        "label":   "Haz muones−",
        "ref_dir": REFERENCE_DIR_MUON_M,
        "cov_dir": COVERED_DIR_MUON_M,
        "marker":  "v",
    },
]

# =========================================================
# FILTROS DE PARTÍCULAS PARA LAS GRÁFICAS
# =========================================================

PLOT_CONFIGS = [

    {
        "title": "Conteos totales",
        "output": "total_ratio.png",
        "particles": None
    },

    {
        "title": "Protones y neutrones",
        "output": "proton_neutron_ratio.png",
        "particles": ["proton", "neutron"]
    },

    {
        "title": "Muones",
        "output": "muon_ratio.png",
        "particles": ["mu+", "mu-"]
    },

    {
        "title": "Piones",
        "output": "pion_ratio.png",
        "particles": ["pi+", "pi-", "pi0"]
    },
]

# =========================================================
# FUNCIONES
# =========================================================

def extract_energy(filename):

    match = re.search(r'(\d+\.?\d*)MeV', filename)

    if match:
        return float(match.group(1))

    match = re.search(r'(\d+\.?\d*)GeV', filename)

    if match:
        return float(match.group(1)) * 1000

    raise ValueError(f"No se pudo extraer energía de: {filename}")


def compute_counts_by_energy(
    directory,
    particle_filter=None,
    use_threshold=True,
    energy_threshold=20
):

    files = glob.glob(os.path.join(directory, "*.csv"))

    if not files:
        raise ValueError(f"No se encontraron CSV en: {directory}")

    counts = {}

    for filepath in files:

        energy = extract_energy(os.path.basename(filepath))

        df = pd.read_csv(
            filepath,
            names=COLUMN_NAMES,
            skiprows=1
        )

        # Conversión segura
        df['Initial_Energy_MeV'] = pd.to_numeric(
            df['Initial_Energy_MeV'],
            errors='coerce'
        )

        # ==========================================
        # FILTRO DE PARTÍCULAS
        # ==========================================

        if particle_filter is None:
            mask = np.ones(len(df), dtype=bool)
        else:
            mask = df['ParticleName'].isin(particle_filter)

        # ==========================================
        # THRESHOLD
        # ==========================================

        if use_threshold:
            mask &= (
                df['Initial_Energy_MeV'] >= energy_threshold
            )

        counts[energy] = int(mask.sum())

    return counts


def align_and_ratio(ref_counts, cov_counts):

    common_energies = sorted(
        set(ref_counts.keys()) &
        set(cov_counts.keys())
    )

    ref_arr = np.array(
        [ref_counts[e] for e in common_energies],
        dtype=float
    )

    cov_arr = np.array(
        [cov_counts[e] for e in common_energies],
        dtype=float
    )

    ratios = np.divide(
        cov_arr,
        ref_arr,
        out=np.zeros_like(cov_arr),
        where=ref_arr != 0
    )

    return (
        np.array(common_energies),
        ratios,
        ref_arr,
        cov_arr
    )

# =========================================================
# PROCESAMIENTO
# =========================================================

threshold_info = (
    f"E_inicial ≥ {ENERGY_THRESHOLD_MEV} MeV"
    if USE_ENERGY_THRESHOLD
    else "sin umbral de energía"
)

print(f"\nModo: {threshold_info}\n")

os.makedirs(OUTPUT_DIR, exist_ok=True)

# =========================================================
# GENERACIÓN DE GRÁFICAS
# =========================================================

for plot_cfg in PLOT_CONFIGS:

    print("\n" + "=" * 70)
    print(f"Procesando gráfica: {plot_cfg['title']}")
    print("=" * 70)

    plt.figure(figsize=(11, 6))

    for beam in BEAMS:

        print(f"  Procesando {beam['label']}...")

        ref_counts = compute_counts_by_energy(
            beam["ref_dir"],
            particle_filter=plot_cfg["particles"],
            use_threshold=USE_ENERGY_THRESHOLD,
            energy_threshold=ENERGY_THRESHOLD_MEV
        )

        cov_counts = compute_counts_by_energy(
            beam["cov_dir"],
            particle_filter=plot_cfg["particles"],
            use_threshold=USE_ENERGY_THRESHOLD,
            energy_threshold=ENERGY_THRESHOLD_MEV
        )

        energies, ratios, ref_arr, cov_arr = align_and_ratio(
            ref_counts,
            cov_counts
        )

        plt.scatter(
            energies,
            ratios,
            label=beam["label"],
            marker=beam["marker"],
            alpha=0.75,
            s=55
        )

    # =====================================================
    # FORMATO GRÁFICA
    # =====================================================

    plt.xscale("log")

    plt.axhline(
        y=1.0,
        color='gray',
        linestyle='--',
        linewidth=1,
        alpha=0.5,
        label='Sin atenuación'
    )

    plt.xlabel(
        "Energía del haz [MeV]",
        fontsize=12
    )

    ylabel = (
        f"Proporción detección (E_inicial ≥ {ENERGY_THRESHOLD_MEV} MeV)"
        if USE_ENERGY_THRESHOLD
        else "Proporción detección"
    )

    plt.ylabel(ylabel, fontsize=12)

    plt.title(
        plot_cfg["title"],
        fontsize=14
    )

    plt.grid(
        True,
        which="both",
        ls="--",
        alpha=0.3
    )

    plt.legend(fontsize=11)

    plt.tight_layout()

    output_file = os.path.join(
        OUTPUT_DIR,
        plot_cfg["output"]
    )

    plt.savefig(output_file, dpi=300)

    print(f"\n✓ Gráfica guardada: {output_file}")

    plt.show()

print("\n✅ Procesamiento completado")
