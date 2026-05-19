#!/usr/bin/env bash
# =============================================================================
# compress_particles.sh
# Organiza ficheros en subcarpetas numeradas (_1 a _9) y las comprime.
# =============================================================================
# Uso:
#   ./compress_particles.sh                  # procesa partículas por defecto
#   ./compress_particles.sh neutron proton   # especifica partículas manualmente
#
# Para añadir muon+ y muon-:
#   ./compress_particles.sh neutron proton "muon+" "muon-"
# =============================================================================

set -euo pipefail

# ── Configuración ─────────────────────────────────────────────────────────────

COMPRESSED_ROOT="compressed_output"
NUM_GROUPS=9

# Partículas a procesar (se pueden sobreescribir con argumentos)
if [ $# -gt 0 ]; then
    PARTICLES=("$@")
else
    PARTICLES=(neutron proton)
    # Descomenta la siguiente línea cuando quieras incluir muones por defecto:
    # PARTICLES=(neutron proton "muon+" "muon-")
fi

# ── Funciones ─────────────────────────────────────────────────────────────────

log()  { echo "[INFO]  $*"; }
warn() { echo "[WARN]  $*" >&2; }
die()  { echo "[ERROR] $*" >&2; exit 1; }

# Convierte "muon+" → "muonplus", "muon-" → "muonminus", resto sin cambios
sanitize_name() {
    local name="$1"
    name="${name//+/plus}"
    name="${name//-/minus}"
    echo "$name"
}

process_particle() {
    local particle="$1"
    local safe_particle
    safe_particle=$(sanitize_name "$particle")

    local input_dir="output_${safe_particle}"
    local output_dir="${COMPRESSED_ROOT}/compressed_${safe_particle}"

    log "════════════════════════════════════════"
    log "Procesando: $particle"
    log "  Entrada : $input_dir"
    log "  Salida  : $output_dir"

    # Validaciones
    [ -d "$input_dir" ] || die "No existe el directorio de entrada: $input_dir"
    mkdir -p "$output_dir"

    # ── Paso 1: crear subcarpetas y mover ficheros ───────────────────────────
    log "Creando subcarpetas y moviendo ficheros..."
    for i in $(seq 1 $NUM_GROUPS); do
        local subdir="${input_dir}/output_${safe_particle}_${i}"
        mkdir -p "$subdir"

        # Busca ficheros (no directorios) que contengan _N en el nombre
        local count=0
        while IFS= read -r -d '' f; do
            mv "$f" "$subdir/"
            (( count++ )) || true
        done < <(find "$input_dir" -maxdepth 1 -type f -name "*_${i}*" -print0)

        if [ "$count" -gt 0 ]; then
            log "  Grupo $i: $count fichero(s) movido(s) → $subdir"
        else
            warn "  Grupo $i: ningún fichero con patrón '*_${i}*' encontrado"
        fi
    done

    # ── Paso 2: comprimir cada subcarpeta ───────────────────────────────────
    log "Comprimiendo subcarpetas..."
    for i in $(seq 1 $NUM_GROUPS); do
        local subdir="${input_dir}/output_${safe_particle}_${i}"
        local tarfile="${output_dir}/o${safe_particle:0:1}${i}.tar.xz"

        if [ -d "$subdir" ]; then
            tar -cJf "$tarfile" "$subdir"
            local size
            size=$(du -sh "$tarfile" | cut -f1)
            log "  ✓ $tarfile ($size)"
        else
            warn "  Subcarpeta no encontrada, saltando: $subdir"
        fi
    done

    log "Completado: $particle"
}

# ── Main ──────────────────────────────────────────────────────────────────────

# Crear directorio raíz de comprimidos
mkdir -p "$COMPRESSED_ROOT"

for particle in "${PARTICLES[@]}"; do
    process_particle "$particle"
done

log "════════════════════════════════════════"
log "Todo completado. Ficheros generados en: $COMPRESSED_ROOT/"
