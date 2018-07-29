//
// Created by Gihyeon Yang on 2018. 7. 17..
//

#ifndef SOC_APP_MISSION_3_AVOID_BOMB_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "stdio.h"
#include "math.h"

#define SOC_APP_MISSION_3_AVOID_BOMB_H

#define MISSION_3_DEFAULT_BOMB_THRESHOLDS 5
#define MISSION_3_DEFAULT_LINE_THRESHOLDS 100

void mission_3_init_global(void);

void mission_3_default_watch_below(void);

int mission_3_default_avoid_bomb(U16 *image);

void mission_3_4_watch_front(void);

int mission_3_4_is_not_front_of_bomb(U16 *image);

void mission_3_7_attach_hurdle(U16 *image);


#endif //SOC_APP_MISSION_3_AVOID_BOMB_H