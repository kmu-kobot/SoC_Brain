//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#include "MISSION_2_RED_BRIDGE.h"

void mission_2_1_watch_below(int repeat) {
    ACTION_WALK(FAST, DOWN, repeat);
    RobotSleep(1);
}

int mission_2_1_wait_front_of_red_bridge(U16 *image) {
    U32 col, row, cntRed = 0;
    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            cntRed += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), RED) ||
                       GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), ORANGE) ||
                       GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), CH2));
        }
    }

    printf("cntRed: %d\n", cntRed);
    printf("RED / AREA: %f\n\n", (double) cntRed * 100 / (WIDTH * HEIGHT));
    printf(((cntRed * 100 / (WIDTH * HEIGHT)) > CASE_2_0_DETECTION) ? "SUCCESS\n" : "FAIL\n");

    if (((cntRed * 100 / (WIDTH * HEIGHT)) > CASE_2_0_DETECTION)) {
        cntRed = 0;
        for (row = 20; row < ROBOT_KNEE; ++row) {
            for (col = 0; col < WIDTH; ++col) {
                cntRed += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), RED) ||
                           GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), ORANGE) ||
                           GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), CH2));
            }
        }

        if ((double) cntRed * 100 / (WIDTH * (ROBOT_KNEE - 20)) > 5) {
            ACTION_WALK(CLOSE, OBLIQUE, 2);
        } else {
            ACTION_WALK(FAST, OBLIQUE, 2);
        }

        return 1;
    } else {
        return 0;
    }
}

void mission_2_2_watch_front(void) {
    ACTION_INIT(MIDDLE, OBLIQUE);
    RobotSleep(1);
}

void mission_2_2_watch_side(void) {
    ACTION_INIT(MIDDLE, LEFT);
}

int mission_2_2_before_bridge_set_center_version2(U16 *image) {
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
        ACTION_MOVE(LONG, ((s > 0) ? DIR_LEFT : DIR_RIGHT), MIDDLE, OBLIQUE, 1);
        ACTION_WALK(CLOSE, OBLIQUE, 1);
        return 0;
    } else {
        ACTION_WALK(CLOSE, OBLIQUE, 2);
        printf("SUCCESS\n");
        return 1;
    }
}

int mission_2_2_before_bridge_set_center(U16 *image, int mode, int length) {
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

    int len = (length != 0) ? length : CASE_0_DEFAULT_LEFT_RANGE;
    int err = ((mode == 3) ? 3 : CASE_0_DEFAULT_RANGE_ERROR);

    if (s < len - err) {
        ACTION_MOVE(LONG, DIR_RIGHT, MIDDLE, LEFT, 1);
        if (mode == 1) {
            ACTION_WALK(CLOSE, LEFT, 1);
        }
        return 0;
    } else if (s > len + err) {
        ACTION_MOVE(LONG, DIR_LEFT, MIDDLE, LEFT, 1);
        if (mode == 1) {
            ACTION_WALK(CLOSE, LEFT, 1);
        }
        return 0;
    } else {
        if (mode == 1 || mode == -1) {
            ACTION_WALK(CLOSE, LEFT, 4);
        } else if (mode == 3) {
            ACTION_WALK(CLOSE, LEFT, 2);
        }
        printf("SUCCESS\n\n");
        return 1;
    }
}

int mission_2_3_escape_red_bridge(void) {
    ACTION_MOTION(MISSION_2_RED_DUMBLING, MIDDLE, OBLIQUE);
    RobotSleep(1);
    return 1;
}

int mission_2_4_after_bridge_set_straight(U16 *image, int mode) {
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

    int l = ((mode) ? CASE_0_DEFAULT_RIGHT_SLOPE : CASE_0_DEFAULT_LEFT_SLOPE);
    printf("%d %d\n", l, (l - 2 <= s && s <= l + 2));

    s *= 100;
    if (!(l - CASE_0_DEFAULT_SLOPE_ERROR <= s && s <= l + CASE_0_DEFAULT_SLOPE_ERROR)) {
        ACTION_TURN(
                LONG,
                (l - CASE_0_DEFAULT_SLOPE_ERROR > s) ?
                DIR_LEFT :
                DIR_RIGHT,
                MIDDLE, (mode) ? RIGHT : LEFT, 2
        );
        return 0;
    } else {
        printf("SUCCESS\n\n");
        return 1;
    }
}

int mission_2_5_after_bridge_set_center(U16 *image) {
    return mission_2_2_before_bridge_set_center(image, 0, 0);
}
