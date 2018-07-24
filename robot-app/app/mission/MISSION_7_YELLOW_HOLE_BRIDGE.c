//
// Created by Gihyeon Yang on 2018. 7. 11..
//

#include "MISSION_7_YELLOW_HOLE_BRIDGE.h"

void mission_7_1_watch_below(int repeat) {
    ACTION_WALK(FAST, DOWN, repeat);
    RobotSleep(2);
}

int mission_7_1_wait_front_of_yellow_hole_bridge(U16 *image, int repeat) {
    U32 col, row, cntYellow = 0;
    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            cntYellow += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW) ||
                          GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW_CH2));
        }
    }

    double r = (double) cntYellow / (WIDTH * HEIGHT) * 100;

    printf("ratio = %f\n", r);

    int rReturn = r > MISSION_7_1_THRESHOLDS;

    if (rReturn) {
        ACTION_WALK(SLOW, OBLIQUE, repeat);
        RobotSleep(2);
    }

    return rReturn;
}

void mission_7_2_watch_below(void) {
    ACTION_INIT(MIDDLE, OBLIQUE);
    RobotSleep(1);
}

int mission_7_2_before_bridge_set_center(U16 *image) {
    U16 dir;
    int col, row, red_bridge[2] = {0,};

    for (dir = 0; dir < 2; ++dir) {
        for (row = 0; row < HEIGHT; ++row) {
            for (col = WIDTH / 2 * (dir); col < WIDTH / 2 * (dir + 1); ++col) {
                red_bridge[dir] += (
                        GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW) ||
                        GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW_CH2)
                );
            }
        }
    }

    // 0: LEFT, 1: RIGHT
    double r[2] = {
            (double) red_bridge[0] / (HEIGHT * WIDTH / 2) * 100,
            (double) red_bridge[1] / (HEIGHT * WIDTH / 2) * 100
    };

    double s = r[0] - r[1];

    printf("\nM5-5: SET CENTER\n");
    printf("LEFT: %f, RIGHT: %f, r: %f\n\n", r[0], r[1], r[0] - r[1]);

    if (((s > 0) ? s : -s) > MISSION_7_2_RED_BRIDGE_THRESHOLDS) {
        ACTION_MOVE(LONG, ((s > 0) ? DIR_LEFT : DIR_RIGHT), MIDDLE, OBLIQUE, 1);
        RobotSleep(1);
    }

    return ((s > 0) ? s : -s) <= MISSION_7_2_RED_BRIDGE_THRESHOLDS;
}

int mission_7_3_climb_yellow_hole_bridge() {
    ACTION_BIT(BIT_FRONT, 1);
    RobotSleep(1);
    ACTION_MOTION(MISSION_5_STAIR_UP, MIDDLE, OBLIQUE);
    RobotSleep(1);
    ACTION_WALK(SLOW, OBLIQUE, 2);
    RobotSleep(1);
    return 1;
}

int mission_7_4_set_center(U16 *image) {
    U32 i, row, col[2] = {MISSION_7_4_COL_POINT_1,
                          MISSION_7_4_COL_POINT_2}, cnt;
    U16 yellow_len[2] = {0,};
    int j;
    for (i = 0; i < 2; ++i) {
        for (row = 30; row < HEIGHT; ++row) {
            cnt = 0;
            for (j = -3; j < 3; ++j) {
                cnt += (GetValueRGBYOBK(GetPtr(image, row, col[i] + j, WIDTH), YELLOW) ||
                        GetValueRGBYOBK(GetPtr(image, row, col[i] + j, WIDTH), YELLOW_CH2));
            }

            if (cnt < 2) {
                yellow_len[i] = (U16) (HEIGHT - row);
                break;
            }
        }
    }

    printf("yellow_len[0] = %d, yellow_len[1] = %d\n", yellow_len[0], yellow_len[1]);

    double e = (double) (yellow_len[0] + yellow_len[1]) / 2;

    printf("distance is %f", e);

    int rResult = 0;
    if (e < MISSION_7_4_YELLOW_LENGTH - MISSION_7_4_YELLOW_LENGTH_ERROR) {
        //오른쪽 이동
        ACTION_MOVE(SHORT, DIR_RIGHT, MIDDLE, LEFT, 1);
    } else if (e > MISSION_7_4_YELLOW_LENGTH + MISSION_7_4_YELLOW_LENGTH_ERROR) {
        //왼쪽 이동
        ACTION_MOVE(SHORT, DIR_LEFT, MIDDLE, LEFT, 1);
    } else {
        rResult = 1;
    }

    if (!rResult) {
        RobotSleep(1);
    }

    return rResult;
}

