# Platform Support

## Overview

FluidNC supports multiple hardware platforms to accommodate different use cases, performance requirements, and cost constraints. This document provides detailed information about supported platforms and their capabilities.

## Supported Platforms

### ESP32 Platform

#### ESP32 (Original)
- **Chip**: ESP32-D0WDQ6
- **Flash**: 4MB (typically)
- **RAM**: 520KB SRAM
- **CPU**: Dual-core 240MHz
- **Features**: WiFi, Bluetooth, extensive peripheral support

**Build Environments**:
- `noradio`: Basic ESP32 without wireless features
- `wifi`: ESP32 with WiFi support
- `bt`: ESP32 with Bluetooth support

#### ESP32-S3
- **Chip**: ESP32-S3
- **Flash**: 8MB (typically)
- **RAM**: 512KB SRAM
- **CPU**: Dual-core 240MHz
- **Features**: WiFi, Bluetooth LE, USB OTG, advanced peripherals

**Build Environments**:
- `noradio_s3`: ESP32-S3 without wireless features
- `wifi_s3`: ESP32-S3 with WiFi support
- `bt_s3`: ESP32-S3 with Bluetooth support

### STM32 Platform

#### STM32F103C8 (stm32_noradio)
- **Chip**: STM32F103C8T6
- **Flash**: 64KB
- **RAM**: 20KB
- **CPU**: 72MHz ARM Cortex-M3
- **Features**: Basic peripheral set, cost-effective

**Capabilities**:
- Basic CNC control
- Limited memory for advanced features
- Excellent for simple 3-axis machines
- Low cost solution

#### STM32F401CC (stm32_basic)
- **Chip**: STM32F401CCU6
- **Flash**: 256KB
- **RAM**: 64KB
- **CPU**: 84MHz ARM Cortex-M4 with FPU
- **Features**: Enhanced peripherals, floating-point unit

**Capabilities**:
- Enhanced CNC control
- Sufficient memory for most features
- Good for 4-5 axis machines
- Balanced performance and cost

#### STM32F407VG (stm32_advanced)
- **Chip**: STM32F407VGT6
- **Flash**: 1MB
- **RAM**: 192KB
- **CPU**: 168MHz ARM Cortex-M4 with FPU
- **Features**: Advanced peripherals, high performance

**Capabilities**:
- Advanced CNC control
- Extensive memory for all features
- Support for complex configurations
- High-performance applications

## Platform Comparison

### Performance Comparison

| Platform | CPU Speed | Flash | RAM | Wireless | Real-time Performance | Cost |
|----------|-----------|--------|-----|----------|----------------------|------|
| ESP32 | 240MHz | 4MB | 520KB | WiFi/BT | Good | Medium |
| ESP32-S3 | 240MHz | 8MB | 512KB | WiFi/BT LE | Good | Medium |
| STM32F103C8 | 72MHz | 64KB | 20KB | None | Excellent | Low |
| STM32F401CC | 84MHz | 256KB | 64KB | None | Excellent | Low |
| STM32F407VG | 168MHz | 1MB | 192KB | None | Excellent | Medium |

### Feature Support Matrix

| Feature | ESP32 | ESP32-S3 | STM32F103C8 | STM32F401CC | STM32F407VG |
|---------|--------|-----------|-------------|-------------|-------------|
| Basic CNC Control | ✅ | ✅ | ✅ | ✅ | ✅ |
| Multi-axis (>3) | ✅ | ✅ | ⚠️ | ✅ | ✅ |
| WiFi | ✅ | ✅ | ❌ | ❌ | ❌ |
| Bluetooth | ✅ | ✅ | ❌ | ❌ | ❌ |
| Web Interface | ✅ | ✅ | ❌ | ❌ | ❌ |
| SD Card | ✅ | ✅ | ⚠️ | ✅ | ✅ |
| Display Support | ✅ | ✅ | ⚠️ | ✅ | ✅ |
| Advanced Features | ✅ | ✅ | ❌ | ⚠️ | ✅ |

**Legend**: ✅ Full Support, ⚠️ Limited Support, ❌ Not Supported

## Platform-Specific Configuration

### ESP32 Configuration

#### Basic ESP32 Setup
```ini
[env:wifi]
platform = https://github.com/platformio/platform-espressif32.git
framework = arduino
board = esp32dev
platform_packages = 
    platformio/framework-arduinoespressif32@^3.20016.0

build_flags = 
    -DCORE_DEBUG_LEVEL=0
    -D_GLIBCXX_HAVE_DIRENT_H
    -D__FLUIDNC

lib_deps = 
    TMCStepper@>=0.7.0,<1.0.0
    thingpulse/ESP8266 and ESP32 OLED driver for SSD1306 displays@4.4.1
    arduinoWebSockets=https://github.com/MitchBradley/arduinoWebSockets#canSend
    WiFi=https://github.com/MitchBradley/WiFi#canWrite
```

