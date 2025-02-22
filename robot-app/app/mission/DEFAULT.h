#ifndef __DEFAULT__
#define __DEFAULT__

#include "./../library/amazon2_sdk.h"
#include "./../library/robot_action.h"
#include "./../library/imageProcess.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DEFAULT_CENTER_DISTANCE 58
#define DEFAULT_CENTER_THRES_LONG 13
#define DEFAULT_CENTER_THRES_SHORT 4

const static double straight[2] = {9, -10.5};

static inline void default_watch(VIEW view, U16 *image) {
    CHECK_INIT(view);
    RobotSleep(1);
    setFPGAVideoData(image);
    RobotSleep(1);
    setFPGAVideoData(image);
    RobotSleep(1);
    setFPGAVideoData(image);
    RobotSleep(1);
    setFPGAVideoData(image);
    setFPGAVideoData(image);
    if (view == LEFT || view == RIGHT) {
        RobotSleep(1);
        setFPGAVideoData(image);
        setFPGAVideoData(image);
        setFPGAVideoData(image);
        // ㅋㅋ 무엇?
    }
}

static inline void default_watch_green(U16 *image) {
    CHECK_INIT(UP);
    RobotAction(250);
    RobotSleep(1);
    setFPGAVideoData(image);
    RobotSleep(1);
    setFPGAVideoData(image);
    RobotSleep(1);
    setFPGAVideoData(image);
    RobotSleep(1);
    setFPGAVideoData(image);
    setFPGAVideoData(image);
}

int default_get_straight_and_center1(U16 *image, VIEW view, U16 center, U16 bot, U16 color1, U16 base);

int default_set_straight_and_center1_long(U16 *image, VIEW view, U16 center, U16 bot, U16 color1, U16 base);

int default_set_straight_long_and_center1_long(U16 *image, VIEW view, U16 center, U16 bot, U16 color1, U16 base);

int default_set_straight_and_center1(U16 *image, VIEW view, U16 center, U16 bot, U16 color1, U16 base);

int default_set_straight_and_center2(U16 *image, VIEW view, U16 center, U16 bot, U16 color1, U16 color2, U16 base);

int default_set_straight1(U16 *image, VIEW view, U16 center, U16 bot, U16 color1, U16 base);

int default_set_center1(U16 *image, VIEW view, U16 center, U16 bot, U16 color1, U16 base);

int default_set_center1_long(U16 *image, VIEW view, U16 center, U16 bot, U16 color1, U16 base);

int set_straight(_line_t line, U16 center, VIEW view);

int set_straight_long(_line_t line, U16 center, VIEW view);

int set_center_long(_line_t line, U16 base, VIEW view);

int set_center(_line_t line, U16 base, VIEW view);

int default_set_not_black(U16 *image);

int point_t_cmp_y(const void *a, const void *b);

#endif
