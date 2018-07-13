#ifndef __EXTRACT_HSV__
#define __EXTRACT_HSV__

#include "./library/amazon2_sdk.h"

#include "./library/graphic_api.h"
#include "./library/uart_api.h"
#include "./library/robot_action.h"

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

void extractHSV(void);
void setFPGAVideoData(U16* buf);
int init_extract(void);
void destroy_extract(U16* buf);

static struct termios inittio, newtio;

#endif
