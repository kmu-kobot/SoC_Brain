//
// Created by Gihyeon Yang on 2018. 7. 17..
//

#include <math.h>
#include "MISSION_3_AVOID_BOMB.h"

void mission_3_4_watch_front(void) {
    ACTION_INIT(LOW, OBLIQUE);
    // TODO: 적당히 사선 보면 좋겠음
    RobotSleep(5);
}

int mission_3_4_is_not_front_of_bomb(U16 *image) {

    U32 row, col, check, i, col_start_end[2] = {MISSION_3_DEFAULT_WIDTH, WIDTH - MISSION_3_DEFAULT_WIDTH};

    check = 0;
    for (i = 0; i < 3; ++i) {
        for (col = col_start_end[0]; col < col_start_end[1]; ++col) {
            for (row = MISSION_3_4_HEIGHT_OFFSET; row < HEIGHT; ++row) {
                check += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK);
            }
        }
    }

    if (check < MISSION_3_4_BOMB) {
        ACTION_WALK(SLOW, DOWN, 3);
    }

    return check < MISSION_3_4_BOMB;
}

void mission_3_default_watch_below(void) {
    ACTION_INIT(LOW, DOWN);
    RobotSleep(5);
}

int mission_3_default_avoid_bomb(U16 *image) {
    U32 row, col, check[3] = {0,}, i, col_start_end[3][2] = {{MISSION_3_DEFAULT_WIDTH_MARGIN, MISSION_3_DEFAULT_WIDTH},
                                                             {MISSION_3_DEFAULT_WIDTH,        WIDTH -
                                                                                              MISSION_3_DEFAULT_WIDTH},
                                                             {WIDTH - MISSION_3_DEFAULT_WIDTH,
                                                                     WIDTH - MISSION_3_DEFAULT_WIDTH_MARGIN}};

    for (i = 0; i < 3; ++i) {
        for (col = col_start_end[i][0]; col < col_start_end[i][1]; ++col) {
            for (row = 10; row < HEIGHT - MISSION_3_DEFAULT_HEIGHT_OFFSET; ++row) {
                check[i] += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK);
            }
        }

        check[i] = ((check[i] > MISSION_3_DEFAULT_BOMB_THRESHOLDS) ?
                    ((check[i] > MISSION_3_DEFAULT_LINE_THRESHOLDS) ? 2 : 1)
                                                                   : 0);

    }

    int s = 0;
    for (i = 0; i < 3; ++i) {
        s += ((check[i] == 0) ? 0 : (int) pow((double) 10, (double) (2 - i)));
    }

    printf("\n\t\t- M3-DEFAULT: AVOID BOMB\n");
    printf("\t\t\t- BOMB: %d %d %d %03d\n\n", check[0], check[1], check[2], s);

    int rReturn = (s == 10) ? 1 : 0;
    if (!rReturn) {
        if (s == 100 || s == 110 || s == 111) {
            if (check[0] == 1) {
                ACTION_MOVE(DIR_RIGHT, LOW, DOWN, 4);
            } else {
                ACTION_MOVE(DIR_RIGHT, LOW, DOWN, 1);
            }
        } else if (s == 1 || s == 11) {
            if (check[2] == 1) {
                ACTION_MOVE(DIR_LEFT, LOW, DOWN, 4);
            } else {
                ACTION_MOVE(DIR_LEFT, LOW, DOWN, 4);
            }
        } else if (s == 101 || s == 0) {
            ACTION_WALK(FAST, DOWN, 3);
            rReturn = 1;
        }
    } else {
        rReturn = 0;

        check[1] = 0;
        check[2] = 0;

        for (row = 10; row < HEIGHT - MISSION_3_DEFAULT_HEIGHT_OFFSET; ++row) {
            check[0] = 0;
            for (col = col_start_end[1][0]; col < col_start_end[1][1]; ++col) {
                check[0] += GetValueRGBYOBK(
                        GetPtr(image, row, col, WIDTH),
                        BLACK
                );
            }
            if (check[1] < check[0] && check[2] < row) {
                check[2] = row;
                check[1] = check[0];
            }
        }

        printf("\t\t\t- O-x: %d", check[2]);

        if (check[2] < WIDTH / 2 - MISSION_3_DEFAULT_AVOID_BOMB_RANGE) {
            ACTION_MOVE(DIR_LEFT, LOW, DOWN, 2);
        } else if (check[2] > WIDTH / 2 + MISSION_3_DEFAULT_AVOID_BOMB_RANGE) {
            ACTION_MOVE(DIR_RIGHT, LOW, DOWN, 2);
        } else {
            // TODO: 지뢰 피하는 동작
//            ACTION_MISSION_3(6);
            rReturn = 1;
        }

    }

    RobotSleep(5);

    return rReturn;
}
