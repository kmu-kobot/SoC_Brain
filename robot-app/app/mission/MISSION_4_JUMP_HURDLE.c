//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#include "MISSION_4_JUMP_HURDLE.h"

void mission_4_1_watch_front(int repeat) {
    Action_WALK_FRONT_SHORT(repeat);
}

int mission_4_2_ready_hurdle(U16 *image) {

    U32 col, row, i;
    U16 checkHurdleLine[MISSION_4_HURDLE_CRITERI] = {0,};

    for (i = 0; i < MISSION_4_HURDLE_CRITERI; ++i) {
        for (row = HEIGHT; row > 0; --row) {
            col = (i + 1) * WIDTH / (MISSION_4_HURDLE_CRITERI + 1);
            if (GetValueRGBYOBK(
                        GetPtr(image, row, col, WIDTH),
                        BLACK
                ) &&
                GetValueRGBYOBK(
                        GetPtr(image, row, col + 1, WIDTH),
                        BLACK
                )) {
                checkHurdleLine[i] = (U16) row;
                break;
            }
        }
    }

    col = 0;
    for (i = 0; i < MISSION_4_HURDLE_CRITERI; ++i) {
        col += checkHurdleLine[i];
    }

    col /= MISSION_4_HURDLE_CRITERI;

    for (i = 0; i < MISSION_4_HURDLE_CRITERI; ++i) {
        row = checkHurdleLine[i] - col;
        if (MISSION_4_2_HURDLE_THRESHOLDS - MISSION_4_HURDLE_ERROR > row ||
            row > MISSION_4_2_HURDLE_THRESHOLDS + MISSION_4_HURDLE_ERROR)
            return 0;
    }

    return 1;
}

int mission_4_4_jump_hurdle(void) {
    Action_MISSION_4_HURDLING();
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

    Action_INIT_ROBOT();

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

int mission_4_6_check_bk_line(U16 *image) {
    U32 col, row, i;
    U16 checkHurdleLine[MISSION_4_HURDLE_CRITERI] = {0,};

    for (row = HEIGHT; row > 0; --row) {
        for (i = 0; i < MISSION_4_HURDLE_CRITERI; ++i) {
            col = (i + 1) * WIDTH / (MISSION_4_HURDLE_CRITERI + 1);
            if (GetValueRGBYOBK(
                        GetPtr(image, row, col, WIDTH),
                        BLACK
                ) &&
                GetValueRGBYOBK(
                        GetPtr(image, row, col + 1, WIDTH),
                        BLACK
                )) {
                checkHurdleLine[i] = (U16) (HEIGHT - row);
                break;
            }
        }
    }

    int rResult = 1;
    for (col = 0; col < 3; ++col) {
        if (checkHurdleLine[col] <= MISSION_4_BK_LINE_RANGE) {
            rResult = 0;
            break;
        }
    }

    if (!rResult) {
        Action_LEFT_TURN_BODY(2);
    }

    return rResult;
}

int mission_4_7_set_center(U16 *image) {
    U32 row, col, i, pos_bk[MISSION_4_HURDLE_CRITERI] = {0,};

    for (i = 0; i < MISSION_4_HURDLE_CRITERI; ++i) {
        for (row = HEIGHT; row > 0; row--) {
            col = (i + 1) * (WIDTH / (MISSION_4_HURDLE_CRITERI + 1));
            if (GetValueRGBYOBK(
                        GetPtr(image, row, col, WIDTH),
                        BLACK
                ) &&
                GetValueRGBYOBK(
                        GetPtr(image, row, col + 1, WIDTH),
                        BLACK
                )) {
                pos_bk[i] = row;
                break;
            }
        }
    }

    Action_INIT_ROBOT();

    int rResult = 0;

    if ((pos_bk[0] > MISSION_4_BK_LINE_LOWER) &&
        (pos_bk[1] > MISSION_4_BK_LINE_LOWER) &&
        (pos_bk[2] > MISSION_4_BK_LINE_LOWER)) {
        Action_LEFT_TURN_BODY(1);
    } else if ((pos_bk[0] > MISSION_4_BK_LINE_LOWER - MISSION_4_BK_LINE_RANGE) &&
               (pos_bk[1] > MISSION_4_BK_LINE_LOWER - MISSION_4_BK_LINE_RANGE) &&
               (pos_bk[2] > MISSION_4_BK_LINE_LOWER - MISSION_4_BK_LINE_RANGE)) {
        Action_RIGHT_TURN_BODY(1);
    } else
        rResult = 1;

    return rResult;
}

void mission_4_7_watch_side(void) {
    Action_RIGHT_TURN_HEAD_LONG();
}