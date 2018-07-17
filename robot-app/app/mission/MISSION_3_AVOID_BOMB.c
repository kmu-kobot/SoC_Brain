//
// Created by Gihyeon Yang on 2018. 7. 17..
//

#include "MISSION_3_AVOID_BOMB.h"

void mission_3_4_watch_front(void) {
    Action_INIT_ROBOT();
    // TODO: 적당히 사선 보면 좋겠음
    RobotSleep(5);
}

int mission_3_4_is_not_front_of_bomb(U16 *image) {

    U32 row, col, check, i, col_start_end[2] = {MISSION_3_DEFAULT_WIDTH, WIDTH - MISSION_3_DEFAULT_WIDTH};

    check = 0;
    for (i = 0; i < 3; ++i) {
        for (col = col_start_end[0]; col < col_start_end[1]; ++col) {
            for (row = MISSION_3_4_HEIGHT_OFFSET; row < HEIGHT; ++row) {
                check += GetValueRGBYOBK(
                        GetPtr(image, row, col, WIDTH),
                        BLACK
                );
            }
        }
    }

    return check < MISSION_3_4_BOMB;
}

void mission_3_default_watch_below(void) {
    Action_WATCH_BELOW_LONG();
    RobotSleep(5);
}

int mission_3_default_avoid_bomb(U16 *image) {
    U32 row, col, check[3] = {0,}, i, col_start_end[3][2] = {{0,                       MISSION_3_DEFAULT_WIDTH},
                                                             {MISSION_3_DEFAULT_WIDTH, WIDTH - MISSION_3_DEFAULT_WIDTH},
                                                             {WIDTH - MISSION_3_DEFAULT_WIDTH, WIDTH}};

    for (i = 0; i < 3; ++i) {
        for (col = col_start_end[i][0]; col < col_start_end[i][1]; ++col) {
            for (row = MISSION_3_DEFAULT_HEIGHT_OFFSET; row < HEIGHT - MISSION_3_DEFAULT_HEIGHT_OFFSET; ++row) {
                check[i] += GetValueRGBYOBK(
                        GetPtr(image, row, col, WIDTH),
                        BLACK
                );
            }
        }

        check[i] = ((check[i] > MISSION_3_DEFAULT_BOMB_THRESHOLDS) ?
                    ((check[i] > MISSION_3_DEFAULT_LINE_THRESHOLDS) ? 2 : 1)
                                                                   : 0);

    }

    int s = 0;
    for (i = 0; i < 3; ++i) {
        s += ((check[i] != 0) ? 1 : 0) * (10 ^ (2 - i));
    }

    int rReturn = (s == 010) ? 1 : 0;
    if (!rReturn) {
        if (s == 100 || s == 110 || s == 111) {
            Action_RIGHT_MOVE_SHORT(check[0]);
        } else if (s == 001 || s == 011) {
            Action_LEFT_MOVE_SHORT(check[2]);
        } else if (s == 101 || s == 000) {
            Action_WALK_FRONT_SHORT(3);
            rReturn = 1;
        }
    } else {
        rReturn = 0;

        check[1] = 0;
        check[2] = 0;

        for (col = col_start_end[1][0]; col < col_start_end[1][1]; ++col) {
            check[0] = 0;
            for (row = MISSION_3_DEFAULT_HEIGHT_OFFSET; row < HEIGHT - MISSION_3_DEFAULT_HEIGHT_OFFSET; ++row) {
                check[0] += GetValueRGBYOBK(
                        GetPtr(image, row, col, WIDTH),
                        BLACK
                );
            }
            if (check[1] < check[0]) {
                check[2] = col;
            }
        }

        check[2] += col_start_end[1][0];

        if (check[2] < WIDTH / 2 - MISSION_3_DEFAULT_AVOID_BOMB_RANGE) {
            Action_LEFT_MOVE_SHORT(2);
        } else if (check[2] > WIDTH / 2 + MISSION_3_DEFAULT_AVOID_BOMB_RANGE) {
            Action_RIGHT_MOVE_SHORT(2);
        } else {
            // TODO: 지뢰 피하는 동작
            Action_WALK_FRONT_SHORT(6);
            rReturn = 1;
        }

    }

    RobotSleep(5);

    return rReturn;
}
