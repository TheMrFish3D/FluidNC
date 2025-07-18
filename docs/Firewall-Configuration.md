# Firewall Configuration

## Overview

This document provides comprehensive guidance for configuring firewalls and network settings to enable FluidNC development and deployment in restricted network environments. The build system includes robust firewall handling and offline capabilities.

## Common Firewall Issues

### PlatformIO Registry Access

FluidNC uses PlatformIO which requires access to several endpoints:

#### Required Endpoints
- `api.registry.platformio.org` - Package registry API
- `api.registry.nm1.platformio.org` - Alternative registry endpoint
- `download.platformio.org` - Package downloads
- `collector.platformio.org` - Analytics and telemetry
- `github.com` - Source code repositories
- `github-releases.githubusercontent.com` - Release downloads

#### Typical Error Messages
```
HTTPClientError:
Tool Manager: Installing espressif/toolchain-xtensa-esp32 @ 8.4.0+2021r2-patch5
```

```
Error: Could not install package 'platformio/toolchain-xtensa-esp32 @ 8.4.0+2021r2-patch5'
```

## Firewall Configuration Solutions

### 1. Firewall Allow List

#### Corporate Firewall Configuration
```
# Allow PlatformIO endpoints
*.platformio.org
*.registry.platformio.org
api.registry.nm1.platformio.org
download.platformio.org
collector.platformio.org

# Allow GitHub access
github.com
*.github.com
github-releases.githubusercontent.com
api.github.com
raw.githubusercontent.com

# Allow Python package index
pypi.org
files.pythonhosted.org

# Allow DNS resolution
8.8.8.8
1.1.1.1
```

#### Port Requirements
```
# HTTPS traffic
443/tcp

# HTTP traffic (fallback)
80/tcp

# DNS resolution
53/udp
53/tcp
```

### 2. Proxy Configuration

#### HTTP/HTTPS Proxy Setup
```bash
# Set proxy environment variables
export HTTP_PROXY=http://proxy.company.com:8080
export HTTPS_PROXY=https://proxy.company.com:8080
export NO_PROXY=localhost,127.0.0.1,.company.com

# Configure Git proxy
git config --global http.proxy http://proxy.company.com:8080
git config --global https.proxy https://proxy.company.com:8080

# Configure pip proxy
pip config set global.proxy http://proxy.company.com:8080
```

#### Proxy Authentication
```bash
# Proxy with authentication
export HTTP_PROXY=http://username:password@proxy.company.com:8080
export HTTPS_PROXY=https://username:password@proxy.company.com:8080

# Configure Git with authentication
git config --global http.proxy http://username:password@proxy.company.com:8080
```

### 3. DNS Configuration

#### Alternative DNS Servers
```bash
# Use public DNS servers
echo "nameserver 8.8.8.8" | sudo tee -a /etc/resolv.conf
echo "nameserver 1.1.1.1" | sudo tee -a /etc/resolv.conf

# Use company DNS servers
echo "nameserver 192.168.1.1" | sudo tee -a /etc/resolv.conf
echo "nameserver 192.168.1.2" | sudo tee -a /etc/resolv.conf
```

#### DNS Configuration Testing
```bash
# Test DNS resolution
nslookup api.registry.platformio.org
nslookup download.platformio.org
nslookup github.com

# Test with alternative DNS
nslookup api.registry.platformio.org 8.8.8.8
nslookup download.platformio.org 1.1.1.1
```

## Offline Build System

### Overview

The offline build system allows FluidNC development in completely isolated environments by pre-caching all dependencies.

### Setting Up Offline Cache

#### Initial Cache Creation
```bash
# Create comprehensive offline cache
./build-offline.sh --cache

# Cache will be created in .offline_cache/
ls -la .offline_cache/
```

#### Cache Contents
```
.offline_cache/
├── pip_cache/          # Python packages
├── platformio_cache/   # PlatformIO packages
├── platforms/          # Platform definitions
├── libraries/          # Global libraries
└── cache_manifest.txt  # Cache inventory
```

### Using Offline Build

#### Build Specific Target
```bash
# Build single environment offline
./build-offline.sh --offline wifi

# Build STM32 target offline
./build-offline.sh --offline stm32_basic

# Build with verbose output
./build-offline.sh --offline --verbose wifi
```

#### Build All Targets
```bash
# Build all environments offline
./build-offline.sh --offline all

# List available environments
./build-offline.sh --list-envs
```

### Offline Build Features

#### Retry Mechanisms
```bash
# Automatic retry on failure
max_attempts=3
attempt=1

while [ $attempt -le $max_attempts ]; do
    if build_command; then
        break
    else
        echo "Attempt $attempt failed, retrying..."
        sleep 10
    fi
    attempt=$((attempt + 1))
done
```

#### Cache Management
```bash
# Clean build cache
./build-offline.sh --clean

# Clean everything including offline cache
./build-offline.sh --clean --all

# Check cache size
du -sh .offline_cache/
```

