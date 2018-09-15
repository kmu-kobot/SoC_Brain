#include "DEFAULT.h"

int default_get_straight_and_center1(U16 *image, VIEW view, U16 center, U16 bot, U16 color1)
{
    _line_t line;

    CHECK_INIT(view);
    if (!linear_regression1(image, center, bot, color1, &line))
    {
        return 0;
    }

    return 1;
}

int default_set_straight_and_center1(U16 *image, VIEW view, U16 center, U16 bot, U16 color1)
{
    _line_t line;

    CHECK_INIT(view);
    if (!linear_regression1(image, center, bot, color1, &line))
    {
        return 0;
    }

    return set_straight(line, center, view) ? set_center(line, center, view) : 0;
}

int default_set_straight_and_center2(U16 *image, VIEW view, U16 center, U16 bot, U16 color1, U16 color2)
{
    _line_t line;

    CHECK_INIT(view);
    if (!linear_regression2(image, center, bot, color1, color2, &line))
    {
        return 0;
    }

    return set_straight(line, center, view) ? set_center(line, center, view) : 0;
}

int default_set_straight1(U16 *image, VIEW view, U16 center, U16 bot, U16 color1)
{
    _line_t line;

    CHECK_INIT(view);
    if (!linear_regression1(image, center, bot, color1, &line))
    {
        return 0;
    }

    return set_straight(line, center, view);
}

int default_set_center1(U16 *image, VIEW view, U16 center, U16 bot, U16 color1)
{
    _line_t line;

    CHECK_INIT(view);
    if (!linear_regression1(image, center, bot, color1, &line))
    {
        return 0;
    }

    return set_center(line, center, view);
}

int set_straight(_line_t line, U16 center, VIEW view)
{
    double angle = atan(line.slope) * 180.0 / M_PI + (view == LEFT ? 10 : -10);
    DIRECTION turn_dir = angle > 0;
    angle = abs(angle);

    if (angle > 5)
    {
        ACTION_TURN(MIDDLE, turn_dir, view, 1);
        RobotSleep(2);
        return 0;
    }
    if (angle > 3)
    {
        ACTION_TURN(SHORT, turn_dir, view, 2);
        RobotSleep(2);
        return 0;
    }
    if (angle > 1)
    {
        ACTION_TURN(SHORT, turn_dir, view, 1);
        RobotSleep(2);
        return 0;
    }

    return 1;
}

int set_center(_line_t line, U16 center, VIEW view)
{
    double dist_err = DEFAULT_CENTER_DISTANCE - (line.slope * (WIDTH>>1) + line.intercept);
    DIRECTION move_dir = (view - LEFT) == (dist_err > 0);
    dist_err = abs(dist_err);

    if (dist_err > DEFAULT_CENTER_THRES_LONG)
    {
        ACTION_MOVE(LONG, move_dir, view, MIN(2, dist_err / DEFAULT_CENTER_THRES_LONG));
        RobotSleep(4);
        return 0;
    }
    if (dist_err > DEFAULT_CENTER_THRES_SHORT)
    {
        ACTION_MOVE(SHORT, move_dir, view, MIN(4, dist_err / 7 + 1));
        RobotSleep(2);
        return 0;
    }

    return 1;
}

int default_set_not_black(U16 *image)
{
    int dist = getDistance1(image, WIDTH>>1, HEIGHT-1, BLACK);

    if (dist > 10)
    {
        ACTION_TURN(LONG, DIR_LEFT, UP, 1);
        RobotSleep(1);
    }
    return dist < 10;
}


int point_t_cmp_y(const void * a, const void * b)
{
    return (((_point_t*)a)->y - ((_point_t*)b)->y);
}
