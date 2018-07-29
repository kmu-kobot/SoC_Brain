//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#include "MISSION_4_JUMP_HURDLE.h"

void mission_4_1_watch_front(int repeat) {
    ACTION_WALK(SLOW, OBLIQUE, repeat);
}

int mission_4_2_ready_hurdle(U16 *image) {
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
    return (CASE_4_0_LENGTH - CASE_4_0_LENGTH_ERROR <= s &&
            s <= CASE_4_0_LENGTH + CASE_4_0_LENGTH_ERROR);
}

int mission_4_4_jump_hurdle(void) {
    ACTION_MOTION(MISSION_4_HURDLING, MIDDLE, OBLIQUE);
    ACTION_WALK(FAST, OBLIQUE, 8);
    RobotSleep(2);
    ACTION_TURN(LONG, DIR_LEFT, MIDDLE, OBLIQUE, 7);
    return 1;
}


void mission_4_5_watch_diagonal_line(void) {
    ACTION_INIT(MIDDLE, OBLIQUE);
}

int mission_4_5_set_front_of_not_bk(U16 *image) {
    U32 col[3] = {85, 95, 90}, row, i;
    int checkHurdleLine[3] = {0,};

    for (i = 0; i < 3; ++i) {
        for (row = HEIGHT - 1; row >= 0; --row) {
            if (GetValueRGBYOBK(GetPtr(image, row, col[i], WIDTH), BLACK) &&
                GetValueRGBYOBK(GetPtr(image, row, col[i] + 1, WIDTH), BLACK)) {
                checkHurdleLine[i] = (HEIGHT - row);
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

    if (s < CASE_4_1_NON_BLACK_LINE) {
        ACTION_TURN(LONG, DIR_LEFT, MIDDLE, OBLIQUE, 1);
        return 0;
    } else {
        ACTION_TURN(LONG, DIR_LEFT, MIDDLE, OBLIQUE, 3);
        return 1;
    }
}

int mission_4_6_set_center(U16 *image, int length) {
    U32 col[3] = {85, 95, 90}, row, i;
    U16 checkHurdleLine[3] = {0,};
    int j;

    for (i = 0; i < 3; ++i) {
        for (row = HEIGHT - 1; row > 0; --row) {
            checkHurdleLine[i] = 0;

            for (j = -3; j < 3; j++) {
                checkHurdleLine[i] += GetValueRGBYOBK(GetPtr(image, row, col[i] + j, WIDTH), BLACK);
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

    int k = (length != 0) ? length : CASE_0_DEFAULT_RIGHT_RANGE;

    if (s < k - CASE_0_DEFAULT_RANGE_ERROR) {
        printf("LEFT\n\n");
        ACTION_MOVE(LONG, DIR_LEFT, MIDDLE, RIGHT, 1);
        return 0;
    } else if (s > k + CASE_0_DEFAULT_RANGE_ERROR) {
        printf("RIGHT\n\n");
        ACTION_MOVE(LONG, DIR_RIGHT, MIDDLE, RIGHT, 1);
        return 0;
    } else {
        printf("SUCCESS\n\n");
        return 1;
    }
}

void mission_4_6_watch_side(void) {
    ACTION_INIT(MIDDLE, RIGHT);
    RobotSleep(1);
}
