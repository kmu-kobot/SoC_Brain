//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#include "MISSION_0_READY_PLAY.h"

int mission_0_1_wait_play(U16 *image) {
    U32 col, row, cntYellow = 0;

    for (row = MISSION_0_UPPER; row < MISSION_0_UPPER + MISSION_0_RANGE; ++row) {
        for (col = 0; col < WIDTH; col++) {
            cntYellow += GetValueRGBYOBK(
                    GetPtr(image, row, col, WIDTH),
                    YELLOW
            );
        }
    }

    return cntYellow * 100 / (WIDTH * MISSION_0_RANGE) > MISSION_0_THRESHOLDS;
}
