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

    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < MISSION_4_HURDLE_CRITERI; ++col) {
            for (i = 0; i < 3; ++i) {
                checkHurdleLine[col] += GetValueRGBYOBK(
                        GetPtr(image, row, ((col + 1) * WIDTH / (MISSION_4_HURDLE_CRITERI + 1)) + i, WIDTH),
                        BLACK
                );
            }
        }
    }

    col = 0;
    for (i = 0; i < MISSION_4_HURDLE_CRITERI; ++i) {
        col += checkHurdleLine[i];
    }
    col /= 3;

    for (i = 0; i < MISSION_4_HURDLE_CRITERI; ++i) {
        row = checkHurdleLine[i] - col;
        if ((-MISSION_4_HURDLE_ERROR) > row && row > MISSION_4_HURDLE_ERROR) {
            return 0;
        }
    }

    return 1;
}

int mission_4_3_jump_hurdle(void) {
    Action_MISSION_4_HURDLING();
    return 1;
}

int mission_4_4_set_straight(U16 *image) {
    U32 row, i, pos_bk[3] = {0,};
    U16 col[3] = {45, 90, 135};

    for (i = 0; i < 3; ++i) {
        for (row = HEIGHT; row > 0; row--) {
            if (
                    GetValueRGBYOBK(
                            GetPtr(image, row, col[i], WIDTH),
                            BLACK
                    ) == 1
                    ) {
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

int mission_4_5_check_bk_line(U16 *image) {
    U32 col, row, i;
    U16 checkHurdleLine[MISSION_4_HURDLE_CRITERI] = {0,};

    for (row = HEIGHT; row > 0; --row) {
        for (col = 0; col < MISSION_4_HURDLE_CRITERI; ++col) {
            for (i = 0; i < MISSION_4_HURDLE_CRITERI; ++i) {
                checkHurdleLine[col] += (GetValueRGBYOBK(
                                                 GetPtr(image, row,
                                                        ((col + 1) * WIDTH / (MISSION_4_HURDLE_CRITERI + 1)) + i,
                                                        WIDTH),
                                                 BLACK
                                         ) == 1) ? 0 : 1;
            }
        }
    }

    int rResult = 1;
    for (col = 0; col < 3; ++col) {
        checkHurdleLine[col] /= MISSION_4_HURDLE_CRITERI;
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

int mission_4_6_set_center(U16 *image) {
    U32 row, i, pos_bk[3] = {0,};
    U16 col[3] = {45, 90, 135};

    for (i = 0; i < 3; ++i) {
        for (row = HEIGHT; row > 0; row--) {
            if (
                    GetValueRGBYOBK(
                            GetPtr(image, row, col[i], WIDTH),
                            BLACK
                    ) == 1
                    ) {
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

void mission_4_6_watch_side(void) {
    Action_RIGHT_TURN_HEAD_LONG();
}