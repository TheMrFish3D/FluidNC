// Copyright (c) 2024 - FluidNC Contributors
// Use of this source code is governed by a GPLv3 license that can be found in the LICENSE file.

#include "Driver/StepTimer.h"
#include "platform.h"

#ifdef STM32

#include <Arduino.h>

static uint32_t step_timer_frequency = 0;
static bool (*step_timer_callback)(void) = nullptr;
static bool step_timer_running = false;
static uint32_t step_timer_ticks = 0;

// STM32 timer instance (would need to be configured for specific STM32 variant)
static HardwareTimer* step_timer = nullptr;

// Timer interrupt handler
void stepTimerISR() {
    if (step_timer_callback) {
        bool continue_stepping = step_timer_callback();
        if (!continue_stepping) {
            stepTimerStop();
        }
    }
}

void stepTimerInit(uint32_t frequency, bool (*fn)(void)) {
    step_timer_frequency = frequency;
    step_timer_callback = fn;
    
    // Initialize hardware timer for STM32
    // This would need to be adapted for specific STM32 variants
    #if defined(STM32F4xx) || defined(STM32F7xx) || defined(STM32H7xx)
    // Use TIM2 for step timing (32-bit timer)
    step_timer = new HardwareTimer(TIM2);
    #elif defined(STM32F1xx) || defined(STM32F0xx)
    // Use TIM1 for step timing (16-bit timer)
    step_timer = new HardwareTimer(TIM1);
    #else
    // Default to TIM2
    step_timer = new HardwareTimer(TIM2);
    #endif
    
    if (step_timer) {
        step_timer->setOverflow(frequency, HERTZ_FORMAT);
        step_timer->attachInterrupt(stepTimerISR);
        step_timer->resume();
    }
}

void stepTimerStop() {
    if (step_timer && step_timer_running) {
        step_timer->pause();
        step_timer_running = false;
    }
}

void stepTimerSetTicks(uint32_t ticks) {
    step_timer_ticks = ticks;
    
    if (step_timer && ticks > 0) {
        // Calculate new frequency based on ticks
        uint32_t new_frequency = step_timer_frequency / ticks;
        if (new_frequency > 0) {
            step_timer->setOverflow(new_frequency, HERTZ_FORMAT);
        }
    }
}

void stepTimerStart() {
    if (step_timer && !step_timer_running) {
        step_timer->resume();
        step_timer_running = true;
    }
}

// Additional STM32 specific timer functions
void stepTimerReset() {
    if (step_timer) {
        step_timer->setCount(0);
    }
}

uint32_t stepTimerGetTicks() {
    return step_timer_ticks;
}

uint32_t stepTimerGetFrequency() {
    return step_timer_frequency;
}

bool stepTimerIsRunning() {
    return step_timer_running;
}

#endif // STM32