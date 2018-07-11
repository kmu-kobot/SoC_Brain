//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#include "MISSION_5_GREEN_BRIDGE.h"

void mission_5_1_watch_below(int repeat) {
    Action_WALK_FRONT_LONG(repeat);

    Action_WATCH_BELOW_LONG();
}

int mission_5_1_check_black_line(U16 *image) {
    Action_INIT_ROBOT();

    U32 col, row, cntBlack = 0;
    for (row = 0; row < MISSION_5_1_BLACK_LINE_UPPER; ++row) {
        for (col = 0; col < WIDTH; col++) {
            cntBlack += GetValueRGBYOBK(
                    GetPtr(image, row, col, WIDTH),
                    BLACK
            );
        }
    }

    return (cntBlack / (MISSION_5_1_BLACK_LINE_UPPER) * WIDTH) > MISSION_5_1_THRESHOLDS;
}

void mission_5_4_watch_below(void) {
    Action_WATCH_BELOW_LONG();
}

int mission_5_4_check_finish_black_line(U16 *image) {
    return mission_5_1_check_black_line(image);
}

int mission_5_4_check_green_bridge_straight(U16 *image) {
    U32 col, i;

    U16 green_len[2] = {0,}, row[2] = {100, 30};

    for (col = 90; col > 0; --col) {
        green_len[0] += GetValueRGBYOBK(GetPtr(image, row[0], col, WIDTH), GREEN);
        green_len[1] += GetValueRGBYOBK(GetPtr(image, row[1], col, WIDTH), GREEN);
    }

    Action_INIT_ROBOT();

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

int mission_5_4_check_green_bridge_center(U16 *image) {
    U32 col, row, line[2] = {20, 40}, direction, cntGreen[2][2] = {{0, 0},
                                                                   {0, 0}};

    for (row = 0; row < 2; ++row) {
        for (col = 0; col < WIDTH / 2; col++) {
            for (direction = 0; direction < 2; ++direction) {
                cntGreen[row][direction] += GetValueRGBYOBK(
                        GetPtr(image, line[row], col + ((direction) ? 0 : WIDTH / 2), WIDTH),
                        GREEN
                );
            }
        }
    }

    cntGreen[0][0] += cntGreen[1][0];
    cntGreen[0][1] += cntGreen[1][1];

    cntGreen[0][0] /= 2;
    cntGreen[0][1] /= 2;

    col = cntGreen[0][0] - cntGreen[0][1];

    int rResult = 0;
    if (col > MISSION_5_4_GREEN_BRIDGE_THRESHOLDS + MISSION_5_4_GREEN_BRIDGE_ERROR) {
        Action_LEFT_MOVE_SHORT(3);
    } else if (col < MISSION_5_4_GREEN_BRIDGE_THRESHOLDS - MISSION_5_4_GREEN_BRIDGE_ERROR) {
        Action_RIGHT_MOVE_SHORT(3);
    } else {
        rResult = 1;
    }

    return rResult;

}

int mission_5_4_short_walk_on_green_bridge(int repeat) {
    Action_WALK_FRONT_SHORT(repeat);
    return 1;
}
