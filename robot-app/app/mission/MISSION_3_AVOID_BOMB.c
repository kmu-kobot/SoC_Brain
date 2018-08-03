//
// Created by Gihyeon Yang on 2018. 7. 17..
//

#include "MISSION_3_AVOID_BOMB.h"

void mission_3_4_watch_front(void) {
    CHECK_INIT(MIDDLE, OBLIQUE);
    RobotSleep(2);
}

int mdir = 0;

int mission_3_avoid(U16 *image) {
    U32 col, row, cnt = 0;

    for (row = 20; row < ROBOT_KNEE; ++row) {
        for (col = 30; col < 150; ++col) {
            cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK);
        }
    }

    ACTION_MOVE((cnt < 4) ? SHORT : LONG, (mdir % 2) ? DIR_RIGHT : DIR_LEFT, MIDDLE, DOWN, (cnt < 4) ? 3 : 1);

    return cnt < 4;
}

int mission_3_measure_line(U16 *image) {
    U32 cols[3] = {85, 95, 90}, row, i;
    U16 line[3] = {0,};
    int range;

    for (i = 0; i < 3; ++i) {
        for (row = HEIGHT - 1; row > 1; --row) {
            line[i] = 0;

            for (range = -4; range < 4; ++range) {
                line[i] += GetValueRGBYOBK(GetPtr(image, row, cols[i] + range, WIDTH), BLACK);
            }

            if (line[i] > 4) {
                line[i] = (U16) (HEIGHT - row);
                break;
            }

        }
    }

    double s = 0;

    for (i = 0; i < 3; ++i) {
        s = (s < line[i]) ? line[i] : s;
    }

    int LEFT = 54;
    int RIGHT = 65;
    int length = (mdir % 2) ? LEFT : RIGHT;
    int err = 6;

    int position = (length - err <= s && s < length + err) ? 0 :
                   (length - err > s) ? ((mdir % 2) ? -1 : 1) : ((mdir % 2) ? 1 : -1);

    mdir = (position == -1) ? 1 : 2;

    return 1;
}

int mission_3_isFrontOf_Blue(U16 *image) {
    U32 col, row, cnt = 0;

    for (row = 0; row < ROBOT_KNEE; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
        }
    }

    return (double) cnt * 100 / ((HEIGHT) * (ROBOT_KNEE)) > 1;
}

void mission_3_default_watch_below(int repeat, U16 *image) {
    ACTION_WALK_CHECK(SLOW, DOWN, repeat, mission_3_default_avoid_bomb, image, 1);
    RobotSleep(1);
}


void mission_3_init_global(void) {
    mdir = 0;
}

void mission_3_4_watch_side(void) {
    CHECK_INIT(MIDDLE, (mdir % 2 == 1) ? LEFT : RIGHT);
    RobotSleep(1);
}

int mission_3_4_getMDir(void) {
    return (mdir % 2 == 1) ? 0 : 1;
}

int mission_3_default_avoid_bomb(U16 *image) {
    U32 col, row, cnt = 0;

    cnt = 0;
    for (row = 0; row < ROBOT_KNEE; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
        }
    }

    if ((double) cnt * 100 / ((HEIGHT) * (ROBOT_KNEE)) > 1) {
        return 1;
    }

    U32 mine = 0, area[2][2] = {
            {60, 25},
            {120, ROBOT_KNEE}
    };

    mine = 0;
    for (row = area[0][1]; row < area[1][1]; ++row) {
        for (col = area[0][0]; col < area[1][0]; ++col) {
            mine += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK);
        }
    }

    return mine > MISSION_3_DEFAULT_BOMB_THRESHOLDS;
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