//
// Created by Gihyeon Yang on 2018. 7. 25..
//

#include "func.h"


void CASE_0_SET_CENTER(U16 *image, int dir) {
    U32 col[3] = {85, 95, 90}, row, i, j;
    U16 checkHurdleLine[3] = {0,};

    for (i = 0; i < 3; ++i) {
        for (row = HEIGHT - 1; row > 0; --row) {
            checkHurdleLine[i] = 0;

            for (j = 0; j < 5; j++) {
                checkHurdleLine[i] += GetValueRGBYOBK(GetPtr(image, row, col[i], WIDTH), BLACK);
            }

            if (checkHurdleLine[i] > 3) {
                checkHurdleLine[i] = (U16) (HEIGHT - row);
                break;
            }
        }
    }

    double s = 0;
    printf("M4-5: BLACK LINE\n");
    for (i = 0; i < 3; ++i) {
        if (s < checkHurdleLine[i]) {
            s = checkHurdleLine[i];
        }
        printf("bk_line[%d]: %d,\t", i, checkHurdleLine[i]);
    }
    printf("\n");

    printf("M4-5: AVG: %f\n", s);

    if (s < ((dir) ? CASE_0_DEFAULT_RIGHT_RANGE : CASE_0_DEFAULT_LEFT_RANGE) - CASE_0_DEFAULT_RANGE_ERROR) {
        printf("LEFT\n\n");
    } else if (s > ((dir) ? CASE_0_DEFAULT_RIGHT_RANGE : CASE_0_DEFAULT_LEFT_RANGE) + CASE_0_DEFAULT_RANGE_ERROR) {
        printf("RIGHT\n\n");
    } else {
        printf("SUCCESS\n\n");
    }
}

void CASE_0_SET_STRAIGHT(U16 *image, int dir) {
    U32 row, i;
    U16 col[2] = {
            80,
            100
    };
    int black_len[2] = {0,};

    for (i = 0; i < 2; ++i) {
        for (row = HEIGHT - 1; row > 0; --row) {
            if (GetValueRGBYOBK(GetPtr(image, row, col[i], WIDTH), BLACK) &&
                GetValueRGBYOBK(GetPtr(image, row, col[i] + 1, WIDTH), BLACK)) {
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

    int l = ((dir) ? CASE_0_DEFAULT_RIGHT_SLOPE : CASE_0_DEFAULT_LEFT_SLOPE);
    printf("%d %d\n", l, (l - 2 <= s && s <= l + 2));
    s *= 100;
    if (!(l - CASE_0_DEFAULT_SLOPE_ERROR < s && s < l + CASE_0_DEFAULT_SLOPE_ERROR)) {
        printf(((l - CASE_0_DEFAULT_SLOPE_ERROR > s) ? "LEFT\n" : "RIGHT\n"));
    } else {
        printf("SUCCESS\n\n");
    }

}

void CASE_1_CHECK_FRONT_OF_BARRICADE(U16 *image) {
    U32 col, row, cntYellow = 0;
    for (row = CASE_1_UPPER; row < CASE_1_LOWER; ++row) {
        for (col = 0; col < WIDTH; col++) {
            cntYellow += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW);
        }
    }

    printf("\n%d~%d, %d\nRESULT: %f\n", CASE_1_UPPER, CASE_1_LOWER, CASE_1_RATIO,
           (double) cntYellow * 100 / (WIDTH * (CASE_1_LOWER - CASE_1_UPPER)));
    printf((cntYellow * 100 / (WIDTH * (CASE_1_LOWER - CASE_1_UPPER)) > CASE_1_RATIO) ? "SUCCESS" : "FAIL");
}

void CASE_2_CHECK_RED_BRIDGE(U16 *image) {
    U32 col, row, cntRed = 0;
    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            cntRed += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), RED) ||
                       GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), ORANGE));
        }
    }

    printf("cntRed: %d\n", cntRed);
    printf("RED / AREA: %f\n\n", (double) cntRed * 100 / (WIDTH * HEIGHT));
    printf(((cntRed * 100 / (WIDTH * HEIGHT)) > CASE_2_0_DETECTION) ? "SUCCESS\n" : "FAIL\n");
}

void CASE_2_SET_CENTER(U16 *image) {
    U16 dir;
    int col, row, red_bridge[2] = {0,};

    for (dir = 0; dir < 2; ++dir) {
        for (row = 0; row < HEIGHT; ++row) {
            for (col = WIDTH / 2 * (dir); col < WIDTH / 2 * (dir + 1); ++col) {
                red_bridge[dir] += (
                        GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), RED) ||
                        GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), ORANGE)
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

    printf("LEFT: %f, RIGHT: %f, r: %f\n\n", r[0], r[1], s);

    if (((s > 0) ? s : (-s)) > CASE_2_1_CENTER) {
        printf((s > 0) ? "LEFT\n" : "RIGHT\n");
    } else {
        printf("SUCCESS\n");
    }
}

