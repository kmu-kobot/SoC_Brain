//
// Created by Gihyeon Yang on 2018. 7. 17..
//

#include "MISSION_3_AVOID_BOMB.h"

int mdir = 0;
int mangle = 0;

void mission_3_attach_mine(U16 *image) {
    double upper_ratio = getColorRatio1(image, 5, ROBOT_KNEE, MINE_RANGE_LEFT, WIDTH - MINE_RANGE_LEFT, BLACK);
    double lower_ratio = getColorRatio1(image, 20, ROBOT_KNEE, MINE_RANGE_LEFT, WIDTH - MINE_RANGE_LEFT, BLACK);
    U16 iter = 0;
    while (lower_ratio < 1.0 && iter++ < 2) {
        if (upper_ratio < 0.4) {
            ACTION_ATTACH(1);
        } else if (lower_ratio < 1.0) {
            ACTION_ATTACH_SHORT(1);
        }
        RobotSleep(1);
        setFPGAVideoData(image);
        upper_ratio = getColorRatio1(image, 5, ROBOT_KNEE, MINE_RANGE_LEFT, WIDTH - MINE_RANGE_LEFT, BLACK);
        lower_ratio = getColorRatio1(image, 20, ROBOT_KNEE, MINE_RANGE_LEFT, WIDTH - MINE_RANGE_LEFT, BLACK);
    }
}

int mission_3_avoid(U16 *image) {
    double blue_ratio = getColorRatio1(image,
                                       20, ROBOT_KNEE + 5,
                                       MINE_RANGE_LEFT, WIDTH - MINE_RANGE_LEFT,
                                       BLUE);

    double black_ratio = getColorRatio1(image,
                                        5, ROBOT_KNEE,
                                        MINE_RANGE_LEFT, WIDTH - MINE_RANGE_LEFT,
                                        BLACK);

    if (blue_ratio > 3.0) {
        return 1;
    }

    U32 repeat = 1;

    if (black_ratio > 0.4) {
        ACTION_MOVE(LONG, (DIRECTION) (mdir & 1), DOWN, (int) repeat);
        RobotSleep(2);
    }
    return black_ratio < 0.4;
}

void mission_3_change_mdir(U16 *image) {
    double thresholdAngle = 13.0;

    _line_t leftline, rightline;
    int leftstate, rightstate;
    default_watch(LEFT, image);
    RobotSleep(1);
    leftstate = linear_regression1(image, WIDTH_CENTER, HEIGHT - 4, BLACK, &leftline);
    mangle = fabs(atan(leftline.slope) * 180.0 / M_PI + (9.0)) > thresholdAngle;

    default_watch(RIGHT, image);
    RobotSleep(1);
    rightstate = linear_regression1(image, WIDTH_CENTER, HEIGHT - 4, BLACK, &rightline);
    mangle |= fabs(atan(rightline.slope) * 180.0 / M_PI + (-11.0)) > thresholdAngle;

    mdir = leftline.slope * WIDTH_CENTER + leftline.intercept > rightline.slope * WIDTH_CENTER + rightline.intercept;

    if (leftstate != 1)
    {
        mdir = DIR_RIGHT;
        return;
    }
    if (rightstate != 1)
    {
        mdir = DIR_LEFT;
        return;
    }
}

int mission_3_check_angle(void) {
    return mangle;
}

int mission_3_isFrontOf_Blue(U16 *image, U16 bot) {
    double ratio = getColorRatio1(image, 10, 40, MINE_RANGE_LEFT, WIDTH - MINE_RANGE_LEFT, BLUE);

    return ratio > 3.0;
}

int mission_3_default_watch_below(U16 *image, int repeat) {
    setFPGAVideoData(image);
    int result = MINE_WALK_CHECK(image, mission_3_walk_avoid_bomb, repeat);
    return result;
}

void mission_3_init_global(void) {
    mdir = 0;
}

int mission_3_4_getMDir(void) {
    return (mdir & 1) + LEFT;
}

int minecount = 0;

int mission_3_default_avoid_bomb(U16 *image) {
    double black_ratio = getColorRatio1(image, 50, MINE_RANGE_BOT, MINE_RANGE_LEFT, WIDTH - MINE_RANGE_LEFT, BLACK);

    minecount += black_ratio > 0.5;
    return black_ratio > 0.5;
}

