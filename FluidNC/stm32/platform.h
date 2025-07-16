// Copyright (c) 2024 - FluidNC Contributors
// Use of this source code is governed by a GPLv3 license that can be found in the LICENSE file.

#pragma once

#ifdef STM32
#include <Arduino.h>

// STM32 platform definitions
#define WEAK_LINK __attribute__((weak))
#define IRAM_ATTR // STM32 doesn't use IRAM attribute

// STM32 specific includes
#include "stm32_def.h"

// Platform capabilities
#define PLATFORM_HAS_DAC 1
#define PLATFORM_HAS_PWM 1
#define PLATFORM_HAS_HARDWARE_TIMERS 1

// Pin type definition for STM32
typedef PinName pinnum_t;

// STM32 GPIO capabilities
struct stm32_gpio_capabilities {
    bool has_input;
    bool has_output;
    bool has_pwm;
    bool has_dac;
    bool has_adc;
    bool has_uart;
    bool has_spi;
    bool has_i2c;
};

// Function declarations for STM32 specific functions
void stm32_platform_init();
stm32_gpio_capabilities stm32_get_pin_capabilities(pinnum_t pin);

#endif // STM32