//
// Created by Gihyeon Yang on 2018. 7. 11..
//

#include "MISSION_7_YELLOW_HOLE_BRIDGE.h"

void mission_7_1_watch_below(U16 *image, int repeat) { // 합쳐도 될듯
    ACTION_WALK_CHECK(OBLIQUE, image, mission_7_1_wait_front_of_yellow_hole_bridge, 1, repeat);
    RobotSleep(1);
}

int mission_7_1_attach_yellow_bridge(U16 *image) { // 무작정 붙이기 쓰는거보다 기울기 보고 회전 조금 시키는것도 좋을거같음
    double ratio = getColorRatio2(image, 20, ROBOT_KNEE, 50, WIDTH - 50, YELLOW, CH2);

    ACTION_ATTACH(1);
    return ratio > 70.0;
}

int mission_7_1_wait_front_of_yellow_hole_bridge(U16 *image) {
    // U32 col, row, cntYellow = 0;
    // for (row = 0; row < HEIGHT; ++row) {
    //     for (col = 0; col < WIDTH; ++col) {
    //         cntYellow += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW) ||
    //                       GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), CH2));
    //     }
    // }
    //
    // double r = (double) cntYellow / (WIDTH * HEIGHT) * 100;
    //
    // printf("ratio = %f\n", r);
    //
    // int rReturn = r > MISSION_7_1_THRESHOLDS;
    //
    // return rReturn;

    double ratio = getColorRatio2(image, 40, HEIGHT, 0, WIDTH, YELLOW, CH2);

    return ratio > MISSION_7_1_THRESHOLDS;
}

int mission_7_6_jump_hole(void) {
    ACTION_MOTION(MISSION_7_YELLOW_DUMBLING, UP);
    RobotSleep(1);
    return 1;
}
