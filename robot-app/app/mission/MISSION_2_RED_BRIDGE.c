//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#include "MISSION_2_RED_BRIDGE.h"

void mission_2_1_watch_below(U16 *image, int repeat) { // 이름을 바꾸는게 좋을거같음
    ACTION_WALK_CHECK(DOWN, image, mission_2_1_wait_front_of_red_bridge, 1, repeat);
    RobotSleep(1); // 슬립 빼도 될거같음
}

int mission_2_1_attach_red_bridge(U16 *image) {
    double ratio = getColorRatio2(image, 20, ROBOT_KNEE, 50, WIDTH - 50, RED, ORANGE);

    if (ratio < 50.0) {
        ACTION_ATTACH(1);
        return 0;
    }
    return 1;
}

int mission_2_1_wait_front_of_red_bridge(U16 *image) {
    return getColorRatio2(image, 0, HEIGHT, 0, WIDTH, RED, ORANGE) > CASE_2_0_DETECTION;
}

int mission_2_3_escape_red_bridge(void) {
    ACTION_MOTION(MISSION_2_RED_DUMBLING, UP);
    return 1;
}
