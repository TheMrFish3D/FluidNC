# Development Prompts

## Overview

This document provides standardized prompt templates for AI-assisted development of FluidNC, particularly for STM32 platform support. These prompts help ensure consistent, high-quality development across different features and improvements.

## Prompt Categories

### 1. Feature Development Prompts
### 2. Bug Fix Prompts
### 3. Performance Optimization Prompts
### 4. Code Review Prompts
### 5. Documentation Prompts
### 6. Testing Prompts

---

## 1. Feature Development Prompts

### STM32 Hardware Support Prompt

```
You are developing STM32 hardware support for FluidNC. Your task is to implement support for [SPECIFIC_STM32_BOARD].

Context:
- FluidNC is a CNC controller firmware
- Current STM32 support includes STM32F103C8, STM32F401CC, STM32F407VG
- Build system uses PlatformIO with offline build capability
- Code follows existing FluidNC architecture patterns

Requirements:
1. Add PlatformIO environment configuration for the new board
2. Implement hardware abstraction layer for GPIO, SPI, I2C, UART
3. Configure memory layout and flash partitioning
4. Add build flags and preprocessor definitions
5. Ensure compatibility with existing FluidNC architecture

Constraints:
- Maintain backward compatibility with existing STM32 configurations
- Follow FluidNC coding standards and patterns
- Include comprehensive error handling
- Add appropriate documentation
- Ensure build system integration

Please provide:
1. PlatformIO configuration changes
2. Hardware abstraction layer implementation
3. Memory configuration
4. Build system integration
5. Testing approach
6. Documentation updates

Focus on minimal, surgical changes that integrate seamlessly with existing code.
```

### Motor Control Enhancement Prompt

```
You are implementing motor control enhancements for FluidNC STM32 platform.

Context:
- FluidNC supports various motor types (stepper, servo, Trinamic drivers)
- STM32 platforms have different timer and PWM capabilities
- Current implementation focuses on ESP32, needs STM32 adaptation
- Performance and real-time constraints are critical

Task: Implement [SPECIFIC_MOTOR_FEATURE] for STM32 platforms.

Requirements:
1. Analyze current ESP32 motor control implementation
2. Identify STM32-specific adaptations needed
3. Implement hardware abstraction for timers and PWM
4. Ensure real-time performance requirements
5. Add support for different STM32 variants

Deliverables:
- Hardware abstraction layer for motor control
- STM32-specific timer and PWM configuration
- Integration with existing motor control API
- Performance optimization for real-time constraints
- Unit tests for motor control functions
- Documentation for configuration and usage

Consider:
- Different STM32 timer capabilities
- Interrupt handling and priority
- Memory constraints on lower-end STM32
- Real-time requirements for motion control
```

### Communication Protocol Prompt

```
You are implementing communication protocol support for FluidNC STM32 platform.

Context:
- FluidNC supports multiple communication protocols (UART, SPI, I2C)
- STM32 platforms have different peripheral configurations
- Protocol implementation must be reliable and efficient
- Integration with existing FluidNC communication architecture

Task: Implement [SPECIFIC_PROTOCOL] support for STM32 platforms.

Requirements:
1. Analyze existing protocol implementation
2. Create STM32-specific peripheral configuration
3. Implement protocol handlers with error handling
4. Ensure compatibility with existing API
5. Add configuration options for different STM32 variants

Focus Areas:
- Peripheral initialization and configuration
- Interrupt handling and DMA usage
- Error detection and recovery
- Performance optimization
- Memory usage optimization

Deliverables:
- STM32 peripheral configuration
- Protocol handler implementation
- Error handling and recovery mechanisms
- Configuration system integration
- Testing framework
- Usage documentation
```

---

## 2. Bug Fix Prompts

### Build System Bug Fix Prompt

```
You are fixing a build system issue in FluidNC that affects STM32 platform support.

Context:
- FluidNC uses PlatformIO with offline build capability
- Build system supports multiple platforms (ESP32, STM32)
- Network restrictions and firewall issues are common
- Build failures can be due to network, dependencies, or configuration

Issue: [SPECIFIC_BUILD_ISSUE]

Analysis Required:
1. Identify root cause of build failure
2. Determine if issue is network-related, dependency-related, or configuration-related
3. Check compatibility with offline build system
4. Verify impact on different platforms

Fix Approach:
1. Implement minimal, surgical fix
2. Add retry mechanisms for network issues
3. Improve error handling and diagnostics
4. Update offline build system if needed
5. Add regression tests

Deliverables:
- Root cause analysis
- Minimal fix implementation
- Enhanced error handling
- Updated build scripts
- Regression tests
- Documentation updates

Consider:
- Network reliability and firewall restrictions
- Cross-platform compatibility
- Backward compatibility
- Build performance impact
```

### Runtime Bug Fix Prompt

