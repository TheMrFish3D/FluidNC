// Copyright (c) 2024 - FluidNC Contributors
// Use of this source code is governed by a GPLv3 license that can be found in the LICENSE file.

#include "platform.h"

#ifdef STM32

#include <Arduino.h>
#include "src/Pins/PinCapabilities.h"

// STM32 GPIO capabilities implementation

PinCapabilities stm32_get_gpio_capabilities(pinnum_t pin) {
    PinCapabilities caps;
    
    // All STM32 GPIO pins support basic input/output
    caps.set(PinCapabilities::Input);
    caps.set(PinCapabilities::Output);
    caps.set(PinCapabilities::PullUp);
    caps.set(PinCapabilities::PullDown);
    
    // Most STM32 pins support PWM through timers
    caps.set(PinCapabilities::PWM);
    
    // ADC capabilities (most GPIO pins on STM32 support ADC)
    caps.set(PinCapabilities::ADC);
    
    // DAC capabilities (only specific pins)
    // For STM32F4xx, typically PA4 and PA5
    if (pin == PA4 || pin == PA5) {
        caps.set(PinCapabilities::DAC);
    }
    
    // UART capabilities (specific pins)
    switch (pin) {
        case PA9:  // UART1 TX
        case PA10: // UART1 RX  
        case PA2:  // UART2 TX
        case PA3:  // UART2 RX
        case PB10: // UART3 TX
        case PB11: // UART3 RX
            caps.set(PinCapabilities::UART);
            break;
    }
    
    // SPI capabilities (specific pins)
    switch (pin) {
        case PA5:  // SPI1 SCK
        case PA6:  // SPI1 MISO
        case PA7:  // SPI1 MOSI
        case PB13: // SPI2 SCK
        case PB14: // SPI2 MISO
        case PB15: // SPI2 MOSI
            caps.set(PinCapabilities::SPI);
            break;
    }
    
    // I2C capabilities (specific pins)
    switch (pin) {
        case PB6:  // I2C1 SCL
        case PB7:  // I2C1 SDA
        case PB8:  // I2C1 SCL (alternate)
        case PB9:  // I2C1 SDA (alternate)
        case PB10: // I2C2 SCL
        case PB11: // I2C2 SDA
            caps.set(PinCapabilities::I2C);
            break;
    }
    
    return caps;
}

// Pin detail information for STM32
struct STM32PinInfo {
    const char* name;
    uint8_t port;
    uint8_t pin;
    bool has_adc;
    bool has_dac;
    uint8_t adc_channel;
    uint8_t dac_channel;
};

// STM32F446RE pin information (BigTreeTech Octopus uses this MCU)
static const STM32PinInfo stm32_pin_info[] = {
    // Add pin definitions here for specific STM32 variant
    // This would be expanded based on the specific STM32 chip used
};

const STM32PinInfo* stm32_get_pin_info(pinnum_t pin) {
    // Return pin information for given pin
    // This would need to be implemented based on the specific STM32 variant
    return nullptr;
}

#endif // STM32