//
// Created by Gihyeon Yang on 2018. 7. 22..
//

#include "MISSION_8_AVOID_BOMB.h"

int mission_8_1_is_not_front_of_bomb(U16 *image) {
    U32 cols[3] = {70, 90, 110}, row, i, cnt;
    U16 checkHurdleLine[3] = {0,};
    int range = 0;

    for (i = 0; i < 3; ++i) {
        for (row = HEIGHT - 1; row > 0; --row) {
            cnt = 0;
            for (range = -5; range < 5; range++) {
                cnt += GetValueRGBYOBK(GetPtr(image, row, cols[i] + range, WIDTH), BLACK);
            }

            if (cnt > 6) {
                checkHurdleLine[i] = (U16) row;
                break;
            }
        }
    }

    if (checkHurdleLine[0] + checkHurdleLine[1] + checkHurdleLine[2] < 50) {
        printf("yet...\n\n");
        return 0;
    }

    int s = 120;
    printf("BLACK LINE\n");
    for (i = 0; i < 3; ++i) {
        if (s >= checkHurdleLine[i]) {
            s = checkHurdleLine[i];
        }
        printf("bk_line[%d]: %d,\t", i, checkHurdleLine[i]);
    }

    printf("\n");

    printf("AVG: %d\n", s);

    return (40 - 10 <= s);
}