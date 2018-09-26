//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#include "MISSION_4_JUMP_HURDLE.h"

int mission_4_4_jump_hurdle(void) {
    CHECK_INIT(UP);
    RobotSleep(2);
    ACTION_MOTION(MISSION_4_HURDLING, UP);
    RobotSleep(1);
    ACTION_TURN(LONG, DIR_LEFT, OBLIQUE, 2);
    RobotSleep(1);
    ACTION_WALK(FAST, OBLIQUE, 6);
    RobotSleep(1);
    ACTION_TURN(LONG, DIR_LEFT, OBLIQUE, 2);
    RobotSleep(3);
    return 1;
}
