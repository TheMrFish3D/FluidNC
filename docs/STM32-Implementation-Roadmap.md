# STM32 Implementation Roadmap

## Overview

This document outlines the implementation roadmap for STM32 platform support in FluidNC. The STM32 support extends FluidNC's capabilities beyond ESP32 to include a broader range of microcontrollers, offering increased flexibility for different hardware configurations.

## Current Status

### ✅ Completed Milestones

1. **Basic STM32 Platform Integration**
   - ✅ PlatformIO configuration for STM32
   - ✅ Three STM32 build environments:
     - `stm32_noradio` - Basic STM32F103C8 configuration
     - `stm32_basic` - STM32F401CC configuration
     - `stm32_advanced` - STM32F407VG with advanced features
   - ✅ Build system modifications for STM32 support
   - ✅ Offline build capability implementation

2. **Network Infrastructure Improvements**
   - ✅ Firewall-aware CI/CD pipeline
   - ✅ Retry mechanisms for network operations
   - ✅ Offline build script for restricted environments
   - ✅ Enhanced error handling and diagnostics

3. **Documentation Framework**
   - ✅ Comprehensive wiki structure
   - ✅ Development workflow documentation
   - ✅ Hardware validation procedures
   - ✅ Community contribution guidelines

## Phase 1: Core STM32 Support (Q1 2024)

### 1.1 Hardware Abstraction Layer
- [ ] **STM32 HAL Integration**
  - [ ] GPIO abstraction for STM32 pins
  - [ ] SPI/I2C communication layers
  - [ ] Timer and PWM configurations
  - [ ] ADC support for sensor inputs

- [ ] **Memory Management**
  - [ ] Flash memory configuration
  - [ ] SRAM optimization
  - [ ] Configuration storage system

- [ ] **Communication Interfaces**
  - [ ] UART/USB communication
  - [ ] SPI interface for external devices
  - [ ] I2C support for sensors and displays

### 1.2 Motor Control Support
- [ ] **Stepper Motor Control**
  - [ ] Step/direction pin configuration
  - [ ] Microstepping support
  - [ ] Acceleration/deceleration profiles

- [ ] **Trinamic Driver Support**
  - [ ] UART communication with TMC drivers
  - [ ] SPI communication protocols
  - [ ] StallGuard and CoolStep integration

### 1.3 Basic G-code Processing
- [ ] **G-code Parser**
  - [ ] Core G-code commands (G0, G1, G28, etc.)
  - [ ] Coordinate system support
  - [ ] Feed rate and spindle speed control

- [ ] **Motion Planning**
  - [ ] Linear interpolation
  - [ ] Circular interpolation (G02/G03)
  - [ ] Look-ahead planner

## Phase 2: Advanced Features (Q2 2024)

### 2.1 Peripheral Support
- [ ] **Display Integration**
  - [ ] SSD1306 OLED support
  - [ ] LCD display options
  - [ ] Status information display

- [ ] **Input/Output Expansion**
  - [ ] Limit switch support
  - [ ] Probe input handling
  - [ ] Spindle control outputs
  - [ ] Coolant control

### 2.2 Safety Features
- [ ] **Emergency Stop**
  - [ ] Hardware emergency stop
  - [ ] Software emergency stop
  - [ ] Safe state recovery

- [ ] **Limit Switch Handling**
  - [ ] Hard limit detection
  - [ ] Soft limit enforcement
  - [ ] Homing procedures

### 2.3 Configuration System
- [ ] **YAML Configuration**
  - [ ] STM32-specific configuration options
  - [ ] Pin mapping configurations
  - [ ] Motor parameter settings

- [ ] **Runtime Configuration**
  - [ ] Parameter modification
  - [ ] Configuration validation
  - [ ] Factory reset capability

## Phase 3: Optimization and Testing (Q3 2024)

### 3.1 Performance Optimization
- [ ] **Real-time Performance**
  - [ ] Interrupt optimization
  - [ ] DMA utilization
  - [ ] Critical section analysis

- [ ] **Memory Optimization**
  - [ ] Code size reduction
  - [ ] RAM usage optimization
  - [ ] Flash storage efficiency

### 3.2 Hardware Validation
- [ ] **Test Suite Development**
  - [ ] Hardware-in-the-loop testing
  - [ ] Automated validation scripts
  - [ ] Performance benchmarking

- [ ] **Board Support**
  - [ ] Reference board designs
  - [ ] Commercial board support
  - [ ] Custom board guidelines

## Phase 4: Production Ready (Q4 2024)

