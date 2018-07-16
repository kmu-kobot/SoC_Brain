//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#ifndef SOC_APP_MISSION_5_GREEN_BRIDGE_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "stdio.h"

#define SOC_APP_MISSION_5_GREEN_BRIDGE_H

#define MISSION_5_1_THRESHOLDS 20

#define MISSION_5_5_GREEN_LINE_ROW_POINT 20
#define MISSION_5_5_GREEN_BRIDGE_RANGE 70
#define MISSION_5_5_GREEN_BRIDGE_THRESHOLDS 8

#define MISSION_5_5_GREEN_BRIDGE_POINT_Y_1 30
#define MISSION_5_5_GREEN_BRIDGE_POINT_Y_2 50
#define MISSION_5_5_GREEN_BRIDGE_SLOPE_RANGE 7
#define MISSION_5_5_GREEN_BRIDGE_SLOPE 5

#define MISSION_5_6_WIDTH_LEFT 0
#define MISSION_5_6_WIDTH_RANGE 30
#define MISSION_5_6_BLACK_LEN_THRESHOLDS 20
#define MISSION_5_6_GREEN_BRIDGE_SLOPE 3

void mission_5_1_watch_below(int repeat);

int mission_5_1_check_black_line(U16 *image, int repeat);

void mission_5_2_watch_side(void);

int mission_5_3_climb_up_stairs(void);

void mission_5_5_watch_below(void);

int mission_5_5_check_finish_black_line(U16 *image);

int mission_5_5_check_green_bridge_straight(U16 *image);

int mission_5_5_check_green_bridge_center(U16 *image);

int mission_5_5_short_walk_on_green_bridge(int repeat);

int mission_5_6_set_only_one_bk_bar(U16 *image);

int mission_5_6_set_straight(U16 *image);

int mission_5_7_climb_down_stairs(void);

#endif //SOC_APP_MISSION_5_GREEN_BRIDGE_H
