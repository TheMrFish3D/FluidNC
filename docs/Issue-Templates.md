# Issue Templates

## Overview

This document describes the standardized issue templates available for FluidNC development, with specific templates for STM32 platform support. These templates help ensure consistent, high-quality issue reporting that facilitates efficient problem resolution.

## Available Templates

### 1. STM32 Bug Report Template
- **File**: `.github/ISSUE_TEMPLATE/stm32-bug-report.yml`
- **Purpose**: Report bugs specific to STM32 platform support
- **Labels**: `bug`, `stm32`, `needs-triage`

### 2. STM32 Feature Request Template
- **File**: `.github/ISSUE_TEMPLATE/stm32-feature-request.yml`
- **Purpose**: Request new features for STM32 platform
- **Labels**: `enhancement`, `stm32`, `feature-request`

### 3. General Templates (Existing)
- **Problem Report**: General bug reports
- **Feature Request**: General feature requests
- **Configuration**: Custom configuration template

## Template Usage Guidelines

### When to Use STM32 Templates

#### Use STM32 Bug Report Template When:
- Issue occurs specifically on STM32 hardware
- Problem is related to STM32 build environments
- Issue involves STM32-specific features or limitations
- Hardware-specific behavior on STM32 platforms

#### Use STM32 Feature Request Template When:
- Requesting STM32-specific features
- Proposing STM32 hardware support
- Suggesting STM32 platform improvements
- Requesting STM32 documentation enhancements

#### Use General Templates When:
- Issue affects multiple platforms
- Problem is platform-agnostic
- General FluidNC functionality issues
- Documentation or process improvements

### Template Selection Guide

```
Issue Type Decision Tree:

Does the issue affect STM32 platforms?
├── Yes: Is it a bug or feature request?
│   ├── Bug → Use STM32 Bug Report Template
│   └── Feature → Use STM32 Feature Request Template
└── No: Is it a bug or feature request?
    ├── Bug → Use General Problem Report Template
    └── Feature → Use General Feature Request Template
```

## STM32 Bug Report Template

### Template Structure

#### Pre-submission Checklist
- Search for existing issues
- Test with latest version
- Try offline build system
- Check build system documentation

#### Environment Information
- **STM32 Board**: Specific board/chip identification
- **Build Environment**: Target build environment
- **Operating System**: Development OS
- **Network Environment**: Firewall/proxy details

#### Bug Description
- Clear bug description
- Expected vs actual behavior
- Reproduction steps
- Build output and logs
- Runtime logs and serial output

#### Configuration Details
- Hardware configuration
- Software configuration
- Network environment
- Additional context

### Example STM32 Bug Report

```yaml
name: STM32 Bug Report
description: Report a bug or issue with STM32 platform support
title: "[STM32] SPI Communication Fails on STM32F407VG"
labels: ["bug", "stm32", "needs-triage"]

# Bug details filled according to template
STM32 Board: STM32F407VG (stm32_advanced)
Build Environment: stm32_advanced
Operating System: Linux (Ubuntu 20.04)

Bug Description:
SPI communication with TMC2130 drivers fails during initialization
on STM32F407VG board. ESP32 works fine with same configuration.

Expected Behavior:
TMC2130 drivers should initialize and respond to SPI commands.

Actual Behavior:
SPI initialization appears successful, but reads return 0xFF.
Driver status shows communication failure.

Reproduction Steps:
1. Flash stm32_advanced environment to STM32F407VG
2. Connect TMC2130 to SPI pins as configured
3. Send initialization commands
4. Observe SPI communication failure
```

## STM32 Feature Request Template

### Template Structure

#### Pre-submission Checklist
- Search for existing requests
- Review STM32 roadmap
- Verify not already planned

#### Feature Classification
- **Category**: Hardware, motor control, communication, etc.
- **Priority**: Low, medium, high, critical
- **Target Boards**: Which STM32 boards affected

#### Feature Description
- Clear feature description
- Use case and benefits
- Proposed implementation
- Alternative solutions considered

#### Technical Requirements
- Hardware requirements
- Software requirements
- Compatibility considerations
- Testing approach

### Example STM32 Feature Request

```yaml
name: STM32 Feature Request
description: Request a new feature for STM32 platform support
title: "[STM32 Feature] Add CAN Bus Support for STM32F407VG"
labels: ["enhancement", "stm32", "feature-request"]

# Feature details filled according to template
Feature Category: Communication (CAN Bus support)
Priority: Medium - Would be helpful
Target Boards: STM32F407VG (stm32_advanced)

Feature Description:
Add CAN bus communication support for STM32F407VG to enable
distributed control systems and communication with external
devices like VFDs and monitoring systems.

Use Case:
- Communicate with VFD for spindle control
- Connect to external monitoring systems
- Distributed I/O modules
- Industrial automation integration

Proposed Solution:
- Add CAN peripheral driver for STM32F407VG
- Implement CAN message protocol for FluidNC
- Add configuration options for CAN bus settings
- Support for standard CAN and extended CAN frames
```

## Template Customization

### Adding New Templates

#### Template File Structure
```yaml
name: Template Name
description: Template description
title: "[PREFIX] "
labels: ["label1", "label2", "label3"]

body:
  - type: markdown
    attributes:
      value: |
        Template introduction and instructions
        
  - type: checkboxes
    id: checklist
    attributes:
      label: Pre-submission Checklist
      options:
        - label: Checklist item 1
          required: true
        - label: Checklist item 2
          required: false
          
  - type: dropdown
    id: category
    attributes:
      label: Category
      options:
        - Option 1
        - Option 2
      default: 0
    validations:
      required: true
      
  - type: textarea
    id: description
    attributes:
      label: Description
      placeholder: Detailed description...
    validations:
      required: true
```

