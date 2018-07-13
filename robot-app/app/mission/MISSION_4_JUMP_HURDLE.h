//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#ifndef SOC_APP_MISSION_4_JUMP_HUDDLE_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "stdio.h"

#define SOC_APP_MISSION_4_JUMP_HUDDLE_H

#define MISSION_4_HURDLE_CRITERI 3
#define MISSION_4_2_HURDLE_THRESHOLDS 104
#define MISSION_4_HURDLE_ERROR 13

#define MISSION_4_5_BK_LINE_RANGE 50
#define MISSION_4_5_BK_LINE_ERROR 3

void mission_4_1_watch_front(int repeat);

int mission_4_2_ready_hurdle(U16 *image);

int mission_4_4_jump_hurdle(void);

void mission_4_5_watch_diagonal_line(void);

int mission_4_5_set_front_of_not_bk(U16 *image);

int mission_4_6_set_straight(U16 *image);

int mission_4_6_set_center(U16 *image);

void mission_4_6_watch_side(void);

#endif //SOC_APP_MISSION_4_JUMP_HUDDLE_H
