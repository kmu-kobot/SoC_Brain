//
// Created by Gihyeon Yang on 2018. 7. 13..
//

#include "MISSION_6_KICK_BALL.h"

void mission_6_1_watch_front(void) {
    ACTION_INIT(LOW, OBLIQUE);
    RobotSleep(3);
}

int mission_6_1_detection_ball(U16 *image) {
    U32 col, row, sum = 0, cnt, detCol = 0, detection = 0;

    for (row = 0; row < HEIGHT; ++row) {
        cnt = 0;
        for (col = 0; col < WIDTH; ++col) {
            cnt += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), ORANGE) ||
                    GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), RED));
        }
        sum += cnt;

        if (cnt > detCol) {
            detCol = cnt;
            detection = col;
        }

    }

    if (sum < 1) {
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
    int range = 4;
    if (detection < 85 || detection > 95) {
        cnt = (U32) (detection < 85);
        detection = ((detection < 85) ? detection : WIDTH - detection);
        range = (0 <= detection && detection < 25) ? 3 :
                (25 <= detection && detection < 55) ? 2 :
                (55 <= detection && detection < 85) ? 1 : 0;

        if (range != 0) {
            if (cnt) {
                ACTION_MOVE(LONG, DIR_LEFT, LOW, OBLIQUE, range);
            } else {
                ACTION_MOVE(LONG, DIR_RIGHT, LOW, OBLIQUE, range);
            }
        }
    } else {
        ACTION_WALK(FAST, OBLIQUE, range);
    }

    return 0;
}

void mission_6_2_watch_below(void) {
    ACTION_INIT(LOW, DOWN);
    RobotSleep(3);
}

int mission_6_2_set_center_of_ball(U16 *image) {
    U32 col, row, isFound, i;
    int selectCnt[2] = {0,}, ballPoint[2] = {0,};

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
                    ballPoint[0] = col;  // LEFT TOP x
                    ballPoint[1] = row;  // LEFT TOP y
                }
            }
        }
    }

    printf("BALL\n\n");
    printf("(%d, %d), cnt: %d\n\n", ballPoint[0], ballPoint[1], selectCnt[0]);

    selectCnt[0] = 80 <= ballPoint[0] && ballPoint[0] < 100;

    // TODO: 높이는 적당히 바꿔주기
    selectCnt[1] = 50 <= ballPoint[1] && ballPoint[1] < 70;

    if (!selectCnt[0]) {
        ACTION_MOVE(SHORT, (DIRECTION) ((80 > ballPoint[0]) ? LEFT : RIGHT), HIGH, DOWN, 3);
    } else {
        if (!selectCnt[1]) {
            ACTION_WALK(SLOW, DOWN, 2);
        }
    }

    return selectCnt[0] && selectCnt[1];
}

int mission_6_3_find_hole(U16 *image) {
    U32 col, row, isFound, i;
    int selectCnt[2] = {0,}, holePoint[2] = {0,};

    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            isFound = 1;
            for (i = 0; i < 2; ++i) {
                isFound &= GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
            }

            if (isFound) {
                selectCnt[1] = countColor(image, row, col, 5, 1);
                if (selectCnt[0] < selectCnt[1]) {
                    selectCnt[0] = selectCnt[1];
                    holePoint[0] = col;  // LEFT TOP x
                    holePoint[1] = row;  // LEFT TOP y
                }
            }
        }
    }

    printf("BALL\n\n");
    printf("(%d, %d), cnt: %d\n\n", holePoint[0], holePoint[1], selectCnt[0]);

    selectCnt[0] = 80 <= holePoint[0] && holePoint[0] < 100;

    if (!selectCnt[0]) {
        ACTION_MOVE(SHORT, (DIRECTION) ((80 > holePoint[0]) ? DIR_LEFT : DIR_RIGHT), HIGH, DOWN, 3);
        ACTION_TURN((DIRECTION) ((80 > holePoint[0]) ? LEFT : RIGHT), LOW, DOWN, 1);
    }

    return selectCnt[0];
}

