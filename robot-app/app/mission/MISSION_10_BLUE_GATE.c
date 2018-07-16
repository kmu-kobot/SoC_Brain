
//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#include "MISSION_10_BLUE_GATE.h"

void mission_10_1_watch_up(void) {
    //TODO: 로봇 고개 드는 동작 넣기
    RobotSleep(5);
}


void mission_10_1_front_walk(int repeat) {
    Action_WALK_FRONT_LONG(repeat);
    RobotSleep(5);
}

int mission_10_1_catch_blue_gate(U16 *image) {
    U32 col, row;
    U16 blue_cnt = 0;

    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            blue_cnt += GetValueRGBYOBK(
                    GetPtr(image, row, col, WIDTH),
                    BLUE
            );
        }
    }

    return (blue_cnt * 100) / (HEIGHT * WIDTH) > MISSION_10_1_BLUE_GATE_RATIO;
}

int mission_10_2_set_center_upper_gate(U16 *image) {
    U32 col, row;
    U16 pos_blue[2] = {0,};

    for (col = 0; col < WIDTH; ++col) {
        for (row = 0; row < HEIGHT; ++row) {
            if (GetValueRGBYOBK(
                        GetPtr(image, row, col, WIDTH),
                        BLUE
                ) &&
                GetValueRGBYOBK(
                        GetPtr(image, row, col + 1, WIDTH),
                        BLUE
                )) {
                pos_blue[0] = (U16) col;
                break;
            }
        }
        if (pos_blue[0] != 0) {
            break;
        }
    }

    for (col = WIDTH; col > 0; --col) {
        for (row = 0; row < HEIGHT; ++row) {
            if (GetValueRGBYOBK(
                        GetPtr(image, row, col, WIDTH),
                        BLUE
                ) &&
                GetValueRGBYOBK(
                        GetPtr(image, row, col + 1, WIDTH),
                        BLUE
                )) {
                pos_blue[1] = (U16) (WIDTH - col);
                break;
            }
        }
        if (pos_blue[1] != 0) {
            break;
        }
    }

    printf("\nM10-1: SET CENTER\n");
    printf("pos_blue[0]: %d, pos_blue[1]: %d.\n\n", pos_blue[0], pos_blue[1]);

    int rResult = 0;
    if (pos_blue[0] - pos_blue[1] > MISSION_10_2_ERROR_BETWEEN_COLS) {
        Action_RIGHT_MOVE_SHORT(3);
    } else if (pos_blue[0] - pos_blue[1] < -MISSION_10_2_ERROR_BETWEEN_COLS) {
        Action_LEFT_MOVE_SHORT(3);
    } else {
        rResult = 1;
    }
    RobotSleep(3);

    return rResult;
}

int mission_10_3_set_straight_upper_gate(U16 *image) {

    U32 col, row;
    U16 pos_blue_col[2] = {0,}, pos_blue_row[2] = {0,};

    for (col = 0; col < WIDTH; ++col) {
        for (row = 0; row < HEIGHT; ++row) {
            if (GetValueRGBYOBK(
                        GetPtr(image, row, col, WIDTH),
                        BLUE
                ) &&
                GetValueRGBYOBK(
                        GetPtr(image, row, col + 1, WIDTH),
                        BLUE
                )) {
                pos_blue_col[0] == col;
                pos_blue_row[0] == row;
                break;
            }
        }
        if (pos_blue_col[0] != 0) {
            break;
        }
    }

    for (col = WIDTH; col > 0; --col) {
        for (row = 0; row < HEIGHT; ++row) {
            if (GetValueRGBYOBK(
                        GetPtr(image, row, col, WIDTH),
                        BLUE
                ) &&
                GetValueRGBYOBK(
                        GetPtr(image, row, col + 1, WIDTH),
                        BLUE
                )) {
                pos_blue_col[0] == col;
                pos_blue_row[0] == row;
                break;
            }
        }
        if (pos_blue_col[0] != 0) {
            break;
        }
    }

    double s = (
            (pos_blue_row[0] - pos_blue_row[1]) /
            pos_blue_col[0] - pos_blue_col[1]
    );

    printf("Slope : %f\n", s);

    s *= 100;

    RobotSleep(3);
    int rResult = 0;
    if (s < MISSION_10_3_UPPER_BLUE_GATE_SLOPE + MISSION_10_3_UPPER_BLUE_GATE_SLOPE_ERROR) {
        Action_RIGHT_TURN_BODY(1);
    } else if (s > MISSION_10_3_UPPER_BLUE_GATE_SLOPE - MISSION_10_3_UPPER_BLUE_GATE_SLOPE_ERROR) {
        Action_LEFT_TURN_BODY(1);
    } else {
        rResult = 1;
    }

    return rResult;

}

int mission_10_4_escape_blue_gate() {
    Action_WALK_FRONT_LONG(5);
    RobotSleep(3);
}




