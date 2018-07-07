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
#include "./library/uart_api.h"
#include "./library/robot_action.h"

#include "./mission/MISSION_0_READY_PLAY.h"
#include "./mission/MISSION_1_YELLOW_BARRICADE.h"
#include "./mission/MISSION_2_RED_BRIDGE.h"

#include <termios.h>

#define SOC_APP_HURO_H

static struct termios inittio, newtio;

int huro(void);

int init_huro(void);

void destroy_huro(U16 *buf);

void setFPGAVideoData(U16 *buf);

#endif //SOC_APP_HURO_H
