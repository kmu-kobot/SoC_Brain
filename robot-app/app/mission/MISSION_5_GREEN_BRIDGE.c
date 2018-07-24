//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#include "MISSION_5_GREEN_BRIDGE.h"

void mission_5_1_watch_below(int repeat) {
    ACTION_WALK(FAST, DOWN, repeat);
    RobotSleep(2);
}

int mission_5_1_check_black_line(U16 *image, int repeat) {
    U32 col, row, cntBlack = 0;
    for (row = 0; row < 60; ++row) {
        for (col = 0; col < WIDTH; col++) {
            cntBlack += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK);
        }
    }

    int rResult = (cntBlack * 100 / (60 * WIDTH)) > MISSION_5_1_THRESHOLDS;

    printf("M5-1: BLACK LINE\n");
    printf("BLACK: %d, BLACK / (WIDTH * HEIGHT) : %f\n",
           cntBlack, (double) (cntBlack * 100 / (HEIGHT * WIDTH)));

    if (rResult && repeat) {
        ACTION_WALK(FAST, DOWN, repeat);
        RobotSleep(2);
    }

    return rResult;
}

void mission_5_2_watch_side(void) {
    ACTION_INIT(MIDDLE, LEFT);
    RobotSleep(2);
}

int mission_5_3_climb_up_stairs(void) {
    ACTION_WALK(SLOW, OBLIQUE, 2);
    RobotSleep(2);
    ACTION_MOTION(MISSION_5_STAIR_UP, MIDDLE, OBLIQUE);
    RobotSleep(2);
    ACTION_WALK(FAST, DOWN, 1);
    RobotSleep(2);
    return 1;
}

void mission_5_5_watch_below(void) {
    ACTION_INIT(MIDDLE, DOWN);
    RobotSleep(1);
}

int mission_5_5_check_finish_black_line(U16 *image) {
    return mission_5_1_check_black_line(image, 0);
}

int mission_5_5_check_green_bridge_straight(U16 *image) {
    U32 col, i, cnt, range = 0, point[2][2] = {{0, MISSION_5_5_GREEN_BRIDGE_POINT_Y_1},
                                               {0, MISSION_5_5_GREEN_BRIDGE_POINT_Y_2}};

    U32 max = MISSION_5_5_GREEN_BRIDGE_SLOPE_RANGE + 1;
    int pixelQueue[MISSION_5_5_GREEN_BRIDGE_SLOPE_RANGE + 1] = {0,};
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
        ACTION_TURN(((r > 0) ? DIR_RIGHT : DIR_LEFT), MIDDLE, DOWN, 1);
    }

    if (!rResult) { RobotSleep(1); }

    return rResult;
}

int mission_5_5_check_green_bridge_center(U16 *image) {
    U16 dir, cnt;
    int col, row, flagSign, green_len[2] = {0,};

    for (dir = 0; dir < 2; ++dir) {
        flagSign = (dir) ? 1 : -1;
        for (col = 0; col < WIDTH / 2; ++col) {
            cnt = 0;
            for (row = 10;
                 row < 60;
                 ++row) {
                if (CheckCol(WIDTH / 2 + 0 + col * flagSign)) {
                    cnt += GetValueRGBYOBK(GetPtr(image, row, WIDTH / 2 + 0 + col * flagSign, WIDTH), GREEN);
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
        RobotSleep(2);
    }

    return ((r > 0) ? r : (-r)) <= MISSION_5_5_GREEN_BRIDGE_THRESHOLDS;
}

int mission_5_5_short_walk_on_green_bridge(int repeat) {
    ACTION_WALK(SLOW, DOWN, repeat);
    return 1;
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

    return blackLen < MISSION_5_6_BLACK_LEN_LENGTH;
}

int mission_5_6_set_straight(U16 *image) {
    U32 cnt, row, i, range;
    int point[2][2] = {{MISSION_5_6_BLACK_COL_1, 0},
                       {MISSION_5_6_BLACK_COL_2, 0}};

    for (i = 0; i < 2; ++i) {
        for (row = 10; row < HEIGHT; ++row) {
            cnt = 0;
            for (range = 0; range < MISSION_5_6_BLACK_RANGE; ++range) {
                cnt += GetValueRGBYOBK(GetPtr(image, row, point[i][0] + range, WIDTH), BLACK);
            }

            if (cnt > 5) {
                point[i][1] = HEIGHT - row;
            }

        }
    }

    printf("\t\t- M5-6: SET STRAIGHT\n");
    printf("\t\t\t+ %d %d\n\n", point[0][1], point[1][1]);

    int r = (point[0][1] - point[1][1]);

    int rResult = 1;
    if (((r > 0) ? r : -r) > MISSION_5_6_GREEN_BRIDGE_SLOPE) {
        ACTION_TURN(((r < 0) ? DIR_LEFT : DIR_RIGHT), MIDDLE, DOWN, 1);
        RobotSleep(1);
        rResult = 0;
    }

    return rResult;
}

int mission_5_7_climb_down_stairs(void) {
    ACTION_BIT(BIT_FRONT, 2);
    RobotSleep(2);
    ACTION_MOTION(MISSION_5_STAIR_DOWN, MIDDLE, OBLIQUE);
    RobotSleep(2);
    ACTION_WALK(SLOW, OBLIQUE, 5);
    RobotSleep(2);
    return 1;
}
