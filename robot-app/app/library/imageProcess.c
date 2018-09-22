#include "imageProcess.h"


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


double getColorRatio1(U16 *image, const U16 top, const U16 bot, const U16 left, const U16 right, const U16 color1) {
    U16 i, j;
    U32 cnt = 0;
    for (i = MAX(0, top); i < MIN(bot, HEIGHT); ++i) {
        for (j = MAX(0, left); j < MIN(right, WIDTH); ++j) {
            cnt += GetValueRGBYOBK(GetPtr(image, i, j, WIDTH), color1);
        }
    }

    double ratio =
            ((double) cnt) / (double) ((MIN(bot, HEIGHT) - MAX(0, top)) * (MIN(right, WIDTH) - MAX(0, left))) * 100.0;

#ifdef DEBUG
    printf("color1 : %d, ratio : %f\n", color1, ratio);
#endif

    return ratio;
}

double getColorRatio2(U16 *image, const U16 top, const U16 bot, const U16 left, const U16 right, const U16 color1,
                      const U16 color2) {
    U16 i, j;
    U32 cnt = 0;
    for (i = MAX(0, top); i < MIN(bot, HEIGHT); ++i) {
        for (j = MAX(0, left); j < MIN(right, WIDTH); ++j) {
            cnt += GetValueRGBYOBK(GetPtr(image, i, j, WIDTH), color1) |
                   GetValueRGBYOBK(GetPtr(image, i, j, WIDTH), color2);
        }
    }

    double ratio =
            ((double) cnt) / (double) ((MIN(bot + 1, HEIGHT) - MAX(0, top)) * (MIN(right + 1, WIDTH) - MAX(0, left))) *
            100.0;

#ifdef DEBUG
    printf("color1 : %d, color2 : %d, ratio : %f\n", color1, color2, ratio);
#endif

    return ratio;
}

double getColorRatio3(U16 *image, const U16 top, const U16 bot, const U16 left, const U16 right, const U16 color1,
                      const U16 color2, const U16 color3) {
    U16 i, j;
    U32 cnt = 0;
    for (i = MAX(0, top); i < MIN(bot, HEIGHT); ++i) {
        for (j = MAX(0, left); j < MIN(right, WIDTH); ++j) {
            cnt += GetValueRGBYOBK(GetPtr(image, i, j, WIDTH), color1) |
                   GetValueRGBYOBK(GetPtr(image, i, j, WIDTH), color2) |
                   GetValueRGBYOBK(GetPtr(image, i, j, WIDTH), color3);
        }
    }

    double ratio =
            ((double) cnt) / (double) ((MIN(bot, HEIGHT) - MAX(0, top)) * (MIN(right, WIDTH) - MAX(0, left))) * 100.0;

#ifdef DEBUG
    printf("color1 : %d, color2 : %d, color3 : %d, ratio : %f\n", color1, color2, color3, ratio);
#endif

    return ratio;
}

int getDistance1(U16 *image, U16 center, U16 bot, U16 color1) {
    U32 i, j, frame;
    U8 color_cnt;
    U32 point_cnt = 0;
    U16 left, right;
    int dist[NUM_DIST_FRAME * NUM_DIST_POINT];

    bot = IN_IMG(0, bot, HEIGHT - 1);

    for (frame = 0; frame < NUM_DIST_FRAME; ++frame) {
        setFPGAVideoData(image);
        for (j = IN_IMG(0, center - (NUM_DIST_POINT >> 1), WIDTH);
             j < IN_IMG(0, center + (NUM_DIST_POINT >> 1), WIDTH); ++j) {
            left = MAX(j - 1, 0);
            right = MIN(j + 1, WIDTH - 1);

            for (i = bot; i >= 0; --i) {
                color_cnt = GetValueRGBYOBK(GetPtr(image, i, left, WIDTH), color1) +
                            GetValueRGBYOBK(GetPtr(image, i, j, WIDTH), color1) +
                            GetValueRGBYOBK(GetPtr(image, i, right, WIDTH), color1);
                if (color_cnt > 1) {
                    dist[point_cnt++] = i;
                    break;
                }
            }
        }
    }

#ifdef DEBUG
    printf("point_cnt : %d\n", point_cnt);
#endif

    if (point_cnt == 0) {
        return 0;
    }

    return average(dist, NUM_DIST_FRAME * NUM_DIST_POINT);
}


