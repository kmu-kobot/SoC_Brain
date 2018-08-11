//
// Created by Gihyeon Yang on 2018. 7. 13..
//

#include "MISSION_9_LAST_BARRICADE.h"

int mission_9_1_go_front(U16 *image) {
    ACTION_WALK_CHECK(SLOW, OBLIQUE, 20, mission_9_1_go_front_of_yellow_barricade, image, 1);
    RobotSleep(1);
}

int mission_9_1_go_front_of_yellow_barricade(U16 *image) {
    U32 cols[6] = {50, 70, 90, 110, 130}, row, i, cnt;
    U16 checkHurdleLine[5] = {0,}, tmp;
    int range = 0;

    for (i = 0; i < 5; ++i) {
        for (row = HEIGHT - 1; row > 0; --row) {
            cnt = 0;
            for (range = -5; range < 5; range++) {
                cnt += GetValueRGBYOBK(GetPtr(image, row, cols[i] + range, WIDTH), BLACK);
            }

            if (cnt > 7) {
                checkHurdleLine[i] = (U16) row;
                break;
            }
        }
    }

    printf("\n\n");
    for (i = 0; i < 5; ++i) {
        printf("%d\t", checkHurdleLine[i]);
    }
    printf("\n\n");

    if (checkHurdleLine[0] + checkHurdleLine[1] + checkHurdleLine[2] + checkHurdleLine[3] + checkHurdleLine[4] < 80) {
        printf("yet...\n\n");
        return 0;
    }

    int j;
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4 - i; ++j) {
            if (checkHurdleLine[i] > checkHurdleLine[i + 1]) {
                tmp = checkHurdleLine[i];
                checkHurdleLine[i] = checkHurdleLine[i + 1];
                checkHurdleLine[i + 1] = tmp;
            }
        }
    }

    printf("AVG: %d\n", checkHurdleLine[2]); // AVG -> MEDIAN

    return (80 <= checkHurdleLine[2]);
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

    printf("M9-1: WAIT: %f\n", (double) cntYellow * 100 / (WIDTH * HEIGHT));
    return (double) cntYellow * 100 / (WIDTH * HEIGHT) > MISSION_9_THRESHOLDS;
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

    printf("M9-2: END: %f\n", (double) cntYellow * 100 / (WIDTH * HEIGHT));
    return (double) cntYellow * 100 / (WIDTH * HEIGHT) < (1); // 1보다 조금 높게 주는게 나을지도? 이거는 대회장 바리게이트 상태에 따라 다르겠다
}

void mission_9_3_escape_yellow_barricade(int repeat) {
    CHECK_INIT(MIDDLE, DOWN);
    RobotSleep(2);
    // TODO:
    ACTION_MOTION(WALK_FAST_SET_9, MIDDLE, DOWN);
    ACTION_MOTION(WALK_FAST_SET_9, MIDDLE, DOWN);
}
