#include "DEFAULT.h"

int default_set_straight_and_center1(U16 *image, VIEW view, U16 center, U16 bot, U16 color1)
{
    _line_t line;

    default_watch_side(view);
    if (!linear_regression1(image, center, bot, color1, &line))
    {
        return 0;
    }

    double angle = atan(line.slope) * 180.0 / M_PI;
    DIRECTION turn_dir = view == angle < 0;
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
    if (angle > DEFAULT_STRAIGHT_THRES_SHORT)
    {
        ACTION_TURN(SHORT, turn_dir, view, angle / DEFAULT_STRAIGHT_THRES_SHORT);
        return 0;
    }

    double dist_err = DEFAULT_CENTER_DISTANCE - (line.slope * center + line.intercept);
    DIRECTION move_dir = dist_err < 0;
    dist_err = abs(dist_err);

    if (dist_err > DEFAULT_CENTER_THRES_LONG)
    {
        ACTION_MOVE(LONG, move_dir, view, dist_err / DEFAULT_CENTER_THRES_LONG);
        return 0;
    }
    if (dist_err > DEFAULT_CENTER_THRES_MIDDLE)
    {
        ACTION_MOVE(LONG, move_dir, view, dist_err / DEFAULT_CENTER_THRES_MIDDLE);
        return 0;
    }
    if (dist_err > DEFAULT_CENTER_THRES_SHORT)
    {
        ACTION_MOVE(LONG, move_dir, view, dist_err / DEFAULT_CENTER_THRES_SHORT);
        return 0;
    }

    return 1;
}
