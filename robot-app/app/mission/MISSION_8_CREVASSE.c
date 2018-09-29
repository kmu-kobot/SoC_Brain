//
// Created by Gihyeon Yang on 2018. 7. 22..
//

#include "MISSION_8_CREVASSE.h"

void mission_8_1_watch_below(U16 *image, int repeat) {
    ACTION_WALK_CHECK(OBLIQUE, image, mission_8_1_wait_front_of_crevasse, 1, repeat);
}

int mission_8_1_wait_front_of_crevasse(U16 *image) {
    double ratio = getColorRatio1(image, 40, 80, 50, WIDTH - 50, BLACK);
    return ratio > 2.0;
}

int mission_8_3_escape_crevasse(void) {
    ACTION_MOTION(MISSION_8_CREVASSE_DUMBLING, UP);
    return 1;
}

int mission_8_2_attach_black(U16 *image) {
    _line_t front_line;
    if (!mission_8_2_get_front_line(image, &front_line, BLACK)) {
        return 0;
    }

    return mission_8_2_set_dist(front_line);
}

int mission_8_2_get_front_line(U16 *image, _line_t *front_line, U16 color) {
    U16 i, j;
    _point_t points[20 * 2];
    U8 black_cnt[3], pos;
    U32 point_cnt = 0;
    U16 left, right;

    for (j = 10; j < 30; ++j) {
        pos = 0;
        memset(black_cnt, 0, 3 * sizeof(U8));
        left = MAX(j - 1, 0);
        right = MIN(j + 1, WIDTH - 1);

        for (i = HEIGHT - 10; i > 0; --i) {
            black_cnt[pos] = GetValueRGBYOBK(GetPtr(image, i, left, WIDTH), color) +
                             GetValueRGBYOBK(GetPtr(image, i, j, WIDTH), color) +
                             GetValueRGBYOBK(GetPtr(image, i, right, WIDTH), color);

            pos = (pos + 1) % 3;

            if (black_cnt[0] + black_cnt[1] + black_cnt[2] > 2) {
                points[point_cnt].x = j;
                points[point_cnt].y = i;
                ++point_cnt;
                break;
            }
        }
    }

    for (j = WIDTH - 10; j > WIDTH - 30; --j) {
        pos = 0;
        memset(black_cnt, 0, 3 * sizeof(U8));
        left = MAX(j - 1, 0);
        right = MIN(j + 1, WIDTH - 1);

        for (i = HEIGHT - 10; i > 0; --i) {
            black_cnt[pos] = GetValueRGBYOBK(GetPtr(image, i, left, WIDTH), color) +
                             GetValueRGBYOBK(GetPtr(image, i, j, WIDTH), color) +
                             GetValueRGBYOBK(GetPtr(image, i, right, WIDTH), color);

            pos = (pos + 1) % 3;

            if (black_cnt[0] + black_cnt[1] + black_cnt[2] > 2) {
                points[point_cnt].x = j;
                points[point_cnt].y = i;
                ++point_cnt;
                break;
            }
        }
    }

#ifdef DEBUG
    printf("point_cnt : %d\n", point_cnt);
#endif

    if (point_cnt < NUM_LIN_REG_POINT >> 1) {
        return 0;
    }

    if (i <= 1) {
        return 0;
    }

    return least_sqaures(image, WIDTH >> 1, points, point_cnt, front_line);
}

int mission_8_2_set_straight(_line_t line) {
    double angle = atan(line.slope) * 180.0 / M_PI;
    DIRECTION turn_dir = (DIRECTION) (angle > 0);
    angle = abs(angle);

    if (angle > 4.0) {
        ACTION_TURN(MIDDLE, turn_dir, DOWN, 1);
    } else if (angle > 3.0) {
        ACTION_TURN(SHORT, turn_dir, DOWN, 2);
    } else if (angle > 2.0) {
        ACTION_TURN(SHORT, turn_dir, DOWN, 1);
    } else {
        return 1;
    }

    RobotSleep(2);
    return 0;
}

int mission_8_2_set_dist(_line_t line) {
    double dist = line.slope * (WIDTH >> 1) + line.intercept;

    if (dist < 55.0) {
        ACTION_ATTACH(1);
        RobotSleep(1);
        return 0;
    }

    return 1;
}
