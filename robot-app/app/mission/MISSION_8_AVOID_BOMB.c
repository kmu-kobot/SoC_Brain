//
// Created by Gihyeon Yang on 2018. 7. 22..
//

#include "MISSION_8_AVOID_BOMB.h"

int mineset = 0;

int mission_8_1_is_not_front_of_bomb(U16 *image) {
    if (mineset++ < 3) {
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

    printf("\n\n");
    for (i = 0; i < 5; ++i) {
        printf("%d\t", checkHurdleLine[i]);
    }
    printf("\n\n");

    if (checkHurdleLine[0] + checkHurdleLine[1] + checkHurdleLine[2] + checkHurdleLine[3] + checkHurdleLine[4] < 80) { // 조금 더 세분화 할 필요가 있는듯
        printf("yet...\n\n");
        return 0;
    }

    int j;
    for (i = 0; i < 4; ++i) { // 솔팅 알고리즘 바꾸는걸로 / 예를들어 간단하게 sort()함수라던지
        for (j = 0; j < 4 - i; ++j) {
            if (checkHurdleLine[i] > checkHurdleLine[i + 1]) {
                tmp = checkHurdleLine[i];
                checkHurdleLine[i] = checkHurdleLine[i + 1];
                checkHurdleLine[i + 1] = tmp;
            }
        }
    }

    printf("AVG: %d\n", checkHurdleLine[2]); // AVG가 아니라 MEDIAN인듯?

    return (60 <= checkHurdleLine[2]); // 이렇게 리턴하면 잘 잡힘?? 좀 불안한데
}

int walkCnt = 0;

int mission_8_4_check_finish_line(U16 *image) {
    if (++walkCnt > 20) {

        ACTION_WALK(FAST, OBLIQUE, 30);

        return 1;
    }

    U32 col, row, cnt, line = 179;

    cnt = 0;
    for (col = 0; col < WIDTH; ++col) {
        for (row = 0; row < HEIGHT; ++row) {
            cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), YELLOW);
        }
    }

    if ((double) cnt * 100 / (WIDTH * HEIGHT) > 10) {
        return 1;
    }

    for (col = 0; col < WIDTH; ++col) { // for문 이렇게 돌릴 필요가 있나? 좀 더 영역을 좁게 잡거나 row가 밖으로 오게 하거나
        cnt = 0;
        for (row = 0; row < HEIGHT; ++row) {
            cnt += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK);
        }

        if (cnt < 4) { // 연속된 검은색을 찾았으면 좋겠음
            line = col;
            break;
        } else {
            line = 179;
        }

    }

    return line < 100;

}
