//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#include "MISSION_5_GREEN_BRIDGE.h"

void mission_5_1_watch_below(int repeat) {
    ACTION_WALK(FAST, DOWN, repeat);
    RobotSleep(1);
}

int mission_5_11_attach(U16 *image) {
    U32 row, col, cnt = 0;
    for (row = 20; row < ROBOT_KNEE; ++row) {
        for (col = 50; col < WIDTH - 50; ++col) {
            cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK);
        }
    }

    printf("\n\n xxx %f \n\n", (double) cnt * 100 / ((ROBOT_KNEE - 20) * WIDTH));

    if ((double) cnt * 100 / ((ROBOT_KNEE - 20) * 80) >= 33) {
        ACTION_WALK(CLOSE, DOWN, 4);
        return 1;
    } else {
        ACTION_WALK(CLOSE, DOWN, 2);
        return 0;
    }
}

int mission_5_1_check_black_line(U16 *image, int repeat) {
    U32 col, row, cntBlack = 0;

    for (row = 0; row < 60; ++row) {
        for (col = 0; col < WIDTH; col++) {
            cntBlack += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK);
        }
    }

    int rResult = (cntBlack * 100 / (60 * WIDTH)) > CASE_5_0_DETECTION;

    printf("M5-1: BLACK LINE\n");
    printf("BLACK: %d, BLACK / (WIDTH * HEIGHT) : %f\n",
           cntBlack, (double) (cntBlack * 100 / (HEIGHT * WIDTH)));
    printf((rResult) ? "SUCCESS\n" : "FAIL\n");

    if (rResult && repeat) {
        cntBlack = 0;
    }

    return rResult;
}

void mission_5_2_watch_side(void) {
    CHECK_INIT(MIDDLE, LEFT);
    RobotSleep(5);
}

int mission_5_3_climb_up_stairs(void) {
    RobotSleep(1);
    ACTION_MOTION(MISSION_5_STAIR_UP, MIDDLE, OBLIQUE);
    RobotSleep(1);
    CHECK_INIT(MIDDLE, DOWN);
    RobotSleep(1);
    return 1;
}

int mission_5_5_check_finish_black_line(U16 *image) {
    return mission_5_1_check_black_line(image, 0);
}

