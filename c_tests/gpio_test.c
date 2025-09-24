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


void delay(uint32_t cycles) {
    for (volatile uint32_t i = 0; i < cycles; ++i) {
        // This loop will be optimized away without 'volatile'
    }
}


void wait_debug(){
    delay(50);
}

/**
 * @brief function to test GPIO functionality.
 */
void test_gpio_output(void) {
    // Configure all GPIOs as outputs
    // The GPIO_MODE register is a compact multireg, so we can write to it directly.
    // Each GPIO's mode is 2 bits, so we can fit 16 GPIO modes in one 32-bit register.
    // We will set all to 'OUTPUT_ACTIVE' (value 1).
    // For 8 GPIOs, we need to set the first 16 bits.
    uint32_t mode_value = 0;
    for (int i = 0; i < GPIO_COUNT; i++) {
        mode_value |= (1 << (i * 2)); // Set mode to 1 for each GPIO
    }

    // printf("Configuring GPIOs as outputs..." NEW_LINE);

    // printf("mode value %08x" NEW_LINE, mode_value);

    REG32(GPIO_MODE_REG_BASE) = mode_value;

    // printf("GPIO_MODE_REG_BASE: %08x" NEW_LINE, REG32(GPIO_MODE_REG_BASE));

    // Enable all GPIOs for sampling
    REG32(GPIO_EN_REG_BASE) = 0xFFFFFFFF; // Assuming we want to enable all possible GPIOs
    wait_debug();

    REG32(GPIO_SET_REG_BASE) = 0xFFFFFFFF;
    wait_debug();

    // printf("Enabling GPIOs..." NEW_LINE);

    // Drive a pattern on the GPIO outputs
    //while (1) {
    //    // Set all GPIOs high
    //    //printf("Setting all GPIOs high..." NEW_LINE);
    //    REG32(GPIO_SET_REG_BASE) = 0xFFFFFFFF;
    //    wait_debug();
//
    //    // Clear all GPIOs
    //    //printf("Clearing all GPIOs..." NEW_LINE);
    //    REG32(GPIO_TOGGLE_REG_BASE) = 0xFFFFFFFF;
    //    wait_debug();
//
    //    // Toggle a pattern
    //    //printf("Toggling GPIOs with pattern 0xAAAAAAAA..." NEW_LINE);
    //    REG32(GPIO_TOGGLE_REG_BASE) = 0xAAAAAAAA;
    //    wait_debug();
//
    //    //printf("Toggling GPIOs with pattern 0x55555555..." NEW_LINE);
    //    REG32(GPIO_TOGGLE_REG_BASE) = 0x55555555;
    //    wait_debug();
//
    //    //printf("Toggling GPIOs with pattern 0x55555555..." NEW_LINE);
    //    REG32(GPIO_TOGGLE_REG_BASE) = 0x55555555;
    //    wait_debug();
//
    //}
//
    //return;
}


void main(void) {
    //uart_enable();
    //busy_delay(1000);
    //uart_puts("Hello, NoX!\n");
    //REPORT_PASS();
    test_gpio_output();
    __asm__ volatile (
        "1: wfi; j 1b\n\t"
    );
} //stuck @ WFI
