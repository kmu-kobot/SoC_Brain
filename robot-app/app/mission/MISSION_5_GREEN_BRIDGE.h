//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#ifndef SOC_APP_MISSION_5_GREEN_BRIDGE_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "stdio.h"

#define SOC_APP_MISSION_5_GREEN_BRIDGE_H
#define MISSION_5_SLOPE 1
#define MISSION_5_SLOPE_RANGE 0.3

#define MISSION_5_1_THRESHOLDS 20

#define MISSION_5_4_WIDTH_LEFT 30
#define MISSION_5_4_WIDTH_RANGE 60
#define MISSION_5_4_BLACK_LEN_THRESHOLDS 20
#define MISSION_5_4_BLACK_LEN_RANGE 30
#define MISSION_5_4_BLACK_LEN_ERROR 10
#define MISSION_5_4_NUMBER_OF_BLACK_LEN 3

#define MISSION_5_4_FRONT_OF_GREEN_BRIDGE_THRESHOLDS 20

#define MISSION_5_5_GREEN_LINE_ROW_POINT 20
#define MISSION_5_5_GREEN_BRIDGE_RANGE 70
#define MISSION_5_5_GREEN_BRIDGE_THRESHOLDS 8

#define MISSION_5_5_GREEN_BRIDGE_POINT_Y_1 30
#define MISSION_5_5_GREEN_BRIDGE_POINT_Y_2 50
#define MISSION_5_5_GREEN_BRIDGE_SLOPE_RANGE 7
#define MISSION_5_5_GREEN_BRIDGE_SLOPE 5

void mission_5_1_watch_below(int repeat);

int mission_5_1_check_black_line(U16 *image, int repeat);

void mission_5_2_watch_side(void);

int mission_5_3_climb_up_stairs(void);

int mission_5_4_set_center_before_green_bridge(U16 *image);

int mission_5_4_set_front_of_green_bridge(U16 *image, int repeat);

void mission_5_5_watch_below(void);

int mission_5_5_check_finish_black_line(U16 *image);

int mission_5_5_check_green_bridge_straight(U16 *image);

int mission_5_5_check_green_bridge_center(U16 *image);

int mission_5_5_short_walk_on_green_bridge(int repeat);

int mission_5_7_climb_down_stairs(void);

#endif //SOC_APP_MISSION_5_GREEN_BRIDGE_H
