// TIMER compare/match test for NoX SoC (RV32I)
// - Initializes SP/GP, minimal C runtime
// - Enables timer, programs CMP0 = COUNT + delta
// - Polls for MATCH, clears status, reports pass

#include <stdint.h>
#include "../include/soc_mmap.h"
#include "../include/uvm_test.h"

extern unsigned long _stack_top;
void main(void);

__attribute__((naked, section(".text.entry"))) void _start(void) {
    __asm__ volatile (
        "la sp, _stack_top\n\t"
        "la gp, __global_pointer$\n\t"
        "jal ra, main\n\t"
        "1: wfi; j 1b\n\t"
    );
}

void main(void) {
    // Enable timer
    REG32(TIMER_CTRL) = TIMER_CTRL_EN;

    // Program compare some cycles ahead
    uint32_t now = REG32(TIMER_COUNT);
    uint32_t delta = 5000u;
    REG32(TIMER_CMP0) = now + delta;

    // Wait for match with timeout
    for (uint32_t t = 0; t < 1000000u; ++t) {
        uint32_t st = REG32(TIMER_STATUS);
        if (st & TIMER_STATUS_MATCH0) {
            // Clear W1C
            REG32(TIMER_STATUS) = TIMER_STATUS_MATCH0;
            REPORT_PASS();
        }
    }

    REPORT_FAIL(0x10); // No match observed
}

