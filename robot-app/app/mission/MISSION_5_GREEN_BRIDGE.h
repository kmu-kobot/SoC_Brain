//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#ifndef SOC_APP_MISSION_5_GREEN_BRIDGE_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "./../library/imageProcess.h"
#include "DEFAULT.h"
#include "stdio.h"

#define SOC_APP_MISSION_5_GREEN_BRIDGE_H
#define CASE_5_0_DETECTION 20

#define MISSION_5_5_GREEN_BRIDGE_THRESHOLDS 5

#define MISSION_5_5_GREEN_BRIDGE_POINT_Y_1 30
#define MISSION_5_5_GREEN_BRIDGE_POINT_Y_2 50
#define MISSION_5_5_GREEN_BRIDGE_SLOPE_RANGE 7
#define MISSION_5_5_GREEN_BRIDGE_SLOPE 1

#define MISSION_5_6_POINT_X_1 10
#define MISSION_5_6_POINT_X_2 170
#define MISSION_5_6_BLACK_LEN_THRESHOLDS 100
#define MISSION_5_6_GREEN_BRIDGE_SLOPE 3

#define MISSION_5_6_BLACK_COL_1 30
#define MISSION_5_6_BLACK_COL_2 150
#define MISSION_5_6_BLACK_RANGE 5
#define MISSION_5_6_BLACK_LEN_LENGTH 65

void mission_5_1_watch_below(U16 *image, int repeat);

int mission_5_1_check_black_line(U16 *image);

int mission_5_1_attach(U16 *image);

int mission_5_2_set_straight_and_center(U16 *image, U16 center);

int mission_5_2_set_straight(_line_t line);

void mission_5_3_climb_up_stairs(void);

int mission_5_4_set_straight_and_center(U16 *image, U16 center);

int mission_5_4_get_left_line(U16 *image, U16 center, _line_t *left_line);

int mission_5_4_get_right_line(U16 *image, U16 center, _line_t *right_line);

int mission_5_4_get_center_line(U16 *image, _line_t left_line, _line_t right_line, _line_t *center_line);

int mission_5_4_set_straight(_line_t center_line);

int mission_5_4_set_center(_line_t center_line);

void mission_5_5_attach_green(void);

int mission_5_6_set_straight_and_center(U16 *image);

int mission_5_6_set_straight(_line_t center_line);

int mission_5_7_watch_below(U16 *image);

int mission_5_7_walk_check(U16 *image);

int mission_5_8_attach_black(U16 *image);

int mission_5_8_set_straight(_line_t line);

int mission_5_8_set_dist(_line_t line);

int mission_5_8_get_front_line(U16 *image, _line_t *front_line, U16 color);

int mission_5_9_attach_black(U16 *image);

int mission_5_9_get_front_line(U16 *image, _line_t *front_line, U16 color);

int mission_5_9_set_straight(_line_t line);

int mission_5_9_set_dist(_line_t line);

int mission_5_10_climb_down_stairs(void);

#endif //SOC_APP_MISSION_5_GREEN_BRIDGE_H
