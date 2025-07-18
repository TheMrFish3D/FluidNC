# Community Contribution Guidelines

## Overview

FluidNC welcomes contributions from the community! This document outlines how to contribute effectively to the FluidNC project, with specific guidance for STM32 platform support development.

## Getting Started

### Before Contributing

1. **Read the documentation**
   - Review the [Development Workflow](Development-Workflow.md)
   - Check the [STM32 Implementation Roadmap](STM32-Implementation-Roadmap.md)
   - Understand the [Build System](Build-System.md)

2. **Set up your development environment**
   - Follow the setup instructions in [Development Workflow](Development-Workflow.md)
   - Test the offline build system if you have network restrictions
   - Verify you can build existing targets

3. **Join the community**
   - Check existing GitHub issues and discussions
   - Introduce yourself in the community forum
   - Ask questions if you need help getting started

## Types of Contributions

### 1. Bug Reports
- Use the appropriate issue template
- For STM32 issues, use the [STM32 Bug Report template](.github/ISSUE_TEMPLATE/stm32-bug-report.yml)
- Provide detailed reproduction steps
- Include build output and configuration details

### 2. Feature Requests
- Use the [STM32 Feature Request template](.github/ISSUE_TEMPLATE/stm32-feature-request.yml)
- Describe the use case and benefits
- Consider implementation complexity and compatibility

### 3. Code Contributions
- Bug fixes
- New features
- Performance improvements
- Documentation updates
- Test improvements

### 4. Documentation
- User guides and tutorials
- API documentation
- Hardware compatibility information
- Troubleshooting guides

### 5. Testing
- Hardware validation on different boards
- Performance benchmarking
- Configuration testing
- Regression testing

## Contribution Process

### 1. Planning Your Contribution

#### For Bug Fixes
1. **Create or find an issue** describing the bug
2. **Discuss the approach** with maintainers
3. **Understand the scope** - prefer minimal fixes
4. **Plan testing strategy**

#### For New Features
1. **Check the roadmap** to see if feature is already planned
2. **Create a feature request** if not already exists
3. **Discuss design and implementation** with the community
4. **Start with a proposal** for significant features
5. **Consider backward compatibility**

### 2. Development Process

#### Fork and Clone
```bash
# Fork the repository on GitHub
# Clone your fork
git clone https://github.com/YOUR-USERNAME/FluidNC.git
cd FluidNC

# Add upstream remote
git remote add upstream https://github.com/bdring/FluidNC.git
```

#### Create Feature Branch
```bash
# Update your fork
git checkout main
git pull upstream main

# Create feature branch
git checkout -b feature/your-feature-name
```

#### Development Guidelines
- **Follow coding standards** (see [CodingStyle.md](../CodingStyle.md))
- **Write tests** for new functionality
- **Update documentation** as needed
- **Keep commits atomic** and well-described
- **Test on multiple platforms** when possible

#### Code Quality
- Use meaningful variable and function names
- Add comments for complex logic
- Follow existing code patterns
- Keep functions small and focused
- Handle errors appropriately

### 3. Testing Your Changes

#### Build Testing
```bash
# Test basic build
pio run -e wifi

# Test STM32 builds
pio run -e stm32_noradio
pio run -e stm32_basic
pio run -e stm32_advanced

# Test offline build
./build-offline.sh --offline wifi
```

#### Unit Testing
```bash
# Run unit tests
pio test -e tests

# Run platform-specific tests
pio test -e tests_nosan  # For Windows
```

#### Hardware Testing
- Test on actual hardware when possible
- Use the [Hardware Validation](Hardware-Validation.md) procedures
- Document any hardware-specific requirements

### 4. Submitting Your Contribution

#### Prepare Your Pull Request
1. **Update your branch** with latest upstream changes
2. **Squash commits** if necessary for clean history
3. **Write clear commit messages**
4. **Update documentation** if needed
5. **Add entry to changelog** if applicable

