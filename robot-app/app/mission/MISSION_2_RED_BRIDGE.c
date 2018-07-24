//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#include "MISSION_2_RED_BRIDGE.h"

void mission_2_1_watch_below(int repeat) {
    ACTION_WALK(FAST, DOWN, repeat);
    RobotSleep(1);
}

int mission_2_1_wait_front_of_red_bridge(U16 *image, int repeat) {
    U32 col, row, cntRed = 0;
    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            cntRed += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), RED) ||
                       GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), ORANGE));
        }
    }

    printf("\t\t- M2-1: WAIT FRONT OF RED BRIDGE\n");
    printf("\t\t\t+ cntRed: %d\n", cntRed);
    printf("\t\t\t+ RED / AREA: %f\n\n", (double) cntRed * 100 / (WIDTH * HEIGHT));

    int rReturn = (cntRed * 100 / (WIDTH * HEIGHT)) > MISSION_2_THRESHOLDS;

    if (rReturn) {
        // TODO: 붙이는 동작
        ACTION_WALK(SLOW, OBLIQUE, repeat);
        RobotSleep(3);
    }

    return rReturn;
}

void mission_2_2_watch_front(void) {
    ACTION_INIT(MIDDLE, OBLIQUE);
    RobotSleep(1);
}

void mission_2_2_watch_side(void) {
    ACTION_INIT(MIDDLE, LEFT);
    RobotSleep(1);
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

    printf("\n\tM5-5: SET CENTER\n");
    printf("\t\tLEFT: %f, RIGHT: %f, r: %f\n\n", r[0], r[1], s);

    if (((s > 0) ? s : (-s)) > MISSION_2_4_BED_BRIDGE_THRESHOLDS) {
        ACTION_MOVE(LONG, ((s > 0) ? DIR_LEFT : DIR_RIGHT), MIDDLE, OBLIQUE, 1);
        RobotSleep(1);
        ACTION_WALK(SLOW, OBLIQUE, 2);
    }

    return ((s > 0) ? s : (-s)) <= MISSION_2_4_BED_BRIDGE_THRESHOLDS;
}

int mission_2_2_before_bridge_set_center(U16 *image) {
    U32 row, i;
    U16 black_len[2] = {0,}, col[2] = {
            MISSION_2_2_BLACK_LINE_COL_POINT_1,
            MISSION_2_2_BLACK_LINE_COL_POINT_2
    };

    for (i = 0; i < 2; ++i) {
        for (row = HEIGHT - 1; row > 0; --row) {
            if (GetValueRGBYOBK(GetPtr(image, row, col[i], WIDTH), BLACK) &&
                GetValueRGBYOBK(GetPtr(image, row, col[i] + 1, WIDTH), BLACK))
                break;
            black_len[i] += 1;
        }
    }


    printf("\t\t- M2-2: SET CENTER\n");
    printf("\t\t\t+ black[0]: %d, black_len[1]: %d\n", black_len[0], black_len[1]);

    black_len[0] = (U16) ((black_len[0] + black_len[1]) / 2);

    printf("\t\t\t+ LENGTH: %d\n\n", black_len[0]);

    int rResult = 0;
    if (black_len[0] < MISSION_2_2_BLACK_LINE_RANGE - MISSION_2_2_BLACK_LINE_ERROR) {
        ACTION_MOVE(LONG, DIR_RIGHT, MIDDLE, LEFT, 1);
    } else if (black_len[0] > MISSION_2_2_BLACK_LINE_RANGE + MISSION_2_2_BLACK_LINE_ERROR) {
        ACTION_MOVE(LONG, DIR_LEFT, MIDDLE, LEFT, 1);
    } else {
        rResult = 1;
    }

    if (!rResult) {
        RobotSleep(2);
    }

    return rResult;
}

int mission_2_3_escape_red_bridge(void) {
    ACTION_MOTION(MISSION_2_RED_DUMBLING, MIDDLE, OBLIQUE);
    return 1;
}

int mission_2_4_after_bridge_set_straight(U16 *image, int mode) {
    U32 row, i;
    U16 col[2] = {
            MISSION_2_4_BLACK_LINE_COL_POINT_1,
            MISSION_2_4_BLACK_LINE_COL_POINT_2
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
            (MISSION_2_4_BLACK_LINE_COL_POINT_1 - MISSION_2_4_BLACK_LINE_COL_POINT_2)
    );

    printf("Slope : %f\n", s);

    s *= 100;
    int rResult = 1;
    if (((s > 0) ? s : -s) > MISSION_2_4_BLACK_LINE_SLOPE) {
        rResult = 0;
        ACTION_TURN(((s < 0) ? DIR_RIGHT : DIR_LEFT), MIDDLE, ((mode == 0) ? LEFT : RIGHT), (-s > 13) ? 3 : 1);
        RobotSleep(1);
    }

    return rResult;
}

int mission_2_5_after_bridge_set_center(U16 *image) {
    return mission_2_2_before_bridge_set_center(image);
}
