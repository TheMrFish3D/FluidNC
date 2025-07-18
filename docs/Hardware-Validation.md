# Hardware Validation

## Overview

This document outlines comprehensive hardware validation procedures for FluidNC, with specific focus on STM32 platform support. Hardware validation ensures that FluidNC operates reliably and safely across different hardware configurations.

## Validation Categories

### 1. Basic Hardware Functions
### 2. CNC-Specific Functions  
### 3. Performance Testing
### 4. Safety Testing
### 5. Compatibility Testing

---

## 1. Basic Hardware Functions

### GPIO Operations

#### Input/Output Testing
```cpp
// Test script for GPIO validation
void validate_gpio() {
    // Test digital output
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_2, 1);
    delay(100);
    gpio_set_level(GPIO_NUM_2, 0);
    
    // Test digital input
    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_INPUT);
    gpio_set_pull_mode(GPIO_NUM_4, GPIO_PULLUP_ONLY);
    int level = gpio_get_level(GPIO_NUM_4);
    
    // Test analog input (if available)
    #ifdef STM32_PLATFORM
    uint32_t adc_value = adc_read(ADC_CHANNEL_0);
    #endif
}
```

#### Validation Criteria
- [ ] All configured GPIO pins respond correctly
- [ ] Pull-up/pull-down resistors function properly
- [ ] Analog inputs provide stable readings
- [ ] GPIO interrupt handling works correctly

### Timer and PWM Functionality

#### PWM Testing
```cpp
// PWM validation for motor control
void validate_pwm() {
    // Initialize PWM for stepper motor
    pwm_config_t pwm_config;
    pwm_config.frequency = 1000; // 1kHz
    pwm_config.duty_cycle = 50;   // 50%
    
    pwm_init(PWM_CHANNEL_0, &pwm_config);
    pwm_start(PWM_CHANNEL_0);
    
    // Test different duty cycles
    for (int duty = 0; duty <= 100; duty += 10) {
        pwm_set_duty_cycle(PWM_CHANNEL_0, duty);
        delay(100);
    }
    
    pwm_stop(PWM_CHANNEL_0);
}
```

#### Validation Criteria
- [ ] PWM frequency accuracy within 1%
- [ ] Duty cycle accuracy within 1%
- [ ] Stable PWM output under load
- [ ] Multiple PWM channels work simultaneously

### Communication Interfaces

#### UART Testing
```cpp
// UART validation
void validate_uart() {
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    
    uart_init(UART_NUM_1, &uart_config);
    
    // Test data transmission
    char test_data[] = "FluidNC UART Test";
    uart_write(UART_NUM_1, test_data, strlen(test_data));
    
    // Test data reception
    char rx_buffer[100];
    int rx_len = uart_read(UART_NUM_1, rx_buffer, sizeof(rx_buffer));
}
```

#### SPI Testing
```cpp
// SPI validation for TMC drivers
void validate_spi() {
    spi_config_t spi_config = {
        .mode = SPI_MODE_0,
        .clock_speed = 1000000, // 1MHz
        .bit_order = SPI_MSB_FIRST
    };
    
    spi_init(SPI_NUM_1, &spi_config);
    
    // Test SPI communication
    uint8_t tx_data[] = {0x01, 0x02, 0x03};
    uint8_t rx_data[3];
    
    spi_transaction_t transaction = {
        .tx_buffer = tx_data,
        .rx_buffer = rx_data,
        .length = 3
    };
    
    spi_transmit(SPI_NUM_1, &transaction);
}
```

#### I2C Testing
```cpp
// I2C validation for sensors and displays
void validate_i2c() {
    i2c_config_t i2c_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 21,
        .scl_io_num = 22,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000 // 100kHz
    };
    
    i2c_init(I2C_NUM_0, &i2c_config);
    
    // Test I2C device scan
    for (int addr = 0x08; addr < 0x78; addr++) {
        if (i2c_device_present(I2C_NUM_0, addr)) {
            printf("I2C device found at address 0x%02X\n", addr);
        }
    }
}
```

