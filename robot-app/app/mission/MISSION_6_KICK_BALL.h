//
// Created by Gihyeon Yang on 2018. 7. 13..
//

#ifndef SOC_APP_MISSION_6_KICK_BALL_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "./../library/graphic_api.h"
#include "stdio.h"

#define SOC_APP_MISSION_6_KICK_BALL_H

#define DEFAULT_SLOPE_ERROR 5
#define DEFAULT_LEFT_RANGE 60
#define DEFAULT_RIGHT_RANGE 66
#define DEFAULT_RANGE_ERROR 6

#define MISSION_6_2_BLACK_LINE_COL_POINT_1 60
#define MISSION_6_2_BLACK_LINE_COL_POINT_2 120
#define MISSION_6_2_KICK_BALL_THRESHOLDS 7
#define MISSION_6_2_BLACK_LINE_SLOPE 10

#define CASE_6_9_NON_BLACK_LINE 110

#define MISSION_6_3_THRES 2

#define MISSION_6_3_CENTER 97

#define MISSION_6_4_THRES 5

#define MISSION_6_4_CENTER 108

#define MISSION_6_6_COL_POINT_1 80
#define MISSION_6_6_COL_POINT_2 120

#define MISSION_6_6_BLUE_GATE_SLOPE 3

#define MISSION_6_6_BLACK_LENGTH 50
#define MISSION_6_6_BLACK_LENGTH_ERROR 5

void mission_6_1_watch_front(void);

int mission_6_1_detection_ball(U16 *image);

void mission_6_2_watch_below(void);

void mission_6_2_watch_right(void);

int mission_6_2_set_center_of_ball(U16 *image);

int mission_6_2_set_straight_black_line(U16 *image, int mode);

int mission_6_3_locate_hole_on_center(U16 *image);

void mission_6_4_turn_to_detect_hole(void);

void mission_6_4_turn_left(void);

int mission_6_3_find_hole_interpolation(U16 *image);

int mission_6_3_find_hole(U16 *image);

int mission_6_3_set_straight_hole(U16 *image);

int countColor(U16 *image, int row, int col, int range, int color);

int mission_6_4_find_ball_interpolation(U16 *image, int limit);

int mission_6_4_find_ball(U16 *image, int limit);

int mission_6_4_set_front_of_ball(U16 *image);

int mission_6_5_kick_ball(void);

void mission_6_6_watch_side(void);

int mission_6_6_set_center_black_line(U16 *image, int mode);

int mission_6_9_set_front_of_not_bk(U16 *image);

int mission_6_9_walk_front(void);

void mission_test(U16 *image);

#endif //SOC_APP_MISSION_6_KICK_BALL_H