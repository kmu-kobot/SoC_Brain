//
// Created by Gihyeon Yang on 2018. 7. 13..
//

#include "MISSION_6_KICK_BALL.h"

U32 ball_points[2] = {0,};
U32 hole_points[2] = {0,};

void mission_6_1_watch_front(void) {
    ACTION_INIT(MIDDLE, OBLIQUE);
    RobotSleep(2);
}

int mission_6_1_detection_ball(U16 *image) {

    mission_6_3_find_hole(image);

    U32 col, row, sum = 0;
    int cnt[2] = {0,};

    for (row = 10; row < HEIGHT - 10; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            if (
                    GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), ORANGE) ||
                    GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), RED)
                    ) {
                sum++;
                cnt[0] = countColor(image, row, col, 4, 0);
                if (cnt[0] > cnt[1]) {
                    cnt[1] = cnt[0];
                    ball_points[0] = col;
                    ball_points[1] = row;
                }
            }
        }
    }


    printf("pixel number is %d, Col is %d, Row is %d\n\n", cnt[1], ball_points[0], ball_points[1]);

    printf("sum is %d\n\n", sum);
    if (sum < 20) {
        return 1;
    }


    // 3        2           1           0
    // 0 25     25 55       55 85       85 95
    // R        LEFT     RIGHT
    // 3:       (0, 25)  (155, 180)
    // 2:       (25, 55) (125, 155)
    // 1:       (55, 85) (155, 180)
    // FRONT:   (85, 95)

    // LEFT or RIGHT
    int range;
    if (ball_points[0] < 80 || ball_points[0] > 100) {
        cnt[1] = (U32) (ball_points[0] < 80);
        ball_points[0] = ((ball_points[0] < 80) ? ball_points[0] : WIDTH - ball_points[0]);
        range = (0 <= ball_points[0] && ball_points[0] < 30) ? 4 :
                (30 <= ball_points[0] && ball_points[0] < 50) ? 3 :
                (50 <= ball_points[0] && ball_points[0] < 80) ? 1 : 0;

        if (range != 0) {
            if (cnt[1]) {
                ACTION_MOVE(LONG, DIR_LEFT, MIDDLE, OBLIQUE, range);
                RobotSleep(2);
            } else {
                ACTION_MOVE(LONG, DIR_RIGHT, MIDDLE, OBLIQUE, range);
                RobotSleep(2);
            }
        }
    } else {
        if (ball_points[1] <= 60) {
            ACTION_WALK(FAST, OBLIQUE, 4);
            RobotSleep(2);
        } else {
            ACTION_WALK(FAST, OBLIQUE, 2);
            RobotSleep(2);
        }

    }

    return 0;
}

void mission_6_2_watch_below(void) {
    ACTION_INIT(MIDDLE, DOWN);
    RobotSleep(2);
}

int mission_6_2_set_center_of_ball(U16 *image) {
    U32 col, row, isFound, i, sum = 0;
    int selectCnt[2] = {0,};

    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            isFound = 1;
            for (i = 0; i < 2; ++i) {
                isFound &= GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), RED) ||
                           GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), ORANGE);
            }


            if (isFound) {
                sum++;
                selectCnt[1] = countColor(image, row, col, 5, 0);
                if (selectCnt[0] < selectCnt[1]) {
                    selectCnt[0] = selectCnt[1];
                    ball_points[0] = col;  // LEFT TOP x
                    ball_points[1] = row;  // LEFT TOP y
                }
            }
        }
    }


    printf("sum is %d\n\n", sum);
    printf("BALL\n\n");
    printf("ball(%d, %d), ball pixel number: %d\n\n", ball_points[0], ball_points[1], selectCnt[0]);

    if (sum < 5) {
        ACTION_WALK(SLOW, DOWN, 2);
        RobotSleep(2);
        return 0;
    }

    // return 0;
    selectCnt[0] = 85 <= ball_points[0] && ball_points[0] < 95;

    // TODO: 높이는 적당히 바꿔주기
    selectCnt[1] = ball_points[1] > 20;

    int rResult = 0;
    if (!selectCnt[0]) {
        if (80 > ball_points[0]) {
            ACTION_MOVE(SHORT, DIR_LEFT, MIDDLE, DOWN, 2);
            RobotSleep(2);
        } else {
            ACTION_MOVE(SHORT, DIR_RIGHT, MIDDLE, DOWN, 2);
            RobotSleep(2);

        }
    } else {
        if (!selectCnt[1]) {
            ACTION_WALK(CLOSE, DOWN, 2);
            RobotSleep(2);
        } else {
            rResult = 1;
            ACTION_INIT(MIDDLE, OBLIQUE);
        }
    }

    return rResult;
}

