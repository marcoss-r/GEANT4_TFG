"""
clean_csv_headers.py
--------------------
Limpia CSVs de GEANT4 que tienen headers duplicados por errores de ejecución.
Conserva solo la última cabecera y elimina cualquier repetición anterior,
junto con todas las filas que haya antes del último header duplicado.

Uso:
    python clean_csv_headers.py                   # limpia PROTON_DIR y NEUTRON_DIR in-place
    python clean_csv_headers.py --dry-run         # solo muestra qué haría, sin modificar nada
    python clean_csv_headers.py --out clean_csvs  # guarda los limpios en otra carpeta
"""

import os
import glob
import shutil
import argparse

# ── Configuración ────────────────────────────────────────────────────────────
PROTON_DIR  = "output_proton"
NEUTRON_DIR = "output_neutron"

# Primera columna del header; se usa para detectar filas de cabecera duplicadas
HEADER_FIRST_COLUMN = "EventID"
# ─────────────────────────────────────────────────────────────────────────────


def find_last_header_line(lines: list[str], header_first_col: str) -> int:
    """
    Devuelve el índice de la ÚLTIMA línea que empieza por `header_first_col`.
    Si solo hay una (la original), devuelve 0.
    """
    last_idx = 0
    for i, line in enumerate(lines):
        if line.startswith(header_first_col):
            last_idx = i
    return last_idx


def clean_file(filepath: str, out_path: str, dry_run: bool, header_first_col: str) -> bool:
    """
    Limpia un CSV eliminando todo hasta (e incluyendo) el último header duplicado,
    dejando ese header como la única cabecera al principio del archivo.

    Devuelve True si el archivo necesitaba limpieza, False si ya estaba limpio.
    """
    with open(filepath, "r", encoding="utf-8") as f:
        lines = f.readlines()

    last_header_idx = find_last_header_line(lines, header_first_col)

    if last_header_idx == 0:
        # No hay duplicados; archivo limpio
        return False

    # Reconstruir: header + datos posteriores al último header duplicado
    clean_lines = [lines[last_header_idx]] + lines[last_header_idx + 1:]

    if not dry_run:
        os.makedirs(os.path.dirname(out_path) if os.path.dirname(out_path) else ".", exist_ok=True)
        with open(out_path, "w", encoding="utf-8") as f:
            f.writelines(clean_lines)

    removed = last_header_idx  # número de líneas eliminadas (0..last_header_idx-1 + duplicados intermedios)
    print(f"  [LIMPIADO] {os.path.basename(filepath)} "
          f"— eliminadas {removed} línea(s) antes del último header (línea {last_header_idx + 1})")
    return True


def process_directory(directory: str, out_dir: str | None, dry_run: bool, header_first_col: str):
    files = glob.glob(os.path.join(directory, "*.csv"))
    if not files:
        print(f"  ⚠ No se encontraron CSVs en '{directory}'")
        return

    cleaned = 0
    for filepath in sorted(files):
        # Destino: misma carpeta (in-place) o carpeta de salida alternativa
        if out_dir:
            rel = os.path.relpath(filepath, directory)
            out_path = os.path.join(out_dir, os.path.basename(directory), rel)
        else:
            out_path = filepath  # sobreescribe el original

        if clean_file(filepath, out_path, dry_run, header_first_col):
            cleaned += 1
        else:
            # Si usamos carpeta de salida, copiar igualmente los archivos ya limpios
            if out_dir and not dry_run:
                os.makedirs(os.path.dirname(out_path), exist_ok=True)
                shutil.copy2(filepath, out_path)

    total = len(files)
    status = "[DRY-RUN] " if dry_run else ""
    print(f"  {status}Resultado: {cleaned}/{total} archivo(s) con headers duplicados en '{directory}'")


# ── Main ─────────────────────────────────────────────────────────────────────
def main():
    parser = argparse.ArgumentParser(description="Limpia headers duplicados en CSVs de GEANT4.")
    parser.add_argument("--dry-run", action="store_true",
                        help="Muestra qué se haría sin modificar ningún archivo.")
    parser.add_argument("--out", metavar="CARPETA", default=None,
                        help="Carpeta de destino para los CSVs limpios (por defecto: in-place).")
    parser.add_argument("--dirs", nargs="+", default=[PROTON_DIR, NEUTRON_DIR],
                        metavar="DIR",
                        help="Directorios a procesar (por defecto: output_proton output_neutron).")
    args = parser.parse_args()

    if args.dry_run:
        print("MODO DRY-RUN: no se modificará ningún archivo.\n")

    for directory in args.dirs:
        print(f"\n Procesando: {directory}")
        process_directory(directory, args.out, args.dry_run, HEADER_FIRST_COLUMN)

    print("\nLimpieza completada.")


if __name__ == "__main__":
    main()