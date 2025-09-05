// WATCHDOG basic feed/expire observation test for NoX SoC (RV32I)
// - Initializes SP/GP, minimal C runtime
// - Enables watchdog with small period, feeds a few times, then stops feeding
// - Observes STATUS.EXPIRED bit and reports pass

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
    // Enable watchdog with a small abstract period (e.g., 256 cycles)
    REG32(WDT_CTRL) = WDT_CTRL_EN | (128u << 1);

    // Feed a few times and confirm not expired
    for (int i = 0; i < 3; ++i) {
        REG32(WDT_FEED) = WDT_FEED_MAGIC;
        uint32_t st = REG32(WDT_STATUS);
        if (st & WDT_STATUS_EXPIRED) {
            REPORT_FAIL(0x20); // Unexpected early expire
        }
        busy_delay(10000);
    }

    // Stop feeding and observe expire flag
    for (uint32_t t = 0; t < 2000000u; ++t) {
        uint32_t st = REG32(WDT_STATUS);
        if (st & WDT_STATUS_EXPIRED) {
            // Clear W1C and pass
            REG32(WDT_STATUS) = WDT_STATUS_EXPIRED;
            REPORT_PASS();
        }
    }

    REPORT_FAIL(0x21); // Did not see expire
}

