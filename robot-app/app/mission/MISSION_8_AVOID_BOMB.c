//
// Created by Gihyeon Yang on 2018. 7. 22..
//

#include "MISSION_8_AVOID_BOMB.h"

int mission_8_1_is_not_front_of_bomb(U16 *image) {
    U32 cols[3] = {80, 90, 100}, row, i, col;
    U16 checkHurdleLine[3] = {0,};

    for (i = 0; i < 3; ++i) {
        for (row = HEIGHT - 1; row > 0; --row) {
            if (GetValueRGBYOBK(GetPtr(image, row, cols[i], WIDTH), BLACK) &&
                GetValueRGBYOBK(GetPtr(image, row, cols[i] + 1, WIDTH), BLACK)) {
                checkHurdleLine[i] = (U16) row;
                break;
            }
        }
    }

    if (checkHurdleLine[0] + checkHurdleLine[1] + checkHurdleLine[2] < 50) {
        printf("yet...\n\n");
        return 0;
    }

    int s = 0;
    printf("BLACK LINE\n");
    for (i = 0; i < 3; ++i) {
        if (s <= checkHurdleLine[i]) {
            s = checkHurdleLine[i];
        }
        printf("bk_line[%d]: %d,\t", i, checkHurdleLine[i]);
    }

    printf("\n");

    printf("AVG: %d\n", s);

    return (40 - 10 <= s);
}