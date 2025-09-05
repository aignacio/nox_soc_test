cmake_minimum_required(VERSION 3.15)
project(nox_soc_c_tests C)

# Output directory inside repository root
set(ARTIFACT_DIR ${CMAKE_SOURCE_DIR}/out)
file(MAKE_DIRECTORY ${ARTIFACT_DIR})

# Building directly inside the container; no docker wrapper

set(RV_CC   riscv64-unknown-elf-gcc)
set(RV_OBJD riscv64-unknown-elf-objdump)
set(RV_OBJC riscv64-unknown-elf-objcopy)

set(COMMON_CFLAGS -Os -g3 -gdwarf-4 -ffunction-sections -fdata-sections -nostdlib -nostartfiles -march=rv32i -mabi=ilp32)
set(LD_FLAGS -Wl,--gc-sections -T link.ld)

set(TESTS uart_test timer_test watchdog_test)

foreach(t ${TESTS})
  set(ELF ${ARTIFACT_DIR}/${t}.elf)
  set(ASM ${ARTIFACT_DIR}/${t}.asm)
  set(ASM_RAW ${ARTIFACT_DIR}/${t}_raw.asm)
  set(BIN ${ARTIFACT_DIR}/${t}.bin)
  set(HEX ${ARTIFACT_DIR}/${t}.hex)

  # Absolute paths within current container filesystem
  set(SRC   ${CMAKE_SOURCE_DIR}/c_tests/${t}.c)
  set(INC   ${CMAKE_SOURCE_DIR}/include)
  set(LDS   ${CMAKE_SOURCE_DIR}/link.ld)

  add_custom_command(
    OUTPUT ${ELF}
    COMMAND ${RV_CC} ${COMMON_CFLAGS} ${SRC} -Wl,--gc-sections -T ${LDS} -I ${INC} -o ${ELF}
    DEPENDS ${SRC} ${LDS} ${INC}/soc_mmap.h ${INC}/uvm_test.h
    VERBATIM
    COMMENT "Building ${t}.elf"
  )

  add_custom_command(
    OUTPUT ${ASM}
    COMMAND /bin/sh -lc "${RV_OBJD} -S -l -d -C ${ELF} > ${ASM}"
    DEPENDS ${ELF}
    VERBATIM
    COMMENT "Disassembling with inline C: ${t}.elf"
  )

  add_custom_command(
    OUTPUT ${ASM_RAW}
    COMMAND /bin/sh -lc "${RV_OBJD} -d -C ${ELF} > ${ASM_RAW}"
    DEPENDS ${ELF}
    VERBATIM
    COMMENT "Disassembling raw asm: ${t}.elf"
  )

  add_custom_command(
    OUTPUT ${HEX}
    COMMAND /bin/sh -lc "${RV_OBJC} -O binary --gap-fill 0x00 ${ELF} ${BIN} && bytes=\$(wc -c < ${BIN}) && rem=\$((bytes % 4)) && if [ \$rem -ne 0 ]; then pad=\$((4 - rem)); dd if=/dev/zero bs=1 count=\$pad status=none >> ${BIN}; fi && od -v -An -tx4 -w4 ${BIN} | sed 's/^ *//' | tr '[:lower:]' '[:upper:]' > ${HEX}"
    DEPENDS ${ELF}
    VERBATIM
    COMMENT "Generating ${t}.hex (one 32-bit word per line)"
  )

  # Optional DWARF line table dump aids debugging inline C issues
  add_custom_command(
    OUTPUT ${ARTIFACT_DIR}/${t}.dwarf.txt
    COMMAND /bin/sh -lc "riscv64-unknown-elf-readelf --debug-dump=decodedline ${ELF} > ${ARTIFACT_DIR}/${t}.dwarf.txt || true"
    DEPENDS ${ELF}
    VERBATIM
    COMMENT "Dumping DWARF line table for ${t}.elf"
  )

  add_custom_target(${t}_all ALL DEPENDS ${ELF} ${ASM} ${ASM_RAW} ${HEX} ${ARTIFACT_DIR}/${t}.dwarf.txt)
endforeach()

add_custom_target(all_tests DEPENDS uart_test_all timer_test_all watchdog_test_all)