int mission_6_4_set_front_of_ball(U16 *image) {
    U32 col, row, sum = 0, cnt, detRow = 0, detection = 0;

    for (col = 0; col < WIDTH; ++col) {
        cnt = 0;
        for (row = 0; row < HEIGHT; ++row) {
            cnt += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), ORANGE) ||
                    GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), RED));
        }
        sum += cnt;

        if (cnt > detRow) {
            detRow = cnt;
            detection = row;
        }

    }

    // TODO: 높이는 적당히 바꿔주기
    if (50 <= detection && detection < 70) {
        return 1;
    } else {
        ACTION_WALK(SLOW, DOWN, 1);
        return 0;
    }
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
        }
    }
    return cnt;
}

int mission_6_5_kick_ball(void) {
    ACTION_MOTION(MISSION_6_RIGHT_KICK, MIDDLE, OBLIQUE);
    ACTION_TURN(LEFT, MIDDLE, OBLIQUE, 6);
    ACTION_WALK(FAST, OBLIQUE, 6);
    return 1;
}

int mission_6_6_set_straight_black_line(U16* image) {
    U32 i, row, col[2] = {MISSION_6_6_COL_POINT_1,
                          MISSION_6_6_COL_POINT_2};
    U16 pos_black[2] = {0,};
    for(i = 0; i < 2; ++i) {
        for(row = HEIGHT;row > 0; --row) {
            if(GetValueRGBYOBK(GetPtr(image, row, col[i], WIDTH), BLACK) == 1 &&
               GetValueRGBYOBK(GetPtr(image, row, col[i] + 1, WIDTH), BLACK) == 1) {
                break;
            }
            pos_black[i] += 1;
        }
    }

    printf("pos_black[0]: %d, pos_black[1]: %d\n", pos_black[0], pos_black[1]);

// s>0 l
// s < 0
    double slope = (double) (pos_black[0] - pos_black[1]);

    printf("Slope is %f\n", slope);

    ACTION_INIT(LOW, OBLIQUE);

    int rResult = 1;
    if (((slope>0)?slope : -slope) > MISSION_6_6_BLUE_GATE_SLOPE) {
        rResult = 0;
        if (slope > 0) {
            ACTION_TURN(DIR_LEFT, LOW, OBLIQUE, 2);
        } else if (slope < 0) {
            ACTION_TURN(DIR_RIGHT, LOW, OBLIQUE, 2);
        }
    }

    ACTION_INIT(LOW, OBLIQUE);

    return rResult;
}

void mission_6_6_watch_side(void) {
    ACTION_INIT(LOW, LEFT);
    RobotSleep(3);

}

int mission_6_6_set_center_black_line(U16* image) {
    U32 i, row, col[2] = {MISSION_6_6_COL_POINT_1,
                          MISSION_6_6_COL_POINT_2};
    U16 black_len[2] = {0,};
    for(i = 0; i < 2;++i) {
        for(row = HEIGHT; row > 0; --row) {
            if(GetValueRGBYOBK(GetPtr(image, row, col[i], WIDTH), BLACK) == 1 &&
               GetValueRGBYOBK(GetPtr(image, row, col[i] + 1, WIDTH), BLACK) == 1) {
                black_len[i] = HEIGHT - row;
                break;
            }
        }
    }

    printf("black_len[0] = %d, black_len[1] = %d\n", black_len[0], black_len[1]);

    double e = (double) (black_len[0] + black_len[1]) / 2;

    printf("distance is %f", e);

    ACTION_INIT(LOW, OBLIQUE);

    int rResult = 0;
    if(e < MISSION_6_6_BLACK_LENGTH - MISSION_6_6_BLACK_LENGTH_ERROR) {
        //오른쪽 이동
        ACTION_MOVE(LONG, DIR_RIGHT, LOW, OBLIQUE, 1);
    }
    else if(e > MISSION_6_6_BLACK_LENGTH + MISSION_6_6_BLACK_LENGTH_ERROR) {
        //왼쪽 이동
        ACTION_MOVE(LONG, DIR_LEFT, LOW, OBLIQUE, 1);
    }
    else {
        rResult = 1;
    }

    ACTION_INIT(LOW, OBLIQUE);

    return rResult;
}