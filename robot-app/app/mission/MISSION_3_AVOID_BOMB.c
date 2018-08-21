//
// Created by Gihyeon Yang on 2018. 7. 17..
//

#include "MISSION_3_AVOID_BOMB.h"

void mission_3_4_watch_front(void) {
    CHECK_INIT(OBLIQUE);
    RobotSleep(2);
}

int mdir = 0;

int mission_3_avoid(U16 *image) {
    // U32 col, row, cnt = 0;
    //
    // for (row = 10; row < ROBOT_KNEE; ++row) {
    //     for (col = 45; col < 135; ++col) {
    //         cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK);
    //     }
    // }
    //
    // if (cnt > 4) {
    //     ACTION_MOVE(LONG, (mdir & 1) ? DIR_RIGHT : DIR_LEFT, DOWN, 1);
    // }
    //
    //
    // return cnt < 4;

    double ratio = getColorRatio1(image, 10, ROBOT_KNEE, 45, WIDTH-45, BLACK);

    if (ratio > 0.1)
    {
        ACTION_MOVE(LONG, !(mdir & 1), DOWN, 1);
    }
    return ratio < 0.1;
}


int mission_3_measure_line(U16 *image) { // 여기도 col 갯수 늘리고 여러 프레임 돌리느넥 좋을듯
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
    int length = (mdir & 1) ? LEFT : RIGHT;
    int err = 6;

    int position = (length - err <= s && s < length + err) ? 0 :
                   (length - err > s) ? ((mdir & 1) ? -1 : 1) : ((mdir & 1) ? 1 : -1);

    mdir = (position == -1) ? 1 : 2;

    return 1;
}

int mission_3_isFrontOf_Blue(U16 *image) {
    // U32 col, row, cnt = 0;
    //
    // for (row = 0; row < 35; ++row) {
    //     for (col = 0; col < WIDTH; ++col) {
    //         cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
    //     }
    // }
    //
    // return (double) cnt * 100 / ((HEIGHT) * (ROBOT_KNEE)) > 5;

    double ratio = getColorRatio1(image, 0, ROBOT_KNEE, 0, WIDTH, BLACK);

    return ratio > 5;
}

void mission_3_default_watch_below(U16 *image, int repeat) {
    ACTION_WALK_CHECK(DOWN, image, mission_3_default_avoid_bomb, 1, repeat);
    RobotSleep(1);
}


void mission_3_init_global(void) {
    mdir = 0;
}

void mission_3_4_watch_side(void) {
    CHECK_INIT((mdir & 1) + LEFT);
    RobotSleep(1);
}

int mission_3_4_getMDir(void) {
    // return (mdir & 1 == 1) ? 0 : 1;
    return !(mdir & 1) + LEFT;
}

int minecount = 0;

int mission_3_default_avoid_bomb(U16 *image) {
    // U32 col, row, cnt = 0;
    //
    // cnt = 0;
    // for (row = 0; row < 35; ++row) {
    //     for (col = 0; col < WIDTH; ++col) {
    //         cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
    //     }
    // }
    //
    // if ((double) cnt * 100 / ((HEIGHT) * (ROBOT_KNEE)) > 10) { // 여기 HEIGHT * ROBOT_KNEE 맞음? 35 * WIDTH 여야되는거같은데
    //     return 1;
    // }

    double blue_ratio = getColorRatio1(image, 0, ROBOT_KNEE, 0, WIDTH, BLUE);

    if (blue_ratio > 10.0)
    {
        return 1;
    }

    // U32 mine = 0, area[2][2] = {
    //         {50, 0},
    //         {130, 40}
    // };
    //
    // mine = 0;
    // for (row = area[0][1]; row < area[1][1]; ++row) {
    //     for (col = area[0][0]; col < area[1][0]; ++col) {
    //         mine += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK);
    //     }
    // }
    //
    // minecount += mine > MISSION_3_DEFAULT_BOMB_THRESHOLDS;
    //
    // return mine > MISSION_3_DEFAULT_BOMB_THRESHOLDS;

    double black_ratio = getColorRatio1(image, 0, ROBOT_KNEE, 50, WIDTH-50, BLACK);

    minecount += black_ratio > 0.1;
    return black_ratio > 0.1;
}

int k = 0;

int mission_3_1_ver2(U16 *image) {
    // U32 col, row, cnt = 0;

    // for (row = 0; row < HEIGHT; ++row) {
    //     for (col = 30; col < WIDTH - 30; ++col) {
    //         cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLUE);
    //     }
    // }
    //
    // double s = (double) cnt * 100 / ((120) * (120)); // 120 * 120 을 HEIGHT * (WIDTH - 30 - 30)으로 하는게 보기 좋을듯

    double s = getColorRatio1(image, 0, HEIGHT, 30, WIDTH-30, BLUE);

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

int point_t_cmp_y(const void * a, const void * b)
{
    return (((_point_t*)a)->y - ((_point_t*)b)->y);
}

int mission_3_set_straight(U16 *image)
{
    _line_t line;
    VIEW view = !(mdir & 1) + LEFT;

    CHECK_INIT(view);
    if (!mission_3_linear_regression(image, WIDTH>>1, HEIGHT-1, BLACK, &line))
    {
        return 0;
    }

    return set_straight(line, WIDTH>>1, view);
}

int mission_3_set_center(U16 *image)
{
    _line_t line;
    VIEW view = !(mdir & 1) + LEFT;

    CHECK_INIT(view);
    if (!mission_3_linear_regression(image, WIDTH>>1, HEIGHT-1, BLACK, &line))
    {
        return 0;
    }

    return set_center(line, WIDTH>>1, view);
}

int mission_3_linear_regression(U16 *image, U16 center, U16 bot, U16 color1, _line_t *line)
{
    U32 i, j, frame;
    _point_t points[NUM_LIN_REG_POINT * NUM_LIN_REG_FRAME];
    U8 color_cnt[3], pos;
    U32 point_cnt = 0;
    U16 left, right;

    bot = IN_IMG(0, bot, HEIGHT - 1);

    for (frame = 0; frame < NUM_LIN_REG_FRAME; ++frame)
    {
        setFPGAVideoData(image);
        for (j = IN_IMG(0, center - (NUM_LIN_REG_POINT>>1), WIDTH); j < IN_IMG(0, center + (NUM_LIN_REG_POINT>>1), WIDTH); ++j)
        {
            pos = 0;
            memset(color_cnt, 0, 3 * sizeof(U8));
            left = MAX(j - 1, 0);
            right = MIN(j + 1, WIDTH - 1);

            for (i = bot; i >= 0; --i)
            {
                color_cnt[pos] = GetValueRGBYOBK(GetPtr(image, i, left, WIDTH), color1) +
                                GetValueRGBYOBK(GetPtr(image, i, j, WIDTH), color1) +
                                GetValueRGBYOBK(GetPtr(image, i, right, WIDTH), color1);

                pos = (pos + 1) % 3;

                if (color_cnt[0] + color_cnt[1] + color_cnt[2] > 2)
                {
                    points[point_cnt].x = j;
                    points[point_cnt].y = i;
                    ++point_cnt;
                    break;
                }
            }
        }
    }

#ifdef DEBUG
    printf("point_cnt : %d\n", point_cnt);
#endif

    if (point_cnt >>1 == 0)
    {
        return 0;
    }
    qsort(points, point_cnt, sizeof(_point_t), point_t_cmp_y);

    return least_sqaures(image, center, points, point_cnt>>1, line);
    return 1;
}
