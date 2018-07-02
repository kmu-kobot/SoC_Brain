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

void DelayLoop(int delay_time) {
    while (delay_time)
        delay_time--;
}

void Send_Command(unsigned char Ldata) {
    unsigned char Command_Buffer[6] = {
            0,
    };

    Command_Buffer[0] = START_CODE;  // Start Byte -> 0xff
    Command_Buffer[1] = START_CODE1; // Start Byte1 -> 0x55
    Command_Buffer[2] = Ldata;
    Command_Buffer[3] = ~Ldata;
    Command_Buffer[4] = Hdata;  // 0x00
    Command_Buffer[5] = Hdata1; // 0xff

    uart1_buffer_write(Command_Buffer, 6);
}

unsigned char Receive_Ack(int status) {
    unsigned char command[6] = {
            0,
    };
    uart1_buffer_read(command, 6);

    printf("[%d] buf_uart1\t", status);

    int i = 0;
    for (i = 0; i < 6; ++i) {
        printf("%d\t", command[i]);
    }
    printf("\n");

    return command[2];
}


#define ERROR 0
#define OK 1

void RobotAction(unsigned char Ldata) {
    Send_Command(Ldata);
    while (Receive_Ack(1) != 4);
}