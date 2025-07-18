# Development Workflow

## Overview

This document outlines the development workflow for FluidNC, including both ESP32 and STM32 platform support. It covers setup, development practices, testing procedures, and contribution guidelines.

## Getting Started

### Prerequisites

- **Python 3.7+** with pip
- **Git** for version control
- **PlatformIO** for embedded development
- **Text editor/IDE** (VS Code recommended)

### Initial Setup

1. **Clone the repository**
   ```bash
   git clone https://github.com/bdring/FluidNC.git
   cd FluidNC
   ```

2. **Install dependencies**
   ```bash
   pip install -r requirements.txt
   ```

3. **Verify installation**
   ```bash
   pio --version
   ```

### Development Environment Setup

#### Option 1: Online Development (Default)
```bash
# Standard build
pio run -e wifi

# List available environments
pio run --list-targets
```

#### Option 2: Offline Development (Restricted Networks)
```bash
# Create offline cache
./build-offline.sh --cache

# Build offline
./build-offline.sh --offline wifi
```

## Project Structure

```
FluidNC/
├── FluidNC/                  # Main source code
│   ├── src/                  # Core application code
│   ├── esp32/                # ESP32-specific code
│   ├── stm32/                # STM32-specific code (future)
│   ├── include/              # Header files
│   └── data/                 # Configuration files
├── docs/                     # Documentation
├── example_configs/          # Example configurations
├── .github/                  # GitHub Actions workflows
├── platformio.ini            # PlatformIO configuration
├── build-offline.sh          # Offline build script
└── requirements.txt          # Python dependencies
```

## Build Targets

### ESP32 Targets
- **`noradio`**: Basic ESP32 without wireless features
- **`wifi`**: ESP32 with WiFi support
- **`bt`**: ESP32 with Bluetooth support
- **`noradio_s3`**: ESP32-S3 without wireless features
- **`wifi_s3`**: ESP32-S3 with WiFi support
- **`bt_s3`**: ESP32-S3 with Bluetooth support

### STM32 Targets
- **`stm32_noradio`**: Basic STM32F103C8 configuration
- **`stm32_basic`**: STM32F401CC configuration
- **`stm32_advanced`**: STM32F407VG with advanced features

### Test Targets
- **`tests`**: Unit tests (Linux/macOS)
- **`tests_nosan`**: Unit tests without sanitizers (Windows)

## Development Workflow

### 1. Feature Development

#### Create Feature Branch
```bash
git checkout -b feature/your-feature-name
```

#### Development Cycle
1. **Write code** following coding standards
2. **Build frequently** to catch issues early
3. **Test locally** before committing
4. **Commit with descriptive messages**

```bash
# Build specific target
pio run -e wifi

# Build all targets
pio run

# Clean build
pio run -t clean
```

#### Code Quality
- Follow existing code style
- Use meaningful variable names
- Add comments for complex logic
- Keep functions small and focused

### 2. Testing

#### Unit Tests
```bash
# Run all tests
pio test

# Run specific test environment
pio test -e tests

# Verbose test output
pio test -v
```

#### Hardware Testing
```bash
# Upload to device
pio run -e wifi -t upload

# Monitor serial output
pio device monitor

# Upload and monitor
pio run -e wifi -t upload -t monitor
```

### 3. Platform-Specific Development

#### ESP32 Development
```bash
# WiFi build
pio run -e wifi

# Bluetooth build
pio run -e bt

# ESP32-S3 build
pio run -e wifi_s3
```

#### STM32 Development
```bash
# Basic STM32 build
pio run -e stm32_noradio

# Advanced STM32 build
pio run -e stm32_advanced

# Upload to STM32 (requires ST-Link)
pio run -e stm32_noradio -t upload
```

### 4. Debugging

#### Serial Debugging
```bash
# Monitor serial output
pio device monitor --baud 115200

# ESP32 exception decoder
pio device monitor --filter esp32_exception_decoder
```

#### Hardware Debugging
```bash
# Debug build
pio run -e debug

# Upload debug build
pio run -e debug -t upload
```

## Network and Firewall Considerations

### Firewall Issues
If you encounter network issues during builds:

1. **Use the firewall-aware CI workflow**
   ```bash
   # Check network connectivity
   curl -I https://api.registry.platformio.org/
   ```

2. **Use offline build capability**
   ```bash
   ./build-offline.sh --cache
   ./build-offline.sh --offline your-target
   ```

3. **Configure proxy settings**
   ```bash
   export HTTP_PROXY=http://your-proxy:port
   export HTTPS_PROXY=http://your-proxy:port
   ```

