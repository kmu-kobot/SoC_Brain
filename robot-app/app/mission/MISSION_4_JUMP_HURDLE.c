//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#include "MISSION_4_JUMP_HURDLE.h"

void mission_4_1_watch_front(int repeat) {
    ACTION_WALK(FAST, OBLIQUE, repeat);
    RobotSleep(5);
}

int mission_4_2_ready_hurdle(U16 *image) {

    U32 col[3] = {70, 60, 90}, row, i;
    U16 checkHurdleLine[MISSION_4_HURDLE_CRITERI] = {0,};

    for (i = 0; i < MISSION_4_HURDLE_CRITERI; ++i) {
        for (row = HEIGHT; row > 0; --row) {
            if (GetValueRGBYOBK(
                        GetPtr(image, row, col[i], WIDTH),
                        BLACK
                ) &&
                GetValueRGBYOBK(
                        GetPtr(image, row, col[i] + 1, WIDTH),
                        BLACK
                )) {
                checkHurdleLine[i] = (U16) row;
                break;
            }
        }
    }

    double s = 0;
    printf("BLACK LINE\n");
    for (i = 0; i < MISSION_4_HURDLE_CRITERI; ++i) {
        s += checkHurdleLine[i];
        printf("bk_line[%d]: %d,\t", i, checkHurdleLine[i]);
    }
    printf("\n");

    s /= MISSION_4_HURDLE_CRITERI;
    printf("AVG: %f\n", s);

    return (MISSION_4_2_HURDLE_THRESHOLDS - MISSION_4_HURDLE_ERROR < s &&
            s < MISSION_4_2_HURDLE_THRESHOLDS + MISSION_4_HURDLE_ERROR);
}

int mission_4_4_jump_hurdle(void) {
    ACTION_WALK(FAST, OBLIQUE, 3);
    RobotSleep(5);
//    ACTION_MOTION(MISSION_4_HURDLING);
    RobotSleep(5);
    ACTION_WALK(FAST, OBLIQUE, 3);
    return 1;
}


void mission_4_5_watch_diagonal_line(void) {
    ACTION_INIT(LOW, OBLIQUE);
    RobotSleep(5);
}

int mission_4_5_set_front_of_not_bk(U16 *image) {
    U32 col[3] = {70, 60, 90}, row, i;
    U16 checkHurdleLine[MISSION_4_HURDLE_CRITERI] = {0,};

    for (i = 0; i < MISSION_4_HURDLE_CRITERI; ++i) {
        for (row = HEIGHT; row > 0; --row) {
            if (GetValueRGBYOBK(
                        GetPtr(image, row, col[i], WIDTH),
                        BLACK
                ) &&
                GetValueRGBYOBK(
                        GetPtr(image, row, col[i] + 1, WIDTH),
                        BLACK
                )) {
                checkHurdleLine[i] = (U16) (HEIGHT - row);
                break;
            }
        }
    }

    double s = 0;
    printf("\nM4-5: BLACK LINE\n");
    for (i = 0; i < MISSION_4_HURDLE_CRITERI; ++i) {
        s += checkHurdleLine[i];
        printf("bk_line[%d]: %d,\t", i, checkHurdleLine[i]);
    }
    printf("\n");

    s /= MISSION_4_HURDLE_CRITERI;
    printf("AVG: %f\n\n", s);

    int rResult = 1;
    if (s < MISSION_4_5_WHITE_RANGE) {
        ACTION_TURN(DIR_LEFT, LOW, OBLIQUE, 5);
        rResult = 0;
    }
    RobotSleep(5);

    return rResult;
}

int mission_4_6_set_center(U16 *image) {
    U32 col[3] = {70, 60, 90}, row, i;
    U16 checkHurdleLine[MISSION_4_HURDLE_CRITERI] = {0,};

    for (i = 0; i < MISSION_4_HURDLE_CRITERI; ++i) {
        for (row = HEIGHT; row > 0; --row) {
            if (GetValueRGBYOBK(
                        GetPtr(image, row, col[i], WIDTH),
                        BLACK
                ) &&
                GetValueRGBYOBK(
                        GetPtr(image, row, col[i] + 1, WIDTH),
                        BLACK
                )) {
                checkHurdleLine[i] = (U16) row;
                break;
            }
        }
    }

    double s = 0;
    printf("M4-5: BLACK LINE\n");
    for (i = 0; i < MISSION_4_HURDLE_CRITERI; ++i) {
        s += checkHurdleLine[i];
        printf("bk_line[%d]: %d,\t", i, checkHurdleLine[i]);
    }
    printf("\n");

    s /= MISSION_4_HURDLE_CRITERI;
    printf("M4-5: AVG: %f\n", s);

    int rResult = 0;
    if (s < MISSION_4_5_BK_LINE_RANGE - MISSION_4_5_BK_LINE_ERROR) {
        ACTION_MOVE(DIR_RIGHT, LOW, OBLIQUE, 1);
    } else if (s > MISSION_4_5_BK_LINE_RANGE + MISSION_4_5_BK_LINE_ERROR) {
        ACTION_MOVE(DIR_LEFT, LOW, OBLIQUE, 1);
    } else {
        rResult = 1;
    }

    if (!rResult) {
        RobotSleep(5);
    }

    return rResult;
}

void mission_4_6_watch_side(void) {
    ACTION_INIT(LOW, RIGHT);
    RobotSleep(5);
}
