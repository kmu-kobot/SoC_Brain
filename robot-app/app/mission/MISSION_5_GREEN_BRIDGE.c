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

int mission_5_4_set_center_before_green_bridge(U16 *image) {
    U32 col, row, cntBlackLen[7] = {0,}, cnt, bk_len = 0, flag = 0;
    int i = -1;
    for (row = HEIGHT; row > 0; --row) {
        cnt = 0;
        for (col = MISSION_5_4_WIDTH_LEFT; col < MISSION_5_4_WIDTH_LEFT + MISSION_5_4_WIDTH_RANGE; ++col) {
            cnt += GetValueRGBYOBK(
                    GetPtr(image, row, col, WIDTH),
                    BLACK
            );
        }

        if (cnt >= MISSION_5_4_BLACK_LEN_THRESHOLDS) {

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

    printf("\t\t- M5-4: SET CENTER\n");
    printf("\t\t\t+ bk_len row: %d, bk: \n", bk_len, i);

    int rResult = 0;
    if (i < MISSION_5_4_NUMBER_OF_BLACK_LEN ||
        bk_len > MISSION_5_4_BLACK_LEN_RANGE + MISSION_5_4_BLACK_LEN_ERROR) {
        Action_LEFT_MOVE_SHORT(3);
    } else if (i > MISSION_5_4_NUMBER_OF_BLACK_LEN ||
               bk_len < MISSION_5_4_BLACK_LEN_RANGE - MISSION_5_4_BLACK_LEN_ERROR) {
        Action_RIGHT_MOVE_SHORT(3);
    } else {
        rResult = 1;
    }

    return rResult;
}

int mission_5_4_set_front_of_green_bridge(U16 *image, int repeat) {
    U32 col, row, cntGreen = 0;
    for (row = 0; row < HEIGHT; ++row) {
        for (col = 0; col < WIDTH; ++col) {
            cntGreen += GetValueRGBYOBK(
                    GetPtr(image, row, col, WIDTH),
                    GREEN
            );
        }
    }

    printf("\t\t- M5-4: SET FRONT OF GREEN BRIDGE\n");
    printf("\t\t\t+ cntGreen: %d\n", cntGreen);
    printf("\t\t\t+ GREEN / AREA: %f\n\n", (double) cntGreen * 100 / (WIDTH * HEIGHT));

    int rReturn = (cntGreen * 100 / (WIDTH * HEIGHT)) > MISSION_5_4_FRONT_OF_GREEN_BRIDGE_THRESHOLDS;

    if (rReturn) {
        Action_WALK_FRONT_SHORT(repeat);
    }
    RobotSleep(5);

    return rReturn;
}

void mission_5_5_watch_below(void) {
    Action_WATCH_BELOW_LONG();
    RobotSleep(5);
}

int mission_5_5_check_finish_black_line(U16 *image) {
    return mission_5_1_check_black_line(image, 0);
}

int mission_5_5_check_green_bridge_straight(U16 *image) {
    U32 col, i, cnt, range, point[2][2] = {{0, MISSION_5_5_GREEN_BRIDGE_POINT_Y_1},
                                           {0, MISSION_5_5_GREEN_BRIDGE_POINT_Y_2}};

    U32 max = MISSION_5_5_GREEN_BRIDGE_SLOPE_RANGE + 1;
    int pixelQueue[max] = {0,};
    int front = 0, rear = 0;

    for (i = 0; i < 2; ++i) {
        front = 0;
        rear = 0;
        for (col = 0; col < WIDTH; ++col) {

            if (front == rear) {
                rear = ++rear % max;
                for (range = 0; range < MISSION_5_5_GREEN_BRIDGE_SLOPE_RANGE; ++range) {
                    pixelQueue[rear] = GetValueRGBYOBK(
                            GetPtr(image, point[i][1], col + range, WIDTH),
                            GREEN
                    );
                    rear = ++rear % max;
                }
            } else {
                front = ++front % max;
                pixelQueue[rear] = GetValueRGBYOBK(
                        GetPtr(image, point[i][1], col + range, WIDTH),
                        GREEN
                );
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

    int r = (point[0][1] - point[1][1]);

    int rResult = 0;
    if (r < -MISSION_5_5_GREEN_BRIDGE_SLOPE) {
        Action_LEFT_TURN_BODY(3);
    } else if (r > MISSION_5_5_GREEN_BRIDGE_SLOPE) {
        Action_RIGHT_TURN_BODY(3);
    } else {
        rResult = 1;
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

            if (cnt >= 2) {
                green_len[dir] = WIDTH / 2 - col;
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
            Action_LEFT_MOVE_SHORT(4);
        } else {
            Action_RIGHT_MOVE_SHORT(4);
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


int mission_5_7_climb_down_stairs(void) {
    RobotSleep(5);
    Action_CLIMB_DOWN_STAIRS();
    RobotSleep(5);
    Action_WALK_FRONT_SHORT(5);
    RobotSleep(5);
    return 1;
}