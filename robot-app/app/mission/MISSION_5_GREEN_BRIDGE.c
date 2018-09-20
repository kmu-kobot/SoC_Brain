//
// Created by Gihyeon Yang on 2018. 7. 9..
//

#include "MISSION_5_GREEN_BRIDGE.h"

void mission_5_1_watch_below(U16 *image, int repeat) {
    ACTION_WALK_CHECK(DOWN, image, mission_5_1_check_black_line, 1, repeat);
    RobotSleep(2);
}

int mission_5_1_attach(U16 *image) {
    double ratio = getColorRatio1(image, 30, ROBOT_KNEE, 50, WIDTH-50, BLACK);

#ifdef DEBUG
    printf("ratio : %f\n", ratio);
#endif

    if (ratio < 30.0)
    {
        ACTION_ATTACH(1);
        return 0;
    }
    return 1;
}

int mission_5_1_check_black_line(U16 *image) {
    U32 col, row, cntBlack = 0, dist = 0;

    for (row = ROBOT_KNEE - 10; row > 0; --row) {
        cntBlack = 0;
        for (col = 50; col < WIDTH - 50; col++) {
            cntBlack += GetValueRGBYOBK(GetPtr(image, row, col, WIDTH), BLACK);
        }
        if (cntBlack > 20) {
            dist = row;
            break;
        }
    }

#ifdef DEBUG
    printf("%d\n", dist);
#endif

    return dist > 20;
}

int mission_5_2_set_straight_and_center(U16 *image, U16 center)
{
    _line_t left_line, right_line;
    _line_t center_line;

    int left_state, right_state;
    left_state = mission_5_4_get_left_line(image, center, &left_line);
    right_state = mission_5_4_get_right_line(image, center, &right_line);

    if (!(left_state || right_state))
    {
        return 0;
    }
    if (!left_state)
    {
        if (right_line.slope / M_PI * 180.0 > 15.0)
        {
            ACTION_ATTACH(1);
            CHECK_INIT(OBLIQUE);
        }
        else
        {
            ACTION_MOVE(SHORT, DIR_LEFT, OBLIQUE, 2);
        }
        return 0;
    }
    if (!right_state)
    {
        if (left_line.slope / M_PI * 180.0 < -15.0)
        {
            ACTION_ATTACH(1);
            CHECK_INIT(OBLIQUE);
        }
        else
        {
            ACTION_MOVE(SHORT, DIR_RIGHT, OBLIQUE, 2);
        }
        return 0;
    }

    int center_state = mission_5_4_get_center_line(image, left_line, right_line, &center_line);

    if (center_state == 0)
    {
        return 0;
    }
    else if (center_state == -1)
    {
        return 2;
    }
    return mission_5_2_set_straight(center_line) && mission_5_4_set_center(center_line);
}

int mission_5_2_set_straight(_line_t line)
{
    double angle = atan(line.slope) / M_PI * 180.0;
    angle = abs(angle);

    if (angle > 15.0)
    {
        ACTION_ATTACH(1);
        CHECK_INIT(OBLIQUE);
    }
    else
    {
        return 1;
    }
    RobotSleep(1);

    return 0;
}

void mission_5_3_climb_up_stairs(void) {
    CHECK_INIT(UP);
    RobotSleep(3);
    ACTION_MOTION(MISSION_5_STAIR_UP, UP);
    RobotSleep(1);
    CHECK_INIT(OBLIQUE);
    RobotSleep(1);
}