void CASE_4_CHECK_LENGTH(U16 *image) {
    U32 col[3] = {70, 60, 90}, row, i;
    U16 checkHurdleLine[3] = {0,};

    for (i = 0; i < 3; ++i) {
        for (row = HEIGHT - 1; row > 0; --row) {
            if (GetValueRGBYOBK(GetPtr(image, row, col[i], WIDTH), BLACK) &&
                GetValueRGBYOBK(GetPtr(image, row, col[i] + 1, WIDTH), BLACK)) {
                checkHurdleLine[i] = (U16) row;
                break;
            }
        }
    }

    double s = 0;
    printf("BLACK LINE\n");
    for (i = 0; i < 3; ++i) {
        s = (s > checkHurdleLine[i]) ? s : checkHurdleLine[i];
        printf("bk_line[%d]: %d,\t", i, checkHurdleLine[i]);
    }
    printf("\n");

    printf("AVG: %f\n", s);

    printf((CASE_4_0_LENGTH - CASE_4_0_LENGTH_ERROR <= s &&
            s <= CASE_4_0_LENGTH + CASE_4_0_LENGTH_ERROR) ? "SUCCESS\n"
                                                          : "FAIL\n");
}

void CASE_4_CHECK_NON_BLACK(U16 *image) {
    U32 col[3] = {85, 95, 90}, row, i;
    U16 checkHurdleLine[3] = {0,};

    for (i = 0; i < 3; ++i) {
        for (row = HEIGHT - 1; row > 0; --row) {
            if (GetValueRGBYOBK(GetPtr(image, row, col[i], WIDTH), BLACK) &&
                GetValueRGBYOBK(GetPtr(image, row, col[i] + 1, WIDTH), BLACK)) {
                checkHurdleLine[i] = (U16) (HEIGHT - row);
                break;
            }
        }
    }

    double s = 0;
    printf("\nM4-5: BLACK LINE\n");
    for (i = 0; i < 3; ++i) {
        s += checkHurdleLine[i];
        printf("bk_line[%d]: %d,\t", i, checkHurdleLine[i]);
    }
    printf("\n");

    s /= 3;
    printf("AVG: %f\n\n", s);
    printf((s < CASE_4_1_NON_BLACK_LINE) ? "SUCCESS\n" : "FAIL\n");
}

void CASE_5_CHECK_BLACK_BLOCK(U16 *image) {
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
}

