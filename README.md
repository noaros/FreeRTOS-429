# FreeRTOS-429

Ok I'm speechless. Shame on me for not exploring Claude Code sooner. I asked it to create a simple demo for FreeRTOS that runs on my specific hardware (STM32F429ZI), which is not the hardware supported by one of the standard FreeRTOS demo's. I made sure the board was plugged in, and Claude took care of everything. It build me simple scripts like I asked for (instead of CMake), and deployed the software, which amazingly, works. I have a blinky green light. If I push the blue button I toggle the blue light. This is logged to she serial port, which I can view by running a monitor script Claude made for me.

<img width="754" height="1098" alt="image" src="https://github.com/user-attachments/assets/a37f77c1-abee-4ff1-a217-1388c77b3709" />

It is the agent mechanism as interacting problem solver I find so amazing. Claude looked at what was in my repo, the software I had installed, whether or not it could detect my board. It then used these findings to construct bare metal and linker code. It downloaded the latest FreeRTOS after checking to see what the latest versions were. It even fixed its own compile error!

What's left to do here is to understand everything about how Claude could do this, and of course to examine every aspect of the project itself to understand it, especially as relates to FreeRTOS, and looks for opportunities to simplify.

My AI skepticism appears to have been misplaced. Also, my attempt to manually learn from the FreeRTOS site itself was more difficult and confusing than this! What a cheatsheet...

=== Below is Claude' README for the project ===

Minimal FreeRTOS demo for the **NUCLEO-F429ZI** (STM32F429ZI, Cortex-M4F).
Bare-metal: no HAL, no CubeMX, no Makefile. Everything is driven by shell scripts.

## What it does

| Task     | Behaviour                                                              |
|----------|------------------------------------------------------------------------|
| `blink`  | Toggles the green LED (LD1) every 500 ms                               |
| `button` | Each press of the blue user button (B1) toggles the blue LED (LD2) and queues an event |
| `report` | Prints button events, or uptime and free heap every 2 s, on the ST-LINK virtual COM port (115200 8N1) |

The red LED (LD3) turns on if FreeRTOS detects a stack overflow or a failed allocation.
The CPU runs at 168 MHz from the internal HSI oscillator through the PLL.

## Requirements

- `arm-none-eabi-gcc` on `PATH`
- `STM32_Programmer_CLI` (STM32CubeProgrammer) on `PATH`
- `git` (to fetch the FreeRTOS kernel)

## Usage

```sh
./scripts/setup.sh     # clone FreeRTOS-Kernel V11.3.1 into third_party/ (build.sh does this automatically)
./scripts/build.sh     # compile to build/demo.{elf,hex,bin}
./scripts/flash.sh     # program over SWD via the on-board ST-LINK and reset
./scripts/monitor.sh   # view UART output (Ctrl-C to quit)
./scripts/build.sh clean
```

Environment variable overrides: `CROSS`, `PROGRAMMER`, `SERIAL_PORT` (default `/dev/ttyACM0`), `BAUD`.

## Layout

```
scripts/        env.sh (shared settings), setup.sh, build.sh, flash.sh, monitor.sh
src/main.c      clock, GPIO, UART setup and the three tasks
src/startup.c   vector table and reset handler
src/stm32f429.h minimal register definitions
src/stm32f429zi.ld  linker script
src/FreeRTOSConfig.h
```
