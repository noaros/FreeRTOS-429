#!/usr/bin/env bash
# Print the demo's UART output (ST-LINK virtual COM port). Ctrl-C to quit.
set -euo pipefail
source "$(dirname "$0")/env.sh"

stty -F "$SERIAL_PORT" "$BAUD" raw -echo -hupcl
echo "Listening on $SERIAL_PORT @ $BAUD (Ctrl-C to quit)"
exec cat "$SERIAL_PORT"
