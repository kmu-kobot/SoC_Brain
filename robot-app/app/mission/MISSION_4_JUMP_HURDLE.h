//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#ifndef SOC_APP_MISSION_4_JUMP_HUDDLE_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"

#define SOC_APP_MISSION_4_JUMP_HUDDLE_H

#define MISSION_4_HURDLE_CRITERI 3
#define MISSION_4_HURDLE_ERROR 7

void mission_4_1_watch_front(int repeat);

int mission_4_2_ready_hurdle(U16 *image);

int mission_4_3_jump_hurdle();

#endif //SOC_APP_MISSION_4_JUMP_HUDDLE_H
