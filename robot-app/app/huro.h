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
// AVOID BOMB
#include "./mission/MISSION_4_JUMP_HURDLE.h"
#include "./mission/MISSION_5_GREEN_BRIDGE.h"
// KICK BALL
#include "./mission/MISSION_7_YELLOW_HOLE_BRIDGE.h"
// AVOID BOMB
#include "mission/MISSION_9_LAST_BARRICADE.h"
// BLUE GATE

#include <termios.h>

#define SOC_APP_HURO_H

static struct termios inittio, newtio;

int huro(void);

int init_huro(void);

void destroy_huro(U16 *buf);

void setFPGAVideoData(U16 *buf);

#endif //SOC_APP_HURO_H
