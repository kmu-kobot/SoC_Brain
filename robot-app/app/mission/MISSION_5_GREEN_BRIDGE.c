//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#include "MISSION_5_GREEN_BRIDGE.h"

void mission_5_1_watch_below(U16 *image, int repeat) {
    ACTION_WALK_CHECK(DOWN, image, mission_5_1_check_black_line, 1, repeat);
}

int mission_5_11_attach(U16 *image) {
    double ratio = getColorRatio1(image, 20, ROBOT_KNEE, 50, WIDTH-50, BLACK);

    if (ratio < 20.0)
    {
        ACTION_ATTACH(1);
        return 0;
    }
    return 1;
}

int mission_5_1_check_black_line(U16 *image) {
    U32 col, row, cntBlack = 0;

    for (row = ROBOT_KNEE - 10; row > 0; --row) {
        cntBlack = 0;
        for (col = 0; col < WIDTH; col++) {
            cntBlack += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK);
        }
        if (cntBlack > 50) {
            cntBlack = row; // 이거 변수 하나로 2개 역할 하는건가?? 그런거면 변수 하나 더만들어서 나눠주는게 나을거같음 그냥 여기서 row 검사해서 바로 return해도 될듯
            break;
        } else {
            cntBlack = 0;
        }
    }

    if (cntBlack < 0) {
        return 0;
    }

#ifdef DEBUG
    printf("%d\n", cntBlack);
#endif

    return cntBlack > 30;
}

void mission_5_3_climb_up_stairs(void) {
    CHECK_INIT(OBLIQUE);
    RobotSleep(3);
    ACTION_MOTION(MISSION_5_STAIR_UP, OBLIQUE);
    RobotSleep(1);
    CHECK_INIT(DOWN);
    RobotSleep(1);
    ACTION_BIT(FRONT, 2);
}

int mission_5_5_check_finish_black_line(U16 *image) {
    U32 col, row, cntBlack = 0;

    for (row = 0; row < 60; ++row) {
        for (col = 0; col < WIDTH; col++) {
            cntBlack += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK);
        }
    }

    int rResult = (cntBlack * 100 / (60 * WIDTH)) > 15;

    printf("M5-1: BLACK LINE\n");
    printf("BLACK: %d, BLACK / (WIDTH * HEIGHT) : %f\n",
           cntBlack, (double) (cntBlack * 100 / (HEIGHT * WIDTH)));
    printf((rResult) ? "SUCCESS\n" : "FAIL\n");

    return rResult;
}

int mission_5_5_check_green_bridge_straight(U16 *image) {

    // 한쪽에 쏠려 있을때 처리하는거 개발
    U32 col, row, cnt;

    // 오른쪽에 많이 붙었을때 왼쪽으로 걷는거 개발
    cnt = 0;  // 한쪽만 보고 맞추는거보다 양쪽 다 보는게 좋을거같음
    for (col = 0; col < 30; col++) {
        for (row = 0; row < HEIGHT; row++) {
            cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), GREEN);
        }
    }

    double s = (double) cnt * 100 / (30 * HEIGHT);

    printf("%d %f\n", cnt, s);

    if (s > 13) {
        printf("GOGO");
        ACTION_MOVE(SHORT, DIR_LEFT, DOWN, 3);
        ACTION_TURN(SHORT, DIR_LEFT, DOWN, 4);
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
    //     ACTION_MOVE(SHORT, DIR_RIGHT, DOWN, 1);
    //     ACTION_TURN(SHORT, DIR_LEFT, DOWN, 1);
    //     return 0;
    // }
    // //

    // 포인트 더 늘리고 여러 프레임 확인하는게 좋을거같음

    U32 i, range = 0, point[2][2] = {{0, MISSION_5_5_GREEN_BRIDGE_POINT_Y_1},
                                     {0, MISSION_5_5_GREEN_BRIDGE_POINT_Y_2}};
    U32 max = MISSION_5_5_GREEN_BRIDGE_SLOPE_RANGE + 1;
    int pixelQueue[7 + 1] = {0,};
    int front = 0, rear = 0;

    for (i = 0; i < 2; ++i) {
        front = 0;
        rear = 0;
        for (col = 0; col < WIDTH - 10; ++col) {

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
                    DOWN, 1);
        RobotSleep(1);
    }

    return rResult;
}

int mission_5_3_attach_green(U16 *image) {
    U32 col, row, cnt = 0;
    for (col = 0; col < 45; ++col) {
        for (row = ROBOT_KNEE; row < HEIGHT; ++row) {
            cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK);
        }
    }

    ACTION_BIT(FRONT, 5);
    return (double) cnt * 100 / ((HEIGHT - ROBOT_KNEE) * 30) > 5;
}

