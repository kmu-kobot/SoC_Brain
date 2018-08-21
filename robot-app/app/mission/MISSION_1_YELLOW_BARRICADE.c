//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#include "MISSION_1_YELLOW_BARRICADE.h"

int mission_1_1_wait_yellow_barricade(U16 *image) {
    // U32 col, row, cntYellow = 0;
    //
    //
    // for (row = CASE_1_UPPER; row < CASE_1_LOWER; ++row) {
    //     for (col = 0; col < WIDTH; col++) {
    //         cntYellow += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW);
    //     }
    // }
    //
    // printf("\n%d~%d, %d\nRESULT: %f\n", CASE_1_UPPER, CASE_1_LOWER, CASE_1_RATIO,
    //        (double) cntYellow * 100 / (WIDTH * (CASE_1_LOWER - CASE_1_UPPER)));
    // printf((cntYellow * 100 / (WIDTH * (CASE_1_LOWER - CASE_1_UPPER)) > CASE_1_RATIO) ? "SUCCESS" : "FAIL");
    // return (cntYellow * 100 / (WIDTH * (CASE_1_LOWER - CASE_1_UPPER)) > CASE_1_RATIO);

    U32 i, success = 0;
    for (i = 0; i < 5 && success < 3; ++i)
    {
        setFPGAVideoData(image);
        success += (getColorRatio1(image, CASE_1_UPPER, CASE_1_LOWER, 0, WIDTH, YELLOW) > CASE_1_RATIO);
    }

    return success >= 3;
}

int mission_1_2_end_yellow_barricade(U16 *image) {
    // U32 col, row, cntYellow = 0;
    // for (row = CASE_1_UPPER; row < CASE_1_LOWER; ++row) {
    //     for (col = 0; col < WIDTH; col++) {
    //         cntYellow += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW);
    //     }
    // }
    //
    // printf("\n%d~%d, %d\nRESULT: %f\n", CASE_1_UPPER, CASE_1_LOWER, CASE_1_RATIO,
    //        (double) cntYellow * 100 / (WIDTH * (CASE_1_LOWER - CASE_1_UPPER)));
    // printf((cntYellow * 100 / (WIDTH * (CASE_1_LOWER - CASE_1_UPPER)) < CASE_1_RATIO) ? "SUCCESS" : "FAIL");
    // return (cntYellow * 100 / (WIDTH * (CASE_1_LOWER - CASE_1_UPPER)) < CASE_1_RATIO);

    U32 i, success = 0;
    for (i = 0; i < 5 && success < 3; ++i)
    {
        setFPGAVideoData(image);
        success += (getColorRatio1(image, CASE_1_UPPER, CASE_1_LOWER, 0, WIDTH, YELLOW) < CASE_1_RATIO);
    }

    return success >= 3;
}

void mission_1_3_escape_yellow_barricade(int repeat) {
    RobotSleep(2);
    // TODO
    ACTION_WALK(FAST, DOWN, 11);
    RobotSleep(1);
}
