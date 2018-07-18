//
// Created by Gihyeon Yang on 2018. 7. 11..
//

#include "MISSION_7_YELLOW_HOLE_BRIDGE.h"

void mission_7_1_watch_below(int repeat) {
    Action_WALK_FRONT_LONG(repeat);
    RobotSleep(5);
    Action_WATCH_BELOW_LONG();
    RobotSleep(5);
}

int mission_7_1_wait_front_of_yellow_hole_bridge(U16 *image, int repeat) {
    U32 col, row, cntYellow = 0;
    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            cntYellow += GetValueRGBYOBK(
                    GetPtr(image, row, col, WIDTH),
                    YELLOW
            );
        }
    }

    double r = (double) cntYellow / (WIDTH * HEIGHT) * 100;

    printf("ratio = %f\n", r);

    int rReturn = r > MISSION_7_1_THRESHOLDS;

    if (rReturn) {
        Action_WALK_FRONT_SHORT(repeat);
        RobotSleep(5);
    }

    return rReturn;
}

void mission_7_2_watch_below(void) {
    Action_INIT_ROBOT();
    RobotSleep(5);
}

int mission_7_2_before_bridge_set_center(U16 *image) {
    U16 dir = 0;
    int col, row, yellow_len[2] = {0,};
    int w = WIDTH / 2;

    for (dir = 0; dir < 2; ++dir) {
        for (col = w * dir; col < w * (dir + 1); ++col) {
            for (row = 0; row < HEIGHT; ++row) {
                yellow_len[dir] += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW);
            }
        }
    }

    // 0: LEFT, 1: RIGHT
    // int r = yellow_len[0] - yellow_len[1];
    double r[2] = {
            (double) yellow_len[0] / (HEIGHT * WIDTH / 2) * 100,
            (double) yellow_len[1] / (HEIGHT * WIDTH / 2) * 100
    };

    double s = r[0] - r[1];

    printf("\nM5-5: SET CENTER\n");
    printf("LEFT: %f, RIGHT: %f, r: %f\n\n", r[0], r[1], r[0] - r[1]);

    if (((s > 0) ? s : -s) > MISSION_7_2_RED_BRIDGE_THRESHOLDS) {
        if (s > 0) {
            Action_LEFT_MOVE_LONG(1);
        } else if (s < 0) {
            Action_RIGHT_MOVE_LONG(1);
        }
        Action_WALK_FRONT_SHORT(1);
        RobotSleep(4);
    }

    return ((s > 0) ? s : -s) < MISSION_7_2_RED_BRIDGE_THRESHOLDS;
}

int mission_7_3_climb_yellow_hole_bridge() {
    RobotSleep(5);
    Action_CLIMB_UP_STAIRS();
    Action_INIT_ROBOT();
    RobotSleep(5);
    Action_WALK_FRONT_SHORT(3);
    RobotSleep(5);
    return 1;
}

int mission_7_4_set_center(U16 *image) {
    U32 i, row, col[2] = {MISSION_7_4_COL_POINT_1,
                          MISSION_7_4_COL_POINT_2};
    U16 yellow_len[2] = {0,};
    for (i = 0; i < 2; ++i) {
        for (row = 30; row < HEIGHT; ++row) {
            if (GetValueRGBYOBK(GetPtr(image, row, col[i], WIDTH), YELLOW) == 1 &&
                GetValueRGBYOBK(GetPtr(image, row, col[i] + 1, WIDTH), YELLOW) == 1) {
                yellow_len[i] = HEIGHT - row;
                break;
            }
        }
    }

    printf("yellow_len[0] = %d, yellow_len[1] = %d\n", yellow_len[0], yellow_len[1]);

    double e = (double) (yellow_len[0] + yellow_len[1]) / 2;

    printf("distance is %f", e);

    Action_INIT_ROBOT();

    int rResult = 0;
    if (e < MISSION_7_4_YELLOW_LENGTH - MISSION_7_4_YELLOW_LENGTH_ERROR) {
        //오른쪽 이동
        Action_RIGHT_MOVE_LONG(1);
    } else if (e > MISSION_7_4_YELLOW_LENGTH + MISSION_7_4_YELLOW_LENGTH_ERROR) {
        //왼쪽 이동
        Action_LEFT_MOVE_LONG(1);
    } else {
        rResult = 1;
    }

    Action_INIT_ROBOT();

    return rResult;
}

