//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#ifndef SOC_APP_MISSION_10_BLUE_GATE_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "stdio.h"

#define SOC_APP_MISSION_10_BLUE_GATE_H

#define MISSION_10_1_BLUE_GATE_RATIO 4

int mission_10_1_catch_blue_gate(U16 *image);

void mission_10_1_front_walk(int repeat);

#endif //SOC_APP_MISSION_10_BLUE_GATE_H
