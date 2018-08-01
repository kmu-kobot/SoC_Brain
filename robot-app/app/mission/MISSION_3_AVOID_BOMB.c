//
// Created by Gihyeon Yang on 2018. 7. 17..
//

#include "MISSION_3_AVOID_BOMB.h"

void mission_3_4_watch_front(void) {
    CHECK_INIT(MIDDLE, OBLIQUE);
    RobotSleep(2);
}

int c = 0;

void mission_3_init_global(void) {
    c = 0;
}


int mission_3_4_is_not_front_of_bomb(U16 *image) {
    U32 row, col, i;
    int check;
    check = 0;

    if (c == 3 || c == 4) return 1;

    if (c == 0) { // 파란선있는지 확인
        for (col = 0; col < WIDTH; col++) {
            for (row = 0; row < 90; row++) {
                check += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
            }
        }

        // printf("%f\n", (double) check * 100 / (WIDTH * 60));

        if ((double) check * 100 / (WIDTH * 60) < 10) return 0;

        c = 1;
    } else {
        for (col = 0; col < WIDTH; col++) {
            for (row = 0; row < HEIGHT; row++) {
                check += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
            }
        }

        return (double) check * 100 / (WIDTH * HEIGHT) < 4;

    }

    int sign;

    U32 h[3] = {0,};
    U32 w[2] = {0, 179};
    U32 cols[3] = {80, 90, 100};

    int range = 0;

    for (i = 0; i < 3; i++) {
        for (row = HEIGHT - 1; row >= 0; row--) {
            check = 0;
            for (range = -3; range < 3; range++) {
                check += GetValueRGBYOBK(GetPtr(image, row, cols[i] + range, WIDTH), BLUE);
            }

            if (check > 3) {
                h[i] = row;
                break;
            }

        }
    }

    int rH = 0;
    for (i = 0; i < 3; i++) {
        if (rH < h[i]) {
            rH = h[i];
        }
    }

    rH += 4;

    for (i = 0; i < 2; i++) {
        sign = (i) ? 1 : -1;
        for (col = 0; col < WIDTH / 2 - 1; ++col) {
            check = 0;
            for (row = 0; row < rH; row++) {
                check += GetValueRGBYOBK(GetPtr(image, row, WIDTH / 2 + col * sign, WIDTH), BLUE);
            }

            if (check < 3) {
                w[i] = col;
                break;
            } else {
                w[i] = (i) ? 89 : 0;
            }
        }
    }

    if (w[0] != 0) {
        w[0] = WIDTH / 2 - w[0] + 20;
    }

    w[1] = WIDTH / 2 + w[1] - 20;

    check = 0;
    for (row = (U32) rH; row < HEIGHT - 10; row++) {
        for (col = w[0]; col < w[1]; col++) {
            check += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK);
        }
    }

    // printf("3. (%d, %d)\t(%d, %d)\t%d\n", w[0], rH, w[1], rH, check);

    int bLen[2] = {
            WIDTH / 2 - (w[0] - 20), (w[1] + 20) - WIDTH / 2
    };

    int r = bLen[0] - bLen[1];
    // printf("%d %d %d\n", bLen[0], bLen[1], r);
    if (((r > 0) ? r : (-r) > 30)) {
        ACTION_TURN(LONG, (r > 0) ? DIR_LEFT : DIR_RIGHT, MIDDLE, OBLIQUE, 1);
        return 3;
    }

    if (rH > 60) {
        c = 3;
        return 1;
    }

    if (check == 0) {
        c = 3;
    }

    return check == 0;
}

void mission_3_default_watch_below(void) {
    CHECK_INIT(MIDDLE, DOWN);
    RobotSleep(2);
}


int mdir = 0;
int mcheck = 0;

void mission_3_init(void) {
    mdir = 0;
    mcheck = 0;
}

void mission_3_4_watch_side(void) {
    CHECK_INIT(MIDDLE, (mdir % 2 == 1) ? LEFT : RIGHT);
    RobotSleep(1);
}

int mission_3_4_getMDir(void) {
    return (mdir % 2 == 1) ? 0 : 1;
}