int mission_7_4_set_straight(U16 *image) {
    U32 i, row, col[2] = {MISSION_7_4_COL_POINT_1,
                          MISSION_7_4_COL_POINT_2};
    U16 pos_yellow[2] = {0,};
    for (i = 0; i < 2; ++i) {
        for (row = 30; row < HEIGHT; ++row) {
            if (GetValueRGBYOBK(GetPtr(image, row, col[i], WIDTH), YELLOW) == 1 &&
                GetValueRGBYOBK(GetPtr(image, row, col[i] + 1, WIDTH), YELLOW) == 1) {
                break;
            }
            pos_yellow[i] += 1;
        }
    }

    printf("pos_yellow[0]: %d, pos_yellow[1]: %d\n", pos_yellow[0], pos_yellow[1]);

// s>0 l
// s < 0
    double slope = (double) (pos_yellow[0] - pos_yellow[1]);

    printf("Slope is %f\n", slope);

    Action_INIT_ROBOT();

    int rResult = 1;
    if (((slope > 0) ? slope : -slope) > MISSION_7_4_YELLOW_BRIDGE_SLOPE) {
        rResult = 0;
        if (slope > 0) {
            Action_LEFT_TURN_BODY(2);
        } else if (slope < 0) {
            Action_RIGHT_TURN_BODY(2);
        }
    }

    return rResult;
}

void mission_7_4_watch_below(int repeat) {
    Action_WATCH_BELOW_LONG();
    RobotSleep(repeat);
}

int mission_7_4_walk_until_black_line(U16 *image) {

    U32 col, row;
    U16 black_cnt = 0;

    for (row = 0; row < MISSION_7_4_HEIGHT_SCOPE; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            if (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK))
                black_cnt++;
        }
    }

    double ratio = black_cnt / (WIDTH * MISSION_7_4_HEIGHT_SCOPE) * 100;

    printf("7-5: walk_until_black_line\n");
    printf("Ratio is %f.\n", ratio);

    int rResult = 1;
    if (ratio < MISSION_7_4_BLACK_RATIO - MISSION_7_4_BLACK_RATIO_ERROR ||
        ratio > MISSION_7_4_BLACK_RATIO + MISSION_7_4_BLACK_RATIO_ERROR) {
        rResult = 0;
        Action_WALK_FRONT_SHORT(2);
        RobotSleep(5);
    }

    return rResult;
}

int mission_7_5_set_center_on_yellow_bridge(U16 *image) {

    U32 row, col, dir;
    int yellow[2] = {0,};
    int w = WIDTH / 2;

    for (dir = 0; dir < 2; ++dir) {
        for (row = 0; row < HEIGHT; ++row) {
            for (col = w * dir; col < w * (dir + 1); ++col) {
                yellow[dir] += (
                        GetValueRGBYOBK(
                                GetPtr(image, row, col, WIDTH),
                                YELLOW
                        )
                );
            }
        }
    }

    double r[2] = {
            (double) yellow[0] / (HEIGHT * w) * 100,
            (double) yellow[1] / (HEIGHT * w) * 100
    };

    printf("M7-5: SET CENTER\n");
    printf("black_len[0]: %f, black_len[1]: %f. error: %f\n", r[0], r[1], r[0] - r[1]);

    double s = (r[0] - r[1]);
    printf("%d\n", (((s > 0) ? s : (-s)) <= MISSION_7_5_BLACK_LINE_RANGE + MISSION_7_5_BLACK_LINE_ERROR));

    int rResult = (((s > 0) ? s : -s) <= MISSION_7_5_BLACK_LINE_RANGE + MISSION_7_5_BLACK_LINE_ERROR);
    if (!rResult) {
        if (s > 0) {
            Action_RIGHT_MOVE_LONG(1);
        } else if (s < 0) {
            Action_LEFT_MOVE_LONG(1);
        }
        RobotSleep(5);
        Action_INIT_ROBOT();
    }

    return rResult;
}

// int mission_7_5_set_straight_on_yellow_bridge(U16 *image) {

//     U32 col, i;
//     U16 yellow_len[2] = {0,}, row[2] = {
//             MISSION_7_5_YELLOW_LINE_ROW_POINT_1,
//             MISSION_7_5_YELLOW_LINE_ROW_POINT_2
//     };

//     for (i = 0; i < 2; ++i) {
//         for (col = 0; col > 0; ++col) {
//             if (GetValueRGBYOBK(
//                         GetPtr(image, row[i], col, WIDTH),
//                         BLACK
//                 ) &&
//                 GetValueRGBYOBK(
//                         GetPtr(image, row[i] + 1, col, WIDTH),
//                         BLACK
//                 )) {
//                 break;
//             }
//             yellow_len[i] += 1;
//         }
//     }

//     printf("M7-5: SLOPE\n");
//     printf("yellow_len[0]: %d, yellow_len[1]: %d.\n", yellow_len[0], yellow_len[1]);


//     double s = (
//             (yellow_len[0] - yellow_len[1]) /
//             (MISSION_7_5_YELLOW_LINE_ROW_POINT_1 - MISSION_7_5_YELLOW_LINE_ROW_POINT_2)
//     );

//     printf("Slope : %f\n", s);

//     s *= 100;

//     Action_INIT_ROBOT();

