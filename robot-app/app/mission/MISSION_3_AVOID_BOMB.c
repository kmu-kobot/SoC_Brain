//
// Created by Gihyeon Yang on 2018. 7. 17..
//

#include <math.h>
#include "MISSION_3_AVOID_BOMB.h"

void mission_3_4_watch_front(void) {
    ACTION_INIT(MIDDLE, OBLIQUE);
    RobotSleep(1);
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
    ACTION_INIT(MIDDLE, DOWN);
    RobotSleep(1);
}

int mission_3_default_avoid_bomb(U16 *image) {
    U32 row, col, check[3] = {0,}, i, col_start_end[3][2] = {{MISSION_3_DEFAULT_WIDTH_MARGIN, MISSION_3_DEFAULT_WIDTH},
                                                             {MISSION_3_DEFAULT_WIDTH,        WIDTH -
                                                                                              MISSION_3_DEFAULT_WIDTH},
                                                             {WIDTH - MISSION_3_DEFAULT_WIDTH,
                                                                     WIDTH - MISSION_3_DEFAULT_WIDTH_MARGIN}};

    for (i = 0; i < 3; ++i) {
        for (col = col_start_end[i][0]; col < col_start_end[i][1]; ++col) {
            for (row = 10; row < ROBOT_KNEE; ++row) {
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
            ACTION_MOVE(((check[0] == 2) ? LONG : SHORT), DIR_RIGHT, MIDDLE, DOWN, 2);
        } else if (s == 1 || s == 11) {
            ACTION_MOVE(((check[2] == 2) ? LONG : SHORT), DIR_LEFT, MIDDLE, DOWN, 2);
        } else if (s == 101 || s == 0) {
             ACTION_WALK(SLOW, DOWN, 3);
             rReturn = 1;
        }
    } else {
        rReturn = 0;

        check[1] = 0;
        check[2] = 0;

        int px, py;

        for (row = 10; row < ROBOT_KNEE; ++row) {
            check[0] = 0;
            for (col = col_start_end[1][0]; col < col_start_end[1][1]; ++col) {
                check[0] += GetValueRGBYOBK(
                        GetPtr(image, row, col, WIDTH),
                        BLACK
                );
            }
            if (check[1] < check[0] && check[2] <= row) {
                check[2] = row;
                check[1] = check[0];
            }
        }

        printf("\t\t\t- O-y: %d\n", check[2]);

        py = check[2];

        check[1] = 0;
        check[2] = 0;
        check[0] = 0;

        for (col = col_start_end[1][0]; col < col_start_end[1][1]; ++col) {
            check[0] = 0;
            for (row = 0; row < ROBOT_KNEE; ++row) {
                check[0] += GetValueRGBYOBK(
                        GetPtr(image, row, col, WIDTH),
                        BLACK
                );
            }

            if (check[1] < check[0]) {
                check[2] = col;
                check[1] = check[0];
            }

        }

        printf("\t\t\t- O-x: %d\n", check[2]);
        px = check[2];
        printf("\t\t\t\t (%d, %d)\n", px, py);

        if (py < MISSION_3_POINT_LENGTH) {
            ACTION_WALK(CLOSE, DOWN,
                        (py < MISSION_3_POINT_LENGTH - 12) ? 4 :
                        (py < MISSION_3_POINT_LENGTH - 5) ? 2 : 1);
            RobotSleep(1);
            return 0;
        }

        if (WIDTH / 2 - MISSION_3_DEFAULT_AVOID_BOMB_RANGE > px) {
            // right
            ACTION_MOVE(SHORT, DIR_RIGHT, MIDDLE, DOWN, 2);
            return 0;
        } else if (px > WIDTH / 2 + MISSION_3_DEFAULT_AVOID_BOMB_RANGE) {
            // left
            ACTION_MOVE(SHORT, DIR_LEFT, MIDDLE, DOWN, 2);
            return 0;
        } else {
            ACTION_INIT(MIDDLE, UP);
            return 1;
        }
    }

    RobotSleep(1);

    return rReturn;
}
