// Copyright (c) 2024 - FluidNC Contributors
// Use of this source code is governed by a GPLv3 license that can be found in the LICENSE file.

#include "Driver/fluidnc_i2c.h"
#include "platform.h"

#ifdef STM32

#include <Wire.h>
#include "src/Config.h"

static TwoWire* i2c_instances[MAX_N_I2C] = { nullptr };
static bool i2c_initialized[MAX_N_I2C] = { false };

bool i2c_master_init(int bus_number, pinnum_t sda_pin, pinnum_t scl_pin, uint32_t frequency) {
    if (bus_number >= MAX_N_I2C) {
        return false;
    }
    
    // Initialize I2C instance based on bus number
    switch (bus_number) {
        case 0:
            i2c_instances[bus_number] = &Wire;
            break;
        case 1:
            i2c_instances[bus_number] = &Wire1;
            break;
        default:
            return false;
    }
    
    if (!i2c_instances[bus_number]) {
        return false;
    }
    
    // Configure I2C with custom pins
    i2c_instances[bus_number]->setSDA(sda_pin);
    i2c_instances[bus_number]->setSCL(scl_pin);
    i2c_instances[bus_number]->setClock(frequency);
    
    // Begin I2C
    i2c_instances[bus_number]->begin();
    
    i2c_initialized[bus_number] = true;
    return true;
}

void i2c_deinit(int bus_number) {
    if (bus_number < MAX_N_I2C && i2c_initialized[bus_number] && i2c_instances[bus_number]) {
        i2c_instances[bus_number]->end();
        i2c_initialized[bus_number] = false;
    }
}

int i2c_write(int bus_number, uint8_t address, const uint8_t* data, size_t count) {
    if (bus_number >= MAX_N_I2C || !i2c_initialized[bus_number] || !i2c_instances[bus_number]) {
        return -1;
    }
    
    i2c_instances[bus_number]->beginTransmission(address);
    
    for (size_t i = 0; i < count; i++) {
        i2c_instances[bus_number]->write(data[i]);
    }
    
    uint8_t result = i2c_instances[bus_number]->endTransmission();
    
    // Convert Arduino Wire result to FluidNC expected result
    return (result == 0) ? 0 : -1;
}

int i2c_read(int bus_number, uint8_t address, uint8_t* data, size_t count) {
    if (bus_number >= MAX_N_I2C || !i2c_initialized[bus_number] || !i2c_instances[bus_number]) {
        return -1;
    }
    
    uint8_t bytes_received = i2c_instances[bus_number]->requestFrom(address, (uint8_t)count);
    
    for (size_t i = 0; i < bytes_received && i < count; i++) {
        data[i] = i2c_instances[bus_number]->read();
    }
    
    return bytes_received;
}

// Write register function
int i2c_write_register(int bus_number, uint8_t address, uint8_t reg, uint8_t value) {
    uint8_t data[2] = { reg, value };
    return i2c_write(bus_number, address, data, 2);
}

// Read register function
int i2c_read_register(int bus_number, uint8_t address, uint8_t reg, uint8_t* value) {
    // Write register address
    if (i2c_write(bus_number, address, &reg, 1) != 0) {
        return -1;
    }
    
    // Read register value
    return i2c_read(bus_number, address, value, 1);
}

#endif // STM32