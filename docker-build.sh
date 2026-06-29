#!/usr/bin/env bash
# Build Luckfox Pico SDK inside Ubuntu 22.04 (required on macOS).
#
# On macOS, the SDK is synced into a Docker volume (case-sensitive Linux FS).
# Bind-mounting the APFS tree breaks the kernel build: Documentation/kbuild
# collides with Documentation/Kbuild on case-insensitive volumes.
set -euo pipefail

ROOT="$(cd "$(dirname "$0")" && pwd)"
IMAGE="${LUCKFOX_DOCKER_IMAGE:-luckfox-pico-sdk:22.04}"
VOLUME="${LUCKFOX_DOCKER_VOLUME:-luckfox-pico-sdk}"
DOCKERFILE="${ROOT}/Dockerfile"
# Use volume on Darwin by default; set LUCKFOX_BIND_MOUNT=1 to force bind (Linux only)
USE_VOLUME=0
if [[ "$(uname -s)" == "Darwin" && "${LUCKFOX_BIND_MOUNT:-0}" != "1" ]]; then
  USE_VOLUME=1
fi

usage() {
  cat <<'USAGE'
Usage: ./docker-build.sh [options] [build.sh args...]

  Run the Luckfox Pico SDK build in Docker (Ubuntu 22.04).
  On macOS the Rockchip toolchain is Linux-only; use this wrapper.

  macOS: sources are rsynced into a case-sensitive Docker volume (avoids
  APFS case-folding issues in the kernel tree), then output/ is synced back.

Options:
  --build-image   (Re)build the Docker image
  --sync          Force host → volume sync before build (macOS)
  --sync-output   Copy volume output/ → host only (macOS; no build)
  --shell         Interactive bash in the container
  --check         Run ./build.sh check
  -h, --help      Show this help

Anything else is passed to ./build.sh inside the container.

Examples:
  ./docker-build.sh --build-image
  ./docker-build.sh --sync --check
  ./docker-build.sh uboot
  ./docker-build.sh all          # full SDK build
  ./docker-build.sh --shell

Env:
  LUCKFOX_DOCKER_IMAGE    default: luckfox-pico-sdk:22.04
  LUCKFOX_DOCKER_VOLUME   default: luckfox-pico-sdk
  LUCKFOX_BIND_MOUNT=1    force bind-mount (not recommended on macOS)
USAGE
}

need_docker() {
  if ! docker info >/dev/null 2>&1; then
    echo "error: Docker is not running. Start Docker Desktop and retry." >&2
    exit 1
  fi
}

build_image() {
  need_docker
  echo "==> Building image ${IMAGE}"
  docker build -t "${IMAGE}" -f "${DOCKERFILE}" "${ROOT}"
}

ensure_image() {
  need_docker
  if ! docker image inspect "${IMAGE}" >/dev/null 2>&1; then
    build_image
  fi
}

ensure_volume() {
  need_docker
  if ! docker volume inspect "${VOLUME}" >/dev/null 2>&1; then
    echo "==> Creating volume ${VOLUME}"
    docker volume create "${VOLUME}" >/dev/null
  fi
}

# Marker so we know volume has been populated at least once
volume_needs_sync() {
  ! docker run --rm -v "${VOLUME}:/sdk" "${IMAGE}" test -f /sdk/.docker-volume-sync
}

sync_host_to_volume() {
  ensure_image
  ensure_volume
  echo "==> Syncing host → volume ${VOLUME} (this can take a few minutes on first run)"
  # Host APFS is case-insensitive and collapses kernel paths like ipt_ECN.h vs
  # ipt_ecn.h. Copy .git and re-checkout on the case-sensitive volume so both
  # names exist with the correct contents.
  docker run --rm \
    --platform linux/amd64 \
    -v "${ROOT}:/host:ro" \
    -v "${VOLUME}:/sdk" \
    "${IMAGE}" \
    bash -lc '
      set -e
      if ! command -v rsync >/dev/null; then
        apt-get update -qq && apt-get install -y -qq rsync git >/dev/null
      fi
      if ! command -v git >/dev/null; then
        apt-get update -qq && apt-get install -y -qq git >/dev/null
      fi
      # Preserve prior build outputs if present
      if [[ -d /sdk/output ]]; then
        mv /sdk/output /tmp/luckfox-output-preserve
      fi
      if [[ -d /sdk/sysdrv/source/objs_kernel ]]; then
        mv /sdk/sysdrv/source/objs_kernel /tmp/luckfox-objs-preserve 2>/dev/null || true
      fi
      rsync -a --delete \
        --exclude .docker-volume-sync \
        --exclude output/ \
        /host/ /sdk/
      cd /sdk
      # Restore case-sensitive paths from the git index (requires .git)
      if [[ -d .git ]]; then
        git checkout -f HEAD
        # Spot-check a known collision pair
        if [[ -f sysdrv/source/kernel/include/uapi/linux/netfilter_ipv4/ipt_ECN.h \
           && -f sysdrv/source/kernel/include/uapi/linux/netfilter_ipv4/ipt_ecn.h ]]; then
          echo "case-check OK: ipt_ECN.h and ipt_ecn.h both present"
        else
          echo "case-check WARN: ipt_ECN / ipt_ecn not both present after checkout" >&2
          ls -la sysdrv/source/kernel/include/uapi/linux/netfilter_ipv4/ | head
        fi
      else
        echo "warn: no .git in volume; case-sensitive restore skipped" >&2
      fi
      if [[ -d /tmp/luckfox-output-preserve ]]; then
        mkdir -p /sdk/output
        rsync -a /tmp/luckfox-output-preserve/ /sdk/output/
        rm -rf /tmp/luckfox-output-preserve
      fi
      if [[ -d /tmp/luckfox-objs-preserve ]]; then
        mkdir -p /sdk/sysdrv/source
        rsync -a /tmp/luckfox-objs-preserve/ /sdk/sysdrv/source/objs_kernel/
        rm -rf /tmp/luckfox-objs-preserve
      fi
      date -u +%Y-%m-%dT%H:%M:%SZ > /sdk/.docker-volume-sync
    '
  echo "==> Sync host → volume done"
}

