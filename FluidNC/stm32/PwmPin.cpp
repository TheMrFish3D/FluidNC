// Copyright (c) 2024 - FluidNC Contributors
// Use of this source code is governed by a GPLv3 license that can be found in the LICENSE file.

#include "Driver/PwmPin.h"
#include "platform.h"

#ifdef STM32

#include <Arduino.h>

class STM32PwmPin : public PwmPin {
private:
    pinnum_t _pin;
    uint32_t _frequency;
    uint32_t _period;
    uint32_t _duty_cycle;
    bool _initialized;

public:
    STM32PwmPin(pinnum_t pin, uint32_t frequency) : 
        _pin(pin), _frequency(frequency), _period(1000), _duty_cycle(0), _initialized(false) {}

    void init() override {
        if (!_initialized) {
            // Configure pin for PWM output
            pinMode(_pin, OUTPUT);
            
            // Calculate period based on frequency
            // For STM32, we'll use analogWrite resolution (typically 0-255 or 0-1023)
            _period = 255; // 8-bit resolution
            
            // Set initial duty cycle to 0
            analogWrite(_pin, 0);
            
            _initialized = true;
        }
    }

    void setDuty(uint32_t duty) override {
        if (_initialized) {
            _duty_cycle = duty;
            
            // Convert duty cycle to analogWrite value
            uint32_t analog_value = (duty * 255) / _period;
            if (analog_value > 255) {
                analog_value = 255;
            }
            
            analogWrite(_pin, analog_value);
        }
    }

    uint32_t getDuty() override {
        return _duty_cycle;
    }

    void setFrequency(uint32_t frequency) override {
        _frequency = frequency;
        // STM32 Arduino framework doesn't easily allow changing PWM frequency
        // This would require low-level timer configuration
    }

    uint32_t getFrequency() override {
        return _frequency;
    }

    uint32_t period() override {
        return _period;
    }

    void setPeriod(uint32_t period) override {
        _period = period;
    }

    ~STM32PwmPin() {
        if (_initialized) {
            analogWrite(_pin, 0);
        }
    }
};

// Factory function to create STM32 PWM pin
PwmPin* pwm_pin_create(pinnum_t pin, uint32_t frequency) {
    return new STM32PwmPin(pin, frequency);
}

// PWM channel management for STM32
static STM32PwmPin* pwm_channels[16] = { nullptr }; // STM32 typically has multiple PWM channels

PwmPin* pwm_pin_init(pinnum_t pin, uint32_t frequency) {
    // Find available PWM channel
    for (int i = 0; i < 16; i++) {
        if (pwm_channels[i] == nullptr) {
            pwm_channels[i] = new STM32PwmPin(pin, frequency);
            pwm_channels[i]->init();
            return pwm_channels[i];
        }
    }
    
    return nullptr; // No available channels
}

void pwm_pin_deinit(PwmPin* pwm_pin) {
    // Find and remove PWM channel
    for (int i = 0; i < 16; i++) {
        if (pwm_channels[i] == pwm_pin) {
            delete pwm_channels[i];
            pwm_channels[i] = nullptr;
            break;
        }
    }
}

#endif // STM32