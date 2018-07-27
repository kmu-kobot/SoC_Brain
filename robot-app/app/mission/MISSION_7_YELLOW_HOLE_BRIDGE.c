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
                          GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), CH2));
        }
    }

    double r = (double) cntYellow / (WIDTH * HEIGHT) * 100;

    printf("ratio = %f\n", r);

    int rReturn = r > MISSION_7_1_THRESHOLDS;

    if (rReturn && repeat != -1) {
        cntYellow = 0;
        for (row = 20; row < ROBOT_KNEE; ++row) {
            for (col = 0; col < WIDTH; ++col) {
                cntYellow += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW) ||
                              GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), CH2));
            }
        }

        if ((double) (cntYellow * 100 / ((ROBOT_KNEE - 20) * WIDTH)) > 5) {
            ACTION_WALK(CLOSE, OBLIQUE, 3);
        } else {
            ACTION_WALK(FAST, OBLIQUE, repeat);
        }
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
                        GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), CH2)
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
        ACTION_MOVE(LONG, ((s > 0) ? DIR_LEFT : DIR_RIGHT), MIDDLE, OBLIQUE, ((s > 0) ? 1 : 2));
    } else {
        ACTION_WALK(CLOSE, OBLIQUE, 2);
    }

    return ((s > 0) ? s : -s) <= MISSION_7_2_RED_BRIDGE_THRESHOLDS;
}

int mission_7_3_climb_yellow_hole_bridge() {
    ACTION_WALK(CLOSE, OBLIQUE, 1);
    RobotSleep(1);
    ACTION_MOTION(MISSION_5_STAIR_UP, MIDDLE, OBLIQUE);
    RobotSleep(1);
    ACTION_INIT(MIDDLE, DOWN);
    ACTION_WALK(FAST, DOWN, 3);
    RobotSleep(1);
    return 1;
}

int mission_7_4_set_center(U16 *image) {
    U32 col[3] = {CASE_7_1_POINT_X_1, CASE_7_1_POINT_X_2, CASE_7_1_POINT_X_3}, row, i, j, k = 0;
    U16 checkHurdleLine[3] = {0,};

    for (i = 0; i < 3; ++i) {
        k = 0;
        for (row = HEIGHT - 1; row >= 0; --row) {
            checkHurdleLine[i] = 0;

            for (j = 0; j < 5; j++) {
                checkHurdleLine[i] += (GetValueRGBYOBK(GetPtr(image, row, col[i] + j, WIDTH), YELLOW) ||
                                       GetValueRGBYOBK(GetPtr(image, row, col[i] + j, WIDTH), CH2));
            }

            if (k == 0) {
                if (checkHurdleLine[i] < 1) {
                    k = 1;
                } else {
                    k = 2;
                }
            }

            if (k == 2) {
                if (checkHurdleLine[i] < 1) {
                    checkHurdleLine[i] = (U16) (HEIGHT - row);
                    break;
                }
            } else { // k == 1
                if (checkHurdleLine[i] > 1) {
                    k = 2;
                    checkHurdleLine[i] = (U16) (HEIGHT - row);
                }
            }
        }
    }

    double s = 0;
    printf("M4-5: BLACK LINE\n");
    for (i = 0; i < 3; ++i) {
        s += checkHurdleLine[i];
        printf("bk_line[%d]: %d,\t", i, checkHurdleLine[i]);
    }
    printf("\n");

    s /= 3;

    printf("M4-5: AVG: %f\n", s);

    if (!(CASE_7_1_CENTER - CASE_7_1_ERROR <= s && s <= CASE_7_1_CENTER + CASE_7_1_ERROR)) {
        ACTION_MOVE(SHORT, (CASE_7_1_CENTER - CASE_7_1_ERROR > s) ? DIR_RIGHT : DIR_LEFT, MIDDLE, LEFT, 1);
        return 0;
    } else {
        return 1;
    }
}

