//
// Created by Gihyeon Yang on 2018. 7. 13..
//

#include "MISSION_6_KICK_BALL.h"
#include <stdlib.h>
#include <math.h>


U32 hole_points[2] = {0,};
U32 ball_points[2] = {0,};
U32 hole_top, hole_bottom, hole_left, hole_right;
U32 ball_top, ball_bottom, ball_left, ball_right;

int mission_6_1_detection_ball(U16 *image) {

    int ball_state = mission_6_4_find_ball_interpolation(image, 0, HEIGHT, 0, WIDTH, 0);

    if (ball_state == 0) {
        return 1;
    } else if (ball_state == 2) {
        return 0;
    }

    // LEFT or RIGHT
    DIRECTION move_dir = ball_points[0] > WIDTH / 2;

    if (ball_points[0] < 70 || ball_points[0] > 110) {
        ACTION_MOVE(LONG, move_dir, UP, MIN(2, abs((WIDTH >> 2) - ball_points[0]) / 10));
    } else {
        ACTION_WALK(FAST, OBLIQUE, (HEIGHT - ball_points[1]) / 15 + 1);
    }
    return 0;
}


int mission_6_3_find_side_hole(U16 *image, U8 step) {
    static MOTION heads[] = {BALL_HEAD_HALF_RIGHT, BALL_HEAD_HALF_LEFT};
    int state;

    BALL_HALF_HEAD(heads[step]);
    state = mission_6_3_find_hole_interpolation(image, 0, HEIGHT, 0, WIDTH, 0);

    if (state == 1) {
        if (step == 0) {
            BALL_MOVE_LONG(DIR_LEFT, UP, 1);
            BALL_TURN(DIR_RIGHT, LONG, 3);
        } else if (step == 1) {
            BALL_MOVE_LONG(DIR_RIGHT, UP, 1);
            BALL_TURN(DIR_LEFT, LONG, 3);
        } else {
            mission_6_4_turn_right();
            return mission_6_3_find_side_hole(image, 0);
        }

        BALL_HEAD(DOWN);
        RobotSleep(4);
        mission_6_4_set_center_of_ball(image);

        BALL_HEAD(UP);
        RobotSleep(4);
        return 1;
    } else {
        if (step > 1) {
            return 0;
        } else {
            return mission_6_3_find_side_hole(image, step + 1);
        }
    }

    return 0;
}

void mission_6_4_turn_right(void) {
    BALL_MOVE(DIR_LEFT, UP, 3);
    BALL_TURN(DIR_RIGHT, MIDDLE, 5);
}

int mission_6_3_find_hole_interpolation(U16 *image, U32 top, U32 bot, U32 left, U32 right, U8 last) {
    int cnt = 0, success = 0, many = 0;
    int state;
    int x[5], y[5];

    for (; cnt < 10 && success < 5; ++cnt) {
        setFPGAVideoData(image);
        state = mission_6_3_find_hole(image, top, bot, left, right, last);
        if (state == 1) {
            x[success] = hole_points[0];
            y[success] = hole_points[1];
            ++success;
        } else if (state == 2) {
            ++many;
        }
    }

#ifdef DEBUG
    printf("hole success : %d\n", success);
#endif

    if (success < 2) {
        return many < 2 ? 0 : mission_6_3_find_hole_interpolation(image, top, bot, left, right, 1);
    } else if (success < 4) {
        hole_points[0] = average(x, success);
        hole_points[1] = average(y, success);
        return 1;
    } else if (success < 6) {
        hole_points[0] = analysis(x, success);
        hole_points[1] = analysis(y, success);
        return 1;
    }

    return 0;
}

int mission_6_3_find_hole(U16 *image, U32 top, U32 bot, U32 left, U32 right, U8 last) {
    int state = mission_6_3_find_hole_row(image, top, bot, left, right, last);
    if (state == 1) {
        return mission_6_3_find_hole_col(image, hole_top, hole_bottom, left, right, last);
    }

    return state;
}


