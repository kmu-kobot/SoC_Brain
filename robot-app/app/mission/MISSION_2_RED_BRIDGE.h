//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#ifndef SOC_APP_MISSION_2_RED_BRIDGE_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "stdio.h"

#define SOC_APP_MISSION_2_RED_BRIDGE_H

#define CASE_2_0_DETECTION 32
#define CASE_0_DEFAULT_LEFT_RANGE 54
#define CASE_0_DEFAULT_RIGHT_RANGE 65
#define CASE_0_DEFAULT_RANGE_ERROR 6
#define CASE_0_DEFAULT_LEFT_SLOPE (-5)
#define CASE_0_DEFAULT_RIGHT_SLOPE 10
#define CASE_0_DEFAULT_SLOPE_ERROR 5

void mission_2_1_watch_below(int repeat, U16 *image);

int mission_2_1_wait_front_of_red_bridge(U16 *image);

void mission_2_2_watch_side(void);

int mission_2_2_before_bridge_set_center(U16 *image, int mode, int length);

int mission_2_3_escape_red_bridge(void);

int mission_2_1_attach_red_bridge(U16 *image);

int mission_2_4_after_bridge_set_straight(U16 *image, int mode, int pppo);

int mission_2_5_after_bridge_set_center(U16 *image);

#endif //SOC_APP_MISSION_2_RED_BRIDGE_H
