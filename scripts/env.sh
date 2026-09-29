# Shared settings, sourced by the other scripts.
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="$ROOT/build"
FREERTOS_DIR="$ROOT/third_party/FreeRTOS-Kernel"
FREERTOS_TAG="V11.3.1"
TARGET="demo"

CROSS="${CROSS:-arm-none-eabi-}"
PROGRAMMER="${PROGRAMMER:-STM32_Programmer_CLI}"
SERIAL_PORT="${SERIAL_PORT:-/dev/ttyACM0}"
BAUD="${BAUD:-115200}"