```
You are fixing a runtime issue in FluidNC STM32 platform support.

Context:
- FluidNC runs on resource-constrained STM32 microcontrollers
- Real-time constraints are critical for motion control
- Memory usage must be optimized
- Hardware compatibility varies across STM32 variants

Issue: [SPECIFIC_RUNTIME_ISSUE]

Debug Process:
1. Reproduce issue on target hardware
2. Analyze symptoms and error conditions
3. Identify affected code paths
4. Determine root cause (memory, timing, hardware)
5. Develop minimal fix

Fix Requirements:
- Maintain real-time performance
- Minimize memory usage impact
- Ensure hardware compatibility
- Add appropriate error handling
- Include diagnostic information

Deliverables:
- Issue reproduction steps
- Root cause analysis
- Minimal fix implementation
- Enhanced error handling
- Diagnostic improvements
- Unit tests
- Hardware validation

Focus on:
- Real-time performance constraints
- Memory usage optimization
- Hardware compatibility
- Error recovery mechanisms
```

---

## 3. Performance Optimization Prompts

### Memory Optimization Prompt

```
You are optimizing memory usage for FluidNC STM32 platform support.

Context:
- STM32 platforms have limited flash and RAM
- FluidNC requires efficient memory usage for real-time operation
- Different STM32 variants have different memory constraints
- Code size and runtime memory usage both matter

Task: Optimize memory usage for [SPECIFIC_COMPONENT/FEATURE].

Analysis:
1. Profile current memory usage (flash and RAM)
2. Identify memory hotspots and inefficiencies
3. Analyze dynamic vs static memory allocation
4. Consider different STM32 memory configurations

Optimization Strategies:
- Reduce code size through compiler optimizations
- Minimize dynamic memory allocation
- Optimize data structures and algorithms
- Use appropriate data types
- Implement memory pooling where beneficial

Deliverables:
- Memory usage analysis
- Optimization implementation
- Performance benchmarks
- Memory configuration updates
- Testing on different STM32 variants
- Documentation updates

Constraints:
- Maintain functionality and performance
- Ensure compatibility across STM32 variants
- Preserve real-time characteristics
- Keep code readable and maintainable
```

### Performance Tuning Prompt

```
You are optimizing performance for FluidNC STM32 platform support.

Context:
- Real-time performance is critical for CNC control
- STM32 platforms have different performance characteristics
- Performance optimization must not compromise functionality
- Different optimization strategies for different STM32 variants

Task: Optimize performance for [SPECIFIC_FUNCTIONALITY].

Performance Analysis:
1. Identify performance bottlenecks
2. Measure timing and resource usage
3. Analyze critical code paths
4. Profile interrupt handling and real-time constraints

Optimization Approaches:
- Algorithm optimization
- Compiler optimization flags
- Hardware-specific optimizations
- Interrupt handling optimization
- Cache utilization (where available)

Deliverables:
- Performance analysis report
- Optimization implementation
- Benchmarking results
- Configuration tuning
- Testing on different STM32 variants
- Performance documentation

Requirements:
- Maintain real-time guarantees
- Preserve functionality
- Ensure cross-platform compatibility
- Document performance characteristics
```

---

## 4. Code Review Prompts

### Code Review Prompt

```
You are conducting a code review for FluidNC STM32 platform support.

Context:
- FluidNC is a safety-critical CNC controller
- Code quality and reliability are paramount
- STM32 platform support is relatively new
- Code must follow FluidNC standards and patterns

Review Focus Areas:
1. Code Quality and Style
2. Architecture and Design
3. Error Handling and Safety
4. Performance and Efficiency
5. Documentation and Maintainability
6. Testing and Validation

Review Criteria:
- Follows FluidNC coding standards
- Proper error handling and recovery
- Efficient resource usage
- Real-time performance considerations
- Hardware compatibility
- Memory safety
- Thread safety (where applicable)

Deliverables:
- Code review comments
- Suggested improvements
- Security and safety analysis
- Performance impact assessment
- Testing recommendations
- Documentation review

Consider:
- Safety-critical nature of CNC control
- Resource constraints of STM32 platforms
- Real-time requirements
- Hardware compatibility
- Maintainability and future development
```

---

## 5. Documentation Prompts

### API Documentation Prompt

```
You are creating API documentation for FluidNC STM32 platform support.

Context:
- FluidNC has multiple platform support (ESP32, STM32)
- API documentation needs to be comprehensive and accurate
- Both users and developers need clear documentation
- Documentation should include examples and best practices

Task: Create comprehensive API documentation for [SPECIFIC_API/MODULE].

Documentation Requirements:
1. API Reference
   - Function/method signatures
   - Parameter descriptions
   - Return values and error codes
   - Usage examples

2. Usage Guide
   - Common use cases
   - Configuration examples
   - Best practices
   - Troubleshooting

3. Implementation Notes
   - Platform-specific considerations
   - Performance characteristics
   - Memory usage
   - Hardware requirements

Format:
- Clear, concise language
- Code examples with syntax highlighting
- Diagrams where helpful
- Cross-references to related documentation
- Version information

Deliverables:
- API reference documentation
- Usage guide with examples
- Implementation notes
- Troubleshooting guide
- Integration examples
```

