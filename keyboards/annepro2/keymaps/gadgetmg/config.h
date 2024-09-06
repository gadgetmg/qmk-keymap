#pragma once

#include "pin_defs.h"

#define LINE_UART_TX B0  // Master TX, LED RX
#define LINE_UART_RX B1  // Master RX, LED TX

#define LINE_BT_UART_TX A4  // Master TX, BLE RX
#define LINE_BT_UART_RX A5  // Master RX, BLE TX

// SPI configuration
#define SPI_DRIVER SPID1
#define SPI_SCK_PIN A0
#define SPI_MOSI_PIN A1
#define SPI_MISO_PIN A2

// Flash configuration
#define EXTERNAL_FLASH_SPI_SLAVE_SELECT_PIN B6
#define EXTERNAL_FLASH_SPI_CLOCK_DIVISOR 16
#define EXTERNAL_FLASH_PAGE_SIZE 256
#define EXTERNAL_FLASH_SECTOR_SIZE 4096
#define EXTERNAL_FLASH_BLOCK_SIZE 4096
#define EXTERNAL_FLASH_SIZE (256 * 1024) // 2M-bit flash size

/* Increase default debounce */
#define DEBOUNCE 10

#define TAPPING_TERM 175
