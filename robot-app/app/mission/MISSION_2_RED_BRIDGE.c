//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#include "MISSION_2_RED_BRIDGE.h"

void mission_2_1_watch_below(int repeat) {
    Action_WALK_FRONT_LONG(repeat);
    Action_WATCH_BELOW_LONG();
}

int mission_2_1_wait_front_of_red_bridge(U16 *image, int repeat) {

    U32 col, row, cntRed = 0;
    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            cntRed += GetValueRGBYOBK(
                    GetPtr(image, row, col, WIDTH),
                    RED
            );
        }
    }

    Action_INIT_ROBOT();

    int rReturn = (cntRed * 100 / (WIDTH * HEIGHT)) > MISSION_2_THRESHOLDS;

    if (rReturn) {
        Action_WALK_FRONT_SHORT(repeat);
    }

    return rReturn;
}

void mission_2_2_watch_side(void) {
    Action_LEFT_TURN_HEAD_LONG();
}

int mission_2_2_before_bridge_set_center(U16 *image) {
    U32 row, i;
    U16 black_len[2] = {0,}, col[2] = {
            MISSION_2_2_BLACK_LINE_COL_POINT_1,
            MISSION_2_2_BLACK_LINE_COL_POINT_2
    };

    for (i = 0; i < 2; ++i) {
        for (row = HEIGHT; row > 0; --row) {
            if (GetValueRGBYOBK(
                        GetPtr(image, row, col[i], WIDTH),
                        BLACK
                ) &&
                GetValueRGBYOBK(
                        GetPtr(image, row, col[i] + 1, WIDTH),
                        BLACK
                ))
                break;
            black_len[i] += 1;
        }
    }


    printf("M2-2: SET CENTER\n");
    printf("black[0]: %d, black_len[1]: %d.\n", black_len[0], black_len[1]);

    Action_INIT_ROBOT();

    black_len[0] = (U16) ((black_len[0] + black_len[1]) / 2);

    printf("length : %d\n", black_len[0]);

    int rResult = 0;
    if (black_len[0] < MISSION_2_2_BLACK_LINE_RANGE - MISSION_2_2_BLACK_LINE_ERROR) {
        Action_RIGHT_MOVE_SHORT(3);
    } else if (black_len[0] > MISSION_2_2_BLACK_LINE_RANGE + MISSION_2_2_BLACK_LINE_ERROR) {
        Action_LEFT_MOVE_SHORT(3);
    } else {
        rResult = 1;
    }

    Action_INIT_ROBOT();
    return rResult;
}

int mission_2_3_escape_red_bridge(void) {
//    Action_ESCAPE_RED_BRIDGE();
    return 1;
}

int mission_2_4_after_bridge_set_straight(U16 *image) {
    U32 row, i;
    U16 black_len[2] = {0,}, col[2] = {
            MISSION_2_4_BLACK_LINE_COL_POINT_1,
            MISSION_2_4_BLACK_LINE_COL_POINT_2
    };

    for (i = 0; i < 2; ++i) {
        for (row = HEIGHT; row > 0; --row) {
            if (GetValueRGBYOBK(
                        GetPtr(image, row, col[i], WIDTH),
                        BLACK
                ) &&
                GetValueRGBYOBK(
                        GetPtr(image, row, col[i] + 1, WIDTH),
                        BLACK
                )) {
                break;
            }
            black_len[i] += 1;
        }
    }

    printf("M2-4: SLOPE\n");
    printf("black[0]: %d, black_len[1]: %d.\n", black_len[0], black_len[1]);

    Action_INIT_ROBOT();

    double s = (
            (black_len[0] - black_len[1]) /
            MISSION_2_4_BLACK_LINE_COL_POINT_1 - MISSION_2_4_BLACK_LINE_COL_POINT_2
    );

    printf("Slope : %f\n", s);

    s *= 100;

    int rResult = 0;
    if (s < MISSION_2_4_BLACK_LINE_SLOPE + MISSION_2_4_BLACK_LINE_SLOPE_ERROR) {
        Action_RIGHT_TURN_BODY(1);
    } else if (s > MISSION_2_4_BLACK_LINE_SLOPE - MISSION_2_4_BLACK_LINE_SLOPE_ERROR) {
        Action_LEFT_TURN_BODY(1);
    } else {
        rResult = 1;
    }

    Action_INIT_ROBOT();

    return rResult;
}

int mission_2_5_after_bridge_set_center(U16 *image) {
    return mission_2_2_before_bridge_set_center(image);
}