int mission_6_3_find_hole_row(U16 *image, U32 top, U32 bot, U32 left, U32 right, U8 last) {
    U32 row, col, hole_cnt, pos = 0;
    U8 hole[4] = {0, 0, 0, 0};

    //hole_top 찾기
    for (row = top; row < MIN(bot + 1, HEIGHT); ++row) {
        hole_cnt = 0;
        for (col = left; col < MIN(right + 1, WIDTH); ++col) {
            hole_cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
        }
        hole[pos] = hole_cnt > MISSION_6_3_THRES;
        if (hole[0] + hole[1] + hole[2] + hole[3] > 2) {
            hole_top = row - 2;

            pos = (pos + 1) % 4;
            break;
        }
        pos = (pos + 1) % 4;
    }


    //hole_bottom 찾기
    for (++row; row < MIN(bot + 1, HEIGHT); ++row) {
        hole_cnt = 0;
        for (col = left; col < MIN(right + 1, WIDTH); ++col) {
            hole_cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
        }
        hole[pos] = hole_cnt > MISSION_6_3_THRES;
        if (hole[0] + hole[1] + hole[2] + hole[3] < 2) {
            hole_bottom = row - 3;
            pos = (pos + 1) % 4;
            break;
        }
        pos = (pos + 1) % 4;
    }

    if (hole[0] + hole[1] + hole[2] + hole[3] > 2) {
        hole_bottom = --row;
    }


    if (row > MIN(bot, HEIGHT - 1)) {
#ifdef DEBUG
        printf("there are no object\n");
#endif
        return 0;
    }

    for (++row; row < MIN(bot + 1, HEIGHT); ++row) {
        hole_cnt = 0;
        for (col = left; col < MIN(right + 1, WIDTH); ++col) {
            hole_cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
        }
        hole[pos] = hole_cnt > MISSION_6_3_THRES;
        if (hole[0] + hole[1] + hole[2] + hole[3] > 2) {
            if (row - hole_bottom > 15) {
                if (last) {
                    return mission_6_3_find_hole_row(image, row - 2, bot, left, right, 1);
                } else {
#ifdef DEBUG
                    printf("there are many object\n");
#endif
                    return 2;
                }
            }

            hole_bottom = row - 1;
        }
        pos = (pos + 1) % 4;
    }

    hole_points[1] = (hole_top + hole_bottom) / 2;

    return 1;
}

int mission_6_3_find_hole_col(U16 *image, U32 top, U32 bot, U32 left, U32 right, U8 last) {
    U32 row, col, hole_cnt, pos = 0;
    U8 hole[4] = {0, 0, 0, 0};
    U32 hole_right = 0, hole_left = 0;

    //hole_left 찾기
    for (col = left; col < MIN(right + 1, WIDTH); ++col) {
        hole_cnt = 0;
        for (row = top; row < MIN(bot + 1, HEIGHT); ++row) {
            hole_cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
        }
        hole[pos] = hole_cnt > MISSION_6_3_THRES;
        if (hole[0] + hole[1] + hole[2] + hole[3] > 2) {
            hole_left = col - 2;
            pos = (pos + 1) % 4;
            break;
        }
        pos = (pos + 1) % 4;
    }

    //hole_right 찾기
    for (++col; col < MIN(right + 1, WIDTH); ++col) {
        hole_cnt = 0;
        for (row = top; row < MIN(bot + 1, HEIGHT); ++row) {
            hole_cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
        }
        hole[pos] = hole_cnt > MISSION_6_3_THRES;
        if (hole[0] + hole[1] + hole[2] + hole[3] < 2) {
            hole_right = col - 3;
            pos = (pos + 1) % 4;
            break;
        }
        pos = (pos + 1) % 4;
    }

    if (hole[0] + hole[1] + hole[2] + hole[3] > 2) {
        hole_right = --col;
    }


    if (col > MIN(right, WIDTH - 1)) {
#ifdef DEBUG
        printf("there are no object\n");
#endif
        return 0;
    }

    for (++col; col < MIN(right + 1, WIDTH); ++col) {
        hole_cnt = 0;
        for (row = top; row < MIN(bot + 1, HEIGHT); ++row) {
            hole_cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
        }
        hole[pos] = hole_cnt > MISSION_6_3_THRES;
        if (hole[0] + hole[1] + hole[2] + hole[3] > 2) {
            if (col - hole_right > 15) {
                if (last) {
                    mission_6_3_find_hole_col(image, top, bot, col - 2, right, 1);
                } else {
#ifdef DEBUG
                    printf("there are many object\n");
#endif
                    return 2;
                }
            }

            hole_right = col - 1;
        }
        pos = (pos + 1) % 4;
    }

    hole_points[0] = (hole_left + hole_right) / 2;

    return 1;
}