---

## 2. CNC-Specific Functions

### Motor Control Validation

#### Stepper Motor Testing
```cpp
// Stepper motor validation
void validate_stepper_motor() {
    stepper_config_t stepper_config = {
        .step_pin = GPIO_NUM_26,
        .dir_pin = GPIO_NUM_27,
        .enable_pin = GPIO_NUM_25,
        .microsteps = 16,
        .steps_per_mm = 80
    };
    
    stepper_init(STEPPER_X_AXIS, &stepper_config);
    
    // Test basic movement
    stepper_move(STEPPER_X_AXIS, 1000, 1000); // 1000 steps at 1000 steps/sec
    delay(2000);
    
    // Test direction change
    stepper_move(STEPPER_X_AXIS, -1000, 1000);
    delay(2000);
    
    // Test acceleration/deceleration
    stepper_move_with_accel(STEPPER_X_AXIS, 5000, 2000, 500); // 5000 steps, 2000 max speed, 500 accel
}
```

#### Validation Criteria
- [ ] Accurate step count and direction
- [ ] Proper acceleration/deceleration profiles
- [ ] Microstep configuration working
- [ ] Enable/disable functionality
- [ ] Consistent timing and speed

### Limit Switch Testing

#### Limit Switch Validation
```cpp
// Limit switch validation
void validate_limit_switches() {
    limit_switch_config_t config = {
        .x_min_pin = GPIO_NUM_14,
        .x_max_pin = GPIO_NUM_15,
        .y_min_pin = GPIO_NUM_16,
        .y_max_pin = GPIO_NUM_17,
        .z_min_pin = GPIO_NUM_18,
        .z_max_pin = GPIO_NUM_19,
        .normally_open = true,
        .pullup_enable = true
    };
    
    limit_switches_init(&config);
    
    // Test limit switch reading
    limit_switch_state_t state = limit_switches_read();
    
    printf("Limit switch states:\n");
    printf("X-min: %d, X-max: %d\n", state.x_min, state.x_max);
    printf("Y-min: %d, Y-max: %d\n", state.y_min, state.y_max);
    printf("Z-min: %d, Z-max: %d\n", state.z_min, state.z_max);
    
    // Test interrupt handling
    limit_switches_enable_interrupts();
}
```

#### Validation Criteria
- [ ] Reliable switch state detection
- [ ] Proper debouncing
- [ ] Interrupt handling works correctly
- [ ] Emergency stop functionality
- [ ] Homing sequence operation

### Probe Input Testing

#### Probe Validation
```cpp
// Probe input validation
void validate_probe() {
    probe_config_t probe_config = {
        .pin = GPIO_NUM_32,
        .normally_open = true,
        .pullup_enable = true
    };
    
    probe_init(&probe_config);
    
    // Test probe reading
    bool probe_state = probe_read();
    printf("Probe state: %s\n", probe_state ? "Triggered" : "Open");
    
    // Test probe interrupt
    probe_enable_interrupt();
}
```

---

## 3. Performance Testing

### Real-time Performance

#### Step Pulse Timing
```cpp
// Step pulse timing validation
void validate_step_timing() {
    const int test_frequency = 10000; // 10kHz
    const int test_duration = 1000;   // 1 second
    
    // Configure high-resolution timer
    timer_config_t timer_config = {
        .frequency = test_frequency,
        .auto_reload = true,
        .callback = step_pulse_callback
    };
    
    timer_init(TIMER_0, &timer_config);
    
    // Measure timing accuracy
    uint32_t start_time = get_microseconds();
    timer_start(TIMER_0);
    delay(test_duration);
    timer_stop(TIMER_0);
    uint32_t end_time = get_microseconds();
    
    uint32_t actual_duration = end_time - start_time;
    float accuracy = (float)actual_duration / (test_duration * 1000) * 100;
    
    printf("Timing accuracy: %.2f%%\n", accuracy);
}
```