#### Template Guidelines
1. **Use clear, descriptive names**
2. **Include comprehensive checklists**
3. **Provide dropdown options** where appropriate
4. **Make required fields mandatory**
5. **Include helpful placeholder text**
6. **Add validation rules**

### Template Maintenance

#### Regular Review
- **Monthly**: Review template usage and effectiveness
- **Quarterly**: Update templates based on feedback
- **Annually**: Comprehensive template review

#### Version Control
- **Track changes** in template files
- **Document modifications** in commit messages
- **Test templates** before deployment

## Issue Triage Process

### Initial Triage

#### STM32 Bug Reports
1. **Validate template completion**
2. **Check for duplicates**
3. **Verify reproduction steps**
4. **Assign appropriate labels**
5. **Set priority based on impact**

#### STM32 Feature Requests
1. **Review against roadmap**
2. **Evaluate feasibility**
3. **Assess community benefit**
4. **Determine implementation complexity**
5. **Assign to appropriate milestone**

### Triage Labels

#### Priority Labels
- `priority-critical`: System-breaking issues
- `priority-high`: Major functionality issues
- `priority-medium`: Important improvements
- `priority-low`: Minor enhancements

#### Status Labels
- `needs-triage`: Requires initial review
- `needs-reproduction`: Needs reproduction steps
- `needs-testing`: Requires testing
- `needs-documentation`: Documentation needed

#### Platform Labels
- `stm32`: STM32-specific issues
- `esp32`: ESP32-specific issues
- `multi-platform`: Affects multiple platforms
- `build-system`: Build system issues

## Quality Assurance

### Template Quality Metrics

#### Completeness
- All required fields completed
- Sufficient detail provided
- Reproduction steps clear
- Configuration information included

#### Accuracy
- Issue description matches problem
- Expected behavior is reasonable
- Reproduction steps are valid
- Configuration is correct

#### Usefulness
- Issue is actionable
- Problem is clearly defined
- Solution approach is feasible
- Community benefit is evident

### Template Effectiveness

#### Success Metrics
- **Response time**: Average time to first response
- **Resolution time**: Time from report to resolution
- **Reproduction rate**: Percentage of issues reproduced
- **Duplicate rate**: Percentage of duplicate issues

#### Improvement Process
1. **Collect feedback** from users and maintainers
2. **Analyze usage patterns** and common issues
3. **Identify template gaps** and improvements
4. **Update templates** based on findings
5. **Test updated templates**

## Community Guidelines

### For Issue Reporters

#### Before Submitting
1. **Search existing issues** for duplicates
2. **Review documentation** for known issues
3. **Test with latest version** of FluidNC
4. **Try offline build** if network-related

#### While Submitting
1. **Use appropriate template** for your issue type
2. **Fill all required fields** completely
3. **Provide clear, detailed descriptions**
4. **Include all relevant information**

#### After Submitting
1. **Respond promptly** to maintainer questions
2. **Test proposed solutions** and provide feedback
3. **Update issue** with additional information
4. **Close issue** when resolved

### For Maintainers

#### Triage Process
1. **Review new issues** daily
2. **Apply appropriate labels** immediately
3. **Ask for clarification** when needed
4. **Assign to appropriate milestone**

#### Response Guidelines
1. **Acknowledge receipt** within 48 hours
2. **Provide estimated timeline** for resolution
3. **Keep reporter informed** of progress
4. **Thank reporters** for their contributions

## Template Examples

### Effective Bug Report Example
```
Title: [STM32] Step pulses irregular on STM32F401CC at high speeds

STM32 Board: STM32F401CC (stm32_basic)
Build Environment: stm32_basic
Operating System: Windows 10

Bug Description:
Step pulses become irregular when step rate exceeds 5000 Hz on 
STM32F401CC. Same configuration works fine on ESP32.

Expected Behavior:
Regular step pulses at configured frequency up to 10000 Hz.

Actual Behavior:
Irregular timing with visible jitter above 5000 Hz.

Reproduction Steps:
1. Build and flash stm32_basic environment
2. Configure X-axis with step rate 8000 Hz
3. Send G0 X100 command
4. Observe step pulse timing with oscilloscope

Build Output:
[Complete build output attached]

Hardware Configuration:
- STM32F401CC on development board
- External oscilloscope on step pin
- Standard stepper driver connection
```

### Poor Bug Report Example
```
Title: STM32 doesn't work

Bug Description:
It's broken.

Expected Behavior:
Should work.

Actual Behavior:
Doesn't work.

Reproduction Steps:
1. Use STM32
2. It doesn't work
```

## Future Improvements

### Planned Template Enhancements

#### Advanced Templates
- **Hardware validation** issue template
- **Performance regression** template
- **Documentation improvement** template
- **Community contribution** template

#### Template Features
- **Automatic diagnostics** collection
- **Pre-filled environment** information
- **Guided troubleshooting** steps
- **Solution suggestions** based on symptoms

#### Integration Improvements
- **Automated testing** of reproduction steps
- **Link to related** documentation
- **Suggest similar** resolved issues
- **Automated triage** based on template data

---

**Effective issue templates improve the quality of bug reports and feature requests, leading to faster resolution and better community collaboration.**

---

**Last Updated**: July 2024  
**Next Review**: August 2024