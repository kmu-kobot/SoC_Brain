//
// Created by Gihyeon Yang on 2018. 7. 11..
//

#ifndef SOC_APP_MISSION_7_YELLOW_HOLE_BRIDGE_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "stdio.h"

#define SOC_APP_MISSION_7_YELLOW_HOLE_BRIDGE_H

#define MISSION_7_THRESHOLDS 40

#define MISSION_7_2_BLACK_LINE_RANGE 50
#define MISSION_7_2_BLACK_LINE_ERROR 3

#define MISSION_7_2_BLACK_LINE_COL_POINT_1 70
#define MISSION_7_2_BLACK_LINE_COL_POINT_2 130

#define MISSION_7_4_YELLOW_LINE_COL_POINT_1 100
#define MISSION_7_4_YELLOW_LINE_COL_POINT_2 160

#define MISSION_7_4_YELLOW_LINE_SLOPE 7.5
#define MISSION_7_4_YELLOW_LINE_SLOPE_ERROR 3

#define MISSION_7_4_YELLOW_LINE_RANGE 50
#define MISSION_7_4_YELLOW_LINE_ERROR 3

#define MISSION_7_5_BLACK_RATIO 10
#define MISSION_7_5_BLACK_RATIO_ERROR 5
#define MISSION_7_5_HEIGHT_SCOPE 60

#define MISSION_7_7_BLACK_LINE_SLOPE 7.5
#define MISSION_7_7_BLACK_LINE_SLOPE_ERROR 3

void mission_7_1_watch_below(int repeat);

int mission_7_1_wait_front_of_yellow_hole_bridge(U16 *image, int repeat);

void mission_7_2_watch_side(void);

int mission_7_2_before_bridge_set_center(U16 *image);

int mission_7_3_climb_yellow_hole_bridge(void);

int mission_7_4_set_straight_on_yellow_bridge(U16 *image);

int mission_7_4_set_center_on_yellow_bridge(U16 *image);

void mission_7_5_watch_below(void);

int mission_7_5_walk_until_black_line(U16* image);

int mission_7_6_jump_hole(void);

int mission_7_7_after_yellow_bridge_set_straight(U16 *image);

int mission_7_7_after_yellow_bridge_set_center(U16 *image);

#endif //SOC_APP_MISSION_7_YELLOW_HOLE_BRIDGE_H

