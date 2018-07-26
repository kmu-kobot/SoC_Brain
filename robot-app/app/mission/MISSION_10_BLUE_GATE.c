
//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#include "MISSION_10_BLUE_GATE.h"

int mission_10_1_catch_blue_gate(U16 *image) {
    U32 col, row;
    U16 blue_cnt = 0;

    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            blue_cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
        }
    }

    double ratio = (double) blue_cnt / (HEIGHT * WIDTH) * 100;

    printf("blue ratio is %f\n\n", ratio);

    return ratio >= MISSION_10_1_BLUE_GATE_RATIO;
}

void mission_10_1_front_walk(int repeat) {
    ACTION_WALK(FAST, DOWN, repeat);
    RobotSleep(2);
}