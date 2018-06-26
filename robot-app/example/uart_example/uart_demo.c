#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "amazon2_uart123.h"

#define DEVICE_FILENAME		"/dev/uart123"
#define DEMO_UART1			UART1
#define DEMO_UART2			UART3

static int uart_config(int dev, int uart_ch, int baud, int bits, int parity, int stops)
{
	struct uart_config u_config;
	int ret;

	u_config.uart_ch = uart_ch;
	u_config.baud = baud;
	u_config.bits = bits;
	u_config.parity = parity;
	u_config.stops = stops;

	ret = ioctl(dev, UART_SET_CONFIG, u_config);

	return ret;
}

int main(void)
{
	int dev;
	int ret;
	unsigned char buf_uart1[512], buf_uart2[512];
	struct uart_data u_data1, u_data2;
	int uart1_len, uart2_len;
	int i;

	printf("================================================================\n");
	printf("              <<<< UART Echo Demo Program >>>>               \n");
	printf("----------------------------------------------------------------\n");
	printf("  Exit : Ctrl-C                                              \n");
	printf("================================================================\n");

	dev = open(DEVICE_FILENAME, O_RDWR|O_NDELAY);
	if(dev < 0) {
		printf("ERR : %s file open error\n", DEVICE_FILENAME);
		return -1;
	}

	ret = uart_config(dev, DEMO_UART1, 9600, 8, UART_PARNONE, 1);
	if(ret < 0) {
		printf("ERR : %s uart config error\n", DEMO_UART1);
		return -1;
	}

	ret = uart_config(dev, DEMO_UART2, 9600, 8, UART_PARNONE, 1);
	if(ret < 0) {
		printf("ERR : %s uart config error\n", DEMO_UART2);
		return -1;
	}

	u_data1.uart_ch = DEMO_UART1;
	u_data1.uart_buf = buf_uart1;

	u_data2.uart_ch = DEMO_UART2;
	u_data2.uart_buf = buf_uart2;

	while(1) {
		if(uart1_len = ioctl(dev, UART_RXBUF_LEVEL, DEMO_UART1) > 0) {
			u_data1.buf_size = uart1_len;
			ioctl(dev, UART_DATA_READ, u_data1);
			ioctl(dev, UART_DATA_WRITE, u_data1);
		}

		if(uart2_len = ioctl(dev, UART_RXBUF_LEVEL, DEMO_UART2) > 0) {
			u_data2.buf_size = uart2_len;
			ioctl(dev, UART_DATA_READ, u_data2);
			ioctl(dev, UART_DATA_WRITE, u_data2);
		}
	}

	return -1;
}


