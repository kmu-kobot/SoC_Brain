//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#ifndef SOC_APP_MISSION_4_JUMP_HUDDLE_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"

#define SOC_APP_MISSION_4_JUMP_HUDDLE_H

#define MISSION_4_HURDLE_CRITERI 3
#define MISSION_4_HURDLE_ERROR 7
#define MISSION_4_BK_LINE_RANGE 10
#define MISSION_4_BK_LINE_LOWER 50

void mission_4_1_watch_front(int repeat);

int mission_4_2_ready_hurdle(U16 *image);

int mission_4_3_jump_hurdle();

int mission_4_4_set_straight(U16 *image);

int mission_4_5_check_bk_line(U16 *image);

int mission_4_6_set_center(U16 *image);

void mission_4_6_watch_side();

#endif //SOC_APP_MISSION_4_JUMP_HUDDLE_H