int mission_5_4_set_straight_and_center(U16 *image, U16 center)
{
    _line_t left_line, right_line;
    _line_t center_line;

    int left_state, right_state;
    left_state = mission_5_4_get_left_line(image, center, &left_line);
    right_state = mission_5_4_get_right_line(image, center, &right_line);

    if (!(left_state || right_state))
    {
        return 0;
    }
    if (!left_state)
    {
        if (right_line.slope / M_PI * 180.0 > 10.0)
        {
            ACTION_TURN(MIDDLE, DIR_LEFT, OBLIQUE, 1);
        }
        else
        {
            ACTION_MOVE(SHORT, DIR_LEFT, OBLIQUE, 2);
        }
        return 0;
    }
    if (!right_state)
    {
        if (left_line.slope / M_PI * 180.0 < -10.0)
        {
            ACTION_TURN(MIDDLE, DIR_RIGHT, OBLIQUE, 1);
        }
        else
        {
            ACTION_MOVE(SHORT, DIR_RIGHT, OBLIQUE, 2);
        }
        return 0;
    }

    int center_state = mission_5_4_get_center_line(image, left_line, right_line, &center_line);

    if (center_state == 0)
    {
        return 0;
    }
    else if (center_state == -1)
    {
        return 2;
    }
    return mission_5_4_set_straight(center_line) && mission_5_4_set_center(center_line);
}

int mission_5_4_get_left_line(U16 *image, U16 center, _line_t *left_line)
{
    U16 i, j;
    _point_t points[NUM_LIN_REG_POINT];
    U8 green_cnt[3], pos;
    U32 point_cnt = 0;
    U32 out_cnt = 0;
    U16 up, down;

    for (i = IN_IMG(0, center - (NUM_LIN_REG_POINT>>1), HEIGHT); i < IN_IMG(0, center + (NUM_LIN_REG_POINT>>1), HEIGHT); ++i)
    {
        pos = 0;
        memset(green_cnt, 0, 3 * sizeof(U8));
        up = IN_IMG(0, i - 1, HEIGHT);
        down = IN_IMG(0, i + 1, HEIGHT);
        for (j = 0; j < WIDTH; ++j)
        {
            green_cnt[pos] = GetValueRGBYOBK(GetPtr(image, up, j, WIDTH), GREEN) +
                            GetValueRGBYOBK(GetPtr(image, i, j, WIDTH), GREEN) +
                            GetValueRGBYOBK(GetPtr(image, down, j, WIDTH), GREEN);

            pos = (pos + 1) % 3;

            if (green_cnt[0] + green_cnt[1] + green_cnt[2] > 2)
            {
                if(j < 2)
                {
                    ++out_cnt;
                    break;
                }
                points[point_cnt].x = j;
                points[point_cnt].y = i;
                ++point_cnt;
                break;
            }
        }
    }

#ifdef DEBUG
    printf("point_cnt : %d\tout_cnt : %d\n", point_cnt, out_cnt);
#endif

    if (point_cnt < 10)
    {
        return 0;
    }
    if ((double)out_cnt / (double)point_cnt > 0.7)
    {
        return 0;
    }

    return least_sqauresT(image, HEIGHT>>1, points, point_cnt, left_line);
}

int mission_5_4_get_right_line(U16 *image, U16 center, _line_t *right_line)
{
    U16 i, j;
    _point_t points[NUM_LIN_REG_POINT];
    U8 green_cnt[3], pos;
    U32 point_cnt = 0;
    U32 out_cnt = 0;
    U16 up, down;

    for (i = IN_IMG(0, center - (NUM_LIN_REG_POINT>>1), HEIGHT); i < IN_IMG(0, center + (NUM_LIN_REG_POINT>>1), HEIGHT); ++i)
    {
        pos = 0;
        memset(green_cnt, 0, 3 * sizeof(U8));
        up = IN_IMG(0, i - 1, HEIGHT);
        down = IN_IMG(0, i + 1, HEIGHT);
        for (j = WIDTH-1; j < 0xfff0; --j)
        {
            green_cnt[pos] = GetValueRGBYOBK(GetPtr(image, up, j, WIDTH), GREEN) +
                            GetValueRGBYOBK(GetPtr(image, i, j, WIDTH), GREEN) +
                            GetValueRGBYOBK(GetPtr(image, down, j, WIDTH), GREEN);

            pos = (pos + 1) % 3;

            if (green_cnt[0] + green_cnt[1] + green_cnt[2] > 2)
            {
                if(j > 177)
                {
                    ++out_cnt;
                    break;
                }
                points[point_cnt].x = j;
                points[point_cnt].y = i;
                ++point_cnt;
                break;
            }
        }
    }

#ifdef DEBUG
    printf("point_cnt : %d\tout_cnt : %d\n", point_cnt, out_cnt);
#endif

    if (point_cnt < 10)
    {
        return 0;
    }
    if ((double)out_cnt / (double)point_cnt > 0.7)
    {
        return 0;
    }

    return least_sqauresT(image, HEIGHT>>1, points, point_cnt, right_line);
}

