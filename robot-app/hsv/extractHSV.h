#ifndef __EXTRACT_HSV__
#define __EXTRACT_HSV__

#include "./library/amazon2_sdk.h"

#include "./library/graphic_api.h"
#include "./library/uart_api.h"
#include "./library/robot_action.h"

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

#define WIDTH 180
#define HEIGHT 120
#define SIZE (WIDTH * HEIGHT)

#define BIT_H 0x07e0//0000011111100000
#define BIT_S 0xf800//1111100000000000
#define BIT_V 0x001f//0000000000011111
#define SHIFT_H >> 3
#define SHIFT_S >> 8
#define SHIFT_V << 3

#define COLOR_WHITE 0xffff
#define COLOR_MAGENTA 0xf81f
#define COLOR_CYAN 0x07ff

void extractHSV(void);
void setFPGAVideoData(U16* buf);
void help(void);
int init_extract(void);
void destroy_extract(U16* buf);

static struct termios inittio, newtio;

#endif