//
// int mission_6_3_set_straight_hole(U16 *image) {
//
//     mission_6_3_find_hole_interpolation(image, 0, HEIGHT, 0, WIDTH, 0);
//
//     double degree = atan2(hole_points[0] - ball_points[0], hole_points[1] - HEIGHT);
//     DIRECTION dir = degree > 0;
//
// #ifdef DEBUG
//     printf("degree : %f\n", degree);
// #endif
//
//     if (abs(degree) > 10.0 * M_PI / 180.0) {
//         ACTION_MOVE(LONG, dir, UP, (int) (abs(degree) * 180.0 / 10.0 / M_PI));
//         return 0;
//     }
//
//     return 1;
// }


int mission_6_3_locate_hole_on_center(U16 *image) {

    if (mission_6_3_find_hole_interpolation(image, 0, HEIGHT, 0, WIDTH, 0) != 1) { // 못찾았을때 주변 탐색 추가
        // mission_6_3_find_side_hole(image, 0);
        return 0;
    }

    if (hole_points[0] < MISSION_6_3_CENTER - 35) {
        BALL_MOVE_LONG(DIR_RIGHT, UP, 1);
        BALL_STABLE(UP);
        BALL_TURN(DIR_LEFT, LONG, 1);
    } else if (hole_points[0] < MISSION_6_3_CENTER - 25) {
        BALL_TURN(DIR_LEFT, MIDDLE, 3);
    } else if (hole_points[0] < MISSION_6_3_CENTER - 15) {
        BALL_TURN(DIR_LEFT, MIDDLE, 2);
    } else if (hole_points[0] < MISSION_6_3_CENTER - 7) {
        BALL_TURN(DIR_LEFT, MIDDLE, 1);
    } else if (hole_points[0] < MISSION_6_3_CENTER - 3) {
        BALL_TURN(DIR_LEFT, SHORT, 1);
    } else if (hole_points[0] > MISSION_6_3_CENTER + 35) {
        BALL_MOVE_LONG(DIR_LEFT, UP, 1);
        BALL_STABLE(UP);
        BALL_TURN(DIR_RIGHT, LONG, 1);
    } else if (hole_points[0] > MISSION_6_3_CENTER + 25) {
        BALL_TURN(DIR_RIGHT, MIDDLE, 3);
    } else if (hole_points[0] > MISSION_6_3_CENTER + 15) {
        BALL_TURN(DIR_RIGHT, MIDDLE, 2);
    } else if (hole_points[0] > MISSION_6_3_CENTER + 7) {
        BALL_TURN(DIR_RIGHT, MIDDLE, 1);
    } else if (hole_points[0] > MISSION_6_3_CENTER + 3) {
        BALL_TURN(DIR_RIGHT, SHORT, 1);
    } else {
        return 1;
    }

    RobotSleep(2);
    return 0;
}

int mission_6_4_find_ball_interpolation(U16 *image, U32 top, U32 bot, U32 left, U32 right, U8 last) {
    int cnt = 0, success = 0, many = 0;
    int state;
    int x[5], y[5];

    for (; cnt < 10 && success < 5; ++cnt) {
        setFPGAVideoData(image);
        state = mission_6_4_find_ball(image, top, bot, left, right, last);
        if (state == 1) {
            x[success] = ball_points[0];
            y[success] = ball_points[1];
            ++success;
        } else if (state == 2) {
            ++many;
        }
    }

#ifdef DEBUG
    printf("ball success : %d\n", success);
#endif

    if (success < 2) {
        return many < 2 ? 0 : mission_6_4_find_ball_interpolation(image, top, bot, left, right, 1);
    } else if (success < 4) {
        ball_points[0] = average(x, success);
        ball_points[1] = average(y, success);
        return 1;
    } else if (success < 6) {
        ball_points[0] = analysis(x, success);
        ball_points[1] = analysis(y, success);
        return 1;
    }

    return 0;
}

