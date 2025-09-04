#include "serial.h"
#include <jo/jo.h>


int g_SerialInitialized = 0;

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
void jo_serial_init(void)
{
    if(g_SerialInitialized == 1)
    {
        // already initialized
        return;
    }
    
    SERIAL_REG_SCR = 0;
    SERIAL_REG_SMR = 0;

    SERIAL_REG_BRR = 123; // TODO: no clue what I should put here
 
    /* Delay for at least one bit. */
    for(unsigned int i = 0; i < 64; i++)
    {
        asm("nop");
    }
    
    SERIAL_REG_SCR |= (SERIAL_BIT_TE|SERIAL_BIT_RE);

    g_SerialInitialized = 1;

    return;
}

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
int jo_serial_send_byte(unsigned char data)
{
    if(!g_SerialInitialized)
    {
        return -1;
    }

    if(!(SERIAL_REG_SSR & SERIAL_BIT_TDRE))
    {
        // not ready to send
        return -2;
    }
   
    SERIAL_REG_TDR = data;
    SERIAL_REG_SSR &= ~SERIAL_BIT_TDRE;

    return 0;
}

/*
 * receiving
 * 1) Read ORER, PER, and FER
bits in SSR
2) RDRF = 1?
3) Read receive data in RDR, and
clear RDRF bit in SSR to 0
*/
int jo_serial_recv_byte(unsigned char* data)
{
    if(!g_SerialInitialized)
    {
        return -1;
    }

    // TODO error checking

    
    if(SERIAL_REG_SSR & ORER)
    {
    	jo_printf(1, 25, "Resetting ORER");
        SERIAL_REG_SSR &= ~ORER;
    }

    if(SERIAL_REG_SSR & FER)
    {
    	jo_printf(1, 26, "Resetting FER");
        SERIAL_REG_SSR &= ~FER;
    }


    if(SERIAL_REG_SSR & PER)
    {
    	jo_printf(1, 27, "Resetting PER");
        SERIAL_REG_SSR &= ~PER;
    }

    if(!(SERIAL_REG_SSR & SERIAL_BIT_RDRF))
    {
        // not ready to send
        return -2;
    }   

    *data = (SERIAL_REG_RDR); // TODO: why is the top bit always set?
    jo_printf(1, 28, "recv: %02x %02x", *data, (*data & 0x7f));

    SERIAL_REG_SSR &= ~SERIAL_BIT_RDRF;
    
    return 0;
}
