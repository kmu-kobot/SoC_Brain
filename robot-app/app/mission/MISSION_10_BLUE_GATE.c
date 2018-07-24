
//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#include "MISSION_10_BLUE_GATE.h"

void mission_10_1_watch_up(void) {
    //TODO: 로봇 고개 드는 동작 넣기
    RobotSleep(5);
}

void mission_10_1_watch_side(int repeat) {
    ACTION_INIT(LOW, RIGHT);
    RobotSleep(repeat);
}


void mission_10_1_front_walk(int repeat) {
    ACTION_WALK(FAST, OBLIQUE, 3);
    RobotSleep(5);
}

int mission_10_1_set_center(U16 *image) {
    U32 i, row, col[2] = {MISSION_10_1_COL_POINT_1,
                          MISSION_10_1_COL_POINT_2};
    U16 black_len[2] = {0,};
    for (i = 0; i < 2; ++i) {
        for (row = HEIGHT; row > 0; --row) {
            if (GetValueRGBYOBK(GetPtr(image, row, col[i], WIDTH), BLACK) == 1 &&
                GetValueRGBYOBK(GetPtr(image, row, col[i] + 1, WIDTH), BLACK) == 1) {
                black_len[i] = HEIGHT - row;
                break;
            }
        }
    }

    printf("black_len[0] = %d, black_len[1] = %d\n", black_len[0], black_len[1]);

    double e = (double) (black_len[0] + black_len[1]) / 2;

    printf("distance is %f", e);

    ACTION_INIT(LOW, OBLIQUE);

    int rResult = 0;
    if (e < MISSION_10_1_BLACK_LENGTH - MISSION_10_1_BLACK_LENGTH_ERROR) {
        //오른쪽 이동
        ACTION_MOVE(DIR_LEFT, LOW, OBLIQUE, 1);
    } else if (e > MISSION_10_1_BLACK_LENGTH + MISSION_10_1_BLACK_LENGTH_ERROR) {
        //왼쪽 이동
        ACTION_MOVE(DIR_RIGHT, LOW, OBLIQUE, 1);
    } else {
        rResult = 1;
    }

    ACTION_INIT(LOW, OBLIQUE);

    return rResult;
}

int mission_10_1_set_straight(U16 *image) {

    U32 i, row, col[2] = {MISSION_10_1_COL_POINT_1,
                          MISSION_10_1_COL_POINT_2};
    U16 pos_black[2] = {0,};
    for (i = 0; i < 2; ++i) {
        for (row = HEIGHT; row > 0; --row) {
            if (GetValueRGBYOBK(GetPtr(image, row, col[i], WIDTH), BLACK) == 1 &&
                GetValueRGBYOBK(GetPtr(image, row, col[i] + 1, WIDTH), BLACK) == 1) {
                break;
            }
            pos_black[i] += 1;
        }
    }

    printf("pos_black[0]: %d, pos_black[1]: %d\n", pos_black[0], pos_black[1]);

// s>0 l
// s < 0
    double slope = (double) (pos_black[0] - pos_black[1]);

    printf("Slope is %f\n", slope);

    ACTION_INIT(LOW, OBLIQUE);

    int rResult = 1;
    if (((slope > 0) ? slope : -slope) > MISSION_10_1_BLUE_GATE_SLOPE) {
        rResult = 0;
        if (slope > 0) {
            ACTION_TURN(DIR_RIGHT, LOW, OBLIQUE, 2);
        } else if (slope < 0) {
            ACTION_TURN(DIR_LEFT, LOW, OBLIQUE, 2);
        }
    }

    return rResult;
    //return 0;

}

void mission_10_2_watch_side(int repeat) {
    ACTION_INIT(LOW, LEFT);
    RobotSleep(repeat);

}

int mission_10_2_catch_blue_gate(U16 *image) {
    U32 col, row;
    U16 blue_cnt = 0;

    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            blue_cnt += GetValueRGBYOBK(
                    GetPtr(image, row, col, WIDTH),
                    BLUE
            );
        }
    }

    double ratio = blue_cnt / (HEIGHT * WIDTH) * 100;

    printf("blue ratio is %f\n\n", ratio);

    return ratio > MISSION_10_1_BLUE_GATE_RATIO;
}

int mission_10_2_catch_green_bridge(U16 *image) {
    U32 col, row;
    U16 green_cnt = 0;
    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            green_cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), GREEN);
        }
    }

    double ratio = green_cnt / (HEIGHT * WIDTH) * 100;

    printf("green ratio is %f\n\n", ratio);

    ACTION_INIT(LOW, OBLIQUE);

    int rResult = 0;
    if (ratio > 5) {
        rResult = 1;
    }

    return rResult;
}

int mission_10_3_escape_blue_gate(void) {
    ACTION_WALK(FAST, OBLIQUE, 3);
    RobotSleep(3);

    return 1;
}
