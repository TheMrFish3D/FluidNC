// Copyright (c) 2024 - FluidNC Contributors
// Use of this source code is governed by a GPLv3 license that can be found in the LICENSE file.

#include "platform.h"

#ifdef STM32

#include <Arduino.h>
#include "src/Config.h"

// STM32 specific platform initialization
void stm32_platform_init() {
    // Initialize system clocks, peripherals, etc.
    // This would be called during system startup
}

// Get GPIO capabilities for specific STM32 pins
stm32_gpio_capabilities stm32_get_pin_capabilities(pinnum_t pin) {
    stm32_gpio_capabilities caps = {
        .has_input = true,
        .has_output = true,
        .has_pwm = false,
        .has_dac = false,
        .has_adc = false,
        .has_uart = false,
        .has_spi = false,
        .has_i2c = false
    };
    
    // Basic capability assignment - this would need to be expanded
    // based on actual STM32 pin capabilities
    
    // Most GPIO pins support PWM
    caps.has_pwm = true;
    
    // DAC pins (typically PA4, PA5 on STM32F4xx)
    if (pin == PA4 || pin == PA5) {
        caps.has_dac = true;
    }
    
    // ADC pins (most GPIO pins support ADC)
    caps.has_adc = true;
    
    // UART pins would need specific mapping
    // SPI pins would need specific mapping  
    // I2C pins would need specific mapping
    
    return caps;
}

// STM32 specific delay functions
void delay_usecs(uint32_t usecs) {
    delayMicroseconds(usecs);
}

void delay_ms(uint32_t ms) {
    delay(ms);
}

// STM32 specific system functions
void system_restart() {
    NVIC_SystemReset();
}

uint32_t system_get_free_heap() {
    // STM32 doesn't have a direct equivalent to ESP32's heap functions
    // This would need to be implemented using FreeRTOS functions if available
    return 0;
}

// STM32 specific watchdog functions
void watchdog_init(uint32_t timeout_ms) {
    // Initialize STM32 independent watchdog
    // This would need STM32 HAL calls
}

void watchdog_feed() {
    // Feed the watchdog timer
    // This would need STM32 HAL calls
}

void watchdog_disable() {
    // Disable the watchdog timer
    // This would need STM32 HAL calls
}

#endif // STM32