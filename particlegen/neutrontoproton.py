###############################################################################
# Script para convertir archivos CSV de neutrones a protones, manteniendo el 
# formato y la energía.
###############################################################################

import pandas as pd
from pathlib import Path

input_folder = "./particlegen/input"
output_folder = "./particlegen/input"

csv_files = list(Path(input_folder).glob("*.csv"))

for input_file in csv_files:
    df = pd.read_csv(input_file)
    df['particle'] = df['particle'].replace('neutron', 'proton')
    df['particle_id'] = df['particle_id'].replace(0, 1)
    
    output_filename = input_file.name.replace('_neutron', '_proton')
    output_path = Path(output_folder) / output_filename
    df.to_csv(output_path, index=False)
    
    print(f"✓ {input_file.name} → {output_filename}")

print(f"\n {len(csv_files)} archivos procesados")