//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#include "MISSION_1_YELLOW_BARRICADE.h"

int mission_1_1_wait_yellow_barricade(U16 *image) {
    U32 col, row, cntYellow = 0;

    for (row = MISSION_1_UPPER; row < MISSION_1_UPPER + MISSION_1_RANGE; ++row) {
        for (col = 0; col < WIDTH; col += 2) {
            cntYellow += GetValueRGBYOBK(
                    GetPtr(image, row, col, WIDTH),
                    YELLOW
            );
        }
    }
    
    printf("M1-1: WAIT: %f", cntYellow * 2 * 100 / (WIDTH * MISSION_1_RANGE));
    return cntYellow * 2 * 100 / (WIDTH * MISSION_1_RANGE) > MISSION_1_THRESHOLDS;
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
    
    printf("M1-2: END: %f", cntYellow * 2 * 100 / (WIDTH * HEIGHT));
    return cntYellow * 2 * 100 / (WIDTH * HEIGHT) <= (MISSION_1_THRESHOLDS - 20);
}

void mission_1_3_escape_yellow_barricade(int repeat) {
    Action_WALK_FRONT_LONG(repeat);
}