#### ESP32-S3 Configuration
```ini
[env:wifi_s3]
platform = https://github.com/platformio/platform-espressif32.git
framework = arduino
board = esp32-s3-devkitc-1
platform_packages = 
    platformio/framework-arduinoespressif32@^3.20016.0

build_flags = 
    -DCORE_DEBUG_LEVEL=0
    -D_GLIBCXX_HAVE_DIRENT_H
    -D__FLUIDNC
    -DESP32_S3

lib_deps = 
    TMCStepper@>=0.7.0,<1.0.0
    thingpulse/ESP8266 and ESP32 OLED driver for SSD1306 displays@4.4.1
    arduinoWebSockets=https://github.com/MitchBradley/arduinoWebSockets#canSend
    WiFi=https://github.com/MitchBradley/WiFi#canWrite
```

### STM32 Configuration

#### Basic STM32 Setup
```ini
[common_stm32]
platform = ststm32
framework = arduino
lib_deps = 
    TMCStepper@>=0.7.0,<1.0.0
    thingpulse/ESP8266 and ESP32 OLED driver for SSD1306 displays@4.4.1

build_flags = 
    -DCORE_DEBUG_LEVEL=0
    -DSTM32_PLATFORM
    -D__FLUIDNC_STM32

build_src_filter = 
    +<*.h> +<*.s> +<*.S> +<*.cpp> +<*.c> +<src/>
    +<stm32>
    +<stdfs>
    -<src/WebUI>
    -<src/BTConfig.cpp>
    -<esp32>

upload_protocol = stlink
debug_tool = stlink
monitor_speed = 115200
```

#### STM32F103C8 Configuration
```ini
[env:stm32_noradio]
extends = common_stm32
board = genericSTM32F103C8
build_flags = 
    ${common_stm32.build_flags}
    -DSTM32F103C8
    -DFLASH_SIZE=64KB
    -DRAM_SIZE=20KB
```

#### STM32F401CC Configuration
```ini
[env:stm32_basic]
extends = common_stm32
board = genericSTM32F401CC
build_flags = 
    ${common_stm32.build_flags}
    -DSTM32F401CC
    -DFLASH_SIZE=256KB
    -DRAM_SIZE=64KB
    -DHAS_FPU
```

#### STM32F407VG Configuration
```ini
[env:stm32_advanced]
extends = common_stm32
board = genericSTM32F407VG
build_flags = 
    ${common_stm32.build_flags}
    -DSTM32F407VG
    -DFLASH_SIZE=1MB
    -DRAM_SIZE=192KB
    -DHAS_FPU
    -DSTM32_ADVANCED_FEATURES
```

## Hardware Abstraction Layer

### GPIO Abstraction
```cpp
// Platform-agnostic GPIO interface
class GPIO {
public:
    virtual void pinMode(uint8_t pin, uint8_t mode) = 0;
    virtual void digitalWrite(uint8_t pin, uint8_t value) = 0;
    virtual int digitalRead(uint8_t pin) = 0;
    virtual int analogRead(uint8_t pin) = 0;
    virtual void analogWrite(uint8_t pin, int value) = 0;
};

// ESP32 implementation
class ESP32_GPIO : public GPIO {
    // ESP32-specific implementation
};

// STM32 implementation
class STM32_GPIO : public GPIO {
    // STM32-specific implementation
};
```

### Timer Abstraction
```cpp
// Platform-agnostic timer interface
class Timer {
public:
    virtual void init(uint32_t frequency) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void setCallback(void (*callback)()) = 0;
};

// Platform-specific implementations
#ifdef ESP32_PLATFORM
class ESP32_Timer : public Timer { /* implementation */ };
#endif

#ifdef STM32_PLATFORM
class STM32_Timer : public Timer { /* implementation */ };
#endif
```

## Build System Integration

### Multi-platform Builds
```bash
# Build all ESP32 variants
pio run -e noradio -e wifi -e bt

# Build all ESP32-S3 variants
pio run -e noradio_s3 -e wifi_s3 -e bt_s3

# Build all STM32 variants
pio run -e stm32_noradio -e stm32_basic -e stm32_advanced

# Build all platforms
pio run
```

### Offline Build Support
```bash
# Cache dependencies for all platforms
./build-offline.sh --cache

# Build specific platform offline
./build-offline.sh --offline wifi
./build-offline.sh --offline stm32_basic

# Build all platforms offline
./build-offline.sh --offline all
```

## Memory Management

### Memory Layout

#### ESP32 Memory Layout
```
0x40000000 - 0x40400000: Internal ROM (4MB)
0x3F400000 - 0x3F800000: External Flash (4MB)
0x3FFE0000 - 0x3FFFFFFF: Internal SRAM (520KB)
0x3F800000 - 0x3FC00000: External SRAM (4MB, optional)
```

#### STM32 Memory Layout
```
# STM32F103C8
0x08000000 - 0x08010000: Flash (64KB)
0x20000000 - 0x20005000: SRAM (20KB)

# STM32F401CC
0x08000000 - 0x08040000: Flash (256KB)
0x20000000 - 0x20010000: SRAM (64KB)

# STM32F407VG
0x08000000 - 0x08100000: Flash (1MB)
0x20000000 - 0x20020000: SRAM (128KB)
0x10000000 - 0x10010000: CCM SRAM (64KB)
```

