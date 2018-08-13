//
// Created by Gihyeon Yang on 2018. 7. 17..
//

#ifndef SOC_APP_MISSION_3_AVOID_BOMB_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "./../library/imageProcess.h"
#include "DEFAULT.h"
#include "stdio.h"
#include "math.h"

#define SOC_APP_MISSION_3_AVOID_BOMB_H

#define MISSION_3_DEFAULT_BOMB_THRESHOLDS 8
#define MISSION_3_DEFAULT_LINE_THRESHOLDS 30

void mission_3_4_watch_side(void);

int mission_3_isFrontOf_Blue(U16 *image);

int mission_3_measure_line(U16 *image);

void mission_3_init_global(void);

int mission_3_avoid(U16 *image);

void mission_3_default_watch_below(U16 *image, int repeat);

int mission_3_default_avoid_bomb(U16 *image);

void mission_3_4_watch_front(void);

int mission_3_4_getMDir(void);

int mission_3_1_ver2(U16 *image);

#endif //SOC_APP_MISSION_3_AVOID_BOMB_H