int mission_5_4_get_center_line(U16 *image, _line_t left_line, _line_t right_line, _line_t *center_line)
{
    double left_degree = atan(left_line.slope);
    double right_degree = atan(right_line.slope);

    if (MAX(left_degree, right_degree) - MIN(left_degree, right_degree) > 70.0 / 180.0 * M_PI)
    {
        return -1;
    }

    center_line->slope = tan((left_degree + right_degree) / 2.0);

    double y = HEIGHT>>1;
    double left_dist = left_line.slope*y + left_line.intercept;
    double right_dist = right_line.slope*y + right_line.intercept;

    center_line->intercept = (left_dist + right_dist) / 2.0 - center_line->slope*y;

#ifdef DEBUG
    double angle = atan(center_line->slope)*180.0/M_PI;
    double distance = (center_line->slope)*y + (center_line->intercept);

    printf("slope : %f, angle : %f, intercept : %f, distance : %f\n\n", center_line->slope, angle, center_line->intercept, distance);
#endif

    drawlineT(image, *center_line, 0xffff);
    draw_fpga_video_data_full(image);
    flip();

    return 1;
}

int mission_5_4_set_straight(_line_t center_line)
{
    double angle = atan(center_line.slope) / M_PI * 180.0;
    DIRECTION turn_dir = angle < 0;
    angle = abs(angle);

    if (angle > 3.0)
    {
        ACTION_TURN(MIDDLE, turn_dir, OBLIQUE, 1);
    }
    else
    {
        return 1;
    }
    RobotSleep(2);

    return 0;
}

int mission_5_4_set_center(_line_t line)
{
    double center = (WIDTH>>1) - line.slope*(HEIGHT>>1) - line.intercept;
    DIRECTION move_dir = center < 0;
    center = abs(center);

    if (center > 8.0)
    {
        ACTION_MOVE(SHORT, move_dir, OBLIQUE, 3);
    }
    else if (center > 5.0)
    {
        ACTION_MOVE(SHORT, move_dir, OBLIQUE, 2);
    }
    else if (center > 3.0)
    {
        ACTION_MOVE(SHORT, move_dir, OBLIQUE, 1);
    }
    else
    {
        return 1;
    }
    RobotSleep(2);

    return 0;
}

void mission_5_5_attach_green(void)
{
    ACTION_WALK(FAST, OBLIQUE, 5);
}

int mission_5_6_set_straight_and_center(U16 *image, U16 center)
{
    _line_t left_line, right_line;
    _line_t center_line;

    int left_state, right_state;
    left_state = mission_5_4_get_left_line(image, center, &left_line);
    right_state = mission_5_4_get_right_line(image, center, &right_line);

    if (!(left_state || right_state))
    {
        return 0;
    }
    if (!left_state)
    {
        if (right_line.slope / M_PI * 180.0 > 10.0)
        {
            ACTION_TURN(MIDDLE, DIR_LEFT, OBLIQUE, 1);
        }
        else
        {
            ACTION_MOVE(SHORT, DIR_LEFT, OBLIQUE, 2);
        }
        return 0;
    }
    if (!right_state)
    {
        if (left_line.slope / M_PI * 180.0 < -10.0)
        {
            ACTION_TURN(MIDDLE, DIR_RIGHT, OBLIQUE, 1);
        }
        else
        {
            ACTION_MOVE(SHORT, DIR_RIGHT, OBLIQUE, 2);
        }
        return 0;
    }

    int center_state = mission_5_4_get_center_line(image, left_line, right_line, &center_line);

    if (center_state == 0)
    {
        return 0;
    }
    else if (center_state == -1)
    {
        return 2;
    }
    return mission_5_6_set_straight(center_line) && mission_5_4_set_center(center_line);
}

