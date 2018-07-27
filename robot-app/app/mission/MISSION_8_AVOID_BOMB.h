//
// Created by Gihyeon Yang on 2018. 7. 22..
//

#ifndef SOC_APP_MISSION_8_AVOID_BOMB_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "stdio.h"

#define SOC_APP_MISSION_8_AVOID_BOMB_H

#define MISSION_8_4_LOWER 40
#define MISSION_8_4_UPPER 80
#define MISSION_8_4_THRESHOLDS 30

int mission_8_4_is_front_of_finish_line(U16 *image);

#endif //SOC_APP_MISSION_8_AVOID_BOMB_H
