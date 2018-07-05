//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#include "MISSION_2_RED_BRIDGE.h"

int mission_2_1_wait_front_of_red_bridge(U16 *image, int repeat, int repeat1) {
    U32 col, row, cntRed = 0;
    for (col = 0; col < repeat; ++col) {
        RobotAction(WALK_FRONT); // TODO: 걷기 동작 넣기
    }

    RobotAction(READY_SHOW_FLOOR); // TODO: 바닥 보기 위한 준비 동작

    for (row = MISSION_2_LOWER; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; col += 2) {
            cntRed += GetValueRGBYOBK(
                    GetPtr(image, row, col, WIDTH),
                    RED
            );
        }
    }

    RobotAction(INIT_ROBOT); // TODO: 걷기 위한 준비 동작

    int rReturn = (cntRed * 2 / (WIDTH * (HEIGHT - MISSION_2_LOWER))) > MISSION_2_THRESHOLDS;

    if (rReturn) {
        mission_2_2_short_walk(repeat1);
    }

    return rReturn;
}

int mission_2_2_short_walk(int repeat) {
    U32 i;
    for (i = 0; i < repeat; ++i) {
        RobotAction(SHORT_WALK_FRONT);
    }
    RobotAction(INIT_ROBOT);
    return 1;
}

int mission_2_2_before_bridge_set_center(U16 *image) {

    RobotAction(LONG_TURN_LEFT_HEAD);

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

    RobotAction(INIT_ROBOT);

    int rResult = 0;

    if (red_len[0] < 55 && red_len[1] < 75) {
        RobotAction(RIGHT_SHORT_MOVE);
    } else if (red_len[0] < 65 && red_len[1] < 85) {
        RobotAction(LEFT_SHORT_MOVE);
    } else {
        rResult = 1;
    }

    return rResult;
}

int mission_2_3_escape_red_bridge() {
    RobotAction(MISSION_2_READY_CREEP_MOTION);

    U32 i;
    for (i = 0; i < 4; ++i) {
        RobotAction(MISSION_2_CREEP);
    }

    RobotAction(INIT_ROBOT); // 일어나는 동작

    return 1;
}

int mission_2_4_after_bridge_set_straight(U16 *image) {

    RobotAction(LONG_TURN_LEFT_HEAD);

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

    RobotAction(INIT_ROBOT); // 원래 로봇 모습으로 바꾸기!

    int rResult = 0;
    if (
            (pos_bk[2] - pos_bk[1]) < -10 &&
            (pos_bk[1] - pos_bk[0] < -10)
            ) {
        RobotAction(TURN_RIGHT_BODY);
    } else if (
            (pos_bk[2] - pos_bk[1]) > 10 &&
            (pos_bk[1] - pos_bk[0] > 10)
            ) {
        RobotAction(TURN_LEFT_BODY);
    } else {
        rResult = 1;
    }

    return rResult;
}

int mission_2_5_after_bridge_set_center(U16 *image) {

    RobotAction(LONG_TURN_LEFT_HEAD);

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

    RobotAction(INIT_ROBOT);

    int rResult = 0;

    // TODO : 길이 바꾸끼!!!!!!!!
    if (black_len[0] < 55 && black_len[1] < 75) {
        RobotAction(RIGHT_SHORT_MOVE);
    } else if (black_len[0] < 65 && black_len[1] < 85) {
        RobotAction(LEFT_SHORT_MOVE);
    } else {
        rResult = 1;
    }

    return rResult;
}