void CASE_5_CHECK_CENTER_ON_GREEN_BRIDGE(U16 *image) {
    U16 dir, cnt;
    int col, row, flagSign, green_len[2] = {0,};

    for (dir = 0; dir < 2; ++dir) {
        flagSign = (dir) ? 1 : -1;
        for (col = 0; col < WIDTH / 2; ++col) {
            cnt = 0;
            for (row = 10;
                 row < 70;
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

    if (((r > 0) ? r : (-r)) > CASE_5_1_CENTER) {
        printf((r > 0) ? "LEFT\n" : "RIGHT\n");
    } else {
        printf("SUCCESS\n\n");
    }
}

void CASE_5_CHECK_STRAIGHT_ON_GREEN_BRIDGE(U16 *image) {
    U32 col, i, cnt, range = 0, point[2][2] = {{0, CASE_5_2_POINT_Y_1},
                                               {0, CASE_5_2_POINT_Y_2}};

    U32 max = CASE_5_2_SLOPE_RANGE + 1;
    int pixelQueue[CASE_5_2_SLOPE_RANGE + 1] = {0,};
    int front = 0, rear = 0;

    for (i = 0; i < 2; ++i) {
        front = 0;
        rear = 0;
        for (col = 0; col < WIDTH; ++col) {

            if (front == rear) {
                for (range = 0; range < CASE_5_2_SLOPE_RANGE; ++range) {
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
            for (range = 0; range < CASE_5_2_SLOPE_RANGE; ++range) {
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
    if (((r > 0) ? r : -r) > CASE_5_2_SLOPE) {
        printf((r > 0) ? "RIGHT\n" : "LEFT\n");
    } else {
        printf("SUCCESS\n");
    }
}

void CASE_5_CHECK_BLACK_LINE_ONLY_ONE(U16 *image) {}

void CASE_5_CHECK_STRAIGHT_ON_BLACK_LINE(U16 *image) {
    U32 cnt, row, i, range;
    int point[2][2] = {{CASE_5_4_POINT_X_1, 0},
                       {CASE_5_4_POINT_X_2, 0}};

    for (i = 0; i < 2; ++i) {
        for (row = 10; row < HEIGHT; ++row) {
            cnt = 0;
            for (range = 0; range < CASE_5_4_RANGE; ++range) {
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

    if (((r > 0) ? r : -r) > CASE_5_4_SLOPE) {
        printf((r < 0) ? "LEFT\n" : "RIGHT\n");
    } else {
        printf("SUCCESS\n\n");
    }
}

void CASE_5_CHECK_LENGTH_ON_BLACK_LINE(U16 *image) {
    U32 col, row, blackLen = 0;
    for (row = 0; row < HEIGHT; ++row) {
        blackLen = 0;
        for (col = CASE_5_5_POINT_X_1;
             col < CASE_5_5_POINT_X_2;
             ++col) {
            blackLen += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK);
        }

        if (blackLen >= CASE_5_5_THRESHOLDS) {
            blackLen = HEIGHT - row;
            break;
        }

    }

    printf("\t\t\t+ bk_len length: %d\n", blackLen);
    printf((blackLen < CASE_5_5_LENGTH) ? "SUCCESS\n" : "FAIL\n");
}

void CASE_7_SET_CENTER(U16 *image) {
    U16 dir;
    int col, row, red_bridge[2] = {0,};

    for (dir = 0; dir < 2; ++dir) {
        for (row = 0; row < HEIGHT; ++row) {
            for (col = WIDTH / 2 * (dir); col < WIDTH / 2 * (dir + 1); ++col) {
                red_bridge[dir] += (
                       GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW) || 
             GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), 6)
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

    printf("LEFT: %f, RIGHT: %f, r: %f\n\n", r[0], r[1], s);

    if (((s > 0) ? s : (-s)) > CASE_7_0_CENTER) {
        printf((s > 0) ? "LEFT\n" : "RIGHT\n");
    } else {
        printf("SUCCESS\n");
    }
}

void CASE_7_SET_CENTER_ON_YELLOW_BRIDGE(U16 *image) {
    U32 col[3] = {CASE_7_1_POINT_X_1, CASE_7_1_POINT_X_2, CASE_7_1_POINT_X_3}, row, i, j;
    U16 checkHurdleLine[3] = {0,};

    for (i = 0; i < 3; ++i) {
        for (row = HEIGHT - 1; row > 0; --row) {
            checkHurdleLine[i] = 0;

            for (j = 0; j < 5; j++) {
                checkHurdleLine[i] += (GetValueRGBYOBK(GetPtr(image, row, col[i] + j, WIDTH), YELLOW)||
                GetValueRGBYOBK(GetPtr(image, row, col[i] + j, WIDTH), 6));
            }

            if (checkHurdleLine[i] < 1) {
                checkHurdleLine[i] = (U16) (HEIGHT - row);
                break;
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
        printf((CASE_7_1_CENTER - CASE_7_1_ERROR > s) ? "RIGHT\n" : "LEFT\n");
    } else {
        printf("SUCCESS\n");
    }
}

void CASE_7_SET_STRAIGHT_ON_YELLOW_BRIDGE(U16 *image) {
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
                GetValueRGBYOBK(GetPtr(image, row, col[i], WIDTH), 6) &&
                GetValueRGBYOBK(GetPtr(image, row, col[i] + 1, WIDTH), YELLOW) ||
                GetValueRGBYOBK(GetPtr(image, row, col[i] + 1, WIDTH), 6)
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
        printf((CASE_7_2_SLOPE - CASE_7_2_SLOPE_ERROR > s) ? "LEFT\n" : "RIGHT\n");
    } else {
        printf("SUCCESS\n");
    }
}

void CASE_7_CHECK_RANGE(U16 *image) {
    U32 col, row;
    U16 black_cnt = 0;

    for (row = 0; row < HEIGHT; ++row) {
        black_cnt = 0;
        for (col = CASE_7_3_POINT_X_1; col < CASE_7_3_POINT_X_2; ++col) {
            black_cnt += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW) || 
             GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), 6));
        }

        if (black_cnt > 7) {
            black_cnt = (U16) row;
            break;
        }

    }

    printf("7-5: walk_until_black_line\n");
    printf("Ratio is %d.\n", black_cnt);

    int rResult = black_cnt > CASE_7_3_LENGTH;
    if (!rResult) {
        printf("FAIL\n");
    } else {
        printf("SUCCESS\n");
    }
}

void CASE_7_CHECK_YELLOW(U16 *image) {
    U32 col, row, cntRed = 0;
    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            cntRed += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW));
        }
    }

    printf("cntYELLOW: %d\n", cntRed);
    printf("YELLOW / AREA: %f\n\n", (double) cntRed * 100 / (WIDTH * HEIGHT));
    printf(((cntRed * 100 / (WIDTH * HEIGHT)) > CASE_7_4_DETETCH_RATIO) ? "SUCCESS\n" : "FAIL\n");
}

void CASE_8_CATCH_BLUE_GATE(U16 *image) {
    U32 col, row;
    U16 blue_cnt = 0;

    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            blue_cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
        }
    }

    double ratio = (double) blue_cnt / (HEIGHT * WIDTH) * 100;

    printf("blue ratio is %f\n\n", ratio);

    printf((ratio >= CASE_8_RATIO) ? "SUCCESS\n" : "FAIL\n");
}
