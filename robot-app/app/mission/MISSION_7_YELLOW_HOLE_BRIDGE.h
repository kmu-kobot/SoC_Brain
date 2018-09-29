//
// Created by Gihyeon Yang on 2018. 7. 11..
//

#ifndef SOC_APP_MISSION_7_YELLOW_HOLE_BRIDGE_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "./../library/imageProcess.h"
#include "DEFAULT.h"
#include "stdio.h"

#define SOC_APP_MISSION_7_YELLOW_HOLE_BRIDGE_H

#define MISSION_7_1_THRESHOLDS 31

void mission_7_1_watch_below(U16 *image, int repeat);

int mission_7_1_attach_yellow_bridge(U16 *image);

int mission_7_1_wait_front_of_yellow_hole_bridge(U16 *image);

void mission_7_2_watch_below(void);

int mission_7_6_jump_hole(void);

#endif //SOC_APP_MISSION_7_YELLOW_HOLE_BRIDGE_H
