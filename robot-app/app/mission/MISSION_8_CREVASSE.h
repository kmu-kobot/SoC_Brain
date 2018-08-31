//
// Created by Gihyeon Yang on 2018. 7. 22..
//

#ifndef SOC_APP_MISSION_8_CREVASSE_H

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "./../library/imageProcess.h"
#include "DEFAULT.h"
#include "stdio.h"

#define SOC_APP_MISSION_8_CREVASSE_H

void mission_8_1_watch_below(U16 *image, int repeat);

int mission_8_1_wait_front_of_crevasse(U16 *image);

int mission_8_2_escape_crevasse(void);

#endif //SOC_APP_MISSION_8_CREVASSE_H