int mission_6_4_find_ball(U16 *image, U32 top, U32 bot, U32 left, U32 right, U8 last) {
    int state;
    state = mission_6_4_find_ball_row(image, top, bot, left, right, last);
    if (state == 1) {
        return mission_6_4_find_ball_col(image, ball_top, ball_bottom, left, right, last);
    }

    return state;
}

int mission_6_4_find_ball_row(U16 *image, U32 top, U32 bot, U32 left, U32 right, U8 last) {
    U32 row, col, ball_cnt, pos = 0;
    U8 ball[4] = {0, 0, 0, 0};

    //ball_top 찾기
    for (row = top; row < MIN(bot + 1, HEIGHT); ++row) {
        ball_cnt = 0;
        for (col = left; col < MIN(right + 1, WIDTH); ++col) {
            ball_cnt += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), RED) ||
                         GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), ORANGE)) &&
                        !GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW);
        }
        ball[pos] = ball_cnt > MISSION_6_4_THRES;
        if (ball[0] + ball[1] + ball[2] + ball[3] > 2) {
            ball_top = row - 2;

            pos = (pos + 1) % 4;
            break;
        }
        pos = (pos + 1) % 4;
    }


    //ball_bottom 찾기
    for (++row; row < MIN(bot + 1, HEIGHT); ++row) {
        ball_cnt = 0;
        for (col = left; col < MIN(right + 1, WIDTH); ++col) {
            ball_cnt += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), RED) ||
                         GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), ORANGE)) &&
                        !GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW);
        }
        ball[pos] = ball_cnt > MISSION_6_4_THRES;
        if (ball[0] + ball[1] + ball[2] + ball[3] < 2) {
            ball_bottom = row - 3;
            pos = (pos + 1) % 4;
            break;
        }
        pos = (pos + 1) % 4;
    }


    if (ball[0] + ball[1] + ball[2] + ball[3] > 2) {
        ball_bottom = --row;
    }

    if (row > MIN(bot, HEIGHT - 1)) {
#ifdef DEBUG
        printf("there are no object\n");
#endif
        return 0;
    }

    for (++row; row < MIN(bot + 1, HEIGHT); ++row) {
        ball_cnt = 0;
        for (col = left; col < MIN(right + 1, WIDTH); ++col) {
            ball_cnt += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), RED) ||
                         GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), ORANGE)) &&
                        !GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW);
        }
        ball[pos] = ball_cnt > MISSION_6_4_THRES;
        if (ball[0] + ball[1] + ball[2] + ball[3] > 2) {
            if (row - ball_bottom > 7) {
                if (last) {
                    return mission_6_4_find_ball_row(image, row - 2, bot, left, right, 1);
                } else {
#ifdef DEBUG
                    printf("there are many object\n");
#endif
                    return 2;
                }
            }

            ball_bottom = row - 1;
        }
        pos = (pos + 1) % 4;
    }

    ball_points[1] = (ball_top + ball_bottom) / 2;

    return 1;
}

