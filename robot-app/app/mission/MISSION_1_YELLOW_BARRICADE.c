//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#include "MISSION_1_YELLOW_BARRICADE.h"

int mission_1_1_wait_yellow_barricade(U16 *image) {
    U32 col, row, cntYellow = 0;

    for (row = MISSION_1_UPPER; row < MISSION_1_LOWER; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            cntYellow += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW);
        }
    }

    printf("M1-1: WAIT: %f\n", (double) cntYellow * 100 / (WIDTH * (MISSION_1_LOWER - MISSION_1_UPPER)));
    return cntYellow * 100 / (WIDTH * (MISSION_1_LOWER - MISSION_1_UPPER)) > MISSION_1_THRESHOLDS;
}

int mission_1_2_end_yellow_barricade(U16 *image) {
    U32 col, row, cntYellow = 0;

    for (row = MISSION_1_UPPER; row < MISSION_1_LOWER; ++row) {
        for (col = 0; col < WIDTH; col++) {
            cntYellow += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW);
        }
    }

    printf("M1-2: END: %d\n", cntYellow * 100 / (WIDTH * HEIGHT));
    return cntYellow * 100 / (WIDTH * HEIGHT) <= MISSION_1_THRESHOLDS;
}

void mission_1_3_escape_yellow_barricade(int repeat) {
    ACTION_WALK(FAST, OBLIQUE, repeat);
}
