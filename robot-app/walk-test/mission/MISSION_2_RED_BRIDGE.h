//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#ifndef SOC_APP_MISSION_2_RED_BRIDGE_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "./../library/imageProcess.h"
#include "DEFAULT.h"
#include "stdio.h"

#define SOC_APP_MISSION_2_RED_BRIDGE_H

#define CASE_2_0_DETECTION 32

void mission_2_1_watch_below(U16 *image, int repeat);

int mission_2_1_wait_front_of_red_bridge(U16 *image);

int mission_2_1_check_center(U16 *image);

int mission_2_1_get_mdir(void);

int mission_2_3_escape_red_bridge(void);

int mission_2_1_attach_red_bridge(U16 *image);

#endif //SOC_APP_MISSION_2_RED_BRIDGE_H
