//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#ifndef SOC_APP_MISSION_5_GREEN_BRIDGE_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"

#define SOC_APP_MISSION_5_GREEN_BRIDGE_H
#define MISSION_5_SLOPE 1
#define MISSION_5_SLOPE_RANGE 0.3

#define MISSION_5_1_BLACK_LINE_UPPER 60

#define MISSION_5_1_THRESHOLDS 30

#define MISSION_5_4_GREEN_BRIDGE_THRESHOLDS 5

#define MISSION_5_4_GREEN_BRIDGE_ERROR 3

void mission_5_1_watch_below(int repeat);

int mission_5_1_check_black_line(U16 *image);

void mission_5_4_watch_below(void);

int mission_5_4_check_finish_black_line(U16 *image);

int mission_5_4_check_green_bridge_straight(U16 *image);

int mission_5_4_check_green_bridge_center(U16 *image);

int mission_5_4_short_walk_on_green_bridge(int repeat);

#endif //SOC_APP_MISSION_5_GREEN_BRIDGE_H
