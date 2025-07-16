// Copyright (c) 2024 - FluidNC Contributors
// Use of this source code is governed by a GPLv3 license that can be found in the LICENSE file.

#include "Driver/restart.h"
#include "platform.h"

#ifdef STM32

#include <Arduino.h>

void restart_system() {
    // Perform system restart on STM32
    NVIC_SystemReset();
}

void restart_into_bootloader() {
    // Restart into STM32 bootloader
    // This would typically involve setting a flag in backup RAM
    // and then performing a system reset
    
    // For now, just do a normal restart
    NVIC_SystemReset();
}

void restart_with_delay(uint32_t delay_ms) {
    // Delay before restart
    delay(delay_ms);
    restart_system();
}

#endif // STM32