int mission_7_4_set_straight(U16 *image) {
    U32 i, row, col[2] = {MISSION_7_4_COL_POINT_1,
                          MISSION_7_4_COL_POINT_2};
    U16 pos_yellow[2] = {0,};
    for (i = 0; i < 2; ++i) {
        for (row = 30; row < HEIGHT; ++row) {
            if (GetValueRGBYOBK(GetPtr(image, row, col[i], WIDTH), YELLOW) ||
                GetValueRGBYOBK(GetPtr(image, row, col[i], WIDTH), YELLOW_CH2) &&
                GetValueRGBYOBK(GetPtr(image, row, col[i] + 1, WIDTH), YELLOW) ||
                GetValueRGBYOBK(GetPtr(image, row, col[i] + 1 , WIDTH), YELLOW_CH2)) {
                break;
            }
            pos_yellow[i] += 1;
        }
    }

    printf("pos_yellow[0]: %d, pos_yellow[1]: %d\n", pos_yellow[0], pos_yellow[1]);

    double slope = (double) (pos_yellow[0] - pos_yellow[1]);

    printf("Slope is %f\n", slope);

    int rResult = 1;
    if (((slope > 0) ? slope : -slope) > MISSION_7_4_YELLOW_BRIDGE_SLOPE) {
        rResult = 0;
        ACTION_TURN(((slope > 0) ? DIR_LEFT : DIR_RIGHT), MIDDLE, LEFT, 1);
    }

    return rResult;
}

void mission_7_4_watch_below(void) {
    ACTION_INIT(MIDDLE, DOWN);
    RobotSleep(3);
}

int mission_7_5_walk_until_line_front_of_feet(U16 *image) {
    U32 col, row;
    U16 black_cnt = 0;

    for (row = 0; row < HEIGHT; ++row) {
        black_cnt = 0;
        for (col = MISSION_7_5_POINT_X_1; col < MISSION_7_5_POINT_X_2; ++col) {
            black_cnt += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW) ||
                          GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW_CH2));
        }

        if (black_cnt > 7) {
            black_cnt = (U16) row;
            break;
        }

    }

    printf("7-5: walk_until_black_line\n");
    printf("Ratio is %d.\n", black_cnt);

    int rResult = black_cnt > MISSION_7_5_LINE_RATIO;
    if (!rResult) {
        ACTION_WALK(SLOW, DOWN, 1);
    }

    ACTION_INIT(MIDDLE, DOWN);
    RobotSleep(1);

    return rResult;
}


int mission_7_6_jump_hole(void) {
    RobotSleep(2);
    ACTION_MOTION(MISSION_5_STAIR_DOWN, MIDDLE, OBLIQUE);
    RobotSleep(2);
    ACTION_WALK(SLOW, LEFT, 3);
    return 1;
}

void mission_7_7_watch_side(void) {
    ACTION_INIT(MIDDLE, LEFT);
    RobotSleep(2);
}

int mission_7_7_after_yellow_bridge_set_straight(U16 *image) {
    U32 row, i;
    U16 black_len[2] = {0,}, col[2] = {
            MISSION_7_2_BLACK_LINE_COL_POINT_1,
            MISSION_7_2_BLACK_LINE_COL_POINT_2
    };

    for (i = 0; i < 2; ++i) {
        for (row = HEIGHT; row > 0; --row) {
            if (GetValueRGBYOBK(
                        GetPtr(image, row, col[i], WIDTH),
                        BLACK
                ) &&
                GetValueRGBYOBK(
                        GetPtr(image, row, col[i] + 1, WIDTH),
                        BLACK
                )) {
                break;
            }
            black_len[i] += 1;
        }
    }

    printf("M7-4: SLOPE\n");
    printf("black[0]: %d, black_len[1]: %d.\n", black_len[0], black_len[1]);

    double s = (
            (black_len[0] - black_len[1]) /
            MISSION_7_2_BLACK_LINE_COL_POINT_1 - MISSION_7_2_BLACK_LINE_COL_POINT_2
    );

    printf("Slope : %f\n", s);

    s *= 100;

    int rResult = 0;
    if (s < MISSION_7_7_BLACK_LINE_SLOPE + MISSION_7_7_BLACK_LINE_SLOPE_ERROR) {
        ACTION_TURN(DIR_RIGHT, MIDDLE, LEFT, 1);
    } else if (s > MISSION_7_7_BLACK_LINE_SLOPE - MISSION_7_7_BLACK_LINE_SLOPE_ERROR) {
        ACTION_TURN(DIR_LEFT, MIDDLE, LEFT, 1);
    } else {
        rResult = 1;
    }

    ACTION_INIT(MIDDLE, LEFT);

    return rResult;
}

int mission_7_7_after_yellow_bridge_set_center(U16 *image) {
    // TODO: 옆에 보고 센터 맞추는거로 다시 바꿔야함
    return mission_7_2_before_bridge_set_center(image);
}
