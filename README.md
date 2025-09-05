# NoX SoC C Tests

This repository contains minimal, self-contained C programs to exercise NoX SoC peripherals on a RISC-V RV32I core. Each test produces an ELF, disassemblies, and a simple word-per-line HEX file suitable for backdoor loading by the UVM environment.

## Project Layout
- `c_tests/`: Single-file tests (`uart_test.c`, `timer_test.c`, `watchdog_test.c`) with their own startup (`_start`).
- `include/`: Common headers (`soc_mmap.h`, `uvm_test.h`).
- `link.ld.in`: Linker script template; CMake generates `build/link.ld` using values from `include/soc_mmap.h`.
- `out/`: Build artifacts (created by CMake).

## Requirements
- Run builds inside a container or environment that has `riscv64-unknown-elf-*` tools.
- CMake (host or container).

## Build

Configure you shortcut for RISC-V docker containers
```bash
alias riscv='docker run -it --rm -v ./:/test/ -w /test/ aignacio/riscv bash'
```

Build the sources
```
riscv
mkdir -p build && cmake -S . -B build
cmake --build build -j
```
Artifacts are written to `out/`:
- `<test>.elf` — linked ELF
- `<test>.asm` — disassembly with inline C
- `<test>_raw.asm` — raw disassembly (no inline C)
- `<test>.hex` — one 32-bit word per line (uppercase hex), derived from binary image

## Run Style
Each test, when it completes, writes `P` (pass) or `F` (fail) to a well-known memory location declared in `include/uvm_test.h`. UVM monitors can watch these locations.

## Notes
- Addresses in `include/soc_mmap.h` are placeholders; align them with the actual NoX SoC memory map.
- Linker script generation: CMake reads `CORE_SRAM_MEM` and `CORE_SRAM_SIZE` from `include/soc_mmap.h` and substitutes them into `link.ld.in` to produce `build/link.ld`. Update those defines to change the SRAM origin and length.
 - The `.hex` format is raw words with no addresses/checksums. Each line is one 32-bit word, big-endian text of the little-endian memory content (e.g., bytes `13 05 00 00` -> line `00000513`).