#### Memory Usage Testing
```cpp
// Memory usage validation
void validate_memory_usage() {
    // Measure free heap at startup
    size_t heap_start = get_free_heap_size();
    
    // Run typical CNC operations
    run_gcode_test_sequence();
    
    // Measure heap after operations
    size_t heap_end = get_free_heap_size();
    
    printf("Heap usage: %zu bytes\n", heap_start - heap_end);
    
    // Check for memory leaks
    if (heap_end < heap_start * 0.9) {
        printf("WARNING: Potential memory leak detected\n");
    }
}
```

### Performance Benchmarks

#### G-code Processing Speed
```cpp
// G-code processing benchmark
void benchmark_gcode_processing() {
    const char* test_gcode[] = {
        "G0 X10 Y10",
        "G1 X20 Y20 F1000",
        "G2 X30 Y30 I5 J5",
        "G3 X40 Y40 I-5 J-5"
    };
    
    uint32_t start_time = get_microseconds();
    
    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < 4; j++) {
            gcode_process_line(test_gcode[j]);
        }
    }
    
    uint32_t end_time = get_microseconds();
    uint32_t processing_time = end_time - start_time;
    
    printf("G-code processing rate: %.2f lines/sec\n", 
           4000.0 / (processing_time / 1000000.0));
}
```

---

## 4. Safety Testing

### Emergency Stop Testing

#### E-stop Validation
```cpp
// Emergency stop validation
void validate_emergency_stop() {
    // Configure emergency stop
    estop_config_t estop_config = {
        .pin = GPIO_NUM_0,
        .normally_closed = true,
        .pullup_enable = true
    };
    
    estop_init(&estop_config);
    
    // Test emergency stop detection
    bool estop_state = estop_read();
    printf("E-stop state: %s\n", estop_state ? "Active" : "Inactive");
    
    // Test emergency stop response
    if (estop_state) {
        // Should immediately stop all motors
        assert(motors_are_stopped());
        assert(spindle_is_stopped());
        printf("Emergency stop response: PASS\n");
    }
}
```

### Fault Handling Testing

#### Fault Response Validation
```cpp
// Fault handling validation
void validate_fault_handling() {
    // Simulate various fault conditions
    fault_condition_t faults[] = {
        FAULT_LIMIT_SWITCH,
        FAULT_PROBE_FAIL,
        FAULT_MOTOR_STALL,
        FAULT_POWER_SUPPLY,
        FAULT_COMMUNICATION
    };
    
    for (int i = 0; i < sizeof(faults) / sizeof(faults[0]); i++) {
        printf("Testing fault: %d\n", faults[i]);
        
        // Trigger fault
        trigger_fault(faults[i]);
        
        // Verify system response
        assert(system_is_in_fault_state());
        assert(motors_are_stopped());
        
        // Test fault recovery
        clear_fault(faults[i]);
        assert(!system_is_in_fault_state());
        
        printf("Fault %d handling: PASS\n", faults[i]);
    }
}
```

---

## 5. Compatibility Testing

### STM32 Variant Testing

#### Multi-board Validation
```cpp
// STM32 board compatibility testing
void validate_stm32_compatibility() {
    board_info_t board_info = get_board_info();
    
    printf("Board: %s\n", board_info.name);
    printf("Flash: %zu KB\n", board_info.flash_size / 1024);
    printf("RAM: %zu KB\n", board_info.ram_size / 1024);
    printf("CPU Frequency: %u MHz\n", board_info.cpu_freq / 1000000);
    
    // Test board-specific features
    if (board_info.has_usb) {
        validate_usb_communication();
    }
    
    if (board_info.has_dac) {
        validate_dac_output();
    }
    
    if (board_info.has_can) {
        validate_can_communication();
    }
}
```

### Configuration Testing

