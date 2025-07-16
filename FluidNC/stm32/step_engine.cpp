// Copyright (c) 2024 - FluidNC Contributors
// Use of this source code is governed by a GPLv3 license that can be found in the LICENSE file.

#include "Driver/step_engine.h"
#include "platform.h"

#ifdef STM32

#include <Arduino.h>
#include "Driver/StepTimer.h"

// STM32 step engine implementation
// This provides the interface for generating step pulses

static bool step_engine_initialized = false;
static bool step_engine_enabled = false;

// Step engine callback function
static bool (*step_engine_callback)(void) = nullptr;

// Initialize the step engine
bool step_engine_init() {
    if (!step_engine_initialized) {
        // Initialize step engine hardware
        step_engine_initialized = true;
        step_engine_enabled = false;
    }
    return step_engine_initialized;
}

// Set the step engine callback
void step_engine_set_callback(bool (*callback)(void)) {
    step_engine_callback = callback;
}

// Enable the step engine
void step_engine_enable() {
    if (step_engine_initialized && !step_engine_enabled) {
        step_engine_enabled = true;
        // Initialize step timer with callback
        if (step_engine_callback) {
            stepTimerInit(1000000, step_engine_callback); // 1MHz base frequency
        }
    }
}

// Disable the step engine
void step_engine_disable() {
    if (step_engine_enabled) {
        stepTimerStop();
        step_engine_enabled = false;
    }
}

// Check if step engine is enabled
bool step_engine_is_enabled() {
    return step_engine_enabled;
}

// Set step timing
void step_engine_set_timing(uint32_t step_ticks) {
    if (step_engine_enabled) {
        stepTimerSetTicks(step_ticks);
    }
}

// Start step generation
void step_engine_start() {
    if (step_engine_enabled) {
        stepTimerStart();
    }
}

// Stop step generation
void step_engine_stop() {
    if (step_engine_enabled) {
        stepTimerStop();
    }
}

// Get current step count (if supported)
uint32_t step_engine_get_steps() {
    // This would need to be implemented based on the specific
    // step counting mechanism used
    return 0;
}

// Reset step count
void step_engine_reset_steps() {
    // Reset step counter if supported
}

#endif // STM32