//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#include "MISSION_5_GREEN_BRIDGE.h"

void mission_5_1_watch_below(int repeat) {
    Action_WALK_FRONT_LONG(repeat);
    RobotSleep(5);

    Action_WATCH_BELOW_LONG();
    RobotSleep(5);
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

    if (rResult && repeat) {
        Action_WALK_FRONT_SHORT(repeat);
        RobotSleep(5);
    }

    return rResult;
}

void mission_5_2_watch_side(void) {
    Action_LEFT_TURN_HEAD_LONG();
    RobotSleep(5);
}

int mission_5_3_climb_up_stairs(void) {
    Action_WALK_FRONT_SHORT(3);
    RobotSleep(5);
    Action_CLIMB_UP_STAIRS();
    RobotSleep(5);
    Action_WALK_FRONT_SHORT(3);
    RobotSleep(5);
    return 1;
}

void mission_5_5_watch_below(void) {
    Action_WATCH_BELOW_LONG();
    RobotSleep(5);
}

int mission_5_5_check_finish_black_line(U16 *image) {
    return mission_5_1_check_black_line(image, 0);
}

int mission_5_5_check_green_bridge_straight(U16 *image) {
    U32 col, i, cnt, range = 0, point[2][2] = {{0, MISSION_5_5_GREEN_BRIDGE_POINT_Y_1},
                                               {0, MISSION_5_5_GREEN_BRIDGE_POINT_Y_2}};

    U32 max = MISSION_5_5_GREEN_BRIDGE_SLOPE_RANGE + 1;
    int pixelQueue[MISSION_5_5_GREEN_BRIDGE_SLOPE_RANGE + 1] = {0,};
    int front = 0, rear = 0;

    for (i = 0; i < 2; ++i) {
        front = 0;
        rear = 0;
        for (col = 0; col < WIDTH; ++col) {

            if (front == rear) {
                for (range = 0; range < MISSION_5_5_GREEN_BRIDGE_SLOPE_RANGE; ++range) {
                    pixelQueue[rear] = GetValueRGBYOBK(
                            GetPtr(image, point[i][1], col + range, WIDTH),
                            GREEN
                    );
                    rear++;
                    rear %= max;
                }
            } else {
                front++;
                front %= max;
                pixelQueue[rear] = GetValueRGBYOBK(
                        GetPtr(image, point[i][1], col + range, WIDTH),
                        GREEN
                );
                rear++;
                rear %= max;
            }

            cnt = 0;
            for (range = 0; range < MISSION_5_5_GREEN_BRIDGE_SLOPE_RANGE; ++range) {
                cnt += pixelQueue[range];
            }

            if (cnt >= 3) {
                point[i][0] = col;
                break;
            }

        }
    }

    int r = (point[0][0] - point[1][0]);

    int rResult = 1;
    if (((r > 0) ? r : -r) > MISSION_5_5_GREEN_BRIDGE_SLOPE) {
        rResult = 0;
        if (r > 0) {
            Action_RIGHT_TURN_BODY(2);
        } else {
            Action_LEFT_TURN_BODY(2);
        }
    }

    if (!rResult) { RobotSleep(5); }

    return rResult;
}

