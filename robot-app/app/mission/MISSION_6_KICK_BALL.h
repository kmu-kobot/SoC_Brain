//
// Created by Gihyeon Yang on 2018. 7. 13..
//

#ifndef SOC_APP_MISSION_6_KICK_BALL_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "stdio.h"

#define SOC_APP_MISSION_6_KICK_BALL_H

#define MISSION_6_6_COL_POINT_1 80
#define MISSION_6_6_COL_POINT_2 120

#define MISSION_6_6_BLUE_GATE_SLOPE 3

#define MISSION_6_6_BLACK_LENGTH 50
#define MISSION_6_6_BLACK_LENGTH_ERROR 5

void mission_6_1_watch_front(void);

int mission_6_1_detection_ball(U16 *image);

void mission_6_2_watch_below(void);

int mission_6_2_set_center_of_ball(U16 *image);

int mission_6_3_find_hole(U16 *image);

int countColor(U16 *image, int row, int col, int range, int color);

int mission_6_4_set_front_of_ball(U16 *image);

int mission_6_5_kick_ball(void);

void mission_6_6_watch_side(void);

int mission_6_6_set_straight_black_line(U16* image);

int mission_6_6_set_center_black_line(U16* image);

#endif //SOC_APP_MISSION_6_KICK_BALL_H
