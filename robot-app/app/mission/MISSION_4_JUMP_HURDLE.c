//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#include "MISSION_4_JUMP_HURDLE.h"

int mission_4_4_jump_hurdle(void) {
    ACTION_MOTION(MISSION_4_HURDLING, OBLIQUE);
    RobotSleep(1);
    ACTION_TURN(LONG, DIR_LEFT, OBLIQUE, 5);
    RobotSleep(1);
    ACTION_WALK(FAST, OBLIQUE, 6);
    RobotSleep(1);
    ACTION_TURN(LONG, DIR_LEFT, OBLIQUE, 3);
    RobotSleep(1);
    return 1;
}
