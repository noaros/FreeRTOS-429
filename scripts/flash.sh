#!/usr/bin/env bash
# Flash the firmware over the on-board ST-LINK and reset the target.
set -euo pipefail
source "$(dirname "$0")/env.sh"

HEX="$BUILD_DIR/$TARGET.hex"
[[ -f "$HEX" ]] || "$ROOT/scripts/build.sh"

"$PROGRAMMER" -c port=SWD mode=UR -w "$HEX" -v -rst
