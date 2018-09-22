#ifndef __IMAGE_PROCESS__
#define __IMAGE_PROCESS__

#include "graphic_api.h"
#include "amazon2_sdk.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define NUM_LIN_REG_POINT 40
#define NUM_LIN_REG_FRAME 3

#define NUM_DIST_POINT 10
#define NUM_DIST_FRAME 1

int average(int *arr, int size);

int analysis(int *arr, int size);

double getColorRatio1(U16 *image, const U16 top, const U16 bot, const U16 left, const U16 right, const U16 color1);

double getColorRatio2(U16 *image, const U16 top, const U16 bot, const U16 left, const U16 right, const U16 color1,
                      const U16 color2);

double getColorRatio3(U16 *image, const U16 top, const U16 bot, const U16 left, const U16 right, const U16 color1,
                      const U16 color2, const U16 color3);

int getDistance1(U16 *image, U16 center, U16 bot, U16 color1);

int getDistance2(U16 *image, U16 center, U16 bot, U16 color1, U16 color2);

int linear_regression1(U16 *image, U16 center, U16 bot, U16 color1, _line_t *line);

int linear_regression2(U16 *image, U16 center, U16 bot, U16 color1, U16 color2, _line_t *line);

int least_sqaures(U16 *image, U16 center, _point_t *points, U32 size, _line_t *line);

int least_sqauresT(U16 *image, U16 center, _point_t *points, U32 size, _line_t *line);

#endif
