// UART smoke test for NoX SoC (RV32I)
// - Initializes SP/GP, minimal C runtime
// - Enables UART and transmits a short string
// - Reports pass/fail via UVM handshake addresses

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
    //uart_enable();
    //busy_delay(1000);
    //uart_puts("Hello, NoX!\n");
    //REPORT_PASS();
    __asm__ volatile (
        "1: wfi; j 1b\n\t"
    );
} //stuck @ WFI