int mission_5_5_check_green_bridge_straight(U16 *image) {

    // 한쪽에 쏠려 있을때 처리하는거 개발
    U32 col, row, cnt;

    // 오른쪽에 많이 붙었을때 왼쪽으로 걷는거 개발
    cnt = 0;
    for (col = 0; col < 30; col++) {
        for (row = 0; row < HEIGHT; row++) {
            cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), GREEN);
        }
    }

    double s = (double) cnt * 100 / (30 * HEIGHT);

    printf("%d %f\n", cnt, s);

    if (s > 13) {
        printf("GOGO");
        ACTION_MOVE(SHORT, DIR_LEFT, MIDDLE, DOWN, 3);
        ACTION_TURN(SHORT, DIR_LEFT, MIDDLE, DOWN, 4);
        return 0;
    }


    // // 왼쪽에 많이 붙었을때 왼쪽으로 걷는거 개발
    // cnt = 0;
    // for (col = 150; col < 180; col++) {
    //     for (row = 0; row < HEIGHT; row++) {
    //         cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), GREEN);
    //     }
    // }

    // s = (double) cnt * 100 / (30 * HEIGHT);

    // printf("%d %f\n", cnt, s);

    // if (s > 13) {
    //     printf("GOGO");
    //     ACTION_MOVE(SHORT, DIR_RIGHT, MIDDLE, DOWN, 1);
    //     ACTION_TURN(SHORT, DIR_LEFT, MIDDLE, DOWN, 1);
    //     return 0;
    // }
    // //

    U32 i, range = 0, point[2][2] = {{0, MISSION_5_5_GREEN_BRIDGE_POINT_Y_1},
                                     {0, MISSION_5_5_GREEN_BRIDGE_POINT_Y_2}};
    U32 max = MISSION_5_5_GREEN_BRIDGE_SLOPE_RANGE + 1;
    int pixelQueue[7 + 1] = {0,};
    int front = 0, rear = 0;

    for (i = 0; i < 2; ++i) {
        front = 0;
        rear = 0;
        for (col = 0; col < WIDTH; ++col) {

            if (front == rear) {
                for (range = 0; range < MISSION_5_5_GREEN_BRIDGE_SLOPE_RANGE; ++range) {
                    pixelQueue[rear] = GetValueRGBYOBK(GetPtr(image, point[i][1], col + range, WIDTH), GREEN);
                    rear++;
                    rear %= max;
                }
            } else {
                front++;
                front %= max;
                pixelQueue[rear] = GetValueRGBYOBK(GetPtr(image, point[i][1], col + range, WIDTH), GREEN);
                rear++;
                rear %= max;
            }

            cnt = 0;
            for (range = 0; range < MISSION_5_5_GREEN_BRIDGE_SLOPE_RANGE; ++range) {
                cnt += pixelQueue[range];
            }

            if (cnt >= 3) {
                point[i][0] = col;
                break;
            }

        }
    }

    int r = (point[0][0] - point[1][0]);
    printf("p0: %d\t p1: %d\t r:%d\t\n", point[0][0], point[1][0], r);

    int rResult = 1;
    if (((r > 0) ? r : -r) > MISSION_5_5_GREEN_BRIDGE_SLOPE) {
        rResult = 0;
        ACTION_TURN(SHORT, ((r > 0) ? DIR_RIGHT : DIR_LEFT),
                    MIDDLE, DOWN, 2);
        RobotSleep(1);
    }

    return rResult;
}

int mission_5_5_check_green_bridge_center(U16 *image) {
    U16 dir, cnt;
    int col, row, flagSign, green_len[2] = {0,};

    for (dir = 0; dir < 2; ++dir) {
        flagSign = (dir) ? 1 : -1;
        for (col = 0; col < WIDTH / 2 - 1; ++col) {
            cnt = 0;
            for (row = 10;
                 row < 60;
                 ++row) {
                if (CheckCol(WIDTH / 2 + ROBOT_OFFSET + col * flagSign)) {
                    cnt += GetValueRGBYOBK(GetPtr(image, row, WIDTH / 2 + ROBOT_OFFSET + col * flagSign, WIDTH), GREEN);
                } else {
                    break;
                }
            }

            if (cnt < 3) {
                green_len[dir] = col;
                break;
            }
        }
    }

    // 0: LEFT, 1: RIGHT
    int r = green_len[0] - green_len[1];

    printf("\nM5-5: SET CENTER\n");
    printf("LEFT: %d, RIGHT: %d, r: %d\n\n", green_len[0], green_len[1], r);

    if (((r > 0) ? r : (-r)) > MISSION_5_5_GREEN_BRIDGE_THRESHOLDS) {
        ACTION_MOVE(SHORT, ((r > 0) ? DIR_LEFT : DIR_RIGHT), MIDDLE, DOWN, 1);
        RobotSleep(1);
    }

    return ((r > 0) ? r : (-r)) <= MISSION_5_5_GREEN_BRIDGE_THRESHOLDS;
}

int mission_5_5_short_walk_on_green_bridge(int repeat) {
    ACTION_WALK(FAST, DOWN, repeat);
    RobotSleep(4);
    return 1;
}

int mission_5_5_get_repeat(U16 *image) {
    U32 col, row, cnt = 0;
    for (row = 0; row < HEIGHT; ++row) {
        for (col = 40; col < 140; ++col) {
            cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), GREEN);
        }
    }

    return ((double) cnt * 100 / (HEIGHT * 100) >= 5) ? 4 : 3;
}


