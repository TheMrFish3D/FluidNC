// Copyright (c) 2024 - FluidNC Contributors
// Use of this source code is governed by a GPLv3 license that can be found in the LICENSE file.

#include "src/Pin.h"
#include "src/Uart.h"
#include "src/Protocol.h"
#include "Driver/fluidnc_gpio.h"
#include "platform.h"

#ifdef STM32

void IRAM_ATTR gpio_write(pinnum_t pin, int value) {
    digitalWrite(pin, value);
}

int IRAM_ATTR gpio_read(pinnum_t pin) {
    return digitalRead(pin);
}

void gpio_mode(pinnum_t pin, int input, int output, int pullup, int pulldown, int opendrain) {
    // Configure pin mode based on parameters
    if (input && output) {
        // Input/output mode not directly supported, default to output
        pinMode(pin, OUTPUT);
    } else if (input) {
        if (pullup) {
            pinMode(pin, INPUT_PULLUP);
        } else if (pulldown) {
            pinMode(pin, INPUT_PULLDOWN);
        } else {
            pinMode(pin, INPUT);
        }
    } else if (output) {
        if (opendrain) {
            pinMode(pin, OUTPUT_OPEN_DRAIN);
        } else {
            pinMode(pin, OUTPUT);
        }
    }
}

void gpio_drive_strength(pinnum_t pin, int strength) {
    // STM32 doesn't have direct drive strength control like ESP32
    // This is a no-op for STM32
    (void)pin;
    (void)strength;
}

void gpio_set_interrupt_type(pinnum_t pin, int mode) {
    // STM32 interrupt type setting
    // This would need to be implemented based on STM32 HAL
    (void)pin;
    (void)mode;
}

void gpio_add_interrupt(pinnum_t pin, int mode, void (*callback)(void*), void* arg) {
    // STM32 interrupt setup
    // This would need to be implemented based on STM32 HAL
    (void)pin;
    (void)mode;
    (void)callback;
    (void)arg;
}

void gpio_remove_interrupt(pinnum_t pin) {
    // STM32 interrupt removal
    // This would need to be implemented based on STM32 HAL
    (void)pin;
}

void gpio_route(pinnum_t pin, uint32_t signal) {
    // STM32 specific pin routing
    // This would need to be implemented based on STM32 HAL
    (void)pin;
    (void)signal;
}

void gpio_set_event(int gpio_num, void* arg, int invert) {
    // STM32 event setting
    (void)gpio_num;
    (void)arg;
    (void)invert;
}

void gpio_clear_event(int gpio_num) {
    // STM32 event clearing
    (void)gpio_num;
}

void poll_gpios() {
    // STM32 GPIO polling
    // This would handle any pending GPIO events
}

#endif // STM32