int mission_5_6_set_straight(_line_t center_line)
{
    double angle = atan(center_line.slope) / M_PI * 180.0;
    DIRECTION turn_dir = angle < 0;
    angle = abs(angle);

    if (angle > 6.0)
    {
        ACTION_TURN(MIDDLE, turn_dir, OBLIQUE, 1);
    }
    else if (angle > 3.0)
    {
        ACTION_TURN(SHORT, turn_dir, OBLIQUE, 2);
    }
    else if (angle > 2.5)
    {
        ACTION_TURN(SHORT, turn_dir, OBLIQUE, 1);
    }
    else
    {
        return 1;
    }
    RobotSleep(2);

    return 0;
}

int mission_5_7_watch_below(U16 *image)
{
    ACTION_WALK_CHECK(OBLIQUE, image, mission_5_7_walk_check, 1, 15);
    setFPGAVideoData(image);

    double ratio = getColorRatio1(image, 50, 70, 0, WIDTH, GREEN);

    _line_t front_line;
    if (mission_5_8_get_front_line(image, &front_line, BLACK) && ratio < 5.0)
    {
        return 1;
    }

    _line_t left_line, right_line;
    _line_t center_line;

    int left_state, right_state;
    left_state = mission_5_4_get_left_line(image, 60, &left_line);
    right_state = mission_5_4_get_right_line(image, 60, &right_line);

    if (!(left_state && right_state))
    {
        return -1;
    }

    int center_state = mission_5_4_get_center_line(image, left_line, right_line, &center_line);
    if (center_state == 0)
    {
        return -1;
    }
    else if (center_state == -1)
    {
        return 1;
    }

    double angle = abs(atan(center_line.slope) / M_PI * 180.0);

    if (angle > 5.0)
    {
        return -1;
    }

    double center = abs((WIDTH>>1) - center_line.slope*(HEIGHT>>1) - center_line.intercept);

    if (center > 7.0)
    {
        return -1;
    }

    return 0;
}

int mission_5_7_walk_check(U16 *image)
{
    double ratio = getColorRatio1(image, 50, 70, 0, WIDTH, GREEN);

    if (ratio < 8.0)
    {
        return 1;
    }

    _line_t left_line, right_line;
    _line_t center_line;

    int left_state, right_state;
    left_state = mission_5_4_get_left_line(image, 60, &left_line);
    right_state = mission_5_4_get_right_line(image, 60, &right_line);

    if (!(left_state && right_state))
    {
        return 1;
    }

    if (!mission_5_4_get_center_line(image, left_line, right_line, &center_line))
    {
        return 1;
    }

    double angle = abs(atan(center_line.slope) / M_PI * 180.0);

    if (angle > 5.0)
    {
        return 1;
    }

    double center = abs((WIDTH>>1) - center_line.slope*(HEIGHT>>1) - center_line.intercept);

    if (center > 7.0)
    {
        return 1;
    }

    return 0;
}

