//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#include "MISSION_1_YELLOW_BARRICADE.h"

int mission_1_1_wait_yellow_barricade(U16 *image) {
    U32 col, row, cntYellow = 0;

    for (row = MISSION_1_UPPER; row < MISSION_1_UPPER + MISSION_1_RANGE; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            cntYellow += GetValueRGBYOBK(
                    GetPtr(image, row, col, WIDTH),
                    YELLOW
            );
        }
    }

    printf("M1-1: WAIT: %d\n", cntYellow * 100 / (WIDTH * MISSION_1_RANGE));
    return cntYellow * 100 / (WIDTH * MISSION_1_RANGE) > MISSION_1_THRESHOLDS;
}

int mission_1_2_end_yellow_barricade(U16 *image) {
    U32 col, row, cntYellow = 0;

    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; col += 2) {
            cntYellow += GetValueRGBYOBK(
                    GetPtr(image, row, col, WIDTH),
                    YELLOW
            );
        }
    }

    printf("M1-2: END: %d\n", cntYellow * 2 * 100 / (WIDTH * HEIGHT));
    return cntYellow * 2 * 100 / (WIDTH * HEIGHT) <= (MISSION_1_THRESHOLDS - 20);
}

void mission_1_3_escape_yellow_barricade(int repeat) {
    int i;
    for (i = 0; i < 2; ++i) {
        RobotSleep(5);
    }
    Action_WALK(FAST, OBLIQUE, repeat);
}
