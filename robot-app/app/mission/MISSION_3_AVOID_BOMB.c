//
// Created by Gihyeon Yang on 2018. 7. 17..
//

#include <math.h>
#include "MISSION_3_AVOID_BOMB.h"

void mission_3_4_watch_front(void) {
    ACTION_INIT(MIDDLE, OBLIQUE);
}

int c = 0;

int mission_3_4_is_not_front_of_bomb(U16 *image) {
    U32 row, col, i;
    int check;
    check = 0;

    if (c == 3) return 1;

    if (c == 0) { // 파란선있는지 확인
        for (col = 0; col < WIDTH; col++) {
            for (row = 0; row < 90; row++) {
                check += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
            }
        }

        printf("%f\n", (double) check * 100 / (WIDTH * 60));

        if ((double) check * 100 / (WIDTH * 60) < 10) return 0;

        c = 1;
    } else {
        for (col = 0; col < WIDTH; col++) {
            for (row = 0; row < 90; row++) {
                check += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
            }
        }

        printf("%f\n", (double) check * 100 / (WIDTH * 60));

        if ((double) check * 100 / (WIDTH * 60) < 10) return 1;
    }

    U32 h[3] = {0,};
    U32 w[2] = {0, 179};
    U32 cols[3] = {80, 90, 100};

    int range = 0, sign;

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
        w[0] = WIDTH / 2 - w[0] + 10;
    }

    w[1] = WIDTH / 2 + w[1] - 10;

    check = 0;
    for (row = rH; row < HEIGHT; row++) {
        for (col = w[0]; col < w[1]; col++) {
            check += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK);
        }
    }

    printf("3. (%d, %d)\t(%d, %d)\t%d\n", w[0], rH, w[1], rH, check);

    if (check < 5) {
        c = 3;
    }

    return check < 5;
}

void mission_3_default_watch_below(void) {
    ACTION_INIT(MIDDLE, DOWN);
    RobotSleep(1);
}

int mo = 0;

int mission_3_default_avoid_bomb(U16 *image) {

    U32 section[5][2] = {
            {20,  40},
            {40,  79},
            {79,  105},
            {105, 147},
            {147, 160}
    };

    U32 heights[5][2] = {
            {0,  HEIGHT},
            {10, ROBOT_KNEE},
            {10, ROBOT_KNEE},
            {10, ROBOT_KNEE},
            {0,  HEIGHT}
    };

    U32 row, col, i, check[5] = {0,};

    for (i = 0; i < 5; ++i) {
        for (col = section[i][0]; col < section[i][1]; ++col) {
            for (row = heights[i][0]; row < heights[i][1]; ++row) {
                check[i] += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK);
            }
        }

        check[i] = ((check[i] > MISSION_3_DEFAULT_BOMB_THRESHOLDS) ?
                    ((check[i] > MISSION_3_DEFAULT_LINE_THRESHOLDS) ? 2 : 1) : 0);

    }

    int s = 0;
    for (i = 0; i < 3; ++i) {
        s += ((check[i + 1] == 0) ? 0 : (int) pow((double) 10, (double) (2 - i)));
    }

    printf("\n\t\t- M3-DEFAULT: AVOID BOMB\n");
    printf("\t\t\t- BOMB: %d %d %d %d %d %03d\n\n", check[0], check[1], check[2], check[3], check[4], s);

    int rReturn = (s == 0) ? 1 : 0;

    // * 000 직진
    // * 001 왼쪽으로 이동, 왼쪽 사이드보고 롱으로할지 쇼트로할지 결정 ( 10010 ) 쇼트, ( 00010 ) 롱
    // * 011 왼쪽으로 이동, 롱으로
    // * 111 양쪽 사이드 보고 좌우 어디로 이동할지 결정, 롱으로 이동 ( 11110 ) 오른, ( 01111 ) 왼 ( 11111 ) 왼 or 오른
    // * 110 오른쪽으로 이동, 롱으로
    // * 100 오른쪽으로 이동, 오른쪽 사이드보고 롱으로 할지 쇼트로할지 결정 ( 01001 ) 쇼트, ( 00010 ) 롱
    // * 101 양쪽 사이드 보고 좌우 어디로 이동할지 결정 ( 11010 ) 오른, ( 01011 ) 왼, ( 11011 ) 왼 or 오른
    // * 010 지뢰 피하는 동작

    if (rReturn) {
        mo++;
        ACTION_WALK(FAST, DOWN, 4);
        RobotSleep(2);
    } else {
        if (s == 10) {
            ACTION_MOVE(
                    LONG,
                    (check[0] > check[4]) ? DIR_RIGHT : DIR_LEFT,
                    MIDDLE, DOWN, 1
            );
        } else if (s == 110 || s == 011) {
            ACTION_MOVE(LONG, (s == 110) ? DIR_RIGHT : DIR_LEFT, MIDDLE, DOWN, 1);
        } else if (s == 100 || s == 001) {

            ACTION_MOVE(
                    SHORT,
                    (s == 100) ? DIR_RIGHT : DIR_LEFT,
                    MIDDLE, DOWN, 2
            );
            // * 001 왼쪽으로 이동, 왼쪽 사이드보고 롱으로할지 쇼트로할지 결정 ( 10010 ) 쇼트, ( 00010 ) 롱
            // * 100 오른쪽으로 이동, 오른쪽 사이드보고 롱으로 할지 쇼트로할지 결정 ( 01001 ) 쇼트, ( 00010 ) 롱
        } else if (s == 101) {
            ACTION_MOVE(
                    LONG,
                    (
                            (check[0] != 0 && check[4] == 0) ? DIR_RIGHT :
                            (check[0] == 0 && check[4] != 0) ? DIR_LEFT :
                            ((mo % 2) ? DIR_RIGHT : DIR_LEFT)
                    ),
                    MIDDLE, DOWN, 1
            );
            // * 101 양쪽 사이드 보고 좌우 어디로 이동할지 결정 ( 11010 ) 오른, ( 01011 ) 왼, ( 11011 ) 왼 or 오른
        } else if (s == 111) {
            ACTION_MOVE(
                    LONG,
                    (
                            (check[0] != 0 && check[4] == 0) ? DIR_RIGHT :
                            (check[0] == 0 && check[4] != 0) ? DIR_LEFT :
                            ((mo % 2) ? DIR_RIGHT : DIR_LEFT)
                    ),
                    MIDDLE, DOWN, 1
            );
            // * 111 양쪽 사이드 보고 좌우 어디로 이동할지 결정, 롱으로 이동 ( 11110 ) 오른, ( 01111 ) 왼 ( 11111 ) 왼 or 오른
        }
    }

    return rReturn;
}
