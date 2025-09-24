#ifndef SOC_MMAP_H
#define SOC_MMAP_H

#include <stdint.h>

// NOTE: Update these base addresses and bit definitions to match the NoX SoC.
// The current values are placeholders chosen for bring-up in simulation.

// Memory-mapped I/O helpers
#define REG32(addr) (*(volatile uint32_t *)(uintptr_t)(addr))

// Core SRAM base address used by the linker script (ORIGIN)
// Update to match the NoX SoC memory map when needed.
#define CORE_SRAM_MEM 0x00000000u

// Core SRAM size (in bytes) used by the linker script (LENGTH)
// Default is 10 KiB for bring-up; update as needed.
#define CORE_SRAM_SIZE 0x00002800u

// UART (simple TX/RX/STATUS/CTRL model)
#define UART_BASE        0x10000000u
#define UART_TXDATA      (UART_BASE + 0x00)
#define UART_RXDATA      (UART_BASE + 0x04)
#define UART_STATUS      (UART_BASE + 0x08)
#define UART_CTRL        (UART_BASE + 0x0C)

#define UART_STATUS_TX_READY   (1u << 0)
#define UART_STATUS_RX_AVAIL   (1u << 1)
#define UART_CTRL_EN           (1u << 0)

// TIMER (free-running counter + compare)
#define TIMER_BASE       0xF0000000u
#define TIMER_COUNT      (TIMER_BASE + 0x00) // ro
#define TIMER_CMP0       (TIMER_BASE + 0x04) // rw
#define TIMER_CTRL       (TIMER_BASE + 0x08) // rw
#define TIMER_STATUS     (TIMER_BASE + 0x0C) // rw (w1c)

#define TIMER_CTRL_EN          (1u << 0)
#define TIMER_STATUS_MATCH0    (1u << 0)

// WATCHDOG (down-counter + feed)
#define WDT_BASE         0x10030000u
#define WDT_CTRL         (WDT_BASE + 0x00) // [0]=EN, [15:1]=period (abstract)
#define WDT_FEED         (WDT_BASE + 0x04) // write magic to feed
#define WDT_STATUS       (WDT_BASE + 0x08) // [0]=expired (w1c)

#define WDT_CTRL_EN            (1u << 0)
#define WDT_FEED_MAGIC         0xACEDFACEu
#define WDT_STATUS_EXPIRED     (1u << 0)


#define GPIO_BASE_ADDR 0x20000000

// Register offsets based on the provided JSON description
// Note: These offsets are calculated based on the register descriptions
// and may need to be adjusted based on the final memory map.
#define GPIO_INFO_REG               (GPIO_BASE_ADDR + 0x000)
#define GPIO_CFG_REG                (GPIO_BASE_ADDR + 0x004)
#define GPIO_MODE_REG_BASE          (GPIO_BASE_ADDR + 0x008) // Compact multireg
#define GPIO_EN_REG_BASE            (GPIO_BASE_ADDR + 0x080) // Compact multireg
#define GPIO_IN_REG_BASE            (GPIO_BASE_ADDR + 0x100) // Compact multireg
#define GPIO_OUT_REG_BASE           (GPIO_BASE_ADDR + 0x180) // Compact multireg
#define GPIO_SET_REG_BASE           (GPIO_BASE_ADDR + 0x200) // Compact multireg
#define GPIO_CLEAR_REG_BASE         (GPIO_BASE_ADDR + 0x280) // Compact multireg
#define GPIO_TOGGLE_REG_BASE        (GPIO_BASE_ADDR + 0x300) // Compact multireg
#define INTRPT_RISE_EN_REG_BASE     (GPIO_BASE_ADDR + 0x380) // Compact multireg
#define INTRPT_FALL_EN_REG_BASE     (GPIO_BASE_ADDR + 0x400) // Compact multireg
#define INTRPT_LVL_HIGH_EN_REG_BASE (GPIO_BASE_ADDR + 0x480) // Compact multireg
#define INTRPT_LVL_LOW_EN_REG_BASE  (GPIO_BASE_ADDR + 0x500) // Compact multireg
#define INTRPT_STATUS_REG_BASE      (GPIO_BASE_ADDR + 0x580) // Compact multireg


// Helper macro for writing to memory-mapped registers
//#define REG32(addr) (*(volatile uint32_t *)(addr))

// Number of GPIOs, assuming GPIOCount parameter is 8
#define GPIO_COUNT 8
volatile uint32_t *gpio_out = (uint32_t *)(GPIO_OUT_REG_BASE);
volatile uint32_t *gpio_en =  (uint32_t *)(GPIO_EN_REG_BASE);
volatile uint32_t *gpio_in =  (uint32_t *)(GPIO_IN_REG_BASE);


// Small delay to pace MMIO writes when needed.
static inline void busy_delay(volatile uint32_t loops) {
    while (loops--) {
        __asm__ volatile ("nop");
    }
}

#endif // SOC_MMAP_H
