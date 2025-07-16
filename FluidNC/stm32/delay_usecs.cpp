// Copyright (c) 2024 - FluidNC Contributors
// Use of this source code is governed by a GPLv3 license that can be found in the LICENSE file.

#include "Driver/delay_usecs.h"
#include "platform.h"

#ifdef STM32

#include <Arduino.h>

// Microsecond delay implementation for STM32
void delay_usecs(uint32_t usecs) {
    delayMicroseconds(usecs);
}

// Millisecond delay implementation for STM32
void delay_ms(uint32_t ms) {
    delay(ms);
}

// High precision delay using system timer
void delay_us_precise(uint32_t usecs) {
    uint32_t start = micros();
    while ((micros() - start) < usecs) {
        // Busy wait for precise timing
    }
}

// Delay with interrupts enabled
void delay_us_interruptible(uint32_t usecs) {
    if (usecs > 1000) {
        // For longer delays, use millisecond delay with interrupts
        delay_ms(usecs / 1000);
        delay_usecs(usecs % 1000);
    } else {
        delay_usecs(usecs);
    }
}

#endif // STM32