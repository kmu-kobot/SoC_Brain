//
// Created by Gihyeon Yang on 2018. 7. 13..
//

#ifndef SOC_APP_MISSION_6_KICK_BALL_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "./../library/graphic_api.h"
#include "./../library/imageProcess.h"
#include "DEFAULT.h"
#include "stdio.h"

#define SOC_APP_MISSION_6_KICK_BALL_H

#define MISSION_6_2_KICK_BALL_THRESHOLDS 7
#define MISSION_6_2_BLACK_LINE_SLOPE 10

#define CASE_6_9_NON_BLACK_LINE 110

#define MISSION_6_3_THRES 2

#define MISSION_6_3_CENTER 90

#define MISSION_6_4_THRES 5

#define MISSION_6_4_CENTER 108

int mission_6_1_detection_ball(U16 *image);

int mission_6_3_locate_hole_on_center(U16 *image);

void mission_6_4_turn_left(void);

int mission_6_3_find_side_hole(U16 *image, U8 step);

int mission_6_3_find_hole_interpolation(U16 *image, U32 top, U32 bot, U32 left, U32 right, U8 last);

int mission_6_3_find_hole(U16 *image, U32 top, U32 bot, U32 left, U32 right, U8 last);

int mission_6_3_find_hole_row(U16 *image, U32 top, U32 bot, U32 left, U32 right, U8 last);

int mission_6_3_find_hole_col(U16 *image, U32 top, U32 bot, U32 left, U32 right, U8 last);

int mission_6_3_set_straight_hole(U16 *image);

int mission_6_4_find_ball_interpolation(U16 *image, U32 top, U32 bot, U32 left, U32 right, U8 last);

int mission_6_4_find_ball(U16 *image, U32 top, U32 bot, U32 left, U32 right, U8 last);

int mission_6_4_find_ball_row(U16 *image, U32 top, U32 bot, U32 left, U32 right, U8 last);

int mission_6_4_find_ball_col(U16 *image, U32 top, U32 bot, U32 left, U32 right, U8 last);

int mission_6_4_set_center_of_ball(U16 *image);

int mission_6_5_kick_ball(void);

int mission_6_9_walk_front(void);

#endif //SOC_APP_MISSION_6_KICK_BALL_H