int mission_6_4_find_ball_col(U16 *image, U32 top, U32 bot, U32 left, U32 right, U8 last) {
    U32 row, col, ball_cnt, pos = 0;
    U8 ball[4] = {0, 0, 0, 0};
    U32 ball_right = 0, ball_left = 0;

    //ball_left 찾기
    for (col = left; col < MIN(right + 1, WIDTH); ++col) {
        ball_cnt = 0;
        for (row = top; row < MIN(bot + 1, HEIGHT); ++row) {
            ball_cnt += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), RED) ||
                         GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), ORANGE)) &&
                        !GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW);
        }
        ball[pos] = ball_cnt > MISSION_6_4_THRES;
        if (ball[0] + ball[1] + ball[2] + ball[3] > 2) {
            ball_left = col - 2;
            pos = (pos + 1) % 4;
            break;
        }
        pos = (pos + 1) % 4;
    }

    //ball_right 찾기
    for (++col; col < MIN(right + 1, WIDTH); ++col) {
        ball_cnt = 0;
        for (row = top; row < MIN(bot + 1, HEIGHT); ++row) {
            ball_cnt += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), RED) ||
                         GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), ORANGE)) &&
                        !GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW);
        }
        ball[pos] = ball_cnt > MISSION_6_4_THRES;
        if (ball[0] + ball[1] + ball[2] + ball[3] < 2) {
            ball_right = col - 3;
            pos = (pos + 1) % 4;
            break;
        }
        pos = (pos + 1) % 4;
    }

    if (ball[0] + ball[1] + ball[2] + ball[3] > 2) {
        ball_right = --col;
    }

    if (col > MIN(right, WIDTH - 1)) {
#ifdef DEBUG
        printf("there are no object\n");
#endif
        return 0;
    }

    for (++col; col < MIN(right + 1, WIDTH); ++col) {
        ball_cnt = 0;
        for (row = top; row < MIN(bot + 1, HEIGHT); ++row) {
            ball_cnt += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), RED) ||
                         GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), ORANGE)) &&
                        !GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW);
        }
        ball[pos] = ball_cnt > MISSION_6_4_THRES;
        if (ball[0] + ball[1] + ball[2] + ball[3] > 2) {
            if (col - ball_right > 7) {
                if (last) {
                    mission_6_4_find_ball_col(image, top, bot, col - 2, right, 1);
                } else {
#ifdef DEBUG
                    printf("there are many object\n");
#endif
                    return 2;
                }
            }

            ball_right = col - 1;
        }
        pos = (pos + 1) % 4;
    }

    ball_points[0] = (ball_left + ball_right) / 2;

    return 1;
}

int mission_6_4_set_center_of_ball(U16 *image) {
    static int need_stable = 0;
    if (mission_6_4_find_ball_interpolation(image, 0, HEIGHT - 15, 0, WIDTH, 0) == 0) {
        need_stable = 1;
        BALL_BIT(FRONT, 5);
        return 0;
    }

    if (ball_points[1] < 20) {
        BALL_BIT(FRONT, 2);
    } else if (ball_points[1] < 25) {
        BALL_BIT(FRONT, 1);
    } else if (ball_points[1] > 35) {
        BALL_BIT(BACK, 1);
    } else if (ball_points[0] < MISSION_6_4_CENTER - 18) {
        BALL_MOVE_LONG(DIR_LEFT, DOWN, 1);
    } else if (ball_points[0] < MISSION_6_4_CENTER - 5) {
        BALL_MOVE(DIR_LEFT, DOWN, 2);
    } else if (ball_points[0] < MISSION_6_4_CENTER - 3) {
        BALL_MOVE(DIR_LEFT, DOWN, 1);
        BALL_STABLE(DOWN);
        need_stable = 0;
        return 0;
    } else if (ball_points[0] > MISSION_6_4_CENTER + 18) {
        BALL_MOVE_LONG(DIR_RIGHT, DOWN, 1);
    } else if (ball_points[0] > MISSION_6_4_CENTER + 5) {
        BALL_MOVE(DIR_RIGHT, DOWN, 2);
    } else if (ball_points[0] > MISSION_6_4_CENTER + 3) {
        BALL_MOVE(DIR_RIGHT, DOWN, 1);
        BALL_STABLE(DOWN);
        need_stable = 0;
        return 0;
    } else {
        if (need_stable)
        {
            BALL_STABLE(DOWN);
        }
        need_stable = 0;
        return 1;
    }

    need_stable = 1;
    RobotSleep(2);
    return 0;
}

int mission_6_5_kick_ball(void) {
    BALL_TURN(DIR_LEFT, MIDDLE, 1);
    BALL_KICK();
    RobotSleep(1);
    ACTION_TURN(LONG, DIR_LEFT, UP, 4);
    RobotSleep(1);
    return 1;
}

int mission_6_9_walk_front(void) {
    ACTION_WALK(FAST, OBLIQUE, 5);
    return 1;
}