int mission_3_default_avoid_bomb(U16 *image) {

    U32 col, row, i, blue_cnt = 0, black_cnt = 0;

    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            blue_cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
        }
    }

    double s = (double) blue_cnt * 100 / ((HEIGHT) * (WIDTH));

    // printf("blue_pixel ratio : %f\n", s);

    if (s > 5) {
        return 4;
    }

    // for(row = ROBOT_KNEE - 20; row < ROBOT_KNEE; ++row) {
    //     for(col = 0; col < WIDTH; ++col) {
    //         black_cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK);
    //     }
    // }

    // s = (double) black_cnt * 100 / (20 * WIDTH);
    // printf("black pixel ratio : %f\n", s);

    // if(s > 5) {
    //     return 4;
    // }

    int mine[2] = {0,};

    U32 section[2][2] = {
            {30, 90},
            {90, 150}
    };

    U32 heights[5][2] = {
            {20, ROBOT_KNEE},
            {20, ROBOT_KNEE}
    };


    for (i = 0; i < 2; ++i) {
        for (col = section[i][0]; col < section[i][1]; ++col) {
            for (row = heights[i][0]; row < heights[i][1]; ++row) {
                mine[i] += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK);
            }
        }
    }

    double per;
    // printf("\n\n");
    for (i = 0; i < 2; ++i) {
        per = (double) mine[i] * 100 / ((heights[i][1] - heights[i][0]) * (section[i][1] - section[i][0]));
        printf("%f ", per);
        mine[i] = ((mine[i] > MISSION_3_DEFAULT_BOMB_THRESHOLDS) ?
                   ((per > MISSION_3_DEFAULT_LINE_THRESHOLDS) ? 2 : 1) : 0);
    }
    // printf("\n\n");

    // if(mine[0] == 2 || mine[1] == 2) {
    //     ACTION_MOVE(LONG, (mine[0] == 2) ? RIGHT : LEFT, MIDDLE, DOWN, 2);
    //     mdir = (mine[0] == 2) ? 1 : 2;
    //     return 0}

    if (mine[0] == 0 && mine[1] == 0) {

        if (mcheck == 1) {

            ACTION_MOVE(SHORT, (mdir % 2 == 1) ? DIR_LEFT : DIR_RIGHT, MIDDLE, DOWN, 1);

            mdir++;
            mcheck = 0;

            return 3;
        }

        ACTION_WALK(FAST, DOWN, 3);

        return 1;
    } else {
        mcheck = 1;
        if (mdir == 0) {
            if (mine[0] != 0) {
                mdir = 2;
            } else if (mine[1] != 0) {
                mdir = 1;
            }
        }

        if (mcheck == 1) {
            ACTION_MOVE(SHORT, (mdir % 2 == 1) ? DIR_LEFT : DIR_RIGHT, MIDDLE, DOWN, 2);
        }

        return 0;

    }
}

int mission_3_7_get_attach(U16 *image) {
    U32 row, col;
    int cnt = 0;

    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
        }
    }

    return ((double) cnt * 100 / (WIDTH * HEIGHT) < 10);
}

void mission_3_7_attach_hurdle(U16 *image) {

    U32 row, col;
    int cnt = 0;

    for (row = 90; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
        }
    }

    ACTION_WALK(FAST, OBLIQUE, 6);

}

int k = 0;

int mission_3_1_ver2(U16 *image) {
    U32 col, row, cnt = 0;

    for (row = 0; row < HEIGHT; ++row) {
        for (col = 30; col < WIDTH - 30; ++col) {
            cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
        }
    }

    double s = (double) cnt * 100 / ((120) * (120));

    if (k == 0) {
        if (s > 5) {
            k = 1;
        } else {
            k = 0;
        }
    } else if (k == 1) {
        if (s < 3) {
            k = 2;
        } else {
            k = 1;
        }
    }

    return k == 2;
}

int mission_3_4_after_bridge_set_straight(U16 *image) {
    U32 row, i;
    U16 col[2] = {
            80,
            100
    };
    int black_len[2] = {0,};

    for (i = 0; i < 2; ++i) {
        for (row = HEIGHT - 1; row > 0; --row) {
            if (GetValueRGBYOBK(GetPtr(image, row, col[i], WIDTH), BLACK) &&
                GetValueRGBYOBK(GetPtr(image, row, col[i] + 1, WIDTH), BLACK)) {
                black_len[i] = -row;
                break;
            }
        }
    }

    // printf("M2-4: SLOPE\n");
    // printf("black[0]: %d, black_len[1]: %d.\n", black_len[0], black_len[1]);

    double s = (
            (double) (black_len[0] - black_len[1]) /
            (20)
    );

    int pi = (black_len[0] - black_len[1]);
    pi = ((pi > 0) ? pi : (-pi));

    // printf("Slope : %f\n", s * 100);

    int l = -5;
    // printf("%d %d\n", l, (l - 2 <= s && s <= l + 2));

    s *= 100;
    if (!(l - 3 <= s && s <= l + 3)) {
        CHECK_INIT(MIDDLE, OBLIQUE);
        RobotSleep(2);
        ACTION_TURN(
                SHORT,
                (l - s > s) ?
                DIR_LEFT :
                DIR_RIGHT,
                MIDDLE, OBLIQUE, pi / 4 + 1
        );
        CHECK_INIT(MIDDLE, LEFT);
        RobotSleep(2);
        return 0;
    } else {
        // printf("SUCCESS\n\n");
        return 1;
    }
}