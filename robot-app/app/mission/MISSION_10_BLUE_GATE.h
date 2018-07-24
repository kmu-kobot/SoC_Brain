//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#ifndef SOC_APP_MISSION_10_BLUE_GATE_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "stdio.h"

#define SOC_APP_MISSION_10_BLUE_GATE_H

#define MISSION_10_1_BLUE_GATE_RATIO 15

#define MISSION_10_1_COL_POINT_1 80
#define MISSION_10_1_COL_POINT_2 120

#define MISSION_10_1_BLUE_GATE_SLOPE_RANGE 7
#define MISSION_10_1_BLUE_GATE_SLOPE 3

#define MISSION_10_1_BLACK_LENGTH 50
#define MISSION_10_1_BLACK_LENGTH_ERROR 5

#define MISSION_10_2_ERROR_BETWEEN_COLS 5

#define MISSION_10_3_UPPER_BLUE_GATE_SLOPE 7.5
#define MISSION_10_3_UPPER_BLUE_GATE_SLOPE_ERROR 3

void mission_10_1_watch_up(void);

void mission_10_1_watch_side(int repeat);

void mission_10_1_front_walk(int repeat);

void mission_10_2_watch_side(int repeat);

int mission_10_2_catch_blue_gate(U16 *image);

int mission_10_2_catch_green_bridge(U16* image);

int mission_10_1_set_center(U16 *image);

int mission_10_1_set_straight(U16 *image);

int mission_10_3_escape_blue_gate(void);

#endif //SOC_APP_MISSION_10_BLUE_GATE_H
