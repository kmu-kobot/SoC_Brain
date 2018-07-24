//
// Created by Gihyeon Yang on 2018. 7. 11..
//

#ifndef SOC_APP_MISSION_7_YELLOW_HOLE_BRIDGE_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "stdio.h"

#define SOC_APP_MISSION_7_YELLOW_HOLE_BRIDGE_H

#define MISSION_7_1_THRESHOLDS 15

#define MISSION_7_2_BLACK_LINE_COL_POINT_1 70
#define MISSION_7_2_BLACK_LINE_COL_POINT_2 130

#define MISSION_7_2_RED_BRIDGE_THRESHOLDS 4

#define MISSION_7_4_YELLOW_BRIDGE_SLOPE 5
#define MISSION_7_4_YELLOW_BRIDGE_SLOPE_ERROR 3

#define MISSION_7_4_COL_POINT_1 100
#define MISSION_7_4_COL_POINT_2 140


//TODO: 거리에 짤게 걷기, 길게 걷기 수정 
#define MISSION_7_4_YELLOW_LENGTH 18
#define MISSION_7_4_YELLOW_LENGTH_ERROR 5

#define MISSION_7_5_LINE_RATIO 43
#define MISSION_7_5_LINE_RATIO_ERROR 5
#define MISSION_7_5_POINT_X_1 70
#define MISSION_7_5_POINT_X_2 110

#define MISSION_7_7_BLACK_LINE_SLOPE 7.5
#define MISSION_7_7_BLACK_LINE_SLOPE_ERROR 3

void mission_7_1_watch_below(int repeat);

int mission_7_1_wait_front_of_yellow_hole_bridge(U16 *image, int repeat);

void mission_7_2_watch_below(void);

int mission_7_2_before_bridge_set_center(U16 *image);

int mission_7_3_climb_yellow_hole_bridge(void);

int mission_7_4_set_center(U16 *image);

int mission_7_4_set_straight(U16 *image);

void mission_7_4_watch_below(void);

int mission_7_5_set_center_on_yellow_bridge(U16 *image);

int mission_7_5_set_straight_on_yellow_bridge(U16 *image);

int mission_7_5_walk_until_line_front_of_feet(U16 *image);

void mission_7_5_watch_below(void);

int mission_7_4_walk_until_black_line(U16 *image);

int mission_7_6_jump_hole(void);

void mission_7_7_watch_side(void);

int mission_7_7_after_yellow_bridge_set_straight(U16 *image);

int mission_7_7_after_yellow_bridge_set_center(U16 *image);

#endif //SOC_APP_MISSION_7_YELLOW_HOLE_BRIDGE_H


