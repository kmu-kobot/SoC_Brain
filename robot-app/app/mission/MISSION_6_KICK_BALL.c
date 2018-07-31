//
// Created by Gihyeon Yang on 2018. 7. 13..
//

#include "MISSION_6_KICK_BALL.h"
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MIN(a, b) (a > b ? b : a)
#define MAX(a, b) (a < b ? b : a)

U32 ball_points[2] = {0,};
U32 hole_points[2] = {0,};

int average(int *arr, int size) {
    int sum = 0;
    int i;

    for (i = 0; i < size; ++i) {
        sum += arr[i];
    }

    return sum / size;
}

int analysis(int *arr, int size) {
    int min_value = INT_MAX, max_value = INT_MIN;
    int i;
    int sum = 0;

    for (i = 0; i < size; ++i) {
        sum += arr[i];
        if (arr[i] < min_value) {
            min_value = arr[i];
        }
        if (arr[i] > max_value) {
            max_value = arr[i];
        }
    }

    return (sum - min_value - max_value) / (size - 2);
}

void mission_6_1_watch_front(void) {
    CHECK_INIT(MIDDLE, OBLIQUE);
    RobotSleep(2);
}

int mission_6_1_detection_ball(U16 *image) {
    hole_points[0] = 90;

    if (mission_6_3_find_hole_interpolation(image) == 1) {
        double degree = atan2(hole_points[0] - ball_points[0], hole_points[1] - ball_points[1]);
        DIRECTION turn_dir = degree < 0;

        if (abs(WIDTH / 2 - ball_points[0] < 20) && abs(degree) > 20.0 * M_PI / 180.0)
        {
            ACTION_TURN(LONG, turn_dir, MIDDLE, OBLIQUE, (int)(abs(degree) * 180.0 / 20.0 / M_PI));
            RobotSleep(2);
            return 0;
        }
    }

    int ball_state = mission_6_4_find_ball_interpolation(image, 0);

    if (ball_state == 0) {
        return 1;
    }
    else if (ball_state == 2) {
        return 0;
    }


    // 3        2           1           0
    // 0 25     25 55       55 85       85 95
    // R        LEFT     RIGHT
    // 3:       (0, 25)  (155, 180)
    // 2:       (25, 55) (125, 155)
    // 1:       (55, 85) (155, 180)
    // FRONT:   (85, 95)

    // LEFT or RIGHT
    DIRECTION move_dir = ball_points[0] > WIDTH / 2;

    if (ball_points[0] < 70 || ball_points[0] > 110) {
        ACTION_MOVE(LONG, move_dir, MIDDLE, OBLIQUE, abs(WIDTH / 2 - ball_points[0]) / 15);
    }
    else if (ball_points[1] <= 60) {
        ACTION_WALK(FAST, OBLIQUE, 4);
    }
    else {
        ACTION_WALK(FAST, OBLIQUE, 3);
    }
    RobotSleep(2);

    return 0;
}

void mission_6_2_watch_below(void) {
    CHECK_INIT(MIDDLE, DOWN);
    RobotSleep(2);
}

int mission_6_2_set_center_of_ball(U16 *image) {

    return mission_6_4_set_front_of_ball(image);
}

void mission_6_2_watch_right(void) {
    CHECK_INIT(MIDDLE, RIGHT);
    RobotSleep(2);
}

