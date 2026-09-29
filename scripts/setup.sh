#!/usr/bin/env bash
# Fetch the FreeRTOS kernel (pinned tag) into third_party/.
set -euo pipefail
source "$(dirname "$0")/env.sh"

if [[ -d "$FREERTOS_DIR/.git" ]]; then
    echo "FreeRTOS-Kernel already present at $FREERTOS_DIR"
    exit 0
fi

mkdir -p "$(dirname "$FREERTOS_DIR")"
git clone --depth 1 --branch "$FREERTOS_TAG" \
    https://github.com/FreeRTOS/FreeRTOS-Kernel.git "$FREERTOS_DIR"
echo "FreeRTOS-Kernel $FREERTOS_TAG ready."
