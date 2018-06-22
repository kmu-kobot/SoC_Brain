/*
 * amazon2_uart.h
 */

#ifndef _AMAZON2_UART123_H_
#define _AMAZON2_UART123_H_

#define	UART1		1
#define	UART2		2
#define	UART3		3

#define UART_PARNONE	0	/* Defines for setting parity */
#define UART_PARODD		1
#define UART_PAREVEN	2

#define	UART_NO_ERR	    0	/* Function call was successful */
#define	UART_BAD_CH	    -1	/* Invalid communications port channel*/
#define	UART_RX_EMPTY	-2	/* Rx buffer is empty, no character available */
#define	UART_TX_FULL	-3	/* Tx buffer is full, could not deposit char */
#define	UART_TX_EMPTY	-4	/* If the Tx buffer is empty. */

struct uart_data {
    int uart_ch; // channel: UART1, UART2, UART3
    int buf_size;
    unsigned char *uart_buf;
};

struct uart_config {
    int uart_ch; // channel: UART1, UART2, UART3
    int baud;    // 115200, 57600, 38400, 19200, 9600, 4800, 2400, 1200
    int bits;    // 8
    int parity;  // UART_PARNONE, UART_PARODD, UART_PAREVEN
    int stops;   // 1, 2
};

/* Function command codes for io_ctl. */
#define UART_SET_CONFIG     0x6000 // use struct uart_config
#define UART_RXBUF_LEVEL    0x6001 // arg: channel
#define UART_DATA_READ      0x6002 // use struct uart_data
#define UART_TXBUF_FULL     0x6003
#define UART_DATA_WRITE     0x6004 // use struct uart_data
#define UART_RXBUF_CLEAR    0x6005
#define UART_TXBUF_CLEAR    0x6006
#define UART_TXBUF_LEVEL    0x6007

#endif // _AMAZON2_UART123_H_
