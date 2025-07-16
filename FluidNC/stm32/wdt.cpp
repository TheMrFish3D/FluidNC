// Copyright (c) 2024 - FluidNC Contributors
// Use of this source code is governed by a GPLv3 license that can be found in the LICENSE file.

#include "platform.h"

#ifdef STM32

#include <Arduino.h>

// STM32 watchdog implementation
// This would typically use the Independent Watchdog (IWDG) peripheral

static bool watchdog_enabled = false;
static uint32_t watchdog_timeout = 0;

void watchdog_init(uint32_t timeout_ms) {
    watchdog_timeout = timeout_ms;
    
    // Initialize STM32 IWDG
    // This would require STM32 HAL calls
    // For now, we just track the state
    watchdog_enabled = true;
}

void watchdog_feed() {
    if (watchdog_enabled) {
        // Feed the watchdog timer
        // This would require STM32 HAL calls to reload the IWDG counter
    }
}

void watchdog_disable() {
    // Note: Once enabled, the STM32 IWDG cannot be disabled
    // until the next system reset
    watchdog_enabled = false;
}

bool watchdog_is_enabled() {
    return watchdog_enabled;
}

uint32_t watchdog_get_timeout() {
    return watchdog_timeout;
}

#endif // STM32