int mission_5_8_attach_black(U16 *image)
{
    _line_t front_line;

    if (getColorRatio1(image, 40, 80, 50 , WIDTH - 50, GREEN) > 30.0 && mission_5_8_get_front_line(image, &front_line, GREEN))
    {
        _line_t left_line, right_line;
        _line_t center_line;

        int left_state, right_state;
        left_state = mission_5_4_get_left_line(image, 105, &left_line);
        right_state = mission_5_4_get_right_line(image, 105, &right_line);

        if (!(left_state || right_state))
        {
            ACTION_ATTACH(1);
            return 0;
        }

        if (!mission_5_8_set_straight(front_line))
        {
            return 0;
        }

        int center_state = mission_5_4_get_center_line(image, left_line, right_line, &center_line);

        if (center_state == 0 || center_state == -1)
        {
            ACTION_ATTACH(1);
            return 0;
        }

        if (mission_5_4_set_center(center_line))
        {
            ACTION_ATTACH(1);
        }

        return 0;
    }

    if (!mission_5_8_get_front_line(image, &front_line, BLACK) && !mission_5_8_get_front_line(image, &front_line, BLACK))
    {
        return 1;
    }

    if (!mission_5_8_set_straight(front_line))
    {
        return 0;
    }

    if (front_line.slope*(WIDTH>>1) + front_line.intercept < 10.0)
    {
        ACTION_ATTACH(1);
        return 0;
    }

    return mission_5_8_set_dist(front_line);
}

int mission_5_8_get_front_line(U16 *image, _line_t *front_line, U16 color)
{
    U16 i, j;
    _point_t points[NUM_LIN_REG_POINT];
    U8 black_cnt[3], pos;
    U32 point_cnt = 0;
    U16 left, right;

    for (j = IN_IMG(0, (WIDTH>>1) - (NUM_LIN_REG_POINT>>1), WIDTH); j < IN_IMG(0, (WIDTH>>1) + (NUM_LIN_REG_POINT>>1), WIDTH); ++j)
    {
        pos = 0;
        memset(black_cnt, 0, 3 * sizeof(U8));
        left = MAX(j - 1, 0);
        right = MIN(j + 1, WIDTH - 1);

        for (i = 0; i < HEIGHT - 20; ++i)
        {
            black_cnt[pos] = GetValueRGBYOBK(GetPtr(image, i, left, WIDTH), color) +
                            GetValueRGBYOBK(GetPtr(image, i, j, WIDTH), color) +
                            GetValueRGBYOBK(GetPtr(image, i, right, WIDTH), color);

            pos = (pos + 1) % 3;

            if (black_cnt[0] + black_cnt[1] + black_cnt[2] > 2)
            {
                points[point_cnt].x = j;
                points[point_cnt].y = i;
                ++point_cnt;
                break;
            }
        }
    }

#ifdef DEBUG
    printf("point_cnt : %d\n", point_cnt);
#endif

    if (point_cnt < NUM_LIN_REG_POINT>>1)
    {
        return 0;
    }

    qsort(points, point_cnt, sizeof(_point_t), point_t_cmp_y);

    int sum = 0;
    for (i = 0; i < point_cnt; ++i)
    {
        sum += points[i].y;
    }

    int aver = sum / point_cnt;

    for (i = 0; i < point_cnt; ++i)
    {
        if (points[i].y > aver + 5)
        {
            break;
        }
    }

    if (i <= 1)
    {
        return 0;
    }

    return least_sqaures(image, WIDTH>>1, points, i - 1, front_line);
}

int mission_5_8_set_straight(_line_t line)
{
    double angle = atan(line.slope) * 180.0 / M_PI;
    DIRECTION turn_dir = angle > 0;
    angle = abs(angle);

    if (angle > 3.0)
    {
        ACTION_TURN(MIDDLE, turn_dir, OBLIQUE, 1);
    }
    else
    {
        return 1;
    }

    RobotSleep(2);
    return 0;
}

int mission_5_8_set_dist(_line_t line)
{
    double dist = line.slope*(WIDTH>>1) + line.intercept;

    if (dist < 30.0)
    {
        ACTION_ATTACH_LIFT(1);
        RobotSleep(1);
        return 0;
    }

    return 1;
}


