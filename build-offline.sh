#!/bin/bash
# FluidNC Offline Build Script
# This script helps build FluidNC in environments with restricted internet access

set -e

# Configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$SCRIPT_DIR"
CACHE_DIR="$PROJECT_DIR/.offline_cache"
LOG_FILE="$PROJECT_DIR/offline_build.log"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Logging function
log() {
    echo -e "${GREEN}[$(date '+%Y-%m-%d %H:%M:%S')] $1${NC}" | tee -a "$LOG_FILE"
}

warn() {
    echo -e "${YELLOW}[$(date '+%Y-%m-%d %H:%M:%S')] WARNING: $1${NC}" | tee -a "$LOG_FILE"
}

error() {
    echo -e "${RED}[$(date '+%Y-%m-%d %H:%M:%S')] ERROR: $1${NC}" | tee -a "$LOG_FILE"
}

# Help function
show_help() {
    cat << EOF
FluidNC Offline Build Script

Usage: $0 [OPTIONS] [ENVIRONMENT]

OPTIONS:
    -h, --help          Show this help message
    -c, --cache         Create offline cache
    -o, --offline       Enable offline mode
    -v, --verbose       Enable verbose output
    -e, --environment   Specify build environment
    --clean             Clean build cache
    --list-envs         List available environments

ENVIRONMENTS:
    noradio            ESP32 without radio features
    wifi               ESP32 with WiFi support  
    bt                 ESP32 with Bluetooth support
    stm32_noradio      STM32 without radio features
    stm32_basic        STM32 basic configuration
    stm32_advanced     STM32 advanced configuration
    all                Build all environments

EXAMPLES:
    $0 --cache                    # Create offline cache
    $0 --offline noradio          # Build noradio environment offline
    $0 --offline all              # Build all environments offline
    $0 --clean                    # Clean build cache

EOF
}

# Check dependencies
check_dependencies() {
    log "Checking dependencies..."
    
    if ! command -v python3 &> /dev/null; then
        error "Python3 is required but not installed"
        exit 1
    fi
    
    if ! command -v pip3 &> /dev/null; then
        error "pip3 is required but not installed"
        exit 1
    fi
    
    # Check if PlatformIO is installed
    if ! command -v pio &> /dev/null; then
        warn "PlatformIO not found, attempting to install..."
        pip3 install platformio
    fi
    
    log "Dependencies check completed"
}

# Create offline cache
create_cache() {
    log "Creating offline cache..."
    
    mkdir -p "$CACHE_DIR"
    
    # Create requirements cache
    if [ -f "$PROJECT_DIR/requirements.txt" ]; then
        log "Downloading Python requirements..."
        pip3 download -r "$PROJECT_DIR/requirements.txt" -d "$CACHE_DIR/pip_cache" || warn "Failed to download some Python requirements"
    fi
    
    # Pre-install PlatformIO platforms and tools
    log "Pre-installing PlatformIO platforms..."
    
    # ESP32 platform
    pio platform install espressif32 || warn "Failed to install ESP32 platform"
    
    # STM32 platform
    pio platform install ststm32 || warn "Failed to install STM32 platform"
    
    # Pre-install common libraries
    log "Pre-installing common libraries..."
    
    # Install libraries globally to cache them
    pio pkg install --global --library "TMCStepper@>=0.7.0,<1.0.0" || warn "Failed to install TMCStepper"
    pio pkg install --global --library "thingpulse/ESP8266 and ESP32 OLED driver for SSD1306 displays@4.4.1" || warn "Failed to install OLED driver"
    
    # Create cache manifest
    cat > "$CACHE_DIR/cache_manifest.txt" << EOF
# FluidNC Offline Cache Manifest
# Generated: $(date)

Python Requirements:
$(ls -la "$CACHE_DIR/pip_cache/" 2>/dev/null || echo "No pip cache found")

PlatformIO Platforms:
$(pio platform list 2>/dev/null || echo "No platforms found")

PlatformIO Libraries:
$(pio pkg list --global 2>/dev/null || echo "No global libraries found")
EOF
    
    log "Offline cache created successfully"
}

# Configure offline mode
configure_offline() {
    log "Configuring offline mode..."
    
    # Set PlatformIO offline settings
    export PLATFORMIO_OFFLINE_MODE=true
    export PLATFORMIO_DOWNLOAD_TIMEOUT=10
    export PLATFORMIO_INSTALL_TIMEOUT=10
    
    # Configure pip to use offline cache
    export PIP_FIND_LINKS="file://$CACHE_DIR/pip_cache"
    export PIP_NO_INDEX=true
    
    log "Offline mode configured"
}

