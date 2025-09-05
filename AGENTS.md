# Repository Guidelines

This repo hosts minimal, self-contained RV32I C tests for the NoX SoC. Each test is a single C file (with its own startup) that exercises one peripheral and reports pass/fail to UVM via a shared memory-mapped status address.

## Project Structure & Module Organization
- `c_tests/`: Single-file tests (`uart_test.c`, `timer_test.c`, `watchdog_test.c`).
- `include/`: Common headers: `soc_mmap.h` (SoC MMIO map), `uvm_test.h` (UVM handshake).
- `link.ld`: RV32I linker script (single 10 KiB `SRAM`).
- `out/`: Build artifacts (`.elf`, `.asm`, `_raw.asm`, `.hex`).

## Build, Test, and Development Commands
- Configure: `cmake -S . -B build`
- Build all: `cmake --build build -j`
- Toolchain is dockerized; CMake automatically prefixes with `docker run -it --rm -v ./:/test/ -w /test/ aignacio/riscv` and uses `riscv64-unknown-elf-*` to emit `.elf`, `.dis`, `.hex` into `out/`.

## Coding Style & Naming Conventions
- Language: C (C11 subset), RV32I only (`-march=rv32i -mabi=ilp32`).
- Indentation: 4 spaces; 100-col soft limit.
- Files: one test per file: `c_tests/<periph>_test.c`.
- Functions: `snake_case`; macros: `UPPER_SNAKE_CASE`.
- No libc; no dynamic memory; use `volatile` MMIO helpers from `soc_mmap.h`.

## Testing Guidelines
- Framework: none; tests are self-checking and must call `REPORT_PASS()` or `REPORT_FAIL(code)` from `uvm_test.h`.
- Disassembly: each build emits `<name>.asm` (objdump `-S -l -d -C`) and `<name>_raw.asm` (objdump `-d -C`).
- Size: all code+data must fit in 10 KiB SRAM.

## Commit & Pull Request Guidelines
- Commits: imperative mood, concise scope prefix, e.g. `c_tests: add uart smoke`.
- Include rationale and relevant artifact names when touching tests or `link.ld`.
- PRs: describe purpose, affected peripherals, and expected UVM observation (e.g., writes to `UVM_STATUS_ADDR`). Link issues when applicable.

## Security & Configuration Tips
- Verify and, if needed, update addresses in `include/soc_mmap.h` to match the current NoX SoC map before running in sim.
- Do not enable extensions beyond RV32I; keep flags minimal to ensure portability across verification environments.
