//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#ifndef SOC_APP_MISSION_5_GREEN_BRIDGE_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "stdio.h"

#define SOC_APP_MISSION_5_GREEN_BRIDGE_H
#define CASE_5_0_DETECTION 15

#define MISSION_5_5_GREEN_BRIDGE_THRESHOLDS 5

#define MISSION_5_5_GREEN_BRIDGE_POINT_Y_1 30
#define MISSION_5_5_GREEN_BRIDGE_POINT_Y_2 50
#define MISSION_5_5_GREEN_BRIDGE_SLOPE_RANGE 7
#define MISSION_5_5_GREEN_BRIDGE_SLOPE 2

#define MISSION_5_6_POINT_X_1 10
#define MISSION_5_6_POINT_X_2 170
#define MISSION_5_6_BLACK_LEN_THRESHOLDS 80
#define MISSION_5_6_GREEN_BRIDGE_SLOPE 3

#define MISSION_5_6_BLACK_COL_1 30
#define MISSION_5_6_BLACK_COL_2 150
#define MISSION_5_6_BLACK_RANGE 5
#define MISSION_5_6_BLACK_LEN_LENGTH 70

void mission_5_1_watch_below(int repeat);

int mission_5_1_check_black_line(U16 *image, int repeat);

void mission_5_2_watch_side(void);

void mission_5_3_climb_up_stairs(void);

int mission_5_5_check_finish_black_line(U16 *image);

int mission_5_5_check_green_bridge_straight(U16 *image);

int mission_5_5_check_green_bridge_center(U16 *image);

int mission_5_5_short_walk_on_green_bridge(int repeat);

int mission_5_5_get_repeat(U16 *image);

int mission_5_6_set_only_one_bk_bar(U16 *image);

int mission_5_6_set_straight(U16 *image);

int mission_5_7_climb_down_stairs(void);

void mission_5_5_set_center(U16 *image);

int mission_5_11_attach(U16 *image);

int mission_5_12_set_straight(U16 *image);

#endif //SOC_APP_MISSION_5_GREEN_BRIDGE_H
