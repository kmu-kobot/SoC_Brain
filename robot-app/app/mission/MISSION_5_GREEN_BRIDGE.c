//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#include "MISSION_5_GREEN_BRIDGE.h"

void mission_5_1_watch_below(int repeat) {
    Action_WALK_FRONT_LONG(repeat);
    Action_WATCH_BELOW_LONG();
}

int mission_5_1_check_black_line(U16 *image, int repeat) {
    U32 col, row, cntBlack = 0;
    for (row = 0; row < MISSION_5_1_BLACK_LINE_UPPER; ++row) {
        for (col = 0; col < WIDTH; col++) {
            cntBlack += GetValueRGBYOBK(
                    GetPtr(image, row, col, WIDTH),
                    BLACK
            );
        }
    }

    int rResult = (cntBlack / (MISSION_5_1_BLACK_LINE_UPPER) * WIDTH) > MISSION_5_1_THRESHOLDS;

    if (rResult && !repeat) {
        Action_WALK_FRONT_SHORT(repeat);
    }

    return rResult;
}

void mission_5_2_watch_side(void) {
    Action_LEFT_TURN_HEAD_LONG();
}

int mission_5_3_climb_up_stairs(void) {
    Action_CLIMB_UP_STAIRS();
    return 1;
}

int mission_5_4_set_center_before_green_bridge(U16 *image) {
    U32 col, i;
    U16 green_len[2] = {0,}, row[2] = {
            MISSION_5_4_GREEN_LINE_ROW_POINT_1,
            MISSION_5_4_GREEN_LINE_ROW_POINT_2
    };

    for (col = 0; col < WIDTH / 2; ++col) {
        for (i = 0; i < 2; ++i) {
            if (GetValueRGBYOBK(
                        GetPtr(image, row[i], WIDTH / 2 + col * ((i) ? 1 : -1), WIDTH),
                        GREEN
                ) &&
                GetValueRGBYOBK(
                        GetPtr(image, row[i] + 1, WIDTH / 2 + col * ((i) ? 1 : -1), WIDTH),
                        GREEN
                )) {
                green_len[i] = (U16) (WIDTH / 2 - col);
                break;
            }
        }
    }

    int r = green_len[0] - green_len[1];

    int rResult = 0;
    if (r > MISSION_5_5_GREEN_BRIDGE_THRESHOLDS + MISSION_5_5_GREEN_BRIDGE_ERROR) {
        Action_LEFT_MOVE_SHORT(1);
    } else if (r < MISSION_5_5_GREEN_BRIDGE_THRESHOLDS - MISSION_5_5_GREEN_BRIDGE_ERROR) {
        Action_RIGHT_MOVE_SHORT(1);
    } else {
        rResult = 1;
    }

    return rResult;
}

void mission_5_5_watch_below(void) {
    Action_WATCH_BELOW_LONG();
}

int mission_5_5_check_finish_black_line(U16 *image) {
    return mission_5_1_check_black_line(image, 0);
}

int mission_5_5_check_green_bridge_straight(U16 *image) {
    U32 col, i;

    U16 green_len[2] = {0,}, row[2] = {100, 30};

    for (col = 90; col > 0; --col) {
        green_len[0] += GetValueRGBYOBK(GetPtr(image, row[0], col, WIDTH), GREEN);
        green_len[1] += GetValueRGBYOBK(GetPtr(image, row[1], col, WIDTH), GREEN);
    }

    int rResult = 0;
    int slope;

    if ((green_len[1] - green_len[0]) == 0) {
        Action_LEFT_TURN_BODY(2);
    } else {
        slope = -70 / (green_len[1] - green_len[0]);
        if (slope < MISSION_5_SLOPE - MISSION_5_SLOPE_RANGE)
            Action_RIGHT_TURN_BODY(2);
        else if (slope > MISSION_5_SLOPE + MISSION_5_SLOPE_RANGE)
            Action_LEFT_TURN_BODY(2);
        else
            rResult = 1;
    }

    return rResult;
}

int mission_5_5_check_green_bridge_center(U16 *image) {
    mission_5_4_set_center_before_green_bridge(image);
}

int mission_5_5_short_walk_on_green_bridge(int repeat) {
    Action_WALK_FRONT_SHORT(repeat);
    return 1;
}


int mission_5_7_climb_down_stairs(void) {
    Action_CLIMB_DOWN_STAIRS();
    return 1;
}