void mission_6_2_watch_right(void) {
    ACTION_INIT(MIDDLE, RIGHT);
    RobotSleep(2);

}

int mission_6_2_set_straight_black_line(U16 *image) {

    U32 row, i;
    U16 col[2] = {
            MISSION_6_2_BLACK_LINE_COL_POINT_1,
            MISSION_6_2_BLACK_LINE_COL_POINT_2
    };
    int black_len[2] = {0,};

    for (i = 0; i < 2; ++i) {
        for (row = HEIGHT; row > 0; --row) {
            if (GetValueRGBYOBK(GetPtr(image, row, col[i], WIDTH), BLACK) &&
                GetValueRGBYOBK(GetPtr(image, row, col[i] + 1, WIDTH), BLACK)) {
                black_len[i] = -row;
                break;
            }
        }
    }

    printf("M6-2: SLOPE\n");
    printf("black[0]: %d, black_len[1]: %d.\n", black_len[0], black_len[1]);

    double s = (
            (double) (black_len[0] - black_len[1]) /
            (MISSION_6_2_BLACK_LINE_COL_POINT_1 - MISSION_6_2_BLACK_LINE_COL_POINT_2)
    );

    printf("Slope : %f\n", s);

    s *= 100;
    int rResult = 1;
    if (((s > 0) ? s : -s) > MISSION_6_2_BLACK_LINE_SLOPE) {
        rResult = 0;
        if (s < 0) {
            ACTION_TURN(SHORT, DIR_LEFT, MIDDLE, RIGHT, (-s > 13) ? 3 : 1);
            RobotSleep(2);
        } else if (s > 0) {
            ACTION_TURN(SHORT, DIR_RIGHT, MIDDLE, RIGHT, (s > 13) ? 3 : 1);
            RobotSleep(2);
        }
        //RobotSleep(5);
    }

    return 1;

}

void mission_6_4_turn_to_detect_hole(void) {

    if (ball_points[0] - hole_points[1] < 0) {
        ACTION_TURN(LONG, DIR_RIGHT, MIDDLE, OBLIQUE, 3);
        RobotSleep(2);
    } else if (ball_points[0] - hole_points[1] > 0) {
        ACTION_TURN(LONG, DIR_LEFT, MIDDLE, OBLIQUE, 3);
        RobotSleep(2);
    }

}

// int mission_6_3_find_hole(U16 *image) {

//     U32 col, row, cnt, sum = 0 , dethole = 0;

//     for (col = 0; col < WIDTH; ++col) {
//         cnt = 0;
//         for (row = 0; row < HEIGHT; ++row) {

//             cnt += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE));
//         }
//         sum += cnt;

//         if (cnt > dethole) {
//             dethole = cnt;
//             hole_points[0] = col;
//         }

//     }

//     dethole = 0;

//     for (row = 0; row < HEIGHT; ++row) {
//         //sum = 0;
//         cnt = 0;
//         for (col = 0; col < WIDTH; ++col) {

//             cnt += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE));
//         }
//         sum += cnt;

//         if (cnt > dethole) {
//             dethole = cnt;
//             hole_points[1] = row;
//         }

//     }

//     printf("HOLE\n\n");
//     printf("hole(%d, %d), hole pixel number: %d\n\n", hole_points[0], hole_points[1], dethole);
//     printf("sum is %d\n\n", sum);

//     return sum > 10;
// }

