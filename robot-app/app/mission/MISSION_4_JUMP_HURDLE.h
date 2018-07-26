//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#ifndef SOC_APP_MISSION_4_JUMP_HUDDLE_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "stdio.h"

#define SOC_APP_MISSION_4_JUMP_HUDDLE_H


#define CASE_4_0_LENGTH 24
#define CASE_4_0_LENGTH_ERROR 15
#define CASE_4_1_NON_BLACK_LINE 90
#define CASE_0_DEFAULT_LEFT_RANGE 60
#define CASE_0_DEFAULT_RIGHT_RANGE 70
#define CASE_0_DEFAULT_RANGE_ERROR 6
#define CASE_0_DEFAULT_LEFT_SLOPE 0
#define CASE_0_DEFAULT_RIGHT_SLOPE 10
#define CASE_0_DEFAULT_SLOPE_ERROR 4

void mission_4_1_watch_front(int repeat);

int mission_4_2_ready_hurdle(U16 *image);

int mission_4_4_jump_hurdle(void);

void mission_4_5_watch_diagonal_line(void);

int mission_4_5_set_front_of_not_bk(U16 *image);

int mission_4_6_set_center(U16 *image, int length);

void mission_4_6_watch_side(void);

#endif //SOC_APP_MISSION_4_JUMP_HUDDLE_H
