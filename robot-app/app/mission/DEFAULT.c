#include "DEFAULT.h"

int default_set_straight_and_center1(U16 *image, VIEW view, U16 center, U16 bot, U16 color1)
{
    _line_t line;

#ifdef DEBUG
    printf("view : %d, center : %d, bot : %d, color1 : %d\n", view, center, bot, color1);
#endif

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

    default_watch_side(view);
    if (!linear_regression2(image, center, bot, color1, color2, &line))
    {
        return 0;
    }

    return set_straight(line, center, view) ? set_center(line, center, view) : 0;
}

int set_straight(_line_t line, U16 center, VIEW view)
{
    double angle = atan(line.slope) * 180.0 / M_PI;
    DIRECTION turn_dir = angle > 0;
    angle = abs(angle);

    if (angle > DEFAULT_STRAIGHT_THRES_LONG)
    {
        ACTION_TURN(LONG, turn_dir, view, angle / DEFAULT_STRAIGHT_THRES_LONG);
        return 0;
    }
    if (angle > DEFAULT_STRAIGHT_THRES_MIDDLE)
    {
        ACTION_TURN(MIDDLE, turn_dir, view, angle / DEFAULT_STRAIGHT_THRES_MIDDLE);
        return 0;
    }

    return 1;
}

int set_center(_line_t line, U16 center, VIEW view)
{
    double dist_err = DEFAULT_CENTER_DISTANCE - (line.slope * center + line.intercept);
    DIRECTION move_dir = (view - LEFT) == (dist_err > 0);
    dist_err = abs(dist_err);

    if (dist_err > DEFAULT_CENTER_THRES_LONG)
    {
        ACTION_MOVE(LONG, move_dir, view, dist_err / DEFAULT_CENTER_THRES_LONG);
        return 0;
    }
    if (dist_err > DEFAULT_CENTER_THRES_MIDDLE)
    {
        ACTION_MOVE(MIDDLE, move_dir, view, dist_err / DEFAULT_CENTER_THRES_MIDDLE);
        return 0;
    }
    if (dist_err > DEFAULT_CENTER_THRES_SHORT)
    {
        ACTION_MOVE(SHORT, move_dir, view, dist_err / DEFAULT_CENTER_THRES_SHORT);
        return 0;
    }

    return 1;
}
