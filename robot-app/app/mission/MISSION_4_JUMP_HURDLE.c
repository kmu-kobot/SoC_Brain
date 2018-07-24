//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#include "MISSION_4_JUMP_HURDLE.h"

void mission_4_1_watch_front(int repeat) {
    ACTION_WALK(SLOW, OBLIQUE, repeat);
    RobotSleep(2);
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

    return (MISSION_4_2_HURDLE_THRESHOLDS - MISSION_4_HURDLE_ERROR <= s &&
            s <= MISSION_4_2_HURDLE_THRESHOLDS + MISSION_4_HURDLE_ERROR);
}

int mission_4_4_jump_hurdle(void) {
    ACTION_WALK(FAST, OBLIQUE, 2);
    RobotSleep(2);
    ACTION_MOTION(MISSION_4_HURDLING, MIDDLE, OBLIQUE);
    RobotSleep(2);
    ACTION_WALK(FAST, OBLIQUE, 10);
    return 1;
}


void mission_4_5_watch_diagonal_line(void) {
    ACTION_INIT(MIDDLE, OBLIQUE);
    RobotSleep(2);
}

int mission_4_5_set_front_of_not_bk(U16 *image) {
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

    int rResult = 1;
    if (s < MISSION_4_5_WHITE_RANGE) {
        ACTION_TURN(DIR_LEFT, MIDDLE, OBLIQUE, 5);
        rResult = 0;
    }
    return rResult;
}

int mission_4_6_set_center(U16 *image, int length) {
    U32 col[3] = {85, 95, 90}, row, i, j;
    U16 checkHurdleLine[MISSION_4_HURDLE_CRITERI] = {0,};

    for (i = 0; i < MISSION_4_HURDLE_CRITERI; ++i) {
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
    for (i = 0; i < MISSION_4_HURDLE_CRITERI; ++i) {
        if (s < checkHurdleLine[i]) {
            s = checkHurdleLine[i];
        }
        printf("bk_line[%d]: %d,\t", i, checkHurdleLine[i]);
    }
    printf("\n");

    printf("M4-5: AVG: %f\n", s);

    int rResult = 0;

    length = (length != 0) ? length : MISSION_4_5_BK_LINE_RANGE;

    if (s < length - MISSION_4_5_BK_LINE_ERROR) {
        ACTION_MOVE(LONG, DIR_LEFT, MIDDLE, RIGHT, 1);
    } else if (s > length + MISSION_4_5_BK_LINE_ERROR) {
        ACTION_MOVE(LONG, DIR_RIGHT, MIDDLE, RIGHT, 1);
    } else {
        rResult = 1;
    }

    if (!rResult) {
        RobotSleep(1);
    }

    return rResult;
}

void mission_4_6_watch_side(void) {
    ACTION_INIT(MIDDLE, RIGHT);
    RobotSleep(1);
}
