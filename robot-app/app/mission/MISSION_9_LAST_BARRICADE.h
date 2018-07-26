//
// Created by Gihyeon Yang on 2018. 7. 13..
//

#ifndef SOC_APP_MISSION_9_LAST_BARRICADE_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "stdio.h"

#define SOC_APP_MISSION_9_LAST_BARRICADE_H

#define MISSION_9_UPPER 0
#define MISSION_9_RANGE 100
#define MISSION_9_THRESHOLDS 10

int mission_9_1_wait_yellow_barricade(U16 *image);

int mission_9_2_end_yellow_barricade(U16 *image);

void mission_9_3_escape_yellow_barricade(int repeat);

#endif //SOC_APP_MISSION_9_LAST_BARRICADE_H
