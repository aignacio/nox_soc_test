#ifndef UVM_TEST_H
#define UVM_TEST_H

#include <stdint.h>
#include <stddef.h>

// Well-known UVM handshake locations. Adjust as needed for your testbench.
#define UVM_STATUS_ADDR   ((volatile uint32_t *)(uintptr_t)0x20000000u) // write 'P' or 'F'
#define UVM_CODE_ADDR     ((volatile uint32_t *)(uintptr_t)0x20000004u) // optional code

static inline void report_pass(void) {
    *UVM_STATUS_ADDR = (uint32_t)'P';
    for (;;) { __asm__ volatile ("wfi"); } // idle for monitors
}

static inline void report_fail(uint32_t code) {
    *UVM_CODE_ADDR = code;
    *UVM_STATUS_ADDR = (uint32_t)'F';
    for (;;) { __asm__ volatile ("wfi"); }
}

#define REPORT_PASS() do { report_pass(); } while (0)
#define REPORT_FAIL(code) do { report_fail((uint32_t)(code)); } while (0)

#endif // UVM_TEST_H

