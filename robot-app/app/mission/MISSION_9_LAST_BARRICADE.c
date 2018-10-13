//
// Created by Gihyeon Yang on 2018. 7. 13..
//

#include "MISSION_9_LAST_BARRICADE.h"

double ratio = 0.0;

int mission_9_1_wait_yellow_barricade(U16 *image) {
    U32 i, success = 0;
    double t;
    for (i = 0; i < 5 && success < 3; ++i) {
        setFPGAVideoData(image);
        t = getColorRatio1(image, MISSION_9_UPPER, MISSION_9_LOWER, 20, WIDTH - 20, YELLOW);
        success += (t > MISSION_9_THRESHOLDS);
        if (t > ratio) {
            t = ratio;
        }
    }

    return success >= 3;
}

int mission_9_2_end_yellow_barricade(U16 *image) {
    U32 i, success = 0;
    double t;
    for (i = 0; i < 5 && success < 3; ++i) {
        setFPGAVideoData(image);
        t = getColorRatio1(image, MISSION_9_UPPER, MISSION_9_LOWER, 20, WIDTH - 20, YELLOW);
        if (t > ratio) {
            ratio = t;
            return 0;
        }
        success += (t < ratio / 2.0);
    }

    return success >= 3;
}

void mission_9_3_escape_yellow_barricade(int repeat) {
    CHECK_INIT(DOWN);
    RobotSleep(2);
    ACTION_WALK(FAST, DOWN, 9);
    ACTION_WALK(FAST, DOWN, 9);
}