int mission_5_5_check_green_bridge_center(U16 *image) {
    U16 dir, cnt;
    int col, row, flagSign, green_len[2] = {0,};

    for (dir = 0; dir < 2; ++dir) {
        flagSign = (dir) ? 1 : -1;
        for (col = 0; col < WIDTH / 2; ++col) {
            cnt = 0;
            for (row = MISSION_5_5_GREEN_LINE_ROW_POINT;
                 row < MISSION_5_5_GREEN_LINE_ROW_POINT + MISSION_5_5_GREEN_BRIDGE_RANGE;
                 ++row) {
                if (CheckCol(WIDTH / 2 + ROBOT_CENTER_OFFSET + col * flagSign)) {
                    cnt += GetValueRGBYOBK(
                            GetPtr(image, row, WIDTH / 2 + ROBOT_CENTER_OFFSET + col * flagSign, WIDTH),
                            GREEN
                    );
                } else {
                    break;
                }
            }

            if (cnt < 3) {
                green_len[dir] = col;
                break;
            }
        }
    }

    // 0: LEFT, 1: RIGHT
    int r = green_len[0] - green_len[1];

    printf("\nM5-5: SET CENTER\n");
    printf("LEFT: %d, RIGHT: %d, r: %d\n\n", green_len[0], green_len[1], r);

    if (((r > 0) ? r : (-r)) > MISSION_5_5_GREEN_BRIDGE_THRESHOLDS) {
        if (r > 0) {
            Action_LEFT_MOVE_LONG(1);
            Action_RIGHT_MOVE_SHORT(4);
        } else {
            Action_RIGHT_MOVE_LONG(1);
            Action_LEFT_MOVE_SHORT(4);
        }
        RobotSleep(5);
    }

    return ((r > 0) ? r : (-r)) < MISSION_5_5_GREEN_BRIDGE_THRESHOLDS;
}

int mission_5_5_short_walk_on_green_bridge(int repeat) {
    RobotSleep(5);
    Action_WALK_FRONT_SHORT(repeat);
    RobotSleep(5);
    return 1;
}


int mission_5_6_set_only_one_bk_bar(U16 *image) {
    U32 col, row, cntBlackLen[3] = {0,}, cnt, bk_len = 0, flag = 0;
    int i = -1;
    for (row = HEIGHT - 10; row > 0; --row) {
        cnt = 0;
        for (col = MISSION_5_6_WIDTH_LEFT;
             col < MISSION_5_6_WIDTH_LEFT + MISSION_5_6_WIDTH_RANGE;
             ++col) {
            cnt += GetValueRGBYOBK(
                    GetPtr(image, row, col, WIDTH),
                    BLACK
            );
        }

        if (cnt >= MISSION_5_6_BLACK_LEN_THRESHOLDS) {

            if (!flag) {
                flag = 1;
                i++;
            }

            bk_len = HEIGHT - row;
            cntBlackLen[i] += 1;

        } else {
            flag = 0;
        }

    }

    printf("\t\t- M5-6: SET CENTER\n");
    printf("\t\t\t+ bk_len row: %d, bk: %d\n", bk_len, ((i != -1) ? i + 1 : 0));

    int rResult = 0;
    if (i != 0) {
        Action_WALK_FRONT_SHORT(1);
    } else {
        rResult = 1;
    }

    return rResult;
}

int mission_5_6_set_straight(U16 *image) {
    U32 cnt, row, i, range;
    int point[2][2] = {{MISSION_5_6_BLACK_COL_1, 0},
                       {MISSION_5_6_BLACK_COL_2, 0}};

    for (i = 0; i < 2; ++i) {
        for (row = 0; row < HEIGHT; ++row) {
            cnt = 0;
            for (range = 0; range < MISSION_5_6_BLACK_RANGE; ++range) {
                cnt += GetValueRGBYOBK(
                        GetPtr(image, row, point[i][0] + range, WIDTH),
                        BLACK
                );
            }

            if (cnt > 2) {
                point[i][1] = HEIGHT - row;
            }

        }
    }

    printf("\t\t- M5-6: SET STRAIGHT\n");
    printf("\t\t\t+ %d %d\n\n", point[0][1], point[1][1]);

    int r = (point[0][1] - point[1][1]);

    int rResult = 1;
    if (((r > 0) ? r : -r) > MISSION_5_6_GREEN_BRIDGE_SLOPE) {
        if (r < 0) {
            Action_RIGHT_TURN_BODY(1);
        } else {
            Action_LEFT_TURN_BODY(1);
        }
        rResult = 0;
        RobotSleep(5);
    }

    return rResult;
}

int mission_5_7_climb_down_stairs(void) {
    RobotSleep(5);
    Action_CLIMB_DOWN_STAIRS();
    RobotSleep(5);
    Action_WALK_FRONT_SHORT(5);
    RobotSleep(5);
    return 1;
}