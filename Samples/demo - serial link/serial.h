#pragma once

// serial registers
#define SERIAL_SMR (volatile unsigned char*)(0xFFFFFE00) // serial mode register
#define SERIAL_BRR (volatile unsigned char*)(0xFFFFFE01) // bit rate register
#define SERIAL_SCR (volatile unsigned char*)(0xFFFFFE02) // serial control register
#define SERIAL_TDR (volatile unsigned char*)(0xFFFFFE03) // transmit data register
#define SERIAL_SSR (volatile unsigned char*)(0xFFFFFE04) // serial status register
#define SERIAL_RDR (volatile unsigned char*)(0xFFFFFE05) // receive data register

/*
 Initializing
1. Select the communication format in the serial mode register (SMR).
2. Write the value corresponding to the bit rate in the bit rate register (BRR) unless an external
clock is used.
3. Select the clock source in the serial control register (SCR). Leave RIE, TIE, TEIE, MPIE, TE
and RE cleared to 0. If clock output is selected in asynchronous mode, clock output starts
immediately after the setting is made in SCR.
4. Wait for at least the interval required to transmit or receive one bit, then set TE or RE in the
serial control register (SCR) to 1. Also set RIE, TIE, TEIE and MPIE as necessary. Setting TE
or RE enables the SCI to use the TxD or RxD pin. The initial states are the mark state when
transmitting, and the idle state when receiving (waiting for a start bit).
*/

/*
 * transmitting
Transmitting Serial Data (Asynchronous Mode): Figure 13.5 shows a sample flowchart for
transmitting serial data. The procedure for transmitting serial data is as follows:
1. SCI status check and transmit data write: read the serial status register (SSR), check that the
TDRE bit is 1, then write transmit data in the transmit data register (TDR) and clear TDRE
to 0.
2. To continue transmitting serial data, read the TDRE bit to check whether it is safe to write (if it
reads 1); if so, write data in TDR, then clear TDRE to 0. When the DMAC is started by a
transmit-data-empty interrupt request (TXI) in order to write data in TDR, the TDRE bit is
checked and cleared automatically.
*/

/*
 * receiving
 * 1) Read ORER, PER, and FER
bits in SSR
2) RDRF = 1?
3) Read receive data in RDR, and
clear RDRF bit in SSR to 0
*/
