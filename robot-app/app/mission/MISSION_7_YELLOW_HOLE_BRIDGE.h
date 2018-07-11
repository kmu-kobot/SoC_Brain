//
// Created by Gihyeon Yang on 2018. 7. 11..
//

#ifndef SOC_APP_MISSION_7_YELLOW_HOLE_BRIDGE_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"

#define SOC_APP_MISSION_7_YELLOW_HOLE_BRIDGE_H

#define MISSION_7_THRESHOLDS 4

void mission_7_1_watch_below(int repeat);

int mission_7_1_wait_front_of_yellow_hole_bridge(U16 *image, int repeat);

void mission_7_2_watch_side(void);

int mission_7_2_before_bridge_set_center(U16 *image);

int mission_7_3_climb_yellow_hole_bridge(void);

int mission_7_4_set_straight_on_yellow_bridge(U16 *image);

int mission_7_4_set_center_on_yellow_bridge(U16 *image);

void mission_7_5_watch_below(void);

int mission_7_6_jump_hole(void);

#endif //SOC_APP_MISSION_7_YELLOW_HOLE_BRIDGE_H