### User Guide Prompt

```
You are creating user documentation for FluidNC STM32 platform support.

Context:
- Users range from beginners to advanced CNC enthusiasts
- STM32 platform support is newer than ESP32 support
- Documentation must be accurate and easy to follow
- Users need clear setup and configuration instructions

Task: Create user guide for [SPECIFIC_FEATURE/FUNCTIONALITY].

User Guide Structure:
1. Introduction and Overview
2. Requirements and Prerequisites
3. Installation and Setup
4. Configuration
5. Usage Instructions
6. Troubleshooting
7. Advanced Topics

Content Requirements:
- Step-by-step instructions
- Screenshots and diagrams
- Configuration examples
- Common issues and solutions
- Hardware-specific notes
- Safety considerations

Target Audience:
- Makers and hobbyists
- Professional CNC users
- Hardware developers
- System integrators

Deliverables:
- Comprehensive user guide
- Configuration examples
- Troubleshooting guide
- Hardware compatibility list
- FAQ section
```

---

## 6. Testing Prompts

### Test Development Prompt

```
You are developing tests for FluidNC STM32 platform support.

Context:
- FluidNC is safety-critical software
- Testing must cover multiple STM32 variants
- Both unit tests and integration tests are needed
- Hardware-in-the-loop testing is important for CNC control

Task: Develop comprehensive tests for [SPECIFIC_COMPONENT/FEATURE].

Test Categories:
1. Unit Tests
   - Individual function testing
   - Edge case handling
   - Error condition testing
   - Mock hardware interfaces

2. Integration Tests
   - Component interaction testing
   - End-to-end functionality
   - Platform-specific testing
   - Performance testing

3. Hardware Tests
   - Real hardware validation
   - Hardware compatibility testing
   - Performance benchmarking
   - Safety feature testing

Test Requirements:
- Automated test execution
- Cross-platform compatibility
- CI/CD integration
- Clear test reporting
- Performance metrics
- Coverage analysis

Deliverables:
- Unit test suite
- Integration test suite
- Hardware test procedures
- Test automation scripts
- Test documentation
- CI/CD integration
```

### Hardware Validation Prompt

```
You are developing hardware validation procedures for FluidNC STM32 platform support.

Context:
- STM32 platforms have different hardware capabilities
- Hardware validation ensures compatibility and reliability
- Validation must cover different STM32 variants
- Safety and performance are critical

Task: Create hardware validation procedures for [SPECIFIC_STM32_PLATFORM].

Validation Areas:
1. Basic Hardware Functions
   - GPIO operations
   - Timer and PWM functionality
   - Communication interfaces
   - Memory operations

2. CNC-Specific Functions
   - Motor control
   - Limit switch handling
   - Emergency stop
   - Probe input

3. Performance Testing
   - Real-time performance
   - Memory usage
   - Power consumption
   - Thermal characteristics

4. Safety Testing
   - Emergency stop response
   - Fault handling
   - Error recovery
   - Fail-safe behavior

Deliverables:
- Hardware validation procedures
- Test scripts and tools
- Validation reports
- Hardware compatibility matrix
- Performance benchmarks
- Safety certification documentation
```

---

## Prompt Usage Guidelines

### 1. Customization
- Replace placeholder text (in brackets) with specific requirements
- Adjust complexity based on task scope
- Add project-specific constraints

### 2. Context Adaptation
- Include relevant project history
- Add specific technical constraints
- Reference existing code patterns

### 3. Quality Assurance
- Always include testing requirements
- Emphasize documentation needs
- Consider safety and reliability

### 4. Collaboration
- Encourage code review
- Include community feedback
- Plan for maintenance

## Template Variables

Common variables used in prompts:
- `[SPECIFIC_STM32_BOARD]` - Target STM32 board/chip
- `[SPECIFIC_MOTOR_FEATURE]` - Motor control feature
- `[SPECIFIC_PROTOCOL]` - Communication protocol
- `[SPECIFIC_BUILD_ISSUE]` - Build system issue
- `[SPECIFIC_RUNTIME_ISSUE]` - Runtime issue
- `[SPECIFIC_FUNCTIONALITY]` - Feature/functionality
- `[SPECIFIC_API/MODULE]` - API or module
- `[SPECIFIC_FEATURE/FUNCTIONALITY]` - User-facing feature
- `[SPECIFIC_COMPONENT/FEATURE]` - Component or feature
- `[SPECIFIC_STM32_PLATFORM]` - STM32 platform variant

## Best Practices

### 1. Prompt Structure
- Clear context and background
- Specific requirements and constraints
- Expected deliverables
- Quality criteria

### 2. Technical Accuracy
- Use correct technical terminology
- Reference existing architecture
- Include relevant constraints
- Specify quality requirements

### 3. Maintainability
- Emphasize documentation
- Include testing requirements
- Consider long-term maintenance
- Plan for future development

---

**Last Updated**: July 2024  
**Next Review**: August 2024