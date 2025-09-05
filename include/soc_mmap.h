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
#define UART_BASE        0x10010000u
#define UART_TXDATA      (UART_BASE + 0x00)
#define UART_RXDATA      (UART_BASE + 0x04)
#define UART_STATUS      (UART_BASE + 0x08)
#define UART_CTRL        (UART_BASE + 0x0C)

#define UART_STATUS_TX_READY   (1u << 0)
#define UART_STATUS_RX_AVAIL   (1u << 1)
#define UART_CTRL_EN           (1u << 0)

// TIMER (free-running counter + compare)
#define TIMER_BASE       0x10020000u
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

// Small delay to pace MMIO writes when needed.
static inline void busy_delay(volatile uint32_t loops) {
    while (loops--) {
        __asm__ volatile ("nop");
    }
}

#endif // SOC_MMAP_H
