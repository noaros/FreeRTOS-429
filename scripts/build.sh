#!/usr/bin/env bash
# Build the firmware with arm-none-eabi-gcc. Usage: build.sh [clean]
set -euo pipefail
source "$(dirname "$0")/env.sh"

if [[ "${1:-}" == "clean" ]]; then
    rm -rf "$BUILD_DIR"
    echo "Cleaned."
    exit 0
fi

[[ -d "$FREERTOS_DIR" ]] || "$ROOT/scripts/setup.sh"

CC="${CROSS}gcc"
OBJCOPY="${CROSS}objcopy"
SIZE="${CROSS}size"

CPU_FLAGS=(-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard)
CFLAGS=("${CPU_FLAGS[@]}" -std=gnu11 -O2 -g3 -Wall -Wextra
        -ffunction-sections -fdata-sections
        -I"$ROOT/src"
        -I"$FREERTOS_DIR/include"
        -I"$FREERTOS_DIR/portable/GCC/ARM_CM4F")
LDFLAGS=("${CPU_FLAGS[@]}" -T"$ROOT/src/stm32f429zi.ld" -nostartfiles
         --specs=nano.specs --specs=nosys.specs
         -Wl,--gc-sections -Wl,--no-warn-rwx-segments
         -Wl,-Map="$BUILD_DIR/$TARGET.map")

SOURCES=(
    "$ROOT"/src/*.c
    "$FREERTOS_DIR"/tasks.c
    "$FREERTOS_DIR"/queue.c
    "$FREERTOS_DIR"/list.c
    "$FREERTOS_DIR"/timers.c
    "$FREERTOS_DIR"/portable/MemMang/heap_4.c
    "$FREERTOS_DIR"/portable/GCC/ARM_CM4F/port.c
)

mkdir -p "$BUILD_DIR/obj"
OBJECTS=()
for src in "${SOURCES[@]}"; do
    obj="$BUILD_DIR/obj/$(basename "${src%.c}").o"
    echo "  CC  ${src#"$ROOT"/}"
    "$CC" "${CFLAGS[@]}" -c "$src" -o "$obj"
    OBJECTS+=("$obj")
done

echo "  LD  build/$TARGET.elf"
"$CC" "${OBJECTS[@]}" "${LDFLAGS[@]}" -o "$BUILD_DIR/$TARGET.elf"
"$OBJCOPY" -O ihex   "$BUILD_DIR/$TARGET.elf" "$BUILD_DIR/$TARGET.hex"
"$OBJCOPY" -O binary "$BUILD_DIR/$TARGET.elf" "$BUILD_DIR/$TARGET.bin"
"$SIZE" "$BUILD_DIR/$TARGET.elf"
