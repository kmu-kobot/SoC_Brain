//
// Created by Gihyeon Yang on 2018. 7. 22..
//

#include "MISSION_8_AVOID_BOMB.h"

int mission_8_4_is_front_of_finish_line(U16 *image) {
    U32 col, row, cntYellow = 0;

    for (row = MISSION_8_4_LOWER; row < MISSION_8_4_UPPER; ++row) {
        for (col = 0; col < WIDTH; col++) {
            cntYellow += GetValueRGBYOBK(
                    GetPtr(image, row, col, WIDTH),
                    YELLOW
            );
        }
    }

    return cntYellow * 100 / (WIDTH * (MISSION_8_4_UPPER - MISSION_8_4_LOWER)) > MISSION_8_4_THRESHOLDS;
}