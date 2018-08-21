//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#include "MISSION_4_JUMP_HURDLE.h"

void mission_4_1_watch_front(int repeat) {
    ACTION_WALK(FAST, OBLIQUE, repeat);
}

int mission_4_2_ready_hurdle(U16 *image) { // 굳이? 필요한가
    // U32 col[3] = {90, 70, 110}, row, i, cnt;
    // U16 checkHurdleLine[3] = {0,};
    // int range = 0;
    //
    // for (i = 0; i < 3; ++i) {
    //     for (row = HEIGHT - 1; row > 0; --row) {
    //         cnt = 0;
    //         for (range = -3; range < 3; ++range) {
    //             cnt += (GetValueRGBYOBK(GetPtr(image, row, col[i] + range, WIDTH), BLACK));
    //         }
    //
    //         if (cnt > 3) {
    //             checkHurdleLine[i] = (U16) row;
    //             break;
    //         }
    //     }
    // }
    //
    // double s = 0;
    // printf("BLACK LINE\n");
    // for (i = 0; i < 3; ++i) {
    //     s = (s > checkHurdleLine[i]) ? s : checkHurdleLine[i];
    //     printf("bk_line[%d]: %d,\t", i, checkHurdleLine[i]);
    // }
    // printf("\n");
    //
    // printf("AVG: %f\n", s);
    //
    // printf((CASE_4_0_LENGTH - CASE_4_0_LENGTH_ERROR <= s &&
    //         s <= CASE_4_0_LENGTH + CASE_4_0_LENGTH_ERROR) ? "SUCCESS\n"
    //                                                       : "FAIL\n");
    // return (CASE_4_0_LENGTH - CASE_4_0_LENGTH_ERROR <= s &&
    //         s <= CASE_4_0_LENGTH + CASE_4_0_LENGTH_ERROR);

    static int prev = -1;
    int dist = getDistance1(image, WIDTH>>1, HEIGHT-1, BLACK);

    if (dist == 0)
    {
        return 0;
    }

    if (abs(dist - prev) < 3)
    {
        prev = dist;
        return 1;
    }
    prev = dist;
    return 0;
}

int mission_4_4_jump_hurdle(void) {
    CHECK_INIT(OBLIQUE);
    RobotSleep(2);
    ACTION_MOTION(MISSION_4_HURDLING, OBLIQUE);
    RobotSleep(1);
    ACTION_WALK(FAST, OBLIQUE, 4);
    RobotSleep(3);
    return 1;
}

int mission_4_6_set_center(U16 *image, int length) { // 얘도 여러 프레임
    U32 col[3] = {85, 95, 90}, row, i;
    U16 checkHurdleLine[3] = {0,};
    int j;

    for (i = 0; i < 3; ++i) {
        for (row = HEIGHT - 1; row > 0; --row) {
            checkHurdleLine[i] = 0;

            for (j = -3; j < 3; j++) {
                checkHurdleLine[i] += GetValueRGBYOBK(GetPtr(image, row, col[i] + j, WIDTH), BLACK);
            }

            if (checkHurdleLine[i] > 3) {
                checkHurdleLine[i] = (U16) (HEIGHT - row);
                break;
            }
        }
    }

    double s = 0;
    printf("M4-5: BLACK LINE\n");
    for (i = 0; i < 3; ++i) {
        if (s < checkHurdleLine[i]) {
            s = checkHurdleLine[i];
        }
        printf("bk_line[%d]: %d,\t", i, checkHurdleLine[i]);
    }
    printf("\n");

    printf("M4-5: AVG: %f\n", s);

    int k = (length != 0) ? length : CASE_0_DEFAULT_RIGHT_RANGE;

    if (s < k - CASE_0_DEFAULT_RANGE_ERROR) {
        printf("LEFT\n\n");
        ACTION_MOVE(LONG, DIR_LEFT, RIGHT, 1);
        return 0;
    } else if (s > k + CASE_0_DEFAULT_RANGE_ERROR) {
        printf("RIGHT\n\n");
        ACTION_MOVE(LONG, DIR_RIGHT, RIGHT, 1);
        return 0;
    } else {
        printf("SUCCESS\n\n");
        return 1;
    }
}

// void mission_4_6_watch_side(void) {
//     CHECK_INIT(RIGHT);
//     RobotSleep(3);
// }
