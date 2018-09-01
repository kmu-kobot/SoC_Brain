//
// Created by Gihyeon Yang on 2018. 7. 17..
//

#include "MISSION_3_AVOID_BOMB.h"

int mdir = 0;

int mission_3_avoid(U16 *image) {
    double ratio = getColorRatio1(image, 20, MINE_RANGE_BOT, 45, WIDTH-45, BLACK);

    if (ratio > 1)
    {
        ACTION_MOVE(LONG, !(mdir & 1), OBLIQUE, 1);
    }
    return ratio < 1;
}

void mission_3_change_mdir(void)
{
    ++mdir;
}

int mission_3_measure_line(U16 *image) { // 여기도 col 갯수 늘리고 여러 프레임 돌리느넥 좋을듯
    _line_t line;
    int i = 0;
    while(!mission_3_linear_regression(image, WIDTH>>1, HEIGHT-11, BLACK, &line) && i++ < 5);

    double dist = line.slope*(WIDTH>>1) + line.intercept;

    mdir += (dist > 70.0);

    return 1;
}

int mission_3_isFrontOf_Blue(U16 *image) {
    double ratio = getColorRatio1(image, 40, HEIGHT, 0, WIDTH, BLUE);

    return ratio > 3;
}

void mission_3_default_watch_below(U16 *image, int repeat) {
    RobotSleep(3);
    ACTION_WALK_CHECK(OBLIQUE, image, mission_3_default_avoid_bomb, 1, repeat);
    RobotSleep(1);
}


void mission_3_init_global(void) {
    mdir = 0;
}

int mission_3_4_getMDir(void) {
    return !(mdir & 1) + LEFT;
}

int minecount = 0;

int mission_3_default_avoid_bomb(U16 *image) {
    double black_ratio = getColorRatio1(image, 20, MINE_RANGE_BOT, 50, WIDTH-50, BLACK);

    minecount += black_ratio > 1;
    return black_ratio > 1;
}

int k = 0;

int mission_3_1_ver2(U16 *image) {
    double s = getColorRatio1(image, 40, HEIGHT, 30, WIDTH-30, BLUE);

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

int mission_3_set_straight_and_center1(U16 *image)
{
    _line_t line;
    VIEW view = !(mdir & 1) + LEFT;

    CHECK_INIT(view);
    if (!mission_3_linear_regression(image, WIDTH>>1, HEIGHT-1, BLACK, &line))
    {
        return 0;
    }

    return set_straight(line, WIDTH>>1, view) && set_center(line, WIDTH>>1, view);
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
}
