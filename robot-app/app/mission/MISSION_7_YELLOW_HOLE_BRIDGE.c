//
// Created by Gihyeon Yang on 2018. 7. 11..
//

#include "MISSION_7_YELLOW_HOLE_BRIDGE.h"

void mission_7_1_watch_below(int repeat) {
    Action_WALK_FRONT_LONG(repeat);
    RobotSleep(1);
    Action_WATCH_BELOW_LONG();
    RobotSleep(1);
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
        RobotSleep(1);
    }

    return rReturn;
}


void mission_7_2_watch_side(void) {
    Action_LEFT_TURN_HEAD_LONG();
    RobotSleep(1);
}

int mission_7_2_before_bridge_set_center(U16 *image) {

    U32 row, i;
    U16 black_len[2] = {0,}, col[2] = {
            MISSION_7_2_BLACK_LINE_COL_POINT_1,
            MISSION_7_2_BLACK_LINE_COL_POINT_2
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


    printf("M7-2: SET CENTER\n");
    printf("black[0]: %d, black_len[1]: %d.\n", black_len[0], black_len[1]);

    black_len[0] = (U16) ((black_len[0] + black_len[1]) / 2);

    printf("length : %d\n", black_len[0]);

    int rResult = 0;
    if (black_len[0] < MISSION_7_2_BLACK_LINE_RANGE - MISSION_7_2_BLACK_LINE_ERROR) {
        Action_RIGHT_MOVE_SHORT(10);
    } else if (black_len[0] > MISSION_7_2_BLACK_LINE_RANGE + MISSION_7_2_BLACK_LINE_ERROR) {
        Action_LEFT_MOVE_SHORT(10);
    } else {
        rResult = 1;
    }

    if (!rResult) {
        RobotSleep(1);
    }
    Action_WALK_FRONT_SHORT(3);
    return rResult;
}


int mission_7_3_climb_yellow_hole_bridge() {
    RobotSleep(1);
    Action_CLIMB_UP_STAIRS();
    Action_INIT_ROBOT();
    RobotSleep(1);
    Action_WALK_FRONT_SHORT(3);
    RobotSleep(1);
    return 0;
}

int mission_7_4_set_straight_on_yellow_bridge(U16 *image) {

    U32 row, i;
    U16 yellow_len[2] = {0,}, col[2] = {
            MISSION_7_4_YELLOW_LINE_COL_POINT_1,
            MISSION_7_4_YELLOW_LINE_COL_POINT_2
    };

    for (i = 0; i < 2; ++i) {
        for (row = HEIGHT; row > 0; --row) {
            if (GetValueRGBYOBK(
                        GetPtr(image, row, col[i], WIDTH),
                        YELLOW
                ) &&
                GetValueRGBYOBK(
                        GetPtr(image, row, col[i] + 1, WIDTH),
                        YELLOW
                )) {
                break;
            }
            yellow_len[i] += 1;
        }
    }

    printf("M7-4: SLOPE\n");
    printf("yellow[0]: %d, yellow_len[1]: %d.\n", yellow_len[0], yellow_len[1]);


    double s = (
            (yellow_len[0] - yellow_len[1]) /
            (MISSION_7_4_YELLOW_LINE_COL_POINT_1 - MISSION_7_4_YELLOW_LINE_COL_POINT_2)
    );

    printf("Slope : %f\n", s);

    s *= 100;

    int rResult = 0;
    if (s < MISSION_7_4_YELLOW_LINE_SLOPE + MISSION_7_4_YELLOW_LINE_SLOPE_ERROR) {
        Action_RIGHT_TURN_BODY(1);
    } else if (s > MISSION_7_4_YELLOW_LINE_SLOPE - MISSION_7_4_YELLOW_LINE_SLOPE_ERROR) {
        Action_LEFT_TURN_BODY(1);
    } else {
        rResult = 1;
    }

    if (!rResult) {
        RobotSleep(1);
    }

    return rResult;
}

int mission_7_4_set_center_on_yellow_bridge(U16 *image) {

    U32 row, i;
    U16 yellow_len[2] = {0,}, col[2] = {
            MISSION_7_4_YELLOW_LINE_COL_POINT_1,
            MISSION_7_4_YELLOW_LINE_COL_POINT_2
    };

    for (i = 0; i < 2; ++i) {
        for (row = HEIGHT; row > 0; --row) {
            if (GetValueRGBYOBK(
                        GetPtr(image, row, col[i], WIDTH),
                        YELLOW
                ) &&
                GetValueRGBYOBK(
                        GetPtr(image, row, col[i] + 1, WIDTH),
                        YELLOW
                ))
                break;
            yellow_len[i] += 1;
        }
    }


    printf("M7-4: SET CENTER\n");
    printf("yellow_len[0]: %d, yellow_len[1]: %d.\n", yellow_len[0], yellow_len[1]);


    yellow_len[0] = (U16) ((yellow_len[0] + yellow_len[1]) / 2);

    printf("length : %d\n", yellow_len[0]);

    int rResult = 0;
    if (yellow_len[0] < MISSION_7_4_YELLOW_LINE_RANGE - MISSION_7_4_YELLOW_LINE_ERROR) {
        Action_RIGHT_MOVE_SHORT(3);
    } else if (yellow_len[0] > MISSION_7_4_YELLOW_LINE_RANGE + MISSION_7_4_YELLOW_LINE_ERROR) {
        Action_LEFT_MOVE_SHORT(3);
    } else {
        rResult = 1;
    }

    if (!rResult) {
        RobotSleep(1);
    }

    return rResult;
}

void mission_7_5_watch_below(void) {
    Action_WATCH_BELOW_LONG();
    RobotSleep(1);
}

int mission_7_5_walk_until_black_line(U16 *image) {

    U32 col, row;
    U16 black_cnt = 0;

    for (row = 0; row < MISSION_7_5_HEIGHT_SCOPE; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            if (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK) == 1)
                black_cnt++;
        }
    }

    double ratio = black_cnt / (WIDTH * MISSION_7_5_HEIGHT_SCOPE) * 100;

    printf("7-5: walk_until_black_line\n");
    printf("Ratio is %f.\n", ratio);

    int rResult = 0;
    if (ratio < MISSION_7_5_BLACK_RATIO - MISSION_7_5_BLACK_RATIO_ERROR &&
        ratio > MISSION_7_5_BLACK_RATIO + MISSION_7_5_BLACK_RATIO_ERROR) {
        rResult = 1;
    }

    Action_INIT_ROBOT();
    RobotSleep(1);

    return rResult;
}