int mission_6_3_find_hole(U16 *image) {
    U32 row, col, blue_cnt, asd = 0;
    U8 prev[3] = {0, 0, 0};
    U8 hole_right = 0, hole_left = 0;

    //ball_top 찾기
    for (col = 0; col < WIDTH; ++col) {
        blue_cnt = 0;
        for (row = 0; row < HEIGHT; ++row) {
            blue_cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
        }
        if (blue_cnt > MISSION_6_3_THRES && (prev[0] + prev[1] + prev[2] > 2)) {
            hole_left = col - 2;
            break;
        }
        prev[2] = prev[1];
        prev[1] = prev[0];
        prev[0] = blue_cnt > MISSION_6_3_THRES;
    }

    printf("hole_left: %d\n\n", hole_left);
    //ball_bottom 찾기
    for (col = hole_left; col < WIDTH; ++col) {
        blue_cnt = 0;
        for (row = 0; row < HEIGHT; ++row) {
            blue_cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
        }

        if (blue_cnt <= MISSION_6_3_THRES & (prev[0] + prev[1] + prev[2] < 2)) {
            hole_right = col - 2;
            break;
        }
        prev[2] = prev[1];
        prev[1] = prev[0];
        prev[0] = blue_cnt > MISSION_6_3_THRES;
    }

    printf("hole_right: %d\n\n", hole_right);

    if (col > WIDTH - 1) {
        return 0;
    }


    hole_points[0] = (int) ((int) hole_left + (int) hole_right) / 2;
    printf("hole_points[0] : %d\n\n", hole_points[0]);

}


int mission_6_3_set_straight_hole(U16 *image) {

    mission_6_3_find_hole(image);

    int center_to_hole = (int) 90 - hole_points[0];
    unsigned int range = center_to_hole / 5;

    int step_number = 4;
    step_number = (14 <= range && range < 18) ? 3 :
                  (9 <= range && range < 14) ? 2 :
                  (3 <= range && range < 9) ? 1 : 0;

    printf("gap is : %d, range is %d\n\n", center_to_hole, range);

    if ((center_to_hole > 0) ? center_to_hole : -center_to_hole > 15) {
        if (center_to_hole < 0) {
            ACTION_MOVE(LONG, DIR_LEFT, MIDDLE, OBLIQUE, step_number);
            RobotSleep(2);
            //ACTION_TURN(DIR_RIGHT, MIDDLE, OBLIQUE, 2);
        } else if (center_to_hole > 0) {
            ACTION_MOVE(LONG, DIR_RIGHT, MIDDLE, OBLIQUE, step_number);
            RobotSleep(2);
            //ACTION_TURN(DIR_LEFT, MIDDLE, OBLIQUE, 2);
        }
    }

    return 1;
}


int mission_6_3_locate_hole_on_center(U16 *image) {

    mission_6_3_find_hole(image);

    int rResult = 0;
    if (hole_points[0] <= 91) {
        //ACTION_MOVE(SHORT, DIR_RIGHT, MIDDLE, OBLIQUE, 1);
        ACTION_TURN(SHORT, DIR_LEFT, MIDDLE, OBLIQUE, 2);
        RobotSleep(2);
    } else if (hole_points[0] >= 102) {
        //ACTION_MOVE(SHORT, DIR_LEFT, MIDDLE, OBLIQUE, 1);
        ACTION_TURN(SHORT, DIR_RIGHT, MIDDLE, OBLIQUE, 1);
        RobotSleep(2);
    } else {
        rResult = 1;
    }

    return rResult;
}

// int mission_6_4_set_front_of_ball(U16 *image) {

//     U32 col, row, isFound, i;
//     int selectCnt[2] = {0,};

//     for (row = 0; row < HEIGHT; ++row) {
//         for (col = 0; col < WIDTH; ++col) {
//             isFound = 1;
//             for (i = 0; i < 2; ++i) {
//                 isFound &= GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), RED) ||
//                            GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), ORANGE);
//             }


//             if (isFound) {
//                 selectCnt[1] = countColor(image, row, col, 5, 0);
//                 if (selectCnt[0] < selectCnt[1]) {
//                     selectCnt[0] = selectCnt[1];
//                     ball_points[0] = col;  // LEFT TOP x
//                     ball_points[1] = row;  // LEFT TOP y
//                 }
//             }
//         }
//     }

//     printf("ball_col: %d, ball_row: %d\n\n", ball_points[0], ball_points[1]);

//     int scope[2] = {110 <= ball_points[0] && ball_points[0] < 120, ball_points[1] > 20};


