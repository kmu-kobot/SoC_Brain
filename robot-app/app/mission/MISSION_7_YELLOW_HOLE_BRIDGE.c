//
// Created by Gihyeon Yang on 2018. 7. 11..
//

#include "MISSION_7_YELLOW_HOLE_BRIDGE.h"

void mission_7_1_watch_below(int repeat) {
    ACTION_WALK(FAST, DOWN, repeat);
    RobotSleep(2);
}

int mission_7_1_wait_front_of_yellow_hole_bridge(U16 *image, int repeat) {
    U32 col, row, cntYellow = 0;
    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            cntYellow += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW) ||
                          GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), CH2));
        }
    }

    double r = (double) cntYellow / (WIDTH * HEIGHT) * 100;

    printf("ratio = %f\n", r);

    int rReturn = r > MISSION_7_1_THRESHOLDS;

    if (rReturn && repeat != -1) {
        // cntYellow = 0;
        // for (row = 20; row < ROBOT_KNEE; ++row) {
        //     for (col = 0; col < WIDTH; ++col) {
        //         cntYellow += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW) ||
        //                       GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), CH2));
        //     }
        // }

        ACTION_WALK(CLOSE, DOWN, 4);

    }

    return rReturn;
}

void mission_7_2_watch_below(void) {
    ACTION_INIT(MIDDLE, OBLIQUE);
}

int mission_7_6_jump_hole(void) {
    ACTION_WALK(CLOSE, OBLIQUE, 1);
    RobotSleep(1);
    ACTION_MOTION(MISSION_7_YELLOW_DUMBLING, MIDDLE, OBLIQUE);
    RobotSleep(1);
    return 1;
}