sync_volume_output_to_host() {
  ensure_image
  ensure_volume
  echo "==> Syncing volume output/ → host"
  mkdir -p "${ROOT}/output"
  docker run --rm \
    --platform linux/amd64 \
    -v "${ROOT}:/host" \
    -v "${VOLUME}:/sdk:ro" \
    -u "$(id -u):$(id -g)" \
    -e HOME=/tmp \
    "${IMAGE}" \
    bash -lc '
      set -e
      if ! command -v rsync >/dev/null; then
        # read-only sdk; use cp from a writeable path — re-run as root for rsync install
        true
      fi
    '
  # Install rsync as root if needed, then copy as host uid
  docker run --rm \
    --platform linux/amd64 \
    -v "${ROOT}:/host" \
    -v "${VOLUME}:/sdk:ro" \
    "${IMAGE}" \
    bash -lc '
      set -e
      if ! command -v rsync >/dev/null; then
        apt-get update -qq && apt-get install -y -qq rsync >/dev/null
      fi
      mkdir -p /host/output
      rsync -a /sdk/output/ /host/output/
      # Fix ownership to match host user if possible
      chown -R '"$(id -u):$(id -g)"' /host/output || true
    '
  echo "==> Output synced to ${ROOT}/output"
}

run_bind() {
  ensure_image
  local uid gid docker_args
  uid="$(id -u)"
  gid="$(id -g)"
  docker_args=(run --rm)
  if [[ -t 0 && -t 1 ]]; then
    docker_args+=(-it)
  fi
  docker "${docker_args[@]}" \
    --platform linux/amd64 \
    -v "${ROOT}:/sdk" \
    -w /sdk \
    -u "${uid}:${gid}" \
    -e HOME=/tmp \
    -e PATH="/sdk/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin" \
    "${IMAGE}" \
    "$@"
}

run_volume() {
  ensure_image
  ensure_volume
  if volume_needs_sync || [[ "${FORCE_SYNC:-0}" == "1" ]]; then
    sync_host_to_volume
  fi
  local docker_args
  docker_args=(run --rm)
  if [[ -t 0 && -t 1 ]]; then
    docker_args+=(-it)
  fi
  # Root in volume avoids permission fights on volume contents
  docker "${docker_args[@]}" \
    --platform linux/amd64 \
    -v "${VOLUME}:/sdk" \
    -w /sdk \
    -e HOME=/tmp \
    -e PATH="/sdk/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin" \
    "${IMAGE}" \
    "$@"
  sync_volume_output_to_host
}

run_in_container() {
  if [[ "${USE_VOLUME}" -eq 1 ]]; then
    run_volume "$@"
  else
    run_bind "$@"
  fi
}

FORCE_SYNC=0
ARGS=()
while [[ $# -gt 0 ]]; do
  case "$1" in
    -h|--help) usage; exit 0 ;;
    --build-image) build_image; shift; [[ $# -eq 0 ]] && exit 0 ;;
    --sync) FORCE_SYNC=1; shift ;;
    --sync-output)
      if [[ "${USE_VOLUME}" -ne 1 ]]; then
        echo "error: --sync-output is for macOS volume builds (Darwin)." >&2
        echo "On Linux, output/ is already on the host bind-mount." >&2
        exit 1
      fi
      sync_volume_output_to_host
      exit 0
      ;;
    --shell)
      shift
      if [[ "${USE_VOLUME}" -eq 1 ]]; then
        ensure_image; ensure_volume
        if volume_needs_sync || [[ "${FORCE_SYNC}" == "1" ]]; then
          sync_host_to_volume
        fi
        docker_args=(run --rm)
        [[ -t 0 && -t 1 ]] && docker_args+=(-it)
        docker "${docker_args[@]}" --platform linux/amd64 -v "${VOLUME}:/sdk" -w /sdk \
          -e HOME=/tmp \
          -e PATH="/sdk/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin" \
          "${IMAGE}" bash
      else
        run_bind bash
      fi
      exit 0
      ;;
    --check)
      shift
      run_in_container bash -lc './build.sh check'
      exit 0
      ;;
    *) ARGS+=("$1"); shift ;;
  esac
done

if [[ ${#ARGS[@]} -eq 0 ]]; then
  # Full default build
  if [[ ! -e "${ROOT}/.BoardConfig.mk" ]]; then
    echo "No .BoardConfig.mk — run: ./docker-build.sh lunch" >&2
    exit 1
  fi
  echo "Starting full ./build.sh (board: $(readlink "${ROOT}/.BoardConfig.mk" 2>/dev/null || echo configured))"
  run_in_container bash -lc './build.sh'
  exit 0
fi

cmd='./build.sh'
for a in "${ARGS[@]}"; do
  cmd+=" $(printf '%q' "$a")"
done
run_in_container bash -lc "${cmd}"