int mission_5_6_set_only_one_bk_bar(U16 *image) {
    U32 col, row, blackLen = 0;
    for (row = 0; row < HEIGHT; ++row) {
        blackLen = 0;
        for (col = MISSION_5_6_POINT_X_1;
             col < MISSION_5_6_POINT_X_2;
             ++col) {
            blackLen += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK);
        }

        if (blackLen >= MISSION_5_6_BLACK_LEN_THRESHOLDS) {
            blackLen = HEIGHT - row;
            break;
        }

    }

    printf("\n\t\t- M5-6: SET CENTER\n");
    printf("\t\t\t+ bk_len length: %d\n", blackLen);

    if (blackLen > MISSION_5_6_BLACK_LEN_LENGTH) {
        if (MISSION_5_6_BLACK_LEN_LENGTH - blackLen >= 15) {
            ACTION_BIT(FRONT, 4);
        } else {
            ACTION_BIT(FRONT, 2);
        }
        RobotSleep(1);
    }

    return blackLen < MISSION_5_6_BLACK_LEN_LENGTH;
}

int mission_5_6_set_straight(U16 *image) {
    U32 cnt, row, i;
    int range, point[2][2] = {{MISSION_5_6_BLACK_COL_1, 0},
                              {MISSION_5_6_BLACK_COL_2, 0}};

    for (i = 0; i < 2; ++i) {
        for (row = 10; row < HEIGHT; ++row) {
            cnt = 0;
            for (range = (-MISSION_5_6_BLACK_RANGE); range < MISSION_5_6_BLACK_RANGE; ++range) {
                cnt += GetValueRGBYOBK(GetPtr(image, row, point[i][0] + range, WIDTH), BLACK);
            }

            if (cnt > 5) {
                point[i][1] = HEIGHT - row;
                break;
            }

        }
    }

    printf("\t\t- M5-6: SET STRAIGHT\n");
    printf("\t\t\t+ %d %d\n\n", point[0][1], point[1][1]);

    int r = (point[0][1] - point[1][1]);

    int rResult = 1;
    if (((r > 0) ? r : -r) > MISSION_5_6_GREEN_BRIDGE_SLOPE) {
        ACTION_TURN(SHORT, ((r < 0) ? DIR_LEFT : DIR_RIGHT), MIDDLE, DOWN, 2);
        rResult = 0;
    }

    return rResult;
}

int mission_5_7_climb_down_stairs(void) {
    ACTION_MOTION(MISSION_5_STAIR_DOWN, MIDDLE, OBLIQUE);
    CHECK_INIT(MIDDLE, OBLIQUE);
    ACTION_WALK(FAST, OBLIQUE, 13);
    return 1;
}

void mission_5_5_set_center(U16 *image) {
    U16 dir, cnt;
    int col, row, flagSign, green_len[2] = {0,};

    for (dir = 0; dir < 2; ++dir) {
        flagSign = (dir) ? 1 : -1;
        for (col = 0; col < WIDTH / 2 - 1; ++col) {
            cnt = 0;
            for (row = ROBOT_KNEE;
                 row < HEIGHT;
                 ++row) {
                if (CheckCol(WIDTH / 2 + ROBOT_OFFSET + col * flagSign)) {
                    cnt += GetValueRGBYOBK(GetPtr(image, row, WIDTH / 2 + ROBOT_OFFSET + col * flagSign, WIDTH), GREEN);
                } else {
                    break;
                }
            }

            if (cnt < 3) {
                green_len[dir] = col;
                break;
            }
        }
    }

    // 0: LEFT, 1: RIGHT
    int r = green_len[0] - green_len[1];

    printf("\nM5-5: SET CENTER\n");
    printf("LEFT: %d, RIGHT: %d, r: %d\n\n", green_len[0], green_len[1], r);

    if (((r > 0) ? r : (-r)) > 7) {
        ACTION_MOVE(SHORT, ((r > 0) ? DIR_LEFT : DIR_RIGHT), MIDDLE, DOWN, 2);
        RobotSleep(1);
    }

}