int mission_7_4_set_straight(U16 *image) {
    U32 row, i;
    U16 col[2] = {
            CASE_7_2_POINT_X_1,
            CASE_7_2_POINT_X_2
    };
    int black_len[2] = {0,};

    for (i = 0; i < 2; ++i) {
        for (row = HEIGHT - 1; row > 0; --row) {
            if (!(
                    GetValueRGBYOBK(GetPtr(image, row, col[i], WIDTH), YELLOW) ||
                    GetValueRGBYOBK(GetPtr(image, row, col[i], WIDTH), CH2) &&
                    GetValueRGBYOBK(GetPtr(image, row, col[i] + 1, WIDTH), YELLOW) ||
                    GetValueRGBYOBK(GetPtr(image, row, col[i] + 1, WIDTH), CH2)
            )) {
                black_len[i] = -row;
                break;
            }
        }
    }

    printf("M2-4: SLOPE\n");
    printf("black[0]: %d, black_len[1]: %d.\n", black_len[0], black_len[1]);

    double s = (
            (double) (black_len[0] - black_len[1]) /
            (20)
    );

    printf("Slope : %f\n", s * 100);

    s *= 100;

    if (!(CASE_7_2_SLOPE - CASE_7_2_SLOPE_ERROR <= s && s <= CASE_7_2_SLOPE + CASE_7_2_SLOPE_ERROR)) {
        ACTION_TURN(SHORT, ((CASE_7_2_SLOPE - CASE_7_2_SLOPE_ERROR > s) ? DIR_LEFT : DIR_RIGHT), MIDDLE, LEFT, 2);
        return 0;
    } else {
        printf("SUCCESS\n");
        return 1;
    }
}

void mission_7_4_watch_below(void) {
    ACTION_INIT(MIDDLE, DOWN);
    RobotSleep(3);
}

int mission_7_5_set_straight(U16 *image) {
    U32 cnt, row, i;
    int range, point[2][2] = {{MISSION_7_5_BLACK_COL_1, 0},
                              {MISSION_7_5_BLACK_COL_2, 0}};

    for (i = 0; i < 2; ++i) {
        for (row = ROBOT_KNEE; row >= 0; --row) {
            cnt = 0;
            for (range = (-MISSION_7_5_BLACK_RANGE); range < MISSION_7_5_BLACK_RANGE; ++range) {
                cnt += (GetValueRGBYOBK(GetPtr(image, row, point[i][0] + range, WIDTH), BLACK) ||
                        GetValueRGBYOBK(GetPtr(image, row, point[i][0] + range, WIDTH), BLUE));
            }

            if (cnt >= 3) {
                point[i][1] = row;
                break;
            }
        }
    }

    printf("\t\t- M5-6: SET STRAIGHT\n");
    printf("\t\t\t+ %d %d\n\n", point[0][1], point[1][1]);

    int r = (point[0][1] - point[1][1]);

    int rResult = 1;
    if (((r > 0) ? r : -r) > MISSION_7_5_YELLOW_BLACK_LINE_SLOPE) {
        ACTION_TURN(SHORT, ((r < 0) ? DIR_RIGHT : DIR_LEFT), MIDDLE, DOWN, 1);
        RobotSleep(1);
        rResult = 0;
    }

    return rResult;
}

int mission_7_5_walk_until_line_front_of_feet(U16 *image) {
    U32 col;
    U16 black_cnt = 0;
    int row;

    for (row = ROBOT_KNEE; row >= 0; --row) {
        black_cnt = 0;
        for (col = MISSION_7_5_POINT_X_1; col < MISSION_7_5_POINT_X_2; ++col) {
            black_cnt += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK) ||
                          GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE));
        }

        if (black_cnt > 7) {
            black_cnt = (U16) row;
            break;
        } else {
            black_cnt = 0;
        }

    }

    printf("7-5: walk_until_black_line\n");
    printf("Ratio is %d\n", black_cnt);

    if (black_cnt <= MISSION_7_5_LINE_RATIO) {
        ACTION_WALK(CLOSE, DOWN, (black_cnt < 10) ? 6 : 2);
    }

    return black_cnt >= MISSION_7_5_LINE_RATIO;
}


int mission_7_6_jump_hole(void) {
    ACTION_WALK(CLOSE, OBLIQUE, 3);
    RobotSleep(1);
    ACTION_MOTION(MISSION_7_YELLOW_DUMBLING, MIDDLE, OBLIQUE);
    RobotSleep(1);
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
        ACTION_TURN(LONG, DIR_RIGHT, MIDDLE, LEFT, 1);
    } else if (s > MISSION_7_7_BLACK_LINE_SLOPE - MISSION_7_7_BLACK_LINE_SLOPE_ERROR) {
        ACTION_TURN(LONG, DIR_LEFT, MIDDLE, LEFT, 1);
    } else {
        rResult = 1;
    }

    ACTION_INIT(MIDDLE, LEFT);

    return rResult;
}