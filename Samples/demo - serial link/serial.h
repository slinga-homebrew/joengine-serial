#pragma once

// serial registers
#define SERIAL_REG_SMR (*(volatile unsigned char*)(0xFFFFFE00)) // serial mode register
#define SERIAL_REG_BRR (*(volatile unsigned char*)(0xFFFFFE01)) // bit rate register
#define SERIAL_REG_SCR (*(volatile unsigned char*)(0xFFFFFE02)) // serial control register
#define SERIAL_REG_TDR (*(volatile unsigned char*)(0xFFFFFE03)) // transmit data register
#define SERIAL_REG_SSR (*(volatile unsigned char*)(0xFFFFFE04)) // serial status register
#define SERIAL_REG_RDR (*(volatile unsigned char*)(0xFFFFFE05)) // receive data register

// serial register bit fields
#define SERIAL_BIT_TDRE     (1<<7)
#define SERIAL_BIT_RDRF     (1<<6)

#define SERIAL_BIT_TE       (1<<5)
#define SERIAL_BIT_RE       (1<<4)

#define ORER                (1<<5)
#define FER                 (1<<4)
#define PER                 (1<<3)

void jo_serial_init(void);
int jo_serial_send_byte(unsigned char data);
int jo_serial_recv_byte(unsigned char* data);