//     int rResult = 0;
//     if (s < MISSION_7_5_BLACK_LINE_SLOPE + MISSION_7_5_BLACK_LINE_SLOPE_ERROR) {
//         Action_RIGHT_TURN_BODY(1);
//     } else if (s > MISSION_7_5_BLACK_LINE_SLOPE - MISSION_7_5_BLACK_LINE_SLOPE_ERROR) {
//         Action_LEFT_TURN_BODY(1);
//     } else {
//         rResult = 1;
//     }

//     if (!rResult) {
//         RobotSleep(5);
//     }

//     Action_INIT_ROBOT();

//     return rResult;
// }

int mission_7_5_set_straight_on_yellow_bridge(U16 *image) {
    U32 col, i, cnt, range = 0, point[2][2] = {{0, MISSION_7_5_YELLOW_BRIDGE_POINT_Y_1},
                                               {0, MISSION_7_5_YELLOW_BRIDGE_POINT_Y_2}};

    U32 max = MISSION_7_5_YELLOW_BRIDGE_SLOPE_RANGE + 1;
    int pixelQueue[MISSION_7_5_YELLOW_BRIDGE_SLOPE_RANGE + 1] = {0,};
    int front = 0, rear = 0;

    for (i = 0; i < 2; ++i) {
        front = 0;
        rear = 0;
        for (col = WIDTH / 2; col < WIDTH; ++col) {

            if (front == rear) {
                for (range = 0; range < MISSION_7_5_YELLOW_BRIDGE_SLOPE_RANGE; ++range) {
                    pixelQueue[rear] = !GetValueRGBYOBK(
                            GetPtr(image, point[i][1], col + range, WIDTH),
                            YELLOW
                    );
                    rear++;
                    rear %= max;
                }
            } else {
                front++;
                front %= max;
                pixelQueue[rear] = !GetValueRGBYOBK(
                        GetPtr(image, point[i][1], col + range, WIDTH),
                        YELLOW
                );
                rear++;
                rear %= max;
            }

            cnt = 0;
            for (range = 0; range < MISSION_7_5_YELLOW_BRIDGE_SLOPE_RANGE; ++range) {
                cnt += pixelQueue[range];
            }

            if (cnt <= 3) {
                point[i][0] = col - WIDTH / 2;
                break;
            }

        }
    }

    int r = (point[0][0] - point[1][0]);

    printf("point[0][0]: %d, point[1][0]: %d, error: %d\n\n", point[0][0], point[1][0], point[0][0] - point[1][0]);

    printf("slope is %d", r);

    int rResult = 1;
    if (((r > 0) ? r : -r) > MISSION_7_5_YELLOW_BRIDGE_SLOPE) {
        rResult = 0;
        if (r > 0) {
            Action_RIGHT_TURN_BODY(2);
        } else {
            Action_LEFT_TURN_BODY(2);
        }
    }

    if (!rResult) { RobotSleep(5); }

    return rResult;

}

int mission_7_5_walk_until_line_front_of_feet(U16 *image) {
    U32 col, row;
    U16 black_cnt = 0;

    for (row = 30; row < HEIGHT; ++row) {
        black_cnt = 0;
        for (col = 55; col < 65; ++col) {
            black_cnt += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK) ||
                          GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW));
        }

        if (black_cnt > 7) {
            black_cnt = row;
            break;
        }

    }

    printf("7-5: walk_until_black_line\n");
    printf("Ratio is %d.\n", black_cnt);

    int rResult = black_cnt > MISSION_7_5_LINE_RATIO;
    if (!rResult) {
        Action_WALK_FRONT_SHORT(0);
    }

    Action_INIT_ROBOT();
    RobotSleep(5);

    return rResult;
}


int mission_7_6_jump_hole(void) {
    RobotSleep(5);
    Action_MISSION_7_YELLOW_HOLE_BRIDGE_DUMBLING();
    RobotSleep(5);
    return 1;
}

void mission_7_7_watch_side(int repeat) {
    Action_LEFT_TURN_HEAD_LONG();
    RobotSleep(repeat);
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

    Action_INIT_ROBOT();

    double s = (
            (black_len[0] - black_len[1]) /
            MISSION_7_2_BLACK_LINE_COL_POINT_1 - MISSION_7_2_BLACK_LINE_COL_POINT_2
    );

    printf("Slope : %f\n", s);

    s *= 100;

    int rResult = 0;
    if (s < MISSION_7_7_BLACK_LINE_SLOPE + MISSION_7_7_BLACK_LINE_SLOPE_ERROR) {
        Action_RIGHT_TURN_BODY(1);
    } else if (s > MISSION_7_7_BLACK_LINE_SLOPE - MISSION_7_7_BLACK_LINE_SLOPE_ERROR) {
        Action_LEFT_TURN_BODY(1);
    } else {
        rResult = 1;
    }

    Action_INIT_ROBOT();

    return rResult;
}

int mission_7_7_after_yellow_bridge_set_center(U16 *image) {
    return mission_7_2_before_bridge_set_center(image);
}