int mission_7_6_jump_hole(void) {
    RobotSleep(1);
    Action_MISSION_7_YELLOW_HOLE_BRIDGE_DUMBLING();
    RobotSleep(1);
    return 1;
}

int mission_7_7_after_yellow_bridge_set_straight(U16 *image) {
    U32 row, i;
    U16 black_len[2] = {0,}, col[2] = {
            MISSION_7_2_BLACK_LINE_COL_POINT_1,
            MISSION_7_2_BLACK_LINE_COL_POINT_2
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

    printf("M7-4: SLOPE\n");
    printf("black[0]: %d, black_len[1]: %d.\n", black_len[0], black_len[1]);

    double s = (
            (black_len[0] - black_len[1]) /
            MISSION_7_2_BLACK_LINE_COL_POINT_1 - MISSION_7_2_BLACK_LINE_COL_POINT_2
    );

    printf("Slope : %f\n", s);

    s *= 100;
    int rResult = 0;
    if (s < MISSION_7_7_BLACK_LINE_SLOPE + MISSION_7_7_BLACK_LINE_SLOPE_ERROR) {
        Action_RIGHT_TURN_BODY(3);
    } else if (s > MISSION_7_7_BLACK_LINE_SLOPE - MISSION_7_7_BLACK_LINE_SLOPE_ERROR) {
        Action_LEFT_TURN_BODY(3);
    } else {
        rResult = 1;
    }
    if (!rResult) {
        RobotSleep(1);
    }

    return rResult;
}

int mission_7_7_after_yellow_bridge_set_center(U16 *image) {
    return mission_7_2_before_bridge_set_center(image);
}




