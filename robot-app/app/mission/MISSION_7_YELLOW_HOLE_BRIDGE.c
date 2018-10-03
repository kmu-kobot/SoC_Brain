//
// Created by Gihyeon Yang on 2018. 7. 11..
//

#include "MISSION_7_YELLOW_HOLE_BRIDGE.h"

void mission_7_1_watch_below(U16 *image, int repeat) {
    ACTION_WALK_CHECK(OBLIQUE, image, mission_7_1_wait_front_of_yellow_hole_bridge, 1, repeat);
    RobotSleep(1);
}

int mission_7_1_attach_yellow_bridge(U16 *image) {
    double ratio = getColorRatio2(image, 20, ROBOT_KNEE, 50, WIDTH - 50, YELLOW, CH2);

    ACTION_ATTACH(1);
    return ratio > 60.0;
}

int mission_7_1_wait_front_of_yellow_hole_bridge(U16 *image) {
    double ratio = getColorRatio2(image, 30, HEIGHT, 0, WIDTH, YELLOW, CH2);

    return ratio > MISSION_7_1_THRESHOLDS;
}

int mission_7_6_jump_hole(void) {
    ACTION_MOTION(MISSION_7_YELLOW_DUMBLING, DOWN);
    RobotSleep(1);
    return 1;
}