## CI/CD Firewall Configuration

### GitHub Actions Firewall-Aware Workflow

#### Enhanced CI Configuration
```yaml
name: FluidNC CI - Firewall Aware Build

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - name: Configure firewall-aware network settings
        run: |
          # Configure DNS
          echo "nameserver 8.8.8.8" | sudo tee -a /etc/resolv.conf
          echo "nameserver 1.1.1.1" | sudo tee -a /etc/resolv.conf
          
          # Configure proxy if available
          if [ -n "$HTTP_PROXY" ]; then
            echo "HTTP_PROXY=$HTTP_PROXY" >> $GITHUB_ENV
            echo "HTTPS_PROXY=$HTTPS_PROXY" >> $GITHUB_ENV
          fi
          
          # Configure Git
          git config --global http.sslVerify true
```

#### Retry Mechanisms
```yaml
      - name: Install PlatformIO with retry
        run: |
          max_attempts=3
          attempt=1
          
          while [ $attempt -le $max_attempts ]; do
            if pip install -r requirements.txt --timeout=300 --retries=5; then
              break
            else
              echo "Installation failed, retrying in 30 seconds..."
              sleep 30
            fi
            attempt=$((attempt + 1))
          done
```

#### Network Diagnostics
```yaml
      - name: Network diagnostics on failure
        if: failure()
        run: |
          echo "=== Network Diagnostics ==="
          nslookup api.registry.platformio.org
          curl -I https://api.registry.platformio.org/
          ping -c 3 8.8.8.8
          env | grep -E "(HTTP|HTTPS|PROXY)"
```

### Self-Hosted Runners

#### Runner Configuration
```yaml
# Configure self-hosted runner with firewall settings
runs-on: self-hosted
env:
  HTTP_PROXY: http://proxy.company.com:8080
  HTTPS_PROXY: https://proxy.company.com:8080
  NO_PROXY: localhost,127.0.0.1,.company.com
```

#### Runner Network Setup
```bash
# Configure runner machine
sudo systemctl edit github-runner.service

# Add proxy configuration
[Service]
Environment=HTTP_PROXY=http://proxy.company.com:8080
Environment=HTTPS_PROXY=https://proxy.company.com:8080
Environment=NO_PROXY=localhost,127.0.0.1,.company.com

# Restart service
sudo systemctl daemon-reload
sudo systemctl restart github-runner.service
```

## Network Diagnostics

### Connectivity Testing

#### Basic Network Tests
```bash
# Test internet connectivity
ping -c 3 8.8.8.8
ping -c 3 google.com

# Test DNS resolution
nslookup api.registry.platformio.org
nslookup download.platformio.org
nslookup github.com

# Test HTTP/HTTPS connectivity
curl -I https://api.registry.platformio.org/
curl -I https://download.platformio.org/
curl -I https://github.com/
```

#### PlatformIO Specific Tests
```bash
# Test PlatformIO registry
curl -v https://api.registry.platformio.org/v3/packages

# Test package download
curl -I https://download.platformio.org/packages/

# Test platform repository
curl -I https://github.com/platformio/platform-espressif32.git

# PlatformIO system info
pio system info
```

### Diagnostic Scripts

#### Network Diagnostic Script
```bash
#!/bin/bash
# network_diagnostic.sh

echo "=== FluidNC Network Diagnostic ==="
echo "Date: $(date)"
echo

# Basic connectivity
echo "=== Basic Connectivity ==="
ping -c 3 8.8.8.8 && echo "Internet: OK" || echo "Internet: FAIL"
ping -c 3 google.com && echo "DNS: OK" || echo "DNS: FAIL"

# PlatformIO endpoints
echo "=== PlatformIO Endpoints ==="
endpoints=(
    "api.registry.platformio.org"
    "api.registry.nm1.platformio.org"
    "download.platformio.org"
    "collector.platformio.org"
)

for endpoint in "${endpoints[@]}"; do
    if curl -I -s --connect-timeout 5 "https://$endpoint/" > /dev/null; then
        echo "$endpoint: OK"
    else
        echo "$endpoint: FAIL"
    fi
done

# GitHub endpoints
echo "=== GitHub Endpoints ==="
github_endpoints=(
    "github.com"
    "api.github.com"
    "github-releases.githubusercontent.com"
)

for endpoint in "${github_endpoints[@]}"; do
    if curl -I -s --connect-timeout 5 "https://$endpoint/" > /dev/null; then
        echo "$endpoint: OK"
    else
        echo "$endpoint: FAIL"
    fi
done

# Environment variables
echo "=== Environment Variables ==="
env | grep -E "(HTTP|HTTPS|PROXY|DNS)" || echo "No proxy variables set"

# PlatformIO configuration
echo "=== PlatformIO Configuration ==="
pio settings get || echo "PlatformIO not configured"

echo "=== Diagnostic Complete ==="
```