# Clean build cache
clean_cache() {
    log "Cleaning build cache..."
    
    # Clean PlatformIO cache
    pio system prune --force || warn "Failed to prune PlatformIO cache"
    
    # Clean project build directory
    if [ -d "$PROJECT_DIR/.pio" ]; then
        rm -rf "$PROJECT_DIR/.pio"
        log "Removed .pio directory"
    fi
    
    # Clean offline cache if requested
    if [ "$1" = "--all" ]; then
        if [ -d "$CACHE_DIR" ]; then
            rm -rf "$CACHE_DIR"
            log "Removed offline cache directory"
        fi
    fi
    
    log "Cache cleaned successfully"
}

# List available environments
list_environments() {
    log "Available build environments:"
    
    # Parse platformio.ini for environments
    if [ -f "$PROJECT_DIR/platformio.ini" ]; then
        grep -E '^\[env:' "$PROJECT_DIR/platformio.ini" | sed 's/\[env:\(.*\)\]/  \1/' | sort
    else
        error "platformio.ini not found"
        exit 1
    fi
}

# Build environment
build_environment() {
    local env="$1"
    local verbose=""
    
    if [ "$VERBOSE" = "true" ]; then
        verbose="-v"
    fi
    
    log "Building environment: $env"
    
    # Build with retry mechanism
    local max_attempts=3
    local attempt=1
    
    while [ $attempt -le $max_attempts ]; do
        log "Build attempt $attempt of $max_attempts..."
        
        if pio run -e "$env" $verbose; then
            log "Build successful for environment: $env"
            return 0
        else
            warn "Build failed for environment: $env (attempt $attempt)"
            
            if [ $attempt -eq $max_attempts ]; then
                error "All build attempts failed for environment: $env"
                return 1
            else
                log "Waiting before retry..."
                sleep 10
            fi
        fi
        
        attempt=$((attempt + 1))
    done
}

# Build all environments
build_all() {
    log "Building all environments..."
    
    local environments=("noradio" "wifi" "bt" "stm32_noradio" "stm32_basic" "stm32_advanced")
    local failed_builds=()
    
    for env in "${environments[@]}"; do
        if build_environment "$env"; then
            log "✓ $env build successful"
        else
            error "✗ $env build failed"
            failed_builds+=("$env")
        fi
    done
    
    if [ ${#failed_builds[@]} -eq 0 ]; then
        log "All environments built successfully!"
    else
        error "Failed to build: ${failed_builds[*]}"
        exit 1
    fi
}

# Main function
main() {
    # Initialize log file
    echo "FluidNC Offline Build Script - $(date)" > "$LOG_FILE"
    
    # Parse command line arguments
    OFFLINE_MODE=false
    VERBOSE=false
    ENVIRONMENT=""
    
    while [[ $# -gt 0 ]]; do
        case $1 in
            -h|--help)
                show_help
                exit 0
                ;;
            -c|--cache)
                check_dependencies
                create_cache
                exit 0
                ;;
            -o|--offline)
                OFFLINE_MODE=true
                shift
                ;;
            -v|--verbose)
                VERBOSE=true
                shift
                ;;
            -e|--environment)
                ENVIRONMENT="$2"
                shift 2
                ;;
            --clean)
                clean_cache "$2"
                exit 0
                ;;
            --list-envs)
                list_environments
                exit 0
                ;;
            all)
                ENVIRONMENT="all"
                shift
                ;;
            *)
                if [ -z "$ENVIRONMENT" ]; then
                    ENVIRONMENT="$1"
                fi
                shift
                ;;
        esac
    done
    
    # Check if we're in the right directory
    if [ ! -f "$PROJECT_DIR/platformio.ini" ]; then
        error "platformio.ini not found. Are you in the FluidNC project directory?"
        exit 1
    fi
    
    # Check dependencies
    check_dependencies
    
    # Configure offline mode if requested
    if [ "$OFFLINE_MODE" = "true" ]; then
        configure_offline
    fi
    
    # Build based on environment
    if [ "$ENVIRONMENT" = "all" ]; then
        build_all
    elif [ -n "$ENVIRONMENT" ]; then
        build_environment "$ENVIRONMENT"
    else
        error "No environment specified. Use --help for usage information."
        exit 1
    fi
    
    log "Build script completed successfully"
}

# Run main function
main "$@"