int mission_5_5_check_green_bridge_center(U16 *image) { // 여러프레임 여러점
    U16 dir, cnt;
    int col, row, flagSign, green_len[2] = {0,};

    for (dir = 0; dir < 2; ++dir) {
        flagSign = (dir) ? 1 : -1;
        for (col = 0; col < WIDTH / 2 - 3; ++col) {
            cnt = 0;
            for (row = 10; row < 60; ++row) {
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
        ACTION_MOVE(SHORT, ((r > 0) ? DIR_LEFT : DIR_RIGHT), DOWN, 1);
        RobotSleep(1);
    }

    return ((r > 0) ? r : (-r)) <= MISSION_5_5_GREEN_BRIDGE_THRESHOLDS;
}

int mission_5_5_short_walk_on_green_bridge(int repeat) {
    // TODO
    ACTION_WALK(FAST, DOWN, 5);
    RobotSleep(3);
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


int mission_5_6_set_only_one_bk_bar(U16 *image) { // 기울어져있을때 똑바로 작동 못함
    U32 col, row, blackLen = 0;
    for (row = 0; row < HEIGHT; ++row) {
        blackLen = 0;
        for (col = MISSION_5_6_POINT_X_1; // 여기도 딱히 이유 없으면 for문 한줄로
             col < MISSION_5_6_POINT_X_2;
             ++col) {
            blackLen += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK); // cnt 변수 따로 만드는게 나을듯
        }

        if (blackLen >= MISSION_5_6_BLACK_LEN_THRESHOLDS) {
            blackLen = HEIGHT - row;
            break;
        }

    }

    printf("\n\t\t- M5-6: SET CENTER\n");
    printf("\t\t\t+ bk_len length: %d\n", blackLen);

    if (blackLen > MISSION_5_6_BLACK_LEN_LENGTH) { // 멀면 fast로
        ACTION_BIT(FRONT, 1);
        RobotSleep(1);
    }

    return blackLen < MISSION_5_6_BLACK_LEN_LENGTH;
}

int mission_5_12_set_straight(U16 *image) { // 여러점 여러프레임
    U32 cnt, row, i;
    int range, point[2][2] = {{80,  0},
                              {100, 0}};

    for (i = 0; i < 2; ++i) {
        for (row = HEIGHT - 1; row > 0; --row) {
            cnt = 0;
            for (range = (-MISSION_5_6_BLACK_RANGE); range < MISSION_5_6_BLACK_RANGE; ++range) {
                cnt += GetValueRGBYOBK(GetPtr(image, row, point[i][0] + range, WIDTH), GREEN);
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
    if (((r > 0) ? r : -r) > MISSION_5_5_GREEN_BRIDGE_SLOPE) {
        ACTION_TURN((((r > 0) ? r : -r) > 6) ? MIDDLE : SHORT, ((r < 0) ? DIR_LEFT : DIR_RIGHT), DOWN, 1);
        rResult = 0;
    }

    return rResult;
}

int mission_5_6_set_straight(U16 *image) { // 여러점 여러프레임
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
        ACTION_TURN(SHORT, ((r < 0) ? DIR_LEFT : DIR_RIGHT), DOWN, 1);
        RobotSleep(1);
        rResult = 0;
    }

    return rResult;
}

int mission_5_7_climb_down_stairs(void) {
    RobotSleep(1);
    ACTION_MOTION(MISSION_5_STAIR_DOWN, OBLIQUE);
    CHECK_INIT(OBLIQUE);
    //TODO
    ACTION_WALK(FAST, DOWN, 9);
    return 1;
}

void mission_5_5_set_center(U16 *image) { // 이거는 중심부터 좌우 초록색 길이 재는건가?? 담에 설명 한번 들어봐야 알듯
    U16 dir;
    int col, row, flagSign, green_len[2] = {0,};

    for (dir = 0; dir < 2; ++dir) {
        flagSign = (dir) ? 1 : -1;
        for (col = 0; col < WIDTH / 2 - 3; ++col) {
            for (row = ROBOT_KNEE;
                 row < HEIGHT;
                 ++row) { // 여기는 for문 왜 이렇게 써둔거?? 이유 딱히 없으면 그냥 평범하게 쓰는게 알아보기 편할듯
                green_len[dir] += GetValueRGBYOBK(GetPtr(image, row, WIDTH / 2 + ROBOT_OFFSET + col * flagSign, WIDTH),
                                                  GREEN);
            }

        }
    }

    // 0: LEFT, 1: RIGHT
    int r = green_len[0] - green_len[1];

    printf("\nM5-5: SET CENTER\n");
    printf("LEFT: %d, RIGHT: %d, r: %d\n\n", green_len[0], green_len[1], r);

    if (((r > 0) ? r : (-r)) > 20) {
        ACTION_MOVE(SHORT, ((r > 0) ? DIR_LEFT : DIR_RIGHT), DOWN, 1);
        RobotSleep(1);
    }

}

int mission_13_attach_black(U16 *image) { // 여러프레임 돌리고 수치에 따라서 fast로 빨리 가버리는게 필요할듯
    U32 col, row, cnt = 0;

    for (col = 0; col < WIDTH; ++col) {
        for (row = 0; row < HEIGHT; ++row) {
            cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), GREEN);
        }
    }

    ACTION_BIT(FRONT, 1);

    return cnt < 100;

}

int mission_14_set_straight(U16 *image) { // 여러프레임 여러점, 수치 가지고 repeat조절
    U32 cnt, row, i;
    int range, point[2][2] = {{75,  0},
                              {105, 0}};

    for (i = 0; i < 2; ++i) {
        for (row = ROBOT_KNEE; row > 0; --row) {
            cnt = 0;
            for (range = (-MISSION_5_6_BLACK_RANGE); range < MISSION_5_6_BLACK_RANGE; ++range) {
                cnt += GetValueRGBYOBK(GetPtr(image, row, point[i][0] + range, WIDTH), BLACK);
            }

            if (cnt > 3) {
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
        ACTION_TURN((((r > 0) ? r : -r) > 5) ? MIDDLE : SHORT, ((r > 0) ? DIR_RIGHT : DIR_LEFT), DOWN, 1);
        RobotSleep(1);
        rResult = 0;
    }

    return rResult;
}