int getDistance2(U16 *image, U16 center, U16 bot, U16 color1, U16 color2) {
    U32 i, j, frame;
    U8 color_cnt;
    U32 point_cnt = 0;
    U16 left, right;
    int dist[NUM_DIST_FRAME * NUM_DIST_POINT];

    bot = IN_IMG(0, bot, HEIGHT - 1);

    for (frame = 0; frame < NUM_DIST_FRAME; ++frame) {
        setFPGAVideoData(image);
        for (j = IN_IMG(0, center - (NUM_DIST_POINT >> 1), WIDTH);
             j < IN_IMG(0, center + (NUM_DIST_POINT >> 1), WIDTH); ++j) {
            left = MAX(j - 1, 0);
            right = MIN(j + 1, WIDTH - 1);

            for (i = bot; i >= 0; --i) {
                color_cnt = (GetValueRGBYOBK(GetPtr(image, i, left, WIDTH), color1) |
                             GetValueRGBYOBK(GetPtr(image, i, left, WIDTH), color2)) +
                            (GetValueRGBYOBK(GetPtr(image, i, j, WIDTH), color1) |
                             GetValueRGBYOBK(GetPtr(image, i, j, WIDTH), color2)) +
                            (GetValueRGBYOBK(GetPtr(image, i, right, WIDTH), color1) |
                             GetValueRGBYOBK(GetPtr(image, i, right, WIDTH), color2));
                if (color_cnt > 1) {
                    dist[point_cnt] = i;
                    break;
                }
            }
        }
    }

#ifdef DEBUG
    printf("point_cnt : %d\n", point_cnt);
#endif

    if (point_cnt == 0) {
        return 0;
    }

    return average(dist, NUM_DIST_FRAME * NUM_DIST_POINT);
}

int linear_regression1(U16 *image, U16 center, U16 bot, U16 color1, _line_t *line) {
    U32 i, j, frame;
    _point_t points[NUM_LIN_REG_POINT * NUM_LIN_REG_FRAME];
    U8 color_cnt[3], pos;
    U32 point_cnt = 0;
    U16 left, right;

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

    if (point_cnt * 3 / 4 == 0) {
        return 0;
    }

    return least_sqaures(image, center, points, point_cnt, line);
}

int linear_regression2(U16 *image, U16 center, U16 bot, U16 color1, U16 color2, _line_t *line) {
    U32 i, j, frame;
    _point_t points[NUM_LIN_REG_POINT * NUM_LIN_REG_FRAME];
    U8 color_cnt[3], pos;
    U32 point_cnt = 0;
    U16 left, right;

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
                color_cnt[pos] = (GetValueRGBYOBK(GetPtr(image, i, left, WIDTH), color1) |
                                  GetValueRGBYOBK(GetPtr(image, i, left, WIDTH), color2)) +
                                 (GetValueRGBYOBK(GetPtr(image, i, j, WIDTH), color1) |
                                  GetValueRGBYOBK(GetPtr(image, i, j, WIDTH), color2)) +
                                 (GetValueRGBYOBK(GetPtr(image, i, right, WIDTH), color1) |
                                  GetValueRGBYOBK(GetPtr(image, i, right, WIDTH), color2));

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

    if (point_cnt * 3 / 4 == 0) {
        return 0;
    }

    return least_sqaures(image, center, points, point_cnt, line);
}


int least_sqaures(U16 *image, U16 center, _point_t *points, U32 size, _line_t *line) {
    U32 i;

    double sum_x = 0;
    double sum_y = 0;
    double sum_xy = 0;
    double sum_xx = 0;

    for (i = 0; i < size; ++i) {
        sum_x += points[i].x;
        sum_y += points[i].y;
        sum_xy += points[i].x * points[i].y;
        sum_xx += points[i].x * points[i].x;
    }

    double factor = (sum_xx * size) - (sum_x * sum_x);

    if (factor == 0) {
        printf("factor is 0\n");
        return 0;
    }

    line->slope = (size * sum_xy - sum_x * sum_y) / factor;
    line->intercept = (-sum_x * sum_xy + sum_xx * sum_y) / factor;

#ifdef DEBUG
    double angle = atan(line->slope) * 180.0 / M_PI;
    double distance = (line->slope) * center + (line->intercept);

    printf("slope : %f, angle : %f, intercept : %f, distance : %f\n\n", line->slope, angle, line->intercept, distance);
#endif

    drawline(image, *line, 0xffff);
    drawpoint(image, points, size, 0xffff);
    draw_fpga_video_data_full(image);
    flip();

    return 1;
}


int least_sqauresT(U16 *image, U16 center, _point_t *points, U32 size, _line_t *line) {
    U32 i;

    double sum_x = 0;
    double sum_y = 0;
    double sum_xy = 0;
    double sum_yy = 0;

    for (i = 0; i < size; ++i) {
        sum_x += points[i].x;
        sum_y += points[i].y;
        sum_xy += points[i].x * points[i].y;
        sum_yy += points[i].y * points[i].y;
    }

    double factor = (sum_yy * size) - (sum_y * sum_y);

    if (factor == 0) {
        printf("factor is 0\n");
        return 0;
    }

    line->slope = (size * sum_xy - sum_x * sum_y) / factor;
    line->intercept = (-sum_y * sum_xy + sum_yy * sum_x) / factor;

#ifdef DEBUG
    double angle = atan(line->slope) * 180.0 / M_PI;
    double distance = (line->slope) * center + (line->intercept);

    printf("slope : %f, angle : %f, intercept : %f, distance : %f\n\n", line->slope, angle, line->intercept, distance);
#endif

    drawlineT(image, *line, 0xffff);
    drawpoint(image, points, size, 0xffff);
    draw_fpga_video_data_full(image);
    flip();

    return 1;
}
