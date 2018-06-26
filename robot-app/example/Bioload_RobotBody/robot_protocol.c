/************************************************************************
  Title     : Robot Body Protocol Source File
  File name : robot_protocol.c    

  Author    : adc inc. (oxyang@adc.co.kr)
  History
		+ v0.0  2007/2/14
		+ v1.0  2008/8/6
************************************************************************/
#include <stdio.h>
#include <string.h>
#include "robot_protocol.h"
#include "uart_api.h"
//////////////////////////////////////////////////// Protocol Test

void DelayLoop(int delay_time)
{
	while (delay_time)
		delay_time--;
}

void Send_Command(unsigned char Ldata, unsigned char Ldata1)
{
	unsigned char Command_Buffer[6] = {
		0,
	};

	Command_Buffer[0] = START_CODE;  // Start Byte -> 0xff
	Command_Buffer[1] = START_CODE1; // Start Byte1 -> 0x55
	Command_Buffer[2] = Ldata;
	Command_Buffer[3] = Ldata1;
	Command_Buffer[4] = Hdata;  // 0x00
	Command_Buffer[5] = Hdata1; // 0xff

	uart1_buffer_write(Command_Buffer, 6);
}

void Read_TxD()
{
	unsigned char Read_TxD[6] = {
		0,
	};

	uart1_buffer_read(Read_TxD, 6);

	for (int i = 0; i < 6; i++)
	{
		printf("%c\t", Read_TxD[i]);
	}
	printf("\n");
}

#define ERROR 0
#define OK 1
void init()
{
	Send_Command(0x01, 0xfe);
	Read_TxD();
}

void InitSlow()
{
	Send_Command(0x02, 0xfd);
	Read_TxD();
}
