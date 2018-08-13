#ifndef __IMAGE_PROCESS__
#define __IMAGE_PROCESS__

#include "graphic_api.h"
#include "amazon2_sdk.h"
#include <string.h>
#include <math.h>
#include <stdio.h>

#define NUM_LIN_REG_POINT 20
#define NUM_LIN_REG_FRAME 3

double getColorRatio1(U16 *image, const U16 top, const U16 bot, const U16 left, const U16 right, const U16 color1);
double getColorRatio2(U16 *image, const U16 top, const U16 bot, const U16 left, const U16 right, const U16 color1, const U16 color2);
double getColorRatio3(U16 *image, const U16 top, const U16 bot, const U16 left, const U16 right, const U16 color1, const U16 color2, const U16 color3);

int default_linear_regression1(U16 *image, U16 center, U16 bot, U16 color1, _line_t *line);
int default_linear_regression2(U16 *image, U16 center, U16 bot, U16 color1, U16 color2, _line_t *line);

int least_sqaures(U16 *image, U16 center, _point_t* points, U32 size, _line_t *line);

#endif