int mission_3_walk_avoid_bomb(U16 *image) {
    double blue_ratio = getColorRatio1(image, 15, 40, MINE_RANGE_LEFT, WIDTH - MINE_RANGE_LEFT, BLUE);

    if (blue_ratio > 3.0) {
        return 1;
    }

#ifdef MINE
    double black_ratio = getColorRatio1(image, 15, MINE_RANGE_BOT, MINE_RANGE_LEFT, WIDTH - MINE_RANGE_LEFT, BLACK);

    minecount += black_ratio > 0.3;
    return black_ratio > 0.3;
#else
    return 0;
#endif
}

int k = 0;

int mission_3_1_ver2(U16 *image) {
    double s = getColorRatio1(image, 0, HEIGHT, 30, WIDTH - 30, BLUE);

    if (!k) {
        k = (s > 5) ? k + 1 : 0;
    } else if (minecount < 3) {
        return 0;
    } else {
        k = (s > 5) ? 0 : (s < 3) ? k + 1 : k;
    }

    return k > 5;
}

int mission_3_set_straight_and_center1_long(U16 *image, U16 center) {
    _line_t line;
    VIEW view = (mdir & 1) + LEFT;
    int state;

    CHECK_INIT(view);
    state = mission_3_linear_regression(image, center, HEIGHT - 4, BLACK, &line);
    if (state != 1) {
        return 0;
    }

    return set_straight(line, center, view) && set_center_long(line, DEFAULT_CENTER_DISTANCE, view);
}

int mission_3_set_straight(U16 *image) {
    _line_t line;
    VIEW view = (mdir & 1) + LEFT;
    int state;

    CHECK_INIT(view);
    state = mission_3_linear_regression(image, WIDTH_CENTER, HEIGHT - 4, BLACK, &line);
    if (state != 1) {
        return 0;
    }

    return set_straight(line, WIDTH_CENTER, view);
}

int mission_3_set_center(U16 *image) {
    _line_t line;
    VIEW view = (mdir & 1) + LEFT;
    int state;

    CHECK_INIT(view);
    state = mission_3_linear_regression(image, WIDTH_CENTER, HEIGHT - 4, BLACK, &line);
    if (state != 1) {
        return 0;
    }

    return set_center(line, DEFAULT_CENTER_DISTANCE, view);
}

int mission_3_linear_regression(U16 *image, U16 center, U16 bot, U16 color1, _line_t *line) {
    U32 i, j, frame;
    _point_t points[NUM_LIN_REG_POINT * NUM_LIN_REG_FRAME];
    U8 color_cnt[3], pos;
    U32 point_cnt = 0;
    U16 left, right;
    int sum, aver;

    bot = IN_IMG(0, bot, HEIGHT - 1);

    for (frame = 0; frame < NUM_LIN_REG_FRAME; ++frame) {
        setFPGAVideoData(image);
        for (j = IN_IMG(0, center - (NUM_LIN_REG_POINT >> 1), WIDTH);
             j < IN_IMG(0, center + (NUM_LIN_REG_POINT >> 1), WIDTH); ++j) {
            pos = 0;
            memset(color_cnt, 0, 3 * sizeof(U8));
            left = MAX(j - 1, 0);
            right = MIN(j + 1, WIDTH - 1);

            for (i = bot; i >= 0; --i) {
                color_cnt[pos] = GetValueRGBYOBK(GetPtr(image, i, left, WIDTH), color1) +
                                 GetValueRGBYOBK(GetPtr(image, i, j, WIDTH), color1) +
                                 GetValueRGBYOBK(GetPtr(image, i, right, WIDTH), color1);

                pos = (pos + 1) % 3;

                if (color_cnt[0] + color_cnt[1] + color_cnt[2] > 2) {
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

    if (point_cnt <= 0) {
        return 0;
    }

    qsort(points, point_cnt, sizeof(_point_t), point_t_cmp_y);

    sum = 0;
    for (i = 0; i < point_cnt; ++i) {
        sum += points[i].y;
    }

    aver = sum / point_cnt;

    for (i = 0; i < point_cnt; ++i) {
        if (points[i].y > aver) {
            break;
        }
    }

    if (i <= 1) {
        return 0;
    }

    return least_sqaures(image, center, points, i - 1, line);
}