//     int rResult = 0;
//     if (!scope[0]) {
//         if(105 > ball_points[0]) {
//             ACTION_MOVE(SHORT, DIR_LEFT, MIDDLE, DOWN, 1);
//             RobotSleep(1);
//         }
//         else {
//             ACTION_MOVE(SHORT, DIR_RIGHT, MIDDLE, DOWN, 1);
//             RobotSleep(1);
//         }
//     } else {
//         if (!scope[1]) {
//             ACTION_WALK(CLOSE, OBLIQUE, 2);
//             RobotSleep(1);
//         }
//         else {
//             rResult = 1;
//         }
//     }

//     return rResult;
// }

int mission_6_4_set_front_of_ball(U16 *image) {
    U32 row, col, ball_cnt, asd = 0;
    U8 prev[3] = {0, 0, 0};
    U8 ball_top = 0, ball_bottom = 0, ball_right = 0, ball_left = 0;

    //ball_left 찾기
    for (col = 0; col < WIDTH; ++col) {
        ball_cnt = 0;
        for (row = 0; row < HEIGHT; ++row) {
            ball_cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), RED) ||
                        GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), ORANGE);
        }
        printf("col : %d, count: %d\n\n", col, ball_cnt);
        if (ball_cnt > MISSION_6_4_THRES && (prev[0] + prev[1] + prev[2] > 2)) {
            ball_left = col - 2;
            break;
        }
        prev[2] = prev[1];
        prev[1] = prev[0];
        prev[0] = ball_cnt > MISSION_6_4_THRES;
    }

    printf("ball_left: %d\n\n", ball_left);
    //ball_right 찾기
    for (col = ball_left; col < WIDTH; ++col) {
        ball_cnt = 0;
        for (row = 0; row < HEIGHT; ++row) {
            ball_cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), RED) ||
                        GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), ORANGE);
        }
        printf("col : %d, count: %d\n\n", col, ball_cnt);


        if (ball_cnt <= MISSION_6_4_THRES & (prev[0] + prev[1] + prev[2] < 2)) {
            ball_right = col - 2;
            break;
        }
        prev[2] = prev[1];
        prev[1] = prev[0];
        prev[0] = ball_cnt > MISSION_6_4_THRES;
    }

    printf("ball_right: %d\n\n", ball_right);

    if (col > WIDTH - 1) {
        return 0;
    }

    //ball_top 찾기
    for (row = 0; row < HEIGHT; ++row) {
        ball_cnt = 0;
        for (col = 0; col < WIDTH; ++col) {
            ball_cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), RED) ||
                        GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), ORANGE);
        }
        printf("row : %d, count: %d\n\n", row, ball_cnt);

        if (ball_cnt > MISSION_6_4_THRES && (prev[0] + prev[1] + prev[2] > 2)) {
            ball_top = row - 2;
            break;
        }
        prev[2] = prev[1];
        prev[1] = prev[0];
        prev[0] = ball_cnt > MISSION_6_4_THRES;
    }

    printf("ball_top: %d\n\n", ball_top);

    //ball_bottom 찾기
    for (row = ball_top; row < HEIGHT; ++row) {
        ball_cnt = 0;
        for (col = 0; col < WIDTH; ++col) {
            ball_cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), RED) ||
                        GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), ORANGE);
        }
        printf("row : %d, count: %d\n\n", row, ball_cnt);

        if (ball_cnt <= MISSION_6_4_THRES & (prev[0] + prev[1] + prev[2] < 2)) {
            ball_bottom = row - 2;
            break;
        }
        prev[2] = prev[1];
        prev[1] = prev[0];
        prev[0] = ball_cnt > MISSION_6_4_THRES;
    }

    printf("ball_bottom: %d\n\n", ball_bottom);

    if (row > HEIGHT - 1) {
        return 0;
    }

    ball_points[0] = (int) ((int) ball_left + (int) ball_right) / 2;
    ball_points[1] = (int) ((int) ball_top + (int) ball_bottom) / 2;
    printf("ball_points[0] : %d\n\n", ball_points[0]);
    printf("ball_points[1] : %d\n\n", ball_points[1]);

    int scope[2] = {110 <= ball_points[0] && ball_points[0] < 120, ball_points[1] > 20};

    int rResult = 0;
    if (!scope[0]) {
        if (105 > ball_points[0]) {
            ACTION_MOVE(SHORT, DIR_LEFT, MIDDLE, DOWN, 1);
            RobotSleep(2);
        } else {
            ACTION_MOVE(SHORT, DIR_RIGHT, MIDDLE, DOWN, 1);
            RobotSleep(2);
        }
    } else {
        if (!scope[1]) {
            ACTION_WALK(CLOSE, OBLIQUE, 2);
            RobotSleep(2);
        } else {
            rResult = 1;
        }
    }

    return rResult;
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
    ACTION_MOTION(MISSION_6_RIGHT_KICK, MIDDLE, OBLIQUE);
    ACTION_WALK(FAST, OBLIQUE, 2);
    ACTION_TURN(LONG, DIR_LEFT, MIDDLE, OBLIQUE, 10);
    ACTION_WALK(FAST, OBLIQUE, 5);
    ACTION_INIT(MIDDLE, OBLIQUE);
    RobotSleep(1);
    return 1;
}

