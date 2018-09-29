#ifndef __DEFAULT__
#define __DEFAULT__

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "./../library/imageProcess.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DEFAULT_CENTER_DISTANCE 52
#define DEFAULT_CENTER_THRES_LONG 15
#define DEFAULT_CENTER_THRES_SHORT 4

static inline void default_watch(VIEW view, U16 *image) {
    CHECK_INIT(view);
    setFPGAVideoData(image);
    RobotSleep(1);
    setFPGAVideoData(image);
    RobotSleep(1);
    setFPGAVideoData(image);
}

int default_get_straight_and_center1(U16 *image, VIEW view, U16 center, U16 bot, U16 color1);

int default_set_straight_and_center1_long(U16 *image, VIEW view, U16 center, U16 bot, U16 color1);

int default_set_straight_and_center1(U16 *image, VIEW view, U16 center, U16 bot, U16 color1);

int default_set_straight_and_center2(U16 *image, VIEW view, U16 center, U16 bot, U16 color1, U16 color2);

int default_set_straight1(U16 *image, VIEW view, U16 center, U16 bot, U16 color1);

int default_set_center1(U16 *image, VIEW view, U16 center, U16 bot, U16 color1);

int default_set_center1_long(U16 *image, VIEW view, U16 center, U16 bot, U16 color1);

int set_straight(_line_t line, U16 center, VIEW view);

int set_center_long(_line_t line, U16 center, VIEW view);

int set_center(_line_t line, U16 center, VIEW view);

int default_set_not_black(U16 *image);

int point_t_cmp_y(const void *a, const void *b);

#endif
