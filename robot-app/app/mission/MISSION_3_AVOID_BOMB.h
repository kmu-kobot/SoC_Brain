//
// Created by Gihyeon Yang on 2018. 7. 17..
//

#ifndef SOC_APP_MISSION_3_AVOID_BOMB_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "stdio.h"

#define SOC_APP_MISSION_3_AVOID_BOMB_H

#define MISSION_3_DEFAULT_WIDTH_MARGIN 40
#define MISSION_3_DEFAULT_WIDTH 60
#define MISSION_3_DEFAULT_BOMB_THRESHOLDS 5
#define MISSION_3_DEFAULT_LINE_THRESHOLDS 100
#define MISSION_3_DEFAULT_AVOID_BOMB_RANGE 6

#define MISSION_3_POINT_LENGTH 27

#define MISSION_3_4_HEIGHT_OFFSET 30
#define MISSION_3_4_BOMB 3

void mission_3_default_watch_below(void);

int mission_3_default_avoid_bomb(U16 *image);

void mission_3_4_watch_front(void);

int mission_3_4_is_not_front_of_bomb(U16 *image);


#endif //SOC_APP_MISSION_3_AVOID_BOMB_H