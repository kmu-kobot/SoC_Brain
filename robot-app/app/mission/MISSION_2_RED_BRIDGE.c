//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#include "MISSION_2_RED_BRIDGE.h"

void mission_2_1_watch_below(U16 *image, int repeat) { // 이름을 바꾸는게 좋을거같음
    ACTION_WALK_CHECK(DOWN, image, mission_2_1_wait_front_of_red_bridge, 1, repeat);
    RobotSleep(1); // 슬립 빼도 될거같음
}

int m2dir = -1;

int mission_2_1_check_center(U16 *image) {
    double leftRatio = getColorRatio2(image, 0, HEIGHT, 0, WIDTH / 2, RED, ORANGE);
    double rightRatio = getColorRatio2(image, 0, HEIGHT, WIDTH / 2, WIDTH, RED, ORANGE);

    // LEFT: 0, RIGHT: 1
    m2dir = abs(leftRatio - rightRatio) < 20 ? -1 : (leftRatio > rightRatio ? 0 : 1);
    return m2dir == -1;
}

int mission_2_1_get_mdir(void) {
    return (m2dir & 1) + LEFT;
}

int mission_2_1_attach_red_bridge(U16 *image) {
    double ratio = getColorRatio2(image, 20, ROBOT_KNEE - 5, 50, WIDTH - 50, RED, ORANGE);

    ACTION_ATTACH(1);
    ACTION_ATTACH_SHORT(1);
    return 1;
    // return ratio > 30.0;
}

int mission_2_1_wait_front_of_red_bridge(U16 *image) {
    return getColorRatio2(image, 0, HEIGHT, 0, WIDTH, RED, ORANGE) > CASE_2_0_DETECTION;
}

int mission_2_3_escape_red_bridge(void) {
    ACTION_MOTION(MISSION_2_RED_DUMBLING, OBLIQUE);
    return 1;
}