#### Build Diagnostic Script
```bash
#!/bin/bash
# build_diagnostic.sh

echo "=== FluidNC Build Diagnostic ==="

# Check PlatformIO installation
echo "=== PlatformIO Installation ==="
which pio && echo "PlatformIO: Found" || echo "PlatformIO: Not found"
pio --version || echo "PlatformIO version: Unknown"

# Check platforms
echo "=== Installed Platforms ==="
pio platform list || echo "No platforms installed"

# Check global libraries
echo "=== Global Libraries ==="
pio pkg list --global || echo "No global libraries"

# Check cache
echo "=== Cache Information ==="
if [ -d ~/.platformio ]; then
    echo "Cache size: $(du -sh ~/.platformio | cut -f1)"
else
    echo "No cache directory found"
fi

# Test build
echo "=== Build Test ==="
pio run -e wifi --dry-run || echo "Build test failed"

echo "=== Build Diagnostic Complete ==="
```

## Troubleshooting Guide

### Common Issues and Solutions

#### Issue: HTTPClientError during build
```
HTTPClientError:
Tool Manager: Installing espressif/toolchain-xtensa-esp32 @ 8.4.0+2021r2-patch5
```

**Solutions:**
1. Use offline build system
2. Configure proxy settings
3. Add endpoints to firewall allow list
4. Use alternative DNS servers

#### Issue: SSL Certificate errors
```
SSL: CERTIFICATE_VERIFY_FAILED
```

**Solutions:**
```bash
# Configure Git SSL
git config --global http.sslVerify true
git config --global http.sslCAInfo /etc/ssl/certs/ca-certificates.crt

# Configure pip SSL
pip config set global.trusted-host pypi.org
pip config set global.trusted-host files.pythonhosted.org

# Configure PlatformIO SSL
pio settings set ssl_ca_path /etc/ssl/certs/ca-certificates.crt
```

#### Issue: Timeout during package downloads
```
Error: Request timeout
```

**Solutions:**
```bash
# Increase timeout values
export PLATFORMIO_DOWNLOAD_TIMEOUT=300
export PLATFORMIO_INSTALL_TIMEOUT=600

# Use retry mechanism
./build-offline.sh --offline your-target
```

### Corporate Environment Solutions

#### Solution 1: Offline Development
```bash
# On machine with internet access
./build-offline.sh --cache

# Transfer .offline_cache to restricted machine
tar -czf offline_cache.tar.gz .offline_cache/
scp offline_cache.tar.gz user@restricted-machine:~/

# On restricted machine
tar -xzf offline_cache.tar.gz
./build-offline.sh --offline wifi
```

#### Solution 2: Proxy Configuration
```bash
# Configure all proxy settings
export HTTP_PROXY=http://proxy.company.com:8080
export HTTPS_PROXY=https://proxy.company.com:8080
export NO_PROXY=localhost,127.0.0.1,.company.com

# Configure Git proxy
git config --global http.proxy $HTTP_PROXY
git config --global https.proxy $HTTPS_PROXY

# Configure pip proxy
pip config set global.proxy $HTTP_PROXY

# Test configuration
./network_diagnostic.sh
```

#### Solution 3: Local Package Mirror
```bash
# Set up local PyPI mirror
pip install bandersnatch
bandersnatch mirror

# Configure pip to use local mirror
pip config set global.index-url http://local-mirror:8080/simple/
pip config set global.trusted-host local-mirror

# Set up local PlatformIO mirror
# This requires custom infrastructure
```

## Best Practices

### Development Environment
1. **Always test offline build** before deploying to restricted environments
2. **Keep offline cache updated** regularly
3. **Document network requirements** for your specific setup
4. **Use version control** for configuration files

### Corporate Environments
1. **Work with IT teams** to configure firewall rules
2. **Use self-hosted runners** for CI/CD
3. **Implement proper proxy configuration**
4. **Consider local package mirrors** for large teams

### Security Considerations
1. **Validate downloaded packages** using checksums
2. **Use trusted sources** for dependencies
3. **Monitor network traffic** for security compliance
4. **Keep certificates updated**

## Monitoring and Maintenance

### Regular Checks
```bash
# Weekly cache update
./build-offline.sh --cache

# Monthly network diagnostic
./network_diagnostic.sh > diagnostic_$(date +%Y%m%d).log

# Quarterly firewall rule review
# Review and update allow list
```

### Automated Monitoring
```bash
# Add to crontab for automated checks
0 6 * * 1 /path/to/build-offline.sh --cache >> /var/log/fluidnc-cache.log 2>&1
0 8 * * * /path/to/network_diagnostic.sh >> /var/log/fluidnc-network.log 2>&1
```

---

**This guide provides comprehensive solutions for firewall and network configuration issues. Choose the appropriate solution based on your specific network environment and security requirements.**

---

**Last Updated**: July 2024  
**Next Review**: August 2024