//
// Created by Gihyeon Yang on 2018. 7. 2..
//

#ifndef SOC_APP_HURO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>

#include "./library/graphic_api.h"
#include "./library/robot_protocol.h"
#include "./library/uart_api.h"

#include <termios.h>

static struct termios inittio, newtio;

#define SOC_APP_HURO_H

#define WIDTH 180
#define HEIGHT 120

int huro(void);

int init_huro(void);

void destroy_huro(U16 *buf);

void setFPGAVideoData(U16 *buf);

#endif //SOC_APP_HURO_H
