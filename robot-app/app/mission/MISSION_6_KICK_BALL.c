//
// Created by Gihyeon Yang on 2018. 7. 13..
//

#include "MISSION_6_KICK_BALL.h"

void mission_6_1_watch_front(void) {
    ACTION_INIT(LOW, OBLIQUE);
    RobotSleep(3);
}

int mission_6_1_detection_ball(U16 *image) {
    U32 col, row, sum = 0, cnt, detCol = 0, detection = 0;

    for (row = 0; row < HEIGHT; ++row) {
        cnt = 0;
        for (col = 0; col < WIDTH; ++col) {
            cnt += (GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW) ||
                    GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), RED));
        }
        sum += cnt;

        if (cnt > detCol) {
            detCol = cnt;
            detection = col;
        }

    }

    if (sum < 1) {
        return 1;
    }

    // 3        2           1           0
    // (0, 30)  (30, 60)    (60, 80)    (80, 85)

    // R        LEFT     RIGHT
    // 3:       (0, 30)  (150, 180)
    // 2:       (30, 60) (120, 150)
    // 1:       (60, 80) (90, 120)
    // FRONT:   (80, 90)

    int range = 4;
    if (detection < 80 || detection > 90) { // LEFT or RIGHT
        cnt = (U32) (detection < 80);
        detection = ((detection < 80) ? detection : WIDTH - detection);
        range = (0 <= detection && detection < 30) ? 3 :
                (30 <= detection && detection < 60) ? 2 :
                (60 <= detection && detection < 80) ? 1 : 0;
        if (cnt) {
            ACTION_MOVE(LONG, DIR_LEFT, LOW, OBLIQUE, range);
        } else {
            ACTION_MOVE(LONG, DIR_RIGHT, LOW, OBLIQUE, range);
        }
    } else {
        ACTION_WALK(FAST, OBLIQUE, range);
    }

    return 0;
}

void mission_6_2_watch_below(void) {
    ACTION_INIT(LOW, DOWN);
    RobotSleep(3);
}

int mission_6_2_set_center_of_ball(U16 *image) {
    // TODO: 공 중심 찾는 알고리즘
    return 1;
}
