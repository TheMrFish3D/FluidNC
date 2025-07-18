// Copyright (c) 2024 - FluidNC Contributors  
// Use of this source code is governed by a GPLv3 license that can be found in the LICENSE file.

#include "Driver/spi.h"
#include "Driver/fluidnc_gpio.h"
#include "platform.h"

#ifdef STM32

#include "src/Config.h"
#include <SPI.h>

static SPIClass* spi_instance = nullptr;
static bool spi_initialized = false;

bool spi_init_bus(pinnum_t sck_pin, pinnum_t miso_pin, pinnum_t mosi_pin, bool dma, int8_t sck_drive_strength, int8_t mosi_drive_strength) {
    if (spi_initialized) {
        return true;
    }
    
    // Initialize SPI with custom pins
    spi_instance = new SPIClass(mosi_pin, miso_pin, sck_pin);
    
    if (!spi_instance) {
        return false;
    }
    
    // Begin SPI
    spi_instance->begin();
    
    // Set default SPI settings
    spi_instance->beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    
    spi_initialized = true;
    
    // Drive strength settings are not applicable for STM32 in the same way as ESP32
    // But we can note them for future reference
    (void)sck_drive_strength;
    (void)mosi_drive_strength;
    (void)dma; // DMA support would need additional implementation
    
    return true;
}

void spi_deinit_bus() {
    if (spi_initialized && spi_instance) {
        spi_instance->end();
        delete spi_instance;
        spi_instance = nullptr;
        spi_initialized = false;
    }
}

// Additional SPI functions that may be needed
bool spi_transfer(uint8_t* tx_data, uint8_t* rx_data, size_t len) {
    if (!spi_initialized || !spi_instance) {
        return false;
    }
    
    for (size_t i = 0; i < len; i++) {
        uint8_t tx_byte = tx_data ? tx_data[i] : 0;
        uint8_t rx_byte = spi_instance->transfer(tx_byte);
        if (rx_data) {
            rx_data[i] = rx_byte;
        }
    }
    
    return true;
}

void spi_set_frequency(uint32_t frequency) {
    if (spi_initialized && spi_instance) {
        spi_instance->setClockDivider(SPI_CLOCK_DIV16); // This would need proper calculation
    }
}

#endif // STM32