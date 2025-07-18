# Build System

## Overview

FluidNC uses PlatformIO as its build system with support for multiple platforms including ESP32 and STM32. This document covers build configuration, offline capabilities, and troubleshooting network issues.

## Build Configuration

### PlatformIO Configuration

The main build configuration is in `platformio.ini`:

```ini
[platformio]
src_dir = FluidNC
include_dir = FluidNC/include
test_dir = FluidNC/tests
data_dir = FluidNC/data
default_envs = wifi
```

### Build Environments

#### ESP32 Environments
- **`noradio`**: Basic ESP32 without wireless features
- **`wifi`**: ESP32 with WiFi support
- **`bt`**: ESP32 with Bluetooth support
- **`noradio_s3`**: ESP32-S3 without wireless features
- **`wifi_s3`**: ESP32-S3 with WiFi support
- **`bt_s3`**: ESP32-S3 with Bluetooth support

#### STM32 Environments
- **`stm32_noradio`**: Basic STM32F103C8 configuration
- **`stm32_basic`**: STM32F401CC configuration
- **`stm32_advanced`**: STM32F407VG with advanced features

#### Test Environments
- **`tests`**: Unit tests with sanitizers (Linux/macOS)
- **`tests_nosan`**: Unit tests without sanitizers (Windows)

## Offline Build System

### Overview

The offline build system addresses network restrictions and firewall issues that prevent PlatformIO from accessing external repositories and registries.

### Offline Build Script

The `build-offline.sh` script provides comprehensive offline build capabilities:

```bash
# Create offline cache
./build-offline.sh --cache

# Build offline
./build-offline.sh --offline wifi

# Build all environments offline
./build-offline.sh --offline all
```

### Script Features

#### Cache Management
- Downloads Python dependencies to local cache
- Pre-installs PlatformIO platforms and tools
- Caches common libraries
- Creates cache manifest for tracking

#### Retry Mechanisms
- Automatic retry for network operations
- Configurable retry attempts and delays
- Graceful fallback for partial failures

#### Verbose Logging
- Detailed build logs
- Network diagnostics
- Error tracking and reporting

## Firewall-Aware CI/CD

### Enhanced GitHub Actions

The `ci-firewall-aware.yml` workflow provides robust CI/CD with network reliability:

```yaml
name: FluidNC CI - Firewall Aware Build
on: [push, pull_request]

jobs:
  build:
    strategy:
      matrix:
        os: [ubuntu-latest, macos-latest, windows-latest]
        pio_env: [noradio, wifi, bt]
```

### Key Features

#### Pre-firewall Setup
- DNS configuration for reliable name resolution
- Proxy configuration when available
- SSL certificate configuration

#### Network Resilience
- Retry mechanisms for downloads
- Timeout configuration
- Alternative installation methods

#### Caching Strategy
- PlatformIO platform caching
- Dependency caching
- Build artifact caching

## Network Troubleshooting

### Common Issues

#### Registry Access Problems
PlatformIO may fail to access:
- `api.registry.platformio.org`
- `download.platformio.org`
- `collector.platformio.org`

#### Symptoms
```
HTTPClientError:
Tool Manager: Installing espressif/toolchain-xtensa-esp32 @ 8.4.0+2021r2-patch5
```

### Diagnostic Tools

#### Network Connectivity Test
```bash
# Test registry access
curl -I https://api.registry.platformio.org/
curl -I https://download.platformio.org/

# DNS resolution test
nslookup api.registry.platformio.org

# Network connectivity
ping 8.8.8.8
```

#### PlatformIO Diagnostics
```bash
# System information
pio system info

# List installed platforms
pio platform list

# List installed libraries
pio pkg list --global

# Check settings
pio settings get
```

### Solutions

#### 1. Offline Build
```bash
# Create offline cache when network is available
./build-offline.sh --cache

# Build offline when network is restricted
./build-offline.sh --offline your-target
```

#### 2. Proxy Configuration
```bash
# Set proxy environment variables
export HTTP_PROXY=http://proxy.example.com:8080
export HTTPS_PROXY=http://proxy.example.com:8080
export NO_PROXY=localhost,127.0.0.1

# Build with proxy
pio run -e wifi
```

#### 3. DNS Configuration
```bash
# Use alternative DNS servers
echo "nameserver 8.8.8.8" | sudo tee -a /etc/resolv.conf
echo "nameserver 1.1.1.1" | sudo tee -a /etc/resolv.conf
```

#### 4. Cache Management
```bash
# Clear PlatformIO cache
pio system prune --force

# Remove and reinstall
rm -rf ~/.platformio
pip install platformio
```

## Build Optimization

### Compile-time Optimizations

#### Build Flags
```ini
[common]
build_flags = 
    -DCORE_DEBUG_LEVEL=0
    -Wno-unused-variable
    -Wno-unused-function
    -O2
    -std=gnu++17
```

#### Platform-specific Optimizations
```ini
[common_esp32]
build_flags = 
    ${common.build_flags}
    -D_GLIBCXX_HAVE_DIRENT_H
    -D__FLUIDNC

[common_stm32]
build_flags = 
    ${common.build_flags}
    -DSTM32_PLATFORM
    -D__FLUIDNC_STM32
```

### Memory Optimization

#### Flash Memory
```ini
# Partition configuration
board_build.partitions = min_littlefs.csv
board_build.filesystem = littlefs
```

