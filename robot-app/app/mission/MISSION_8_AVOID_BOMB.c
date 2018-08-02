//
// Created by Gihyeon Yang on 2018. 7. 22..
//

#include "MISSION_8_AVOID_BOMB.h"

int mineset = 0;

int mission_8_1_is_not_front_of_bomb(U16 *image) {

    if (mineset++ < 4) {
        return 0;
    }

    U32 cols[6] = {50, 70, 90, 110, 130}, row, i, cnt;
    U16 checkHurdleLine[5] = {0,}, tmp;
    int range = 0;

    for (i = 0; i < 5; ++i) {
        for (row = HEIGHT - 1; row > 0; --row) {
            cnt = 0;
            for (range = -5; range < 5; range++) {
                cnt += GetValueRGBYOBK(GetPtr(image, row, cols[i] + range, WIDTH), BLACK);
            }

            if (cnt > 7) {
                checkHurdleLine[i] = (U16) row;
                break;
            }
        }
    }

    if (checkHurdleLine[0] + checkHurdleLine[1] + checkHurdleLine[2] + checkHurdleLine[3] + checkHurdleLine[4] < 80) {
        printf("yet...\n\n");
        return 0;
    }

    int j;
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4 - i; ++j) {
            if (checkHurdleLine[i] > checkHurdleLine[i + 1]) {
                tmp = checkHurdleLine[i];
                checkHurdleLine[i] = checkHurdleLine[i + 1];
                checkHurdleLine[i + 1] = tmp;
            }
        }
    }

    printf("AVG: %d\n", checkHurdleLine[2]);

    return (40 - 10 <= checkHurdleLine[2]);
}