### 4.1 Documentation and Examples
- [ ] **User Documentation**
  - [ ] Installation guides
  - [ ] Configuration examples
  - [ ] Troubleshooting guides

- [ ] **Developer Documentation**
  - [ ] API reference
  - [ ] Porting guidelines
  - [ ] Custom board development

### 4.2 Community Support
- [ ] **Release Management**
  - [ ] Stable release process
  - [ ] Version compatibility
  - [ ] Migration guidelines

- [ ] **Community Resources**
  - [ ] Example projects
  - [ ] Video tutorials
  - [ ] Forum support

## Technical Specifications

### Supported STM32 Families

| Family | Flash | RAM | Features | Status |
|--------|-------|-----|----------|--------|
| STM32F103 | 64KB | 20KB | Basic CNC control | ✅ Implemented |
| STM32F401 | 256KB | 64KB | Enhanced performance | ✅ Implemented |
| STM32F407 | 512KB | 128KB | Advanced features | ✅ Implemented |
| STM32F411 | 512KB | 128KB | High performance | 🔄 Planned |
| STM32F446 | 512KB | 128KB | Advanced peripherals | 🔄 Planned |
| STM32H743 | 2MB | 1MB | High-end features | 🔄 Future |

### Build Environments

```ini
# Basic STM32 configuration
[env:stm32_noradio]
board = genericSTM32F103C8
build_flags = -DSTM32_PLATFORM -D__FLUIDNC_STM32

# Enhanced STM32 configuration
[env:stm32_basic]
board = genericSTM32F401CC
build_flags = -DSTM32_PLATFORM -D__FLUIDNC_STM32

# Advanced STM32 configuration
[env:stm32_advanced]
board = genericSTM32F407VG
build_flags = -DSTM32_PLATFORM -D__FLUIDNC_STM32 -DSTM32_ADVANCED_FEATURES
```

## Development Workflow

### 1. Local Development Setup
```bash
# Clone repository
git clone https://github.com/bdring/FluidNC.git
cd FluidNC

# Install dependencies
pip install -r requirements.txt

# Build STM32 target
pio run -e stm32_noradio
```

### 2. Offline Development
```bash
# Create offline cache
./build-offline.sh --cache

# Build offline
./build-offline.sh --offline stm32_noradio
```

### 3. Testing and Validation
```bash
# Run unit tests
pio test -e tests

# Hardware validation
./scripts/validate-hardware.sh stm32
```

## Known Limitations

### Current Limitations
1. **No WiFi/Bluetooth Support**: STM32 builds do not include wireless connectivity
2. **Limited Flash Memory**: Some STM32 variants have limited flash for full feature set
3. **Documentation Gaps**: Some STM32-specific features need better documentation

### Future Improvements
1. **Wireless Modules**: External WiFi/Bluetooth module support
2. **Advanced Motion Control**: More sophisticated motion planning algorithms
3. **Industrial Features**: Additional safety and diagnostic features

## Risk Assessment

### Technical Risks
- **Memory Constraints**: Limited flash/RAM on some STM32 variants
- **Performance**: Real-time requirements may be challenging on lower-end STM32
- **Compatibility**: Maintaining compatibility across different STM32 families

### Mitigation Strategies
- **Modular Design**: Optional features can be disabled for memory-constrained targets
- **Performance Testing**: Comprehensive benchmarking and optimization
- **Abstraction Layer**: Hardware abstraction layer for cross-platform compatibility

## Success Metrics

### Quality Metrics
- [ ] 100% of existing ESP32 functionality ported to STM32
- [ ] Build time < 2 minutes for all STM32 targets
- [ ] Memory usage < 80% of available flash/RAM
- [ ] Zero critical bugs in production release

### Performance Metrics
- [ ] G-code execution speed >= 95% of ESP32 performance
- [ ] Real-time constraints met for all motion control functions
- [ ] Boot time < 5 seconds
- [ ] Configuration load time < 1 second

## Community Feedback

We encourage community feedback on this roadmap. Please:

1. **Review the roadmap** and provide feedback via GitHub issues
2. **Suggest additional features** that would benefit your use case
3. **Volunteer for testing** specific STM32 hardware configurations
4. **Contribute code** for features you're interested in

## Contact and Support

For questions about STM32 implementation:

- **GitHub Issues**: Use the STM32 issue template
- **Development Discussion**: FluidNC development forum
- **Hardware Questions**: Hardware validation team

---

**Last Updated**: July 2024  
**Next Review**: August 2024  
**Status**: Active Development