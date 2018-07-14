//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#include "MISSION_5_GREEN_BRIDGE.h"

void mission_5_1_watch_below(int repeat) {
    Action_WALK_FRONT_LONG(repeat);
    RobotSleep();

    Action_WATCH_BELOW_LONG();
    RobotSleep();
}

int mission_5_1_check_black_line(U16 *image, int repeat) {
    U32 col, row;
    int cntBlack = 0;
    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; col++) {
            cntBlack += GetValueRGBYOBK(
                    GetPtr(image, row, col, WIDTH),
                    BLACK
            );
        }
    }

    int rResult = (cntBlack * 100 / (HEIGHT * WIDTH)) > MISSION_5_1_THRESHOLDS;

    printf("M5-1: BLACK LINE\n");
    printf("BLACK: %d, BLACK / (WIDTH * HEIGHT) : %f\n",
           cntBlack, (double) (cntBlack * 100 / (HEIGHT * WIDTH)));

    RobotSleep();
    if (rResult && repeat) {
        Action_WALK_FRONT_SHORT(repeat);
    }
    RobotSleep();

    return rResult;
}

void mission_5_2_watch_side(void) {
    Action_LEFT_TURN_HEAD_LONG();
    RobotSleep();
}

int mission_5_3_climb_up_stairs(void) {
    Action_WALK_FRONT_SHORT(3);
    RobotSleep();
    Action_CLIMB_UP_STAIRS();
    RobotSleep();
    Action_WALK_FRONT_SHORT(4);
    RobotSleep();
    return 1;
}

int mission_5_4_set_center_before_green_bridge(U16 *image) {
    // TODO: 센터 맞추는거 개발해야함
    int rResult = 1;
    return 1;
}

void mission_5_5_watch_below(void) {
    Action_WATCH_BELOW_LONG();
    RobotSleep();
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
    double slope;

    RobotSleep();
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
    RobotSleep();

    return rResult;
}

int mission_5_5_check_green_bridge_center(U16 *image) {
    int col, i, cnt, row, flagDirection, flagSign, halfWidth = WIDTH / 2, green_len[2] = {0,};
    for (i = 0; i < 2; ++i) {
        flagDirection = (i) ? 0 : halfWidth - 1;
        flagSign = (i) ? 1 : -1;
        for (col = 0; col < halfWidth; ++col) {
            cnt = 0;
            for (row = MISSION_5_5_GREEN_BRIDGE_THRESHOLDS;
                 row < MISSION_5_5_GREEN_BRIDGE_THRESHOLDS + MISSION_5_5_GREEN_BRIDGE_RANGE;
                 ++row) {
                cnt += GetValueRGBYOBK(
                        GetPtr(image, row, flagDirection + col * flagSign),
                        GREEN
                );
            }

            if (cnt >= 2) {
                green_len[i] += (U16) (halfWidth - col);
                break;
            }
        }
    }

    double r = (green_len[0] - green_len[1]) / 2;

    printf("M5-5: SET CENTER\n");
    printf("LEFT: %f, RIGHT: %f, r: %f\n",
           (double) green_len[0] / 2, (double) green_len[1] / 2, r);

    RobotSleep();
    if (((r > 0) ? r : (-r)) > MISSION_5_5_GREEN_BRIDGE_THRESHOLDS) {
        if (r > 0) {
            Action_LEFT_MOVE_SHORT(4);
        } else {
            Action_RIGHT_MOVE_SHORT(4);
        }
    }
    RobotSleep();

    return ((r > 0) ? r : (-r)) < MISSION_5_5_GREEN_BRIDGE_THRESHOLDS;
}

int mission_5_5_short_walk_on_green_bridge(int repeat) {
    RobotSleep();
    Action_WALK_FRONT_SHORT(repeat);
    RobotSleep();
    return 1;
}


int mission_5_7_climb_down_stairs(void) {
    RobotSleep();
    Action_CLIMB_DOWN_STAIRS();
    RobotSleep();
    return 1;
}