#### Pull Request Guidelines
- **Use descriptive title** that explains the change
- **Fill out the PR template** completely
- **Link to related issues** using keywords (fixes #123)
- **Provide clear description** of changes and rationale
- **Include testing information**
- **Request appropriate reviewers**

#### Example PR Description
```markdown
## Summary
Brief description of the changes made.

## Changes Made
- Added STM32F446 support to platformio.ini
- Implemented STM32F446-specific GPIO configuration
- Added hardware validation tests
- Updated documentation

## Testing
- [x] Builds successfully on all platforms
- [x] Unit tests pass
- [x] Hardware tested on STM32F446 board
- [x] Offline build works
- [x] No regression in existing functionality

## Related Issues
Fixes #123
Related to #456

## Breaking Changes
None / List any breaking changes

## Checklist
- [x] Code follows project style guidelines
- [x] Tests added for new functionality
- [x] Documentation updated
- [x] Changelog updated
- [x] Hardware tested (if applicable)
```

## Code Review Process

### What to Expect
1. **Initial review** within 1-2 weeks
2. **Feedback and discussion** on implementation
3. **Iteration** based on reviewer feedback
4. **Final approval** and merge

### Review Criteria
- **Code quality** and adherence to standards
- **Functionality** and correctness
- **Testing** coverage and quality
- **Documentation** completeness
- **Compatibility** with existing code
- **Performance** impact

### Addressing Review Feedback
- **Respond promptly** to reviewer comments
- **Ask for clarification** if feedback is unclear
- **Make requested changes** in new commits
- **Explain your reasoning** for any disagreements
- **Be open to learning** from feedback

## Community Guidelines

### Code of Conduct
- **Be respectful** and professional
- **Welcome newcomers** and help them get started
- **Provide constructive feedback**
- **Focus on technical merit**
- **Assume good intentions**

### Communication
- **Use clear, concise language**
- **Provide context** for your comments
- **Be patient** with response times
- **Use appropriate channels** for different types of discussions

### Collaboration
- **Share knowledge** and help others learn
- **Credit contributors** appropriately
- **Coordinate work** to avoid duplicated effort
- **Respect different perspectives** and approaches

## Platform-Specific Contributions

### STM32 Platform Development

#### Getting Started with STM32
1. **Review STM32 roadmap** to understand current status
2. **Set up STM32 development environment**
3. **Start with simple changes** to understand the architecture
4. **Test on actual hardware** when possible

#### STM32 Contribution Areas
- **Hardware support** for new STM32 boards
- **Peripheral drivers** (GPIO, SPI, I2C, UART)
- **Motor control** improvements
- **Safety features** implementation
- **Performance optimization**
- **Documentation** and examples

#### STM32 Testing Requirements
- **Build testing** on all STM32 environments
- **Hardware validation** on target boards
- **Performance benchmarking**
- **Memory usage analysis**
- **Safety feature testing**

### ESP32 Platform Development

#### ESP32 Considerations
- **Maintain compatibility** with existing ESP32 code
- **Consider wireless features** and their interaction
- **Test on different ESP32 variants** (ESP32, ESP32-S2, ESP32-S3)
- **Ensure real-time performance** is maintained

## Recognition and Credits

### Contributor Recognition
- **Contributors list** maintained in project documentation
- **Commit attribution** preserves your contribution history
- **Release notes** acknowledge significant contributions
- **Community recognition** for ongoing contributors

### Becoming a Maintainer
Active contributors may be invited to become maintainers with:
- **Code review privileges**
- **Issue triage responsibilities**
- **Release management participation**
- **Community leadership roles**

## Resources for Contributors

### Documentation
- [Development Workflow](Development-Workflow.md)
- [STM32 Implementation Roadmap](STM32-Implementation-Roadmap.md)
- [Build System](Build-System.md)
- [Hardware Validation](Hardware-Validation.md)
- [Development Prompts](Development-Prompts.md)

### Tools and Resources
- **PlatformIO documentation**: https://docs.platformio.org/
- **STM32 documentation**: https://www.st.com/en/microcontrollers-microprocessors/stm32-32-bit-arm-cortex-mcus.html
- **ESP32 documentation**: https://docs.espressif.com/projects/esp-idf/
- **FluidNC forum**: Community discussions and support

### Getting Help
- **GitHub Issues**: Technical questions and bugs
- **GitHub Discussions**: General questions and ideas
- **Community Forum**: Broader community discussions
- **Documentation**: Check existing documentation first

## Contribution Examples

### Example 1: Adding STM32 Board Support
```cpp
// 1. Add to platformio.ini
[env:stm32f446]
extends = common_stm32
board = genericSTM32F446RE
build_flags = ${common_stm32.build_flags} -DSTM32F446xx

// 2. Add hardware configuration
#ifdef STM32F446xx
#define MAX_STEP_RATE 100000
#define GPIO_COUNT 82
#define UART_COUNT 4
#define SPI_COUNT 4
#define I2C_COUNT 3
#endif

// 3. Add to documentation
- STM32F446RE: High-performance board with 512KB flash, 128KB RAM
```

### Example 2: Bug Fix Contribution
```cpp
// Issue: STM32 SPI initialization fails on some boards
// Root cause: Missing GPIO alternate function configuration

// Fix: Add proper GPIO configuration
void spi_init(spi_num_t spi_num, spi_config_t* config) {
    // Configure GPIO alternate functions
    gpio_set_af(config->sck_pin, GPIO_AF5_SPI1);
    gpio_set_af(config->miso_pin, GPIO_AF5_SPI1);
    gpio_set_af(config->mosi_pin, GPIO_AF5_SPI1);
    
    // Existing initialization code...
}
```

### Example 3: Documentation Improvement
```markdown
# Before
The STM32 platform is supported.

# After
## STM32 Platform Support

FluidNC supports multiple STM32 variants:
- **STM32F103C8**: Entry-level with 64KB flash, 20KB RAM
- **STM32F401CC**: Mid-range with 256KB flash, 64KB RAM  
- **STM32F407VG**: High-performance with 512KB flash, 128KB RAM

### Configuration Example
```yaml
board: stm32f103c8
axes:
  x:
    steps_per_mm: 80
    max_rate: 5000
```

## Frequently Asked Questions

### General Questions

**Q: How do I get started contributing?**
A: Start by setting up your development environment, building the existing code, and looking for "good first issue" labels on GitHub.

**Q: What skills do I need?**
A: Basic C/C++ programming, familiarity with embedded systems, and willingness to learn. STM32 experience is helpful but not required.

**Q: How long does the review process take?**
A: Initial review typically happens within 1-2 weeks, but complex changes may take longer.

### STM32-Specific Questions

**Q: Which STM32 boards are supported?**
A: Currently STM32F103C8, STM32F401CC, and STM32F407VG. See the roadmap for planned additions.

**Q: How do I test STM32 code without hardware?**
A: Use the build system to verify compilation, but hardware testing is recommended for final validation.

**Q: Can I add support for my custom STM32 board?**
A: Yes! Follow the hardware support contribution process and provide detailed documentation.

### Build System Questions

**Q: Why do builds fail with network errors?**
A: Use the offline build system: `./build-offline.sh --cache` then `./build-offline.sh --offline your-target`

**Q: How do I set up the development environment in a restricted network?**
A: Use the firewall-aware CI workflow as a guide, or use the offline build system.

## Contact Information

- **GitHub Issues**: https://github.com/bdring/FluidNC/issues
- **GitHub Discussions**: https://github.com/bdring/FluidNC/discussions
- **Community Forum**: [Link to community forum]
- **Email**: [Maintainer contact information]

---

**Thank you for contributing to FluidNC!** Your contributions help make FluidNC better for everyone in the CNC community.

---

**Last Updated**: July 2024  
**Next Review**: August 2024