void mission_6_6_watch_side(void) {
    ACTION_INIT(MIDDLE, LEFT);
    RobotSleep(2);
}

int mission_6_6_set_straight_black_line(U16 *image) {
    U32 i, row, col[2] = {MISSION_6_6_COL_POINT_1,
                          MISSION_6_6_COL_POINT_2};
    U16 pos_black[2] = {0,};
    for (i = 0; i < 2; ++i) {
        for (row = HEIGHT; row > 0; --row) {
            if (GetValueRGBYOBK(GetPtr(image, row, col[i], WIDTH), BLACK) == 1 &&
                GetValueRGBYOBK(GetPtr(image, row, col[i] + 1, WIDTH), BLACK) == 1) {
                break;
            }
            pos_black[i] += 1;
        }
    }

    printf("pos_black[0]: %d, pos_black[1]: %d\n", pos_black[0], pos_black[1]);

// s > 0 l
// s < 0 r
    double slope = (double) (pos_black[0] - pos_black[1]);

    printf("Slope is %f\n", slope);

    int rResult = 1;
    if (((slope > 0) ? slope : -slope) > MISSION_6_6_BLUE_GATE_SLOPE) {
        rResult = 0;
        if (slope > 0) {
            ACTION_TURN(SHORT, DIR_LEFT, MIDDLE, LEFT, 3);
            RobotSleep(2);
        } else if (slope < 0) {
            ACTION_TURN(SHORT, DIR_RIGHT, MIDDLE, LEFT, 3);
            RobotSleep(2);
        }
    }

    return rResult;
}

int mission_6_6_set_center_black_line(U16 *image) {
    U32 i, row, col[2] = {MISSION_6_6_COL_POINT_1,
                          MISSION_6_6_COL_POINT_2};
    U16 black_len[2] = {0,};
    for (i = 0; i < 2; ++i) {
        for (row = HEIGHT; row > 0; --row) {
            if (GetValueRGBYOBK(GetPtr(image, row, col[i], WIDTH), BLACK) == 1 &&
                GetValueRGBYOBK(GetPtr(image, row, col[i] + 1, WIDTH), BLACK) == 1) {
                black_len[i] = HEIGHT - row;
                break;
            }
        }
    }

    printf("black_len[0] = %d, black_len[1] = %d\n", black_len[0], black_len[1]);

    double e = (double) (black_len[0] + black_len[1]) / 2;

    printf("distance is %f", e);

    int rResult = 0;
    if (e < MISSION_6_6_BLACK_LENGTH - MISSION_6_6_BLACK_LENGTH_ERROR) {
        //오른쪽 이동
        ACTION_MOVE(SHORT, DIR_RIGHT, MIDDLE, LEFT, 2);
        RobotSleep(2);
    } else if (e > MISSION_6_6_BLACK_LENGTH + MISSION_6_6_BLACK_LENGTH_ERROR) {
        //왼쪽 이동
        ACTION_MOVE(SHORT, DIR_LEFT, MIDDLE, LEFT, 2);
        RobotSleep(2);
    } else {
        rResult = 1;
    }

    return rResult;
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