int mission_5_9_attach_black(U16 *image)
{
    _line_t front_line;
    if (!mission_5_9_get_front_line(image, &front_line, BLACK))
    {
        return 0;
    }

    return mission_5_9_set_straight(front_line) && mission_5_9_set_dist(front_line);
}

int mission_5_9_get_front_line(U16 *image, _line_t *front_line, U16 color)
{
    U16 i, j;
    _point_t points[20 * 2];
    U8 black_cnt[3], pos;
    U32 point_cnt = 0;
    U16 left, right;

    for (j = 10; j < 30; ++j)
    {
        pos = 0;
        memset(black_cnt, 0, 3 * sizeof(U8));
        left = MAX(j - 1, 0);
        right = MIN(j + 1, WIDTH - 1);

        for (i = 0; i < HEIGHT - 20; ++i)
        {
            black_cnt[pos] = GetValueRGBYOBK(GetPtr(image, i, left, WIDTH), color) +
                            GetValueRGBYOBK(GetPtr(image, i, j, WIDTH), color) +
                            GetValueRGBYOBK(GetPtr(image, i, right, WIDTH), color);

            pos = (pos + 1) % 3;

            if (black_cnt[0] + black_cnt[1] + black_cnt[2] > 2)
            {
                points[point_cnt].x = j;
                points[point_cnt].y = i;
                ++point_cnt;
                break;
            }
        }
    }

    for (j = WIDTH - 10; j > WIDTH - 30; --j)
    {
        pos = 0;
        memset(black_cnt, 0, 3 * sizeof(U8));
        left = MAX(j - 1, 0);
        right = MIN(j + 1, WIDTH - 1);

        for (i = 0; i < HEIGHT - 20; ++i)
        {
            black_cnt[pos] = GetValueRGBYOBK(GetPtr(image, i, left, WIDTH), color) +
                            GetValueRGBYOBK(GetPtr(image, i, j, WIDTH), color) +
                            GetValueRGBYOBK(GetPtr(image, i, right, WIDTH), color);

            pos = (pos + 1) % 3;

            if (black_cnt[0] + black_cnt[1] + black_cnt[2] > 2)
            {
                points[point_cnt].x = j;
                points[point_cnt].y = i;
                ++point_cnt;
                break;
            }
        }
    }

#ifdef DEBUG
    printf("point_cnt : %d\n", point_cnt);
#endif

    if (point_cnt < NUM_LIN_REG_POINT>>1)
    {
        return 0;
    }

    if (i <= 1)
    {
        return 0;
    }

    return least_sqaures(image, WIDTH>>1, points, point_cnt, front_line);
}

int mission_5_9_set_straight(_line_t line)
{
    double angle = atan(line.slope) * 180.0 / M_PI;
    DIRECTION turn_dir = angle > 0;
    angle = abs(angle);

    if (angle > 4.0)
    {
        ACTION_TURN(MIDDLE, turn_dir, DOWN, 1);
    }
    else
    {
        return 1;
    }

    RobotSleep(2);
    return 0;
}

int mission_5_9_set_dist(_line_t line)
{
    double dist = line.slope*(WIDTH>>1) + line.intercept;

    if (dist < 40.0)
    {
        ACTION_ATTACH_LIFT(1);
        RobotSleep(1);
        return 0;
    }
    else if (dist < 50.0)
    {
        ACTION_ATTACH(1);
        RobotSleep(1);
        return 0;
    }
    else if (dist < 61.0)
    {
        ACTION_ATTACH_SHORT(1);
        RobotSleep(1);
        return 0;
    }

    return 1;
}

int mission_5_10_climb_down_stairs(void) {
    RobotSleep(1);
    ACTION_MOTION(MISSION_5_STAIR_DOWN, UP);
    CHECK_INIT(UP);
    //TODO
    ACTION_WALK(FAST, OBLIQUE, 2);
    return 1;
}