### Network Diagnostics
```bash
# Test PlatformIO registry access
curl -I https://api.registry.platformio.org/
curl -I https://download.platformio.org/

# DNS resolution test
nslookup api.registry.platformio.org

# PlatformIO system info
pio system info
```

## Continuous Integration

### GitHub Actions Workflows

1. **ci.yml**: Standard CI/CD pipeline
2. **ci-firewall-aware.yml**: Enhanced CI with firewall handling
3. **manual.yml**: Manual release builds

### Local CI Testing
```bash
# Run the same checks as CI
pio run -e noradio
pio run -e wifi
pio run -e bt
pio test -e tests
```

## Code Review Process

### Before Submitting PR
1. **Build all targets** successfully
2. **Run all tests** and ensure they pass
3. **Check code style** follows project conventions
4. **Update documentation** if needed
5. **Test on actual hardware** if possible

### PR Submission
1. **Create descriptive PR title**
2. **Fill out PR template**
3. **Link related issues**
4. **Request appropriate reviewers**

### Review Criteria
- Code quality and style
- Test coverage
- Documentation updates
- Backward compatibility
- Performance impact

## Common Development Tasks

### Adding New Features

1. **Plan the feature**
   - Define requirements
   - Consider platform compatibility
   - Plan testing approach

2. **Implement incrementally**
   - Start with basic functionality
   - Add platform-specific optimizations
   - Maintain backward compatibility

3. **Test thoroughly**
   - Unit tests
   - Integration tests
   - Hardware validation

### Fixing Bugs

1. **Reproduce the issue**
   - Create minimal test case
   - Verify on multiple platforms
   - Document steps to reproduce

2. **Implement fix**
   - Make minimal changes
   - Add regression tests
   - Verify fix doesn't break other features

3. **Validate fix**
   - Test original issue is resolved
   - Run full test suite
   - Test on actual hardware

### Performance Optimization

1. **Measure current performance**
   - Use profiling tools
   - Identify bottlenecks
   - Set performance targets

2. **Optimize systematically**
   - Profile-guided optimization
   - Focus on critical paths
   - Measure impact of changes

3. **Validate optimizations**
   - Ensure correctness maintained
   - Verify performance improvements
   - Test on different hardware

## Troubleshooting

### Build Issues

#### Network Problems
```bash
# Check network connectivity
ping google.com

# Test PlatformIO registry
curl -I https://api.registry.platformio.org/

# Use offline build
./build-offline.sh --offline your-target
```

#### Dependency Issues
```bash
# Clean PlatformIO cache
pio system prune --force

# Reinstall dependencies
pip install -r requirements.txt

# Update PlatformIO
pio upgrade
```

#### Platform Issues
```bash
# Update platforms
pio platform update

# Install specific platform
pio platform install espressif32
pio platform install ststm32
```

### Runtime Issues

#### Serial Communication
```bash
# Check available ports
pio device list

# Monitor with specific baud rate
pio device monitor --baud 115200

# Use different port
pio device monitor --port /dev/ttyUSB0
```

#### Memory Issues
```bash
# Check memory usage
pio run -e wifi -v | grep -E "(RAM|Flash)"

# Optimize build
pio run -e wifi -t clean
pio run -e wifi --verbose
```

## Performance Guidelines

### Memory Management
- Minimize dynamic allocation
- Use appropriate data types
- Optimize string operations
- Consider memory-constrained targets

### Real-time Performance
- Minimize interrupt latency
- Use efficient algorithms
- Optimize critical paths
- Consider hardware limitations

### Code Size
- Use compiler optimizations
- Remove unused features
- Optimize for target platform
- Consider flash memory constraints

## Documentation Standards

### Code Documentation
- Document public interfaces
- Explain complex algorithms
- Add examples for usage
- Keep documentation current

### Commit Messages
```
type(scope): brief description

Longer description if needed

Closes #123
```

Types: feat, fix, docs, style, refactor, test, chore

### Release Notes
- Document breaking changes
- List new features
- Note bug fixes
- Include migration guide

## Support and Help

### Getting Help
- **GitHub Issues**: Technical questions and bugs
- **Discussions**: General questions and ideas
- **Documentation**: Check existing documentation first
- **Code Review**: Ask for feedback on complex changes

### Contributing Help
- **Review PRs**: Help review community contributions
- **Test Features**: Test new features on different hardware
- **Improve Documentation**: Help improve and update documentation
- **Report Issues**: Report bugs and suggest improvements

---

**Last Updated**: July 2024  
**Next Review**: August 2024