#### RAM Optimization
```ini
# PSRAM configuration (ESP32)
board_build.arduino.memory_type = qio_qspi
```

## Advanced Configuration

### Custom Build Scripts

#### Pre-build Scripts
```python
# custom_build.py
Import("env")

def before_build(source, target, env):
    print("Custom pre-build step")
    # Add custom logic here

env.AddPreAction("buildprog", before_build)
```

#### Post-build Scripts
```python
# custom_post_build.py
Import("env")

def after_build(source, target, env):
    print("Custom post-build step")
    # Add custom logic here

env.AddPostAction("buildprog", after_build)
```

### Environment Variables

#### Build Configuration
```bash
# Debug build
export BUILD_TYPE=debug
pio run -e debug

# Release build
export BUILD_TYPE=release
pio run -e wifi
```

#### Network Configuration
```bash
# Timeout settings
export PLATFORMIO_DOWNLOAD_TIMEOUT=300
export PLATFORMIO_INSTALL_TIMEOUT=600

# Offline mode
export PLATFORMIO_OFFLINE_MODE=true
```

## Platform-Specific Configurations

### ESP32 Configuration

#### Framework Version
```ini
[common_esp32_base]
platform = https://github.com/platformio/platform-espressif32.git
framework = arduino
platform_packages = 
    platformio/framework-arduinoespressif32@^3.20016.0
```

#### Board Configuration
```ini
board = esp32dev
board_build.f_flash = 80000000L
board_build.arduino.upstream_packages = no
```

### STM32 Configuration

#### Platform Setup
```ini
[common_stm32]
platform = ststm32
framework = arduino
upload_protocol = stlink
debug_tool = stlink
```

#### Board Variants
```ini
# Basic STM32
[env:stm32_noradio]
board = genericSTM32F103C8

# Enhanced STM32
[env:stm32_basic]
board = genericSTM32F401CC

# Advanced STM32
[env:stm32_advanced]
board = genericSTM32F407VG
```

## Testing Integration

### Unit Testing

#### Test Configuration
```ini
[tests_common]
platform = native
test_framework = googletest
test_build_src = true
build_flags = -std=c++17 -g
```

#### Test Execution
```bash
# Run all tests
pio test

# Run specific test environment
pio test -e tests

# Verbose test output
pio test -e tests -v
```

### Hardware Testing

#### Upload and Monitor
```bash
# Upload to device
pio run -e wifi -t upload

# Monitor serial output
pio device monitor --baud 115200

# Combined upload and monitor
pio run -e wifi -t upload -t monitor
```

## Continuous Integration

### GitHub Actions Integration

#### Build Matrix
```yaml
strategy:
  matrix:
    os: [ubuntu-latest, macos-latest, windows-latest]
    pio_env: [noradio, wifi, bt, stm32_noradio, stm32_basic]
```

#### Caching Strategy
```yaml
- name: Cache PlatformIO
  uses: actions/cache@v4
  with:
    path: |
      ~/.platformio
      ~/.cache/platformio
    key: platformio-${{ runner.os }}-${{ hashFiles('platformio.ini') }}
```

### Local CI Testing

#### Pre-commit Testing
```bash
# Build all targets
pio run

# Run all tests
pio test

# Check code style
clang-format --dry-run --Werror src/**/*.cpp
```

## Performance Monitoring

### Build Time Optimization

#### Parallel Builds
```bash
# Use multiple cores
pio run -e wifi -j 4

# Environment variable
export PLATFORMIO_BUILD_JOBS=4
```

#### Cache Utilization
```bash
# Check cache usage
du -sh ~/.platformio/

# Clean selective cache
pio system prune --dry-run
```

### Memory Usage Analysis

#### Flash Usage
```bash
# Analyze flash usage
pio run -e wifi -v | grep -E "(Flash|RAM)"

# Memory map
pio run -e wifi -t size
```

## Troubleshooting Guide

### Common Build Failures

#### Network Issues
```
Error: Could not install package 'platformio/toolchain-xtensa-esp32 @ 8.4.0+2021r2-patch5'
```

**Solution**: Use offline build or configure proxy

#### Memory Issues
```
Error: region `dram0_0_seg' overflowed by X bytes
```

**Solution**: Optimize code size or use larger memory partition

#### Dependency Issues
```
Error: Could not find the package with 'TMCStepper' requirements
```

**Solution**: Clean cache and reinstall dependencies

### Debug Information

#### Verbose Build
```bash
# Enable verbose output
pio run -e wifi -v

# Very verbose output
pio run -e wifi -vv
```

#### Environment Information
```bash
# Show all environment variables
pio run -e wifi -t envdump

# Show build configuration
pio run -e wifi -t idedata
```

## Best Practices

### Development Workflow
1. **Use offline builds** for reliable development
2. **Cache dependencies** to speed up builds
3. **Test multiple platforms** before committing
4. **Monitor build performance** and optimize as needed

### CI/CD Configuration
1. **Use firewall-aware workflows** for reliability
2. **Implement retry mechanisms** for network operations
3. **Cache build artifacts** to reduce build times
4. **Monitor build metrics** for performance degradation

### Network Resilience
1. **Configure multiple DNS servers** for reliability
2. **Use proxy settings** when available
3. **Implement timeout configurations** for network operations
4. **Have offline fallback options** for critical builds

---

**Last Updated**: July 2024  
**Next Review**: August 2024