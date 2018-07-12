//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#ifndef SOC_APP_MISSION_1_YELLOW_BARRICADE_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "stdio.h"

#define SOC_APP_MISSION_1_YELLOW_BARRICADE_H

#define MISSION_1_UPPER 60
#define MISSION_1_RANGE 30

#define MISSION_1_THRESHOLDS 30

int mission_1_1_wait_yellow_barricade(U16 *image);

int mission_1_2_end_yellow_barricade(U16 *image);

void mission_1_3_escape_yellow_barricade(int repeat);

#endif //SOC_APP_MISSION_1_YELLOW_BARRICADE_H
