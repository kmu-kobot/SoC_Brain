//
// Created by Gihyeon Yang on 2018. 7. 13..
//

#ifndef SOC_APP_MISSION_6_KICK_BALL_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "stdio.h"

#define SOC_APP_MISSION_6_KICK_BALL_H

void mission_6_1_watch_front(void);

int mission_6_1_detection_ball(U16 *image);

void mission_6_2_watch_below(void);

int mission_6_2_set_center_of_ball(U16 *image);

#endif //SOC_APP_MISSION_6_KICK_BALL_H
