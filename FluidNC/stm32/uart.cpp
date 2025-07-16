// Copyright (c) 2024 - FluidNC Contributors
// Use of this source code is governed by a GPLv3 license that can be found in the LICENSE file.

#include <stdio.h>
#include <Driver/fluidnc_uart.h>
#include "platform.h"

#ifdef STM32

#include "src/Protocol.h"
#include "src/UartTypes.h"

const int PINNUM_MAX = 64;
const int UART_NUM_MAX = 4; // STM32 typical UART count

InputPin* objects[UART_NUM_MAX][PINNUM_MAX] = { nullptr };
uint8_t   last[UART_NUM_MAX] = { 0 };
HardwareSerial* uart_instances[UART_NUM_MAX] = { nullptr };

void uart_data_callback(int uart_num, uint8_t* buf, int* len) {
    int in_len = *len;
    int in, out;
    for (in = 0, out = 0; in < in_len; in++, out++) {
        uint8_t c = buf[in];
        if (out != in) {
            buf[out] = c;
        }
        if (last[uart_num]) {
            --out;
            uint8_t pinnum = c & (PINNUM_MAX - 1);
            protocol_send_event_from_ISR(last[uart_num] == 0xc4 ? &pinInactiveEvent : &pinActiveEvent, 
                                       (void*)objects[uart_num][pinnum]);
            last[uart_num] = 0;
        } else {
            if (c == 0xc4 || c == 0xc5) {
                --out;
                last[uart_num] = c;
            }
        }
    }
    *len = out;
}

void uart_register_input_pin(int uart_num, uint8_t pinnum, InputPin* object) {
    if (uart_num < UART_NUM_MAX && pinnum < PINNUM_MAX) {
        objects[uart_num][pinnum] = object;
        last[uart_num] = 0;
    }
}

void uart_init(int uart_num) {
    if (uart_num >= UART_NUM_MAX) {
        return;
    }
    
    // Initialize UART instance based on uart_num
    switch (uart_num) {
        case 0:
            uart_instances[uart_num] = &Serial;
            break;
        case 1:
            uart_instances[uart_num] = &Serial1;
            break;
        case 2:
            uart_instances[uart_num] = &Serial2;
            break;
        case 3:
            uart_instances[uart_num] = &Serial3;
            break;
        default:
            return;
    }
    
    // Basic UART setup - baudrate would be configured elsewhere
    if (uart_instances[uart_num]) {
        uart_instances[uart_num]->begin(115200);
    }
}

void uart_mode(int uart_num, unsigned long baud, UartData dataBits, UartParity parity, UartStop stopBits) {
    if (uart_num >= UART_NUM_MAX || !uart_instances[uart_num]) {
        return;
    }
    
    // Configure UART mode
    uint32_t config = SERIAL_8N1; // Default
    
    // Set data bits
    switch (dataBits) {
        case UartData::Bits7:
            config = SERIAL_7N1;
            break;
        case UartData::Bits8:
            config = SERIAL_8N1;
            break;
        case UartData::Bits9:
            // Not supported in Arduino STM32
            config = SERIAL_8N1;
            break;
    }
    
    // Set parity
    switch (parity) {
        case UartParity::None:
            // Already set in config
            break;
        case UartParity::Even:
            config = (config & ~SERIAL_PARITY_MASK) | SERIAL_PARITY_EVEN;
            break;
        case UartParity::Odd:
            config = (config & ~SERIAL_PARITY_MASK) | SERIAL_PARITY_ODD;
            break;
    }
    
    // Set stop bits
    switch (stopBits) {
        case UartStop::Bits1:
            // Already set in config
            break;
        case UartStop::Bits2:
            config = (config & ~SERIAL_STOP_BIT_MASK) | SERIAL_STOP_BIT_2;
            break;
    }
    
    uart_instances[uart_num]->begin(baud, config);
}

bool uart_half_duplex(int uart_num) {
    // STM32 UART half-duplex support
    return false; // Not implemented
}

int uart_read(int uart_num, uint8_t* buf, int len, int timeout_ms) {
    if (uart_num >= UART_NUM_MAX || !uart_instances[uart_num]) {
        return 0;
    }
    
    uart_instances[uart_num]->setTimeout(timeout_ms);
    return uart_instances[uart_num]->readBytes(buf, len);
}

int uart_write(int uart_num, const uint8_t* buf, int len) {
    if (uart_num >= UART_NUM_MAX || !uart_instances[uart_num]) {
        return 0;
    }
    
    return uart_instances[uart_num]->write(buf, len);
}

void uart_xon(int uart_num) {
    if (uart_num < UART_NUM_MAX && uart_instances[uart_num]) {
        uart_instances[uart_num]->write(0x11); // XON character
    }
}

void uart_xoff(int uart_num) {
    if (uart_num < UART_NUM_MAX && uart_instances[uart_num]) {
        uart_instances[uart_num]->write(0x13); // XOFF character
    }
}

void uart_sw_flow_control(int uart_num, bool on, int xon_threshold, int xoff_threshold) {
    // Software flow control implementation
    // This would need to be implemented based on buffer levels
    (void)uart_num;
    (void)on;
    (void)xon_threshold;
    (void)xoff_threshold;
}

bool uart_pins(int uart_num, int tx_pin, int rx_pin, int rts_pin, int cts_pin) {
    if (uart_num >= UART_NUM_MAX || !uart_instances[uart_num]) {
        return false;
    }
    
    // STM32 pin configuration for UART
    // This would need to be implemented based on STM32 HAL
    (void)tx_pin;
    (void)rx_pin;
    (void)rts_pin;
    (void)cts_pin;
    
    return true;
}

int uart_buflen(int uart_num) {
    if (uart_num >= UART_NUM_MAX || !uart_instances[uart_num]) {
        return 0;
    }
    
    return uart_instances[uart_num]->available();
}

void uart_discard_input(int uart_num) {
    if (uart_num < UART_NUM_MAX && uart_instances[uart_num]) {
        while (uart_instances[uart_num]->available()) {
            uart_instances[uart_num]->read();
        }
    }
}

bool uart_wait_output(int uart_num, int timeout_ms) {
    if (uart_num >= UART_NUM_MAX || !uart_instances[uart_num]) {
        return false;
    }
    
    uart_instances[uart_num]->flush();
    return true;
}

#endif // STM32