### Memory Optimization

#### Flash Usage Optimization
```cpp
// Use PROGMEM for constant data (ESP32)
#ifdef ESP32_PLATFORM
const char config_template[] PROGMEM = "yaml configuration...";
#endif

// Use const data in flash (STM32)
#ifdef STM32_PLATFORM
const char config_template[] = "yaml configuration...";
#endif
```

#### RAM Usage Optimization
```cpp
// Use static allocation where possible
static char g_buffer[1024];

// Use stack allocation for temporary data
void process_data() {
    char local_buffer[256];
    // Process data in local buffer
}
```

## Performance Characteristics

### Real-time Performance

#### Step Pulse Generation
| Platform | Max Step Rate | Timing Accuracy | Jitter |
|----------|---------------|-----------------|---------|
| ESP32 | 80 kHz | ±2 µs | <1 µs |
| ESP32-S3 | 100 kHz | ±1 µs | <0.5 µs |
| STM32F103C8 | 50 kHz | ±0.5 µs | <0.2 µs |
| STM32F401CC | 80 kHz | ±0.3 µs | <0.1 µs |
| STM32F407VG | 120 kHz | ±0.2 µs | <0.1 µs |

#### G-code Processing
| Platform | Lines/Second | Memory Usage | Latency |
|----------|--------------|--------------|---------|
| ESP32 | 1000 | 50KB | 10ms |
| ESP32-S3 | 1200 | 45KB | 8ms |
| STM32F103C8 | 800 | 15KB | 15ms |
| STM32F401CC | 1000 | 30KB | 12ms |
| STM32F407VG | 1500 | 60KB | 6ms |

## Platform Selection Guide

### Choose ESP32 If:
- **WiFi connectivity** is required
- **Web interface** is needed
- **OTA updates** are desired
- **Flexible configuration** is important
- **Cost** is not the primary concern

### Choose ESP32-S3 If:
- **Latest features** are needed
- **USB connectivity** is required
- **High performance** with wireless is needed
- **Future-proofing** is important

### Choose STM32F103C8 If:
- **Cost** is the primary concern
- **Simple 3-axis** CNC is sufficient
- **Real-time performance** is critical
- **No wireless** connectivity needed

### Choose STM32F401CC If:
- **Balanced performance** and cost
- **4-5 axis** CNC support needed
- **Floating-point** calculations required
- **Good peripheral** set needed

### Choose STM32F407VG If:
- **Maximum performance** is required
- **Complex configurations** are needed
- **Advanced features** are desired
- **Extensive I/O** is required

## Migration Guide

### ESP32 to STM32 Migration

#### Configuration Changes
```yaml
# ESP32 configuration
board: esp32dev
axes:
  x:
    step_pin: gpio.26
    direction_pin: gpio.27

# STM32 configuration
board: stm32f407vg
axes:
  x:
    step_pin: gpio.PB0
    direction_pin: gpio.PB1
```

#### Code Changes
```cpp
// ESP32 code
#include <WiFi.h>
#include <WebServer.h>

// STM32 code
// Remove WiFi and web server includes
// Add STM32-specific includes
#include <stm32f4xx_hal.h>
```

### STM32 Variant Migration

#### Memory Considerations
```cpp
// STM32F103C8 (limited memory)
#define MAX_AXES 3
#define BUFFER_SIZE 512

// STM32F407VG (more memory)
#define MAX_AXES 6
#define BUFFER_SIZE 2048
```

## Troubleshooting

### Common Issues

#### Build Failures
- **Network issues**: Use offline build system
- **Memory issues**: Reduce buffer sizes or features
- **Dependency issues**: Clear cache and rebuild

#### Runtime Issues
- **Performance problems**: Check step rates and acceleration
- **Memory leaks**: Monitor heap usage
- **Timing issues**: Verify real-time constraints

### Platform-Specific Issues

#### ESP32 Issues
- **WiFi connectivity**: Check network configuration
- **Brown-out reset**: Verify power supply
- **Watchdog reset**: Check for infinite loops

#### STM32 Issues
- **Upload failures**: Check ST-Link connection
- **Clock configuration**: Verify HSE/HSI settings
- **Memory overflow**: Reduce code size or increase memory

## Future Platform Support

### Planned Platforms
- **STM32F411**: High-performance Cortex-M4
- **STM32F446**: Advanced connectivity
- **STM32H743**: High-end performance
- **STM32G4**: Mixed-signal capabilities

### Evaluation Criteria
- **Performance**: Real-time capabilities
- **Memory**: Flash and RAM availability
- **Peripherals**: Required I/O capabilities
- **Cost**: Target market pricing
- **Ecosystem**: Development tool support

---

**Last Updated**: July 2024  
**Next Review**: August 2024