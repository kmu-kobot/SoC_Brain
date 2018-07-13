//
// Created by Gihyeon Yang on 2018. 7. 11..
//

#include "MISSION_7_YELLOW_HOLE_BRIDGE.h"

void mission_7_1_watch_below(int repeat) {
    Action_WALK_FRONT_LONG(repeat);

    Action_WATCH_BELOW_LONG();
}

int mission_7_1_wait_front_of_yellow_hole_bridge(U16 *image, int repeat) {
    U32 col, row, cntYellow = 0;
    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            cntYellow += GetValueRGBYOBK(
                    GetPtr(image, row, col, WIDTH),
                    YELLOW
            );
        }
    }

    int rReturn = (cntYellow * 100 / (WIDTH * HEIGHT)) > MISSION_7_THRESHOLDS;

    if (rReturn) {
        Action_WALK_FRONT_SHORT(repeat);
    }

    return rReturn;
}


void mission_7_2_watch_side(void) {
    Action_LEFT_TURN_HEAD_LONG();
}

int mission_7_2_before_bridge_set_center(U16 *image) {
    return 0;
}


int mission_7_3_climb_yellow_hole_bridge() {
    Action_CLIMB_UP_STAIRS();
    return 0;
}

int mission_7_4_set_straight_on_yellow_bridge(U16 *image) {
    return 1;
}

int mission_7_4_set_center_on_yellow_bridge(U16 *image) {
    return 1;
}

void mission_7_5_watch_below(void) {
    Action_WATCH_BELOW_LONG();
}

int mission_7_6_jump_hole(void) {
    Action_MISSION_7_YELLOW_HOLE_BRIDG_DUMBLING();
    return 1;
}