int mission_6_2_set_straight_black_line(U16 *image, int mode) {

    U32 row, i;
    U16 col[2] = {
            MISSION_6_2_BLACK_LINE_COL_POINT_1,
            MISSION_6_2_BLACK_LINE_COL_POINT_2
    };
    int black_len[2] = {0,};

    for (i = 0; i < 2; ++i) {
        for (row = HEIGHT- 20; row > 0; --row) {
            if (GetValueRGBYOBK(GetPtr(image, row, col[i] - 1, WIDTH), BLACK) &&
                GetValueRGBYOBK(GetPtr(image, row, col[i], WIDTH), BLACK) &&
                GetValueRGBYOBK(GetPtr(image, row, col[i] + 1, WIDTH), BLACK)) {
                black_len[i] = HEIGHT - row;
                break;
            }
        }
    }

    printf("M6-2: SLOPE\n");
    printf("black[0]: %d, black_len[1]: %d.\n", black_len[0], black_len[1]);

    double degree = atan2(black_len[1] - black_len[0], col[1] - col[0]) * 180.0 / M_PI;
    DIRECTION dir = degree < 0;

    printf("degree : %f, dir : %d\n", degree, dir);

    if (abs(degree) > 10.0) {
        printf("degree > 10.0\n");
        ACTION_TURN(LONG, dir, MIDDLE, mode ? RIGHT : LEFT, (int)(abs(degree) / 10.0));
    }
    else {
        printf("degree <= 10.0\n");
        return 1;
    }

    RobotSleep(2);

    return 0;
}

void mission_6_4_turn_to_detect_hole(void) {
    ACTION_MOVE(LONG, DIR_LEFT, MIDDLE, RIGHT, hole_points[0] / 50 + 1);
    ACTION_TURN(SHORT, DIR_RIGHT, MIDDLE, RIGHT, hole_points[0] / 7 + 3);
}

void mission_6_4_turn_left(void) {
    ACTION_MOVE(LONG, DIR_RIGHT, MIDDLE, OBLIQUE, 1);
    ACTION_TURN(SHORT, DIR_LEFT, MIDDLE, OBLIQUE, 5);
}

