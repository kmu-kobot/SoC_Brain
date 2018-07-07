//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#include "MISSION_2_RED_BRIDGE.h"

int mission_2_1_wait_front_of_red_bridge(U16 *image, int repeat, int repeat1) {

    Action_WALK_FRONT_LONG(repeat);

    Action_WATCH_BELOW_SHORT();

    U32 col, row, cntRed = 0;
    for (row = MISSION_2_LOWER; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; col += 2) {
            cntRed += GetValueRGBYOBK(
                    GetPtr(image, row, col, WIDTH),
                    RED
            );
        }
    }

    Action_INIT_ROBOT();

    int rReturn = (cntRed * 2 * 100 / (WIDTH * (HEIGHT - MISSION_2_LOWER))) > MISSION_2_THRESHOLDS;

    if (rReturn) {
        Action_WALK_FRONT_SHORT(repeat1);
    }

    return rReturn;
}

int mission_2_2_before_bridge_set_center(U16 *image) {

    Action_LEFT_TURN_HEAD_LONG();

    U32 col, i;
    U16 red_len[2] = {0,}, row[2] = {0, 30};

    for (col = 0; col < WIDTH; ++col) {
        for (i = 0; i < 2; ++i) {
            red_len[i] += GetValueRGBYOBK(
                    GetPtr(image, row[i], col, WIDTH),
                    RED
            );
        }
    }

    Action_INIT_ROBOT();

    int rResult = 0;
    if (red_len[0] < 55 && red_len[1] < 75) {
        Action_RIGHT_MOVE_SHORT(1);
    } else if (red_len[0] < 65 && red_len[1] < 85) {
        Action_LEFT_MOVE_SHORT(1);
    } else {
        rResult = 1;
    }

    return rResult;
}

int mission_2_3_escape_red_bridge(void) {
    Action_ESCAPE_RED_BRIDGE();
    return 1;
}

int mission_2_4_after_bridge_set_straight(U16 *image) {

    Action_LEFT_TURN_HEAD_LONG();

    U32 row, i, pos_bk[3] = {0,};
    U16 col[3] = {45, 90, 135};

    for (i = 0; i < 3; ++i) {
        for (row = HEIGHT; row > 0; row--) {
            if (
                    GetValueRGBYOBK(
                            GetPtr(image, row, col[i], WIDTH),
                            BLACK
                    ) == 1
                    ) {
                pos_bk[i] = row;
                break;
            }
        }
    }

    Action_INIT_ROBOT();

    int rResult = 0;
    if (
            (pos_bk[2] - pos_bk[1]) < -10 &&
            (pos_bk[1] - pos_bk[0] < -10)
            ) {
        Action_RIGHT_TURN_BODY_LONG(1);
    } else if (
            (pos_bk[2] - pos_bk[1]) > 10 &&
            (pos_bk[1] - pos_bk[0] > 10)
            ) {
        Action_LEFT_TURN_BODY_LONG(1);
    } else {
        rResult = 1;
    }

    return rResult;
}

int mission_2_5_after_bridge_set_center(U16 *image) {

    Action_LONG_TURN_LEFT_HEAD();

    U32 col, i;

    U16 black_len[2] = {0,}, row[2] = {0, 50};

    for (col = 0; col < WIDTH; ++col) {
        for (i = 0; i < 2; ++i) {
            black_len[i] += GetValueRGBYOBK(
                    GetPtr(image, row[i], col, WIDTH),
                    BLACK
            );
        }
    }

    Action_INIT_ROBOT();

    int rResult = 0;

    // TODO : 길이 바꾸끼!!!!!!!!
    if (black_len[0] < 55 && black_len[1] < 75) {
        Action_RIGHT_SHORT_MOVE(1);
    } else if (black_len[0] < 65 && black_len[1] < 85) {
        Action_LEFT_SHORT_MOVE(1);
    } else {
        rResult = 1;
    }

    return rResult;
}
