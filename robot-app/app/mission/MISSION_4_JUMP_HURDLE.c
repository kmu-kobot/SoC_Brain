//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#include "MISSION_4_JUMP_HURDLE.h"

int mission_4_2_ready_hurdle(U16 *image) { // 굳이? 필요한가
    static int prev = -1;
    int dist = getDistance1(image, WIDTH>>1, HEIGHT-1, BLACK);

    if (dist == 0)
    {
        return 0;
    }

    if (abs(dist - prev) < 3)
    {
        prev = dist;
        return 1;
    }
    prev = dist;
    return 0;
}

int mission_4_4_jump_hurdle(void) {
    CHECK_INIT(OBLIQUE);
    RobotSleep(2);
    ACTION_MOTION(MISSION_4_HURDLING, OBLIQUE);
    RobotSleep(1);
    ACTION_WALK(FAST, OBLIQUE, 4);
    RobotSleep(3);
    return 1;
}
