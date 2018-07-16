//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#ifndef SOC_APP_MISSION_10_BLUE_GATE_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "stdio.h"

#define SOC_APP_MISSION_10_BLUE_GATE_H

#define MISSION_10_1_BLUE_GATE_RATIO 5

#define MISSION_10_2_ERROR_BETWEEN_COLS 5

#define MISSION_10_3_UPPER_BLUE_GATE_SLOPE 7.5
#define MISSION_10_3_UPPER_BLUE_GATE_SLOPE_ERROR 3

void mission_10_1_watch_up(void);

void mission_10_1_front_walk(int repeat);

int mission_10_1_catch_blue_gate(U16 *image);

int mission_10_2_set_center_upper_gate(U16 *image);

int mission_10_3_set_straight_upper_gate(U16 *image);

int mission_10_4_escape_blue_gate();

#endif //SOC_APP_MISSION_10_BLUE_GATE_H