#### YAML Configuration Validation
```cpp
// Configuration validation
void validate_configuration() {
    // Test various configuration files
    const char* config_files[] = {
        "config_basic.yaml",
        "config_advanced.yaml",
        "config_minimal.yaml"
    };
    
    for (int i = 0; i < 3; i++) {
        printf("Testing configuration: %s\n", config_files[i]);
        
        // Load configuration
        config_result_t result = config_load(config_files[i]);
        
        assert(result.success);
        assert(result.motor_count > 0);
        assert(result.axis_count > 0);
        
        // Validate configuration values
        validate_motor_config(result.motors);
        validate_axis_config(result.axes);
        validate_io_config(result.io);
        
        printf("Configuration %s: PASS\n", config_files[i]);
    }
}
```

---

## Validation Procedures

### Automated Test Suite

#### Test Runner Script
```bash
#!/bin/bash
# FluidNC Hardware Validation Script

echo "Starting FluidNC Hardware Validation..."

# Basic hardware tests
echo "Running basic hardware tests..."
./test_gpio
./test_timers
./test_uart
./test_spi
./test_i2c

# CNC-specific tests
echo "Running CNC-specific tests..."
./test_motors
./test_limits
./test_probe

# Performance tests
echo "Running performance tests..."
./test_timing
./test_memory
./test_gcode_performance

# Safety tests
echo "Running safety tests..."
./test_estop
./test_fault_handling

# Compatibility tests
echo "Running compatibility tests..."
./test_stm32_variants
./test_configurations

echo "Hardware validation complete!"
```

### Manual Validation Checklist

#### Pre-validation Setup
- [ ] Hardware properly connected and powered
- [ ] All safety devices (E-stop, limits) installed
- [ ] Appropriate configuration loaded
- [ ] Test equipment calibrated

#### Basic Function Tests
- [ ] GPIO input/output operations
- [ ] Timer and PWM functionality
- [ ] UART communication
- [ ] SPI communication (if used)
- [ ] I2C communication (if used)

#### CNC Function Tests
- [ ] Stepper motor operation
- [ ] Limit switch functionality
- [ ] Probe input operation
- [ ] Spindle control (if applicable)
- [ ] Coolant control (if applicable)

#### Safety Tests
- [ ] Emergency stop response
- [ ] Limit switch protection
- [ ] Fault detection and handling
- [ ] Safe state on power-up

#### Performance Tests
- [ ] Step pulse timing accuracy
- [ ] G-code processing speed
- [ ] Memory usage within limits
- [ ] Real-time performance maintained

### Validation Reports

#### Test Report Template
```
FluidNC Hardware Validation Report
==================================

Test Date: [DATE]
Hardware: [HARDWARE_DESCRIPTION]
Firmware Version: [VERSION]
Configuration: [CONFIG_FILE]

Test Results:
-------------
Basic Hardware Tests: [PASS/FAIL]
CNC Function Tests: [PASS/FAIL]
Safety Tests: [PASS/FAIL]
Performance Tests: [PASS/FAIL]

Detailed Results:
-----------------
[DETAILED_TEST_RESULTS]

Issues Found:
-------------
[LIST_OF_ISSUES]

Recommendations:
----------------
[RECOMMENDATIONS]

Validated by: [NAME]
Date: [DATE]
```

---

## Quality Assurance

### Test Coverage Requirements
- **Basic Hardware**: 100% of configured peripherals
- **CNC Functions**: 100% of enabled features
- **Safety Systems**: 100% of safety-critical functions
- **Performance**: All real-time constraints validated

### Acceptance Criteria
- All safety tests must pass
- Performance must meet specifications
- Memory usage within acceptable limits
- No critical or high-severity bugs

### Continuous Validation
- Automated tests run on every build
- Hardware validation on new releases
- Regression testing on configuration changes
- Performance monitoring in production

---

**Last Updated**: July 2024  
**Next Review**: August 2024