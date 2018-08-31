//
// Created by Gihyeon Yang on 2018. 7. 22..
//

#include "MISSION_8_CREVASSE.h"

void mission_8_1_watch_below(U16 *image, int repeat)
{
    ACTION_WALK_CHECK(DOWN, image, mission_8_1_wait_front_of_crevasse, 1, repeat);
}

int mission_8_1_wait_front_of_crevasse(U16 *image)
{
    double ratio = getColorRatio1(image, 0, ROBOT_KNEE, 50, WIDTH - 50, BLACK);
    return ratio > 5.0;
}

int mission_8_2_escape_crevasse(void)
{
    ACTION_MOTION(MISSION_8_CREVASSE_DUMBLING, OBLIQUE);
    return 1;
}
