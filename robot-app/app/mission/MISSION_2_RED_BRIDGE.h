//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#ifndef SOC_APP_MISSION_2_RED_BRIDGE_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"

#define SOC_APP_MISSION_2_RED_BRIDGE_H

#define MISSION_2_THRESHOLDS 4

void mission_2_1_watch_below(int repeat);

int mission_2_1_wait_front_of_red_bridge(U16 *image, int repeat);

void mission_2_2_watch_side(void);

int mission_2_2_before_bridge_set_center(U16 *image);

int mission_2_3_escape_red_bridge(void);

int mission_2_4_after_bridge_set_straight(U16 *image);

int mission_2_5_after_bridge_set_center(U16 *image);

#endif //SOC_APP_MISSION_2_RED_BRIDGE_H
