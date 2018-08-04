//
// Created by Gihyeon Yang on 2018. 7. 22..
//

#ifndef SOC_APP_MISSION_8_AVOID_BOMB_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "stdio.h"

#define SOC_APP_MISSION_8_AVOID_BOMB_H

int mission_8_1_is_not_front_of_bomb(U16 *image);

int mission_8_4_check_finish_line(U16 *image);

#endif //SOC_APP_MISSION_8_AVOID_BOMB_H
