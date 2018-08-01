//
// Created by Gihyeon Yang on 2018. 7. 13..
//

#include "MISSION_9_LAST_BARRICADE.h"

int mission_9_1_go_front_of_yellow_barricade(U16 *image) {
    U32 cols[3] = {70, 90, 110}, row, i, cnt;
    U16 checkHurdleLine[3] = {0,};
    int range = 0;

    for (i = 0; i < 3; ++i) {
        for (row = HEIGHT - 1; row > 0; --row) {
            cnt = 0;
            for (range = -5; range < 5; range++) {
                cnt += GetValueRGBYOBK(GetPtr(image, row, cols[i] + range, WIDTH), BLACK);
            }

            if (cnt > 6) {
                checkHurdleLine[i] = (U16) row;
                break;
            }
        }
    }


    double s = 0;
    for (i = 0; i < 3; i++) {
        s += checkHurdleLine[i];
    }

    s /= 3;

    if (s > 60) {
        CHECK_INIT(MIDDLE, OBLIQUE);
        CHECK_INIT(MIDDLE, UP);
        return 1;
    } else {
        ACTION_WALK(FAST, OBLIQUE, 3);
        return 0;
    }

}

int mission_9_1_wait_yellow_barricade(U16 *image) {
    U32 col, row, cntYellow = 0;

    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            cntYellow += GetValueRGBYOBK(
                    GetPtr(image, row, col, WIDTH),
                    YELLOW
            );
        }
    }

    printf("M9-1: WAIT: %d\n", cntYellow * 100 / (WIDTH * HEIGHT));
    return cntYellow * 100 / (WIDTH * HEIGHT) > MISSION_9_THRESHOLDS;
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
    return cntYellow * 100 / (WIDTH * HEIGHT) < (5);
}

void mission_9_3_escape_yellow_barricade(int repeat) {
    CHECK_INIT(MIDDLE, OBLIQUE);
    RobotSleep(2);
    ACTION_WALK(FAST, OBLIQUE, repeat);
}
