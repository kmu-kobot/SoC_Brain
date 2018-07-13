//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#include "MISSION_4_JUMP_HURDLE.h"

void mission_4_1_watch_front(int repeat) {
    Action_WALK_FRONT_LONG(repeat);
    RobotSleep();
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

    return (MISSION_4_2_HURDLE_THRESHOLDS - MISSION_4_HURDLE_ERROR > s ||
            s > MISSION_4_2_HURDLE_THRESHOLDS + MISSION_4_HURDLE_ERROR);
}

int mission_4_4_jump_hurdle(void) {
    RobotSleep();
    Action_MISSION_4_HURDLING();
    RobotSleep();
    return 1;
}

int mission_4_5_set_straight(U16 *image) {
    U32 row, i, pos_bk[3] = {0,};
    U16 col[3] = {45, 90, 135};

    for (i = 0; i < 3; ++i) {
        for (row = HEIGHT; row > 0; row--) {
            if (GetValueRGBYOBK(
                        GetPtr(image, row, col[i], WIDTH),
                        BLACK
                ) &&
                GetValueRGBYOBK(
                        GetPtr(image, row, col[i] + 1, WIDTH),
                        BLACK
                )) {
                pos_bk[i] = row;
                break;
            }
        }
    }

    RobotSleep();
    int rResult = 0;
    if (
            (pos_bk[2] - pos_bk[1]) < -10 &&
            (pos_bk[1] - pos_bk[0] < -10)
            ) {
        Action_LEFT_TURN_BODY(1);
    } else if (
            (pos_bk[2] - pos_bk[1]) > 10 &&
            (pos_bk[1] - pos_bk[0] > 10)
            ) {
        Action_RIGHT_TURN_BODY(1);
    } else {
        rResult = 1;
    }

    return rResult;
}

int mission_4_5_set_center(U16 *image) {

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

    RobotSleep();
    int rResult = 0;
    if (s < MISSION_4_5_BK_LINE_RANGE - MISSION_4_5_BK_LINE_ERROR) {
        Action_RIGHT_MOVE_SHORT(3);
    } else if (s > MISSION_4_5_BK_LINE_RANGE + MISSION_4_5_BK_LINE_ERROR) {
        Action_LEFT_MOVE_SHORT(3);
    } else {
        rResult = 1;
    }

    return rResult;
}

void mission_4_5_watch_side(void) {
    Action_RIGHT_TURN_HEAD_LONG();
    RobotSleep();
}