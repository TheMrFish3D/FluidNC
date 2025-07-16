# STM32 Support for FluidNC

This document outlines the STM32 platform support implementation for FluidNC, specifically targeting BigTreeTech Octopus boards.

## Overview

The STM32 implementation provides a complete hardware abstraction layer for STM32 microcontrollers, allowing FluidNC to run on STM32-based CNC controller boards.

## Supported Hardware

### Primary Target: BigTreeTech Octopus v1.1
- **MCU**: STM32F446RE
- **Flash**: 512KB
- **RAM**: 128KB
- **GPIO**: 114 pins
- **Timers**: 14 timers
- **UART**: 4 UARTs
- **SPI**: 4 SPI interfaces
- **I2C**: 3 I2C interfaces
- **ADC**: 3 ADCs with 16 channels
- **DAC**: 2 DAC channels

## Platform Implementation

### Driver Layer
The STM32 implementation provides complete drivers for:

- **GPIO**: Digital I/O with interrupt support
- **UART**: Serial communication with configurable parameters
- **SPI**: High-speed serial communication
- **I2C**: Two-wire interface for sensors and peripherals
- **PWM**: Timer-based PWM generation
- **ADC**: Analog-to-digital conversion
- **DAC**: Digital-to-analog conversion (on supported pins)
- **Timers**: Precision step timing and general-purpose timers

### Key Features

1. **Pin Compatibility**: Maintains compatibility with FluidNC pin naming
2. **Configuration**: Uses standard FluidNC YAML configuration
3. **Stepping**: Supports precision step generation using STM32 timers
4. **Interrupts**: Hardware interrupt support for limit switches and probes
5. **Communication**: Multiple UART interfaces for TMC drivers and host communication

## Pin Mapping

### BigTreeTech Octopus Pin Assignments

#### Stepper Motors
- **X-axis**: Step=PF14, Dir=PF13, Enable=PF15
- **Y-axis**: Step=PF12, Dir=PF11, Enable=PF15
- **Z-axis**: Step=PF9, Dir=PC4, Enable=PF15
- **E0-axis**: Step=PC3, Dir=PC1, Enable=PC7
- **E1-axis**: Step=PC6, Dir=PC8, Enable=PC7

#### Endstops
- **X-min**: PG6
- **Y-min**: PG9
- **Z-min**: PG10
- **X-max**: PG5
- **Y-max**: PG4
- **Z-max**: PG11

#### Communication
- **UART1**: TX=PA9, RX=PA10 (Host communication)
- **UART2**: TX=PA2, RX=PA3 (TMC drivers)
- **SPI1**: SCK=PA5, MISO=PA6, MOSI=PA7 (SD card, TMC drivers)
- **I2C1**: SCL=PB6, SDA=PB7 (OLED, sensors)

#### PWM Outputs
- **Spindle**: PE5
- **Laser**: PE6
- **Fan0**: PA8
- **Fan1**: PE5
- **Fan2**: PD12

#### Analog Inputs
- **Thermistor 0**: PF4
- **Thermistor 1**: PF5
- **Thermistor Bed**: PF3

## Configuration

### Example Configuration (stm32_octopus_basic.yaml)

```yaml
name: BigTreeTech Octopus STM32
board: BigTreeTech Octopus v1.1
stepping:
  engine: RMT
  pulse_us: 4
  dir_delay_us: 1

axes:
  x:
    steps_per_mm: 80
    max_rate_mm_per_min: 5000
    motor0:
      standard_stepper:
        direction_pin: PF13:low
        step_pin: PF14
        disable_pin: PF15
      limit_neg_pin: PG6:low
```

## Build Configuration

### PlatformIO Configuration

```ini
[env:stm32_octopus]
platform = ststm32
framework = arduino
board = genericSTM32F446RE
build_flags = -DSTM32 -DBOARD_OCTOPUS
```

## Implementation Status

### ✅ Completed
- Basic GPIO operations
- UART communication
- SPI interface
- I2C interface
- PWM generation
- Step timer implementation
- Platform abstraction layer
- Build system integration

### 🔄 In Progress
- Hardware-specific optimizations
- Interrupt handling
- Advanced timer features
- Flash filesystem support

### 📋 Planned
- TMC driver integration testing
- SD card support
- OLED display support
- Advanced PWM features
- Power management

## Usage

1. **Hardware Setup**: Connect BigTreeTech Octopus board
2. **Configuration**: Use provided YAML configuration as starting point
3. **Build**: Use PlatformIO with `stm32_octopus` environment
4. **Flash**: Upload firmware via USB or ST-Link
5. **Configure**: Upload YAML configuration via web interface

## Limitations

- Some ESP32-specific features may not be available
- Pin capabilities may differ from ESP32 implementation
- Performance characteristics may vary from ESP32

## Contributing

When adding STM32 support:
1. Follow existing ESP32 implementation patterns
2. Maintain compatibility with FluidNC driver interfaces
3. Add proper error handling and validation
4. Update documentation and examples
5. Test with actual hardware when possible

## References

- [STM32F446RE Datasheet](https://www.st.com/resource/en/datasheet/stm32f446re.pdf)
- [BigTreeTech Octopus Documentation](https://github.com/bigtreetech/BIGTREETECH-OCTOPUS-V1.0)
- [FluidNC Documentation](http://wiki.fluidnc.com)