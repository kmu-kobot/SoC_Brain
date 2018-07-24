//
// Created by Gihyeon Yang on 2018. 7. 13..
//

#include "MISSION_9_LAST_BARRICADE.h"

int mission_9_1_wait_yellow_barricade(U16 *image) {
    U32 col, row, cntYellow = 0;

    for (row = MISSION_9_UPPER; row < MISSION_9_UPPER + MISSION_9_RANGE; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            cntYellow += GetValueRGBYOBK(
                    GetPtr(image, row, col, WIDTH),
                    YELLOW
            );
        }
    }

    printf("M9-1: WAIT: %d\n", cntYellow * 100 / (WIDTH * MISSION_9_RANGE));
    return cntYellow * 100 / (WIDTH * MISSION_9_RANGE) > MISSION_9_THRESHOLDS;
}

int mission_9_2_end_yellow_barricade(U16 *image) {
    U32 col, row, cntYellow = 0;

    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; col++) {
            cntYellow += GetValueRGBYOBK(
                    GetPtr(image, row, col, WIDTH),
                    YELLOW
            );
        }
    }

    printf("M9-2: END: %d\n", cntYellow * 100 / (WIDTH * HEIGHT));
    return cntYellow * 2 * 100 / (WIDTH * HEIGHT) <= (MISSION_9_THRESHOLDS - 20);
}

void mission_9_3_escape_yellow_barricade(int repeat) {
    int i;
    for (i = 0; i < 2; ++i) {
        RobotSleep(5);
    }
    ACTION_WALK(FAST, OBLIQUE, repeat);
}
