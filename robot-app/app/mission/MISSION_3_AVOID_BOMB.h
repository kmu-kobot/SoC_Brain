//
// Created by Gihyeon Yang on 2018. 7. 17..
//

#ifndef SOC_APP_MISSION_3_AVOID_BOMB_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "./../library/imageProcess.h"
#include "DEFAULT.h"
#include "stdio.h"
#include <stdlib.h>
#include "math.h"

#define SOC_APP_MISSION_3_AVOID_BOMB_H
#define MINE_RANGE_BOT (HEIGHT-35)
#define MINE_RANGE_LEFT 45

int mission_3_isFrontOf_Blue(U16 *image, U16 bot);

int mission_3_measure_line(U16 *image);

void mission_3_attach_mine(U16 *image);

int mission_3_check_angle(void);

void mission_3_init_global(void);

int mission_3_avoid(U16 *image);

int mission_3_change_mdir(U16 *image);

void mission_3_change_mdir_opposite(void);

int mission_3_default_watch_below(U16 *image, int repeat);

int mission_3_walk_avoid_bomb(U16 *image);

int mission_3_default_avoid_bomb(U16 *image);

int mission_3_4_getMDir(void);

int mission_3_1_ver2(U16 *image);

int mission_3_set_straight_and_center1_long(U16 *image, U16 center);

int mission_3_set_straight(U16 *image);

int mission_3_set_center(U16 *image);

int mission_3_linear_regression(U16 *image, U16 center, U16 bot, U16 color1, _line_t *line);

#endif //SOC_APP_MISSION_3_AVOID_BOMB_H
