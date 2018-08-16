#ifndef __DEFAULT__
#define __DEFAULT__

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "./../library/imageProcess.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DEFAULT_STRAIGHT_THRES_LONG 10
#define DEFAULT_STRAIGHT_THRES_MIDDLE 5

#define DEFAULT_CENTER_DISTANCE 50
#define DEFAULT_CENTER_THRES_LONG 30
#define DEFAULT_CENTER_THRES_MIDDLE 15
#define DEFAULT_CENTER_THRES_SHORT 5

static inline void default_watch_side(VIEW view)
{
    CHECK_INIT(view);
    RobotSleep(2);
}

int default_set_straight_and_center1(U16 *image, VIEW view, U16 center, U16 bot, U16 color1);
int default_set_straight_and_center2(U16 *image, VIEW view, U16 center, U16 bot, U16 color1, U16 color2);

int set_straight(_line_t line, U16 center, VIEW view);
int set_center(_line_t line, U16 center, VIEW view);


#endif
