// Copyright (c) 2024 - FluidNC Contributors
// Use of this source code is governed by a GPLv3 license that can be found in the LICENSE file.

#include <stdio.h>
#include <Driver/fluidnc_uart.h>
#include "platform.h"

#ifdef STM32

#include "src/Protocol.h"

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

void uart_set_baudrate(int uart_num, uint32_t baudrate) {
    if (uart_num < UART_NUM_MAX && uart_instances[uart_num]) {
        uart_instances[uart_num]->end();
        uart_instances[uart_num]->begin(baudrate);
    }
}

int uart_read_bytes(int uart_num, uint8_t* buf, uint32_t length, uint32_t timeout_ms) {
    if (uart_num >= UART_NUM_MAX || !uart_instances[uart_num]) {
        return 0;
    }
    
    return uart_instances[uart_num]->readBytes(buf, length);
}

int uart_write_bytes(int uart_num, const uint8_t* buf, uint32_t length) {
    if (uart_num >= UART_NUM_MAX || !uart_instances[uart_num]) {
        return 0;
    }
    
    return uart_instances[uart_num]->write(buf, length);
}

void uart_flush(int uart_num) {
    if (uart_num < UART_NUM_MAX && uart_instances[uart_num]) {
        uart_instances[uart_num]->flush();
    }
}

#endif // STM32