int mission_6_3_find_hole_interpolation(U16 *image) {
    int cnt = 0, success = 0, many = 0;
    int state;
    int x[5], y[5];

    for (; cnt < 10 && success < 5; ++cnt)
    {
        setFPGAVideoData(image);
        state = mission_6_3_find_hole(image);
        if (state == 1) {
            x[success] = hole_points[0];
            y[success] = hole_points[1];
            ++success;
        } else if (state == 2) {
            ++many;
        }
    }

    printf("hole success : %d\n", success);

    if (success < 2) {
        return many < 2 ? 0 : 2;
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

int mission_6_3_find_hole(U16 *image) {
    U32 row, col, hole_cnt, pos;
    U8 hole[4] = {0, 0, 0, 0};
    U8 hole_right = 0, hole_left = 0, hole_top = 0, hole_bottom = 0;
    U8 error_hole_left = 0, error_hole_top = 0;

    pos = 0;
    //hole_left 찾기
    for (col = 0; col < WIDTH; ++col) {
        hole_cnt = 0;
        for (row = 0; row < HEIGHT; ++row) {
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
    for (++col; col < WIDTH; ++col) {
        hole_cnt = 0;
        for (row = 0; row < HEIGHT; ++row) {
            hole_cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
        }
        hole[pos] = hole_cnt > MISSION_6_3_THRES;
        if (hole[0] + hole[1] + hole[2] + hole[3] < 2) {
            hole_right = col - 3;
            pos = (pos + 1);
            break;
        }
        pos = (pos + 1) % 4;
    }

    if (hole[0] + hole[1] + hole[2] + hole[3] > 2) {
        hole_right = --col;
    }


    if (col > WIDTH - 1) {
        printf("there are no object\n");
        return 0;
    }

    for (++col; col < WIDTH; ++col)
    {
       hole_cnt = 0;
        for (row = 0; row < HEIGHT; ++row) {
            hole_cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
        }
        hole[pos] = hole_cnt > MISSION_6_3_THRES;
        if (hole[0] + hole[1] + hole[2] + hole[3] > 2) {
            error_hole_left = col - 2;
            pos = (pos + 1) % 4;
            break;
        }
        pos = (pos + 1) % 4;
    }

    hole[0] = hole[1] = hole[2] = hole[3] = 0;
    pos = 0;

    //hole_top 찾기
    for (row = 0; row < HEIGHT; ++row) {
        hole_cnt = 0;
        for (col = MAX(hole_left - 5, 0); col < MIN(hole_right + 6 , WIDTH); ++col) {
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
    for (++row; row < HEIGHT; ++row) {
        hole_cnt = 0;
        for (col = MAX(hole_left - 5, 0); col < MIN(hole_right + 6 , WIDTH); ++col) {
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

    if (row > HEIGHT - 1) {
        printf("there are no object\n");
        return 0;
    }

    for (++row; row < HEIGHT; ++row) {
        hole_cnt = 0;
        for (col = MAX(hole_left - 5, 0); col < MIN(hole_right + 6 , WIDTH); ++col) {
            hole_cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
        }
        hole[pos] = hole_cnt > MISSION_6_3_THRES;
        if (hole[0] + hole[1] + hole[2] + hole[3] > 2) {
            error_hole_top = row - 2;
            pos = (pos + 1) % 4;
            break;
        }
        pos = (pos + 1) % 4;
    }

    hole_points[0] = (int) ((int) hole_left + (int) hole_right) / 2;
    hole_points[1] = (int) ((int) hole_top + (int) hole_bottom) / 2;
    int hole_size = hole_right - hole_left;
    printf("hole_points : %d, %d\n", hole_points[0], hole_points[1]);
    

    return 1;
}


int mission_6_3_set_straight_hole(U16 *image) {

    mission_6_3_find_hole_interpolation(image);

    double degree = atan2(hole_points[0] - ball_points[0], hole_points[1] - HEIGHT);
    DIRECTION dir = degree > 0;

    printf("degree : %f\n", degree);

    if (abs(degree) > 10.0 * M_PI / 180.0) {
        ACTION_MOVE(LONG, dir, MIDDLE, OBLIQUE, (int)(abs(degree) * 180.0 / 10.0 / M_PI));
        return 0;
    }

    return 1;
}


int mission_6_3_locate_hole_on_center(U16 *image) {

    if(mission_6_3_find_hole_interpolation(image) != 1) {
        return 0;
    }

    if (hole_points[0] < MISSION_6_3_CENTER - 60) {
        ACTION_MOVE(SHORT, DIR_RIGHT, MIDDLE, OBLIQUE, 2);
        ACTION_TURN(SHORT, DIR_LEFT, MIDDLE, OBLIQUE, 3);
    } else if (hole_points[0] < MISSION_6_3_CENTER - 30) {
        ACTION_MOVE(SHORT, DIR_RIGHT, MIDDLE, OBLIQUE, 1);
        ACTION_TURN(SHORT, DIR_LEFT, MIDDLE, OBLIQUE, 3);
    } else if (hole_points[0] < MISSION_6_3_CENTER - 15) {
        ACTION_TURN(SHORT, DIR_LEFT, MIDDLE, OBLIQUE, 2);
    } else if (hole_points[0] < MISSION_6_3_CENTER - 8) {
        ACTION_TURN(SHORT, DIR_LEFT, MIDDLE, OBLIQUE, 1);
    } else if (hole_points[0] < MISSION_6_3_CENTER - 3) {
        ACTION_TURN(SHORT, DIR_LEFT, MIDDLE, OBLIQUE, 1);
        RobotSleep(5);
    } else if (hole_points[0] > MISSION_6_3_CENTER + 60) {
        ACTION_MOVE(SHORT, DIR_LEFT, MIDDLE, OBLIQUE, 2);
        ACTION_TURN(SHORT, DIR_RIGHT, MIDDLE, OBLIQUE, 3);
    } else if (hole_points[0] > MISSION_6_3_CENTER + 30) {
        ACTION_MOVE(SHORT, DIR_LEFT, MIDDLE, OBLIQUE, 1);
        ACTION_TURN(SHORT, DIR_RIGHT, MIDDLE, OBLIQUE, 3);
    } else if (hole_points[0] > MISSION_6_3_CENTER + 15) {
        ACTION_TURN(SHORT, DIR_RIGHT, MIDDLE, OBLIQUE, 2);
    } else if (hole_points[0] > MISSION_6_3_CENTER + 8) {
        ACTION_TURN(SHORT, DIR_RIGHT, MIDDLE, OBLIQUE, 1);
    } else if (hole_points[0] > MISSION_6_3_CENTER + 3) {
        ACTION_TURN(SHORT, DIR_RIGHT, MIDDLE, OBLIQUE, 1);
        RobotSleep(5);
    } else {
        return 1;
    }
    
    RobotSleep(5);

    return 0;
}

int mission_6_4_find_ball_interpolation(U16 *image, int limit) {
    int cnt = 0, success = 0, many = 0;
    int state;
    int x[5], y[5];

    for (; cnt < 10 && success < 5; ++cnt)
    {
        setFPGAVideoData(image);
        state = mission_6_4_find_ball(image, limit);
        if (state == 1) {
            x[success] = ball_points[0];
            y[success] = ball_points[1];
            ++success;
        }
        else if (state == 2) {
            ++many;
        }
    }

    printf("ball success : %d\n", success);

    if (success < 2) {
        return many < 2 ? 0 : 2;
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

int mission_6_4_find_ball(U16 *image, int limit) {
    U32 row, col, ball_cnt, pos = 0;
    U8 ball[4] = {0, 0, 0, 0};
    U8 ball_top = 0, ball_bottom = 0, ball_right = 0, ball_left = 0;

    //ball_left 찾기
    for (col = 0; col < WIDTH; ++col) {
        ball_cnt = 0;
        for (row = 0; row < HEIGHT - limit; ++row) {
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
    for (++col; col < WIDTH; ++col) {
        ball_cnt = 0;
        for (row = 0; row < HEIGHT - limit; ++row) {
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


    if (col > WIDTH - 1) {
        printf("there are no object\n");
        return 0;
    }

    for (++col; col < WIDTH; ++col) {
        ball_cnt = 0;
        for (row = 0; row < HEIGHT - limit; ++row) {
            ball_cnt += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), RED) ||
                        GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), ORANGE)) &&
                        !GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW);
        }
        ball[pos] = ball_cnt > MISSION_6_4_THRES;
        if (ball[0] + ball[1] + ball[2] + ball[3] > 2) {
            if (col - ball_right > 7) {
                printf("there are many object\n");
                return 2;
            }

            ball_right = col - 1;
        }
        pos = (pos + 1) % 4;
    }

    ball[0] = ball[1] = ball[2] = ball[3] = 0;
    pos = 0;

    //ball_top 찾기
    for (row = 0; row < HEIGHT - limit; ++row) {
        ball_cnt = 0;
        for (col = MAX(ball_left - 5, 0); col < MIN(ball_right + 6 , WIDTH); ++col) {
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
    for (++row; row < HEIGHT - limit; ++row) {
        ball_cnt = 0;
        for (col = MAX(ball_left - 5, 0); col < MIN(ball_right + 6 , WIDTH); ++col) {
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
        ball_bottom = --col;
    }


    if (row > HEIGHT - 1) {
        printf("there are no object\n");
        return 0;
    }

    for (++row; row < HEIGHT - limit; ++row) {
        ball_cnt = 0;
        for (col = MAX(ball_left - 5, 0); col < MIN(ball_right + 6 , WIDTH); ++col) {
            ball_cnt += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), RED) ||
                        GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), ORANGE)) &&
                        !GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW);
        }
        ball[pos] = ball_cnt > MISSION_6_4_THRES;
        if (ball[0] + ball[1] + ball[2] + ball[3] > 2) {
            if (row - ball_bottom > 7) {
                printf("there are many object\n");
                return 2;
            }

            ball_bottom = row - 1;
        }
        pos = (pos + 1) % 4;
    }

    ball_points[0] = (U32) ((int) ball_left + (int) ball_right) / 2;
    ball_points[1] = (U32) ((int) ball_top + (int) ball_bottom) / 2;
    printf("ball_points : %d, %d\n", ball_points[0], ball_points[1]);

    return 1;
}

int mission_6_4_set_front_of_ball(U16 *image) {

    if(mission_6_4_find_ball_interpolation(image, 10) == 0) {
        ACTION_WALK(FAST, DOWN, 2);
        RobotSleep(2);
        return 0;
    }

    if (ball_points[1] < 25) {
        ACTION_WALK(CLOSE, DOWN, 2);
    }
    else if (ball_points[1] > 35) {
       ACTION_BIT(BACK, 1);
    }
    else if (ball_points[0] < MISSION_6_4_CENTER - 45 ) {
        ACTION_MOVE(LONG, DIR_LEFT, MIDDLE, DOWN, 2);
    }
    else if (ball_points[0] < MISSION_6_4_CENTER - 15) {
        ACTION_MOVE(LONG, DIR_LEFT, MIDDLE, DOWN, 1);
    }
    else if (ball_points[0] < MISSION_6_4_CENTER - 8) {
        ACTION_MOVE(SHORT, DIR_LEFT, MIDDLE, DOWN, 2);
        RobotSleep(2);
    }
    else if (ball_points[0] < MISSION_6_4_CENTER - 2) {
        ACTION_MOVE(SHORT, DIR_LEFT, MIDDLE, DOWN, 1);
        RobotSleep(2);
    }
    else if (ball_points[0] > MISSION_6_4_CENTER + 45) {
        ACTION_MOVE(LONG, DIR_RIGHT, MIDDLE, DOWN, 2);
    }
    else if (ball_points[0] > MISSION_6_4_CENTER + 15) {
        ACTION_MOVE(LONG, DIR_RIGHT, MIDDLE, DOWN, 1);
    }
    else if (ball_points[0] > MISSION_6_4_CENTER + 8) {
        ACTION_MOVE(SHORT, DIR_RIGHT, MIDDLE, DOWN, 2);
        RobotSleep(2);
    }
    else if (ball_points[0] > MISSION_6_4_CENTER + 2) {
        ACTION_MOVE(SHORT, DIR_RIGHT, MIDDLE, DOWN, 1);
        RobotSleep(2);
    }
    else {
        return 1;
    }

    RobotSleep(2);

    return 0;
}

int countColor(U16 *image, int row, int col, int range, int color) {
    int cnt = 0, x, y;
    // 0: RED, ORANGE 1: BLUE

    for (x = col - range; x < col + range + 1; ++x) {
        for (y = row - range; y < row + range + 1; ++y) {
            cnt += (color) ?
                   GetValueRGBYOBK(GetPtr(image, y, x, WIDTH), BLUE) :
                   GetValueRGBYOBK(GetPtr(image, y, x, WIDTH), RED) ||
                   GetValueRGBYOBK(GetPtr(image, y, x, WIDTH), ORANGE);

            if (cnt > 180 || cnt < 0) {
                continue;
            }
        }

    }
    return cnt;
}

int mission_6_5_kick_ball(void) {
    RobotSleep(1);
    //ACTION_TURN();
    ACTION_MOTION(MISSION_6_RIGHT_KICK, MIDDLE, OBLIQUE);
    ACTION_TURN(FAST, DIR_LEFT, MIDDLE, OBLIQUE, 5);
    RobotSleep(1);
    return 1;
}

void mission_6_6_watch_side(void) {
    CHECK_INIT(MIDDLE, LEFT);
    RobotSleep(2);
}

int mission_6_9_set_front_of_not_bk(U16 *image) {
    U32 col[3] = {85, 95, 90}, row, i;
    int checkHurdleLine[3] = {0,};

    for (i = 0; i < 3; ++i) {
        for (row = HEIGHT - 1; row >= 5; --row) {
            if (GetValueRGBYOBK(GetPtr(image, row, col[i], WIDTH), BLACK) &&
                GetValueRGBYOBK(GetPtr(image, row, col[i] + 1, WIDTH), BLACK)) {
                checkHurdleLine[i] = (HEIGHT - row);
                break;
            }
        }
        if (row < 5) {
            checkHurdleLine[i] = HEIGHT - 5;
        }
    }

    double s = 0;
    printf("\nM6-5: BLACK LINE\n");
    for (i = 0; i < 3; ++i) {
        s += checkHurdleLine[i];
        printf("bk_line[%d]: %d,\t", i, checkHurdleLine[i]);
    }
    printf("\n");

    s /= 3;
    printf("AVG: %f\n\n", s);

    printf((s > CASE_6_9_NON_BLACK_LINE) ? "SUCCESS\n" : "FAIL\n");

    if (s > CASE_6_9_NON_BLACK_LINE) {
        ACTION_TURN(LONG, DIR_LEFT, MIDDLE, OBLIQUE, 1);
        return 1;
    } else {
        ACTION_TURN(LONG, DIR_LEFT, MIDDLE, OBLIQUE, 2);
        return 0;
    }
}

int mission_6_6_set_center_black_line(U16 *image, int mode) {
   U32 col[3] = {85, 95, 90}, row, i, j;
    U16 checkHurdleLine[3] = {0,};

    for (i = 0; i < 3; ++i) {
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
    printf("M6-6: BLACK LINE\n");
    for (i = 0; i < 3; ++i) {
        if (s < checkHurdleLine[i]) {
            s = checkHurdleLine[i];
        }
        printf("bk_line[%d]: %d,\t", i, checkHurdleLine[i]);
    }
    printf("\n");

    printf("M6-6: AVG: %f\n", s);

    if (s < DEFAULT_RIGHT_RANGE - ((mode == 3) ? 3 : DEFAULT_RANGE_ERROR)) {
        ACTION_MOVE(LONG, DIR_LEFT, MIDDLE, RIGHT, 1);
        if (mode == 1) {
            ACTION_WALK(CLOSE, RIGHT, 2);
        }
        return 0;
    } else if (s > DEFAULT_RIGHT_RANGE + ((mode == 3) ? 3 : DEFAULT_RANGE_ERROR)) {
        ACTION_MOVE(LONG, DIR_RIGHT, MIDDLE, RIGHT, 1);
        if (mode == 1) {
            ACTION_WALK(CLOSE, RIGHT, 2);
        }
        return 0;
    } else {
        if (mode == 1 || mode == -1 || mode == 3) {
            ACTION_WALK(CLOSE, RIGHT, 3);
        }
        printf("SUCCESS\n\n");
        return 1;
    }

}

int mission_6_9_walk_front(void) {
    ACTION_WALK(FAST, OBLIQUE, 5);
    RobotSleep(2);
    return 1;
}

void mission_test(U16 *image) {

    U32 col, row, isFound, i, sum = 0, ball_points[2] = {0,};
    int selectCnt[2] = {0,};

    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            isFound = 1;
            for (i = 0; i < 2; ++i) {
                isFound &= GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), RED) ||
                           GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), ORANGE);
            }


            if (isFound) {
                selectCnt[1] = countColor(image, row, col, 3, 0);
                if (selectCnt[0] < selectCnt[1]) {
                    selectCnt[0] = selectCnt[1];
                    ball_points[0] = col;  // LEFT TOP x
                    ball_points[1] = row;  // LEFT TOP y
                }
            }
        }
    }

    printf("\n\nball_point[0] is %d, ball_point[1] is %d\n\n", ball_points[0], ball_points[1]);
    char a;
    a = getchar();

    if (a == 'a' || a == 'A') {
        ACTION_MOVE(SHORT, DIR_LEFT, MIDDLE, LEFT, 1);
    } else if (a == 'd' || a == 'D') {
        ACTION_MOVE(SHORT, DIR_RIGHT, MIDDLE, LEFT, 1);
    } else if (a == 'w' || a == 'W') {
        ACTION_TURN(SHORT, DIR_LEFT, MIDDLE, LEFT, 1);
    } else if (a == 's' || a == 'S') {
        ACTION_TURN(SHORT, DIR_RIGHT, MIDDLE, LEFT, 1);
    }

}