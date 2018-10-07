#include "DEFAULT.h"

int default_get_straight_and_center1(U16 *image, VIEW view, U16 center, U16 bot, U16 color1, U16 base) {
    _line_t line;
    int state;

    CHECK_INIT(view);
    return state = linear_regression1(image, center, bot, color1, &line);
}

int default_set_straight_and_center1_long(U16 *image, VIEW view, U16 center, U16 bot, U16 color1, U16 base) {
    _line_t line;
    int state;

    CHECK_INIT(view);
    state = linear_regression1(image, center, bot, color1, &line);
    if (state != 1) {
        return state;
    }

    return set_straight(line, center, view) ? set_center_long(line, base, view) : 0;
}

int default_set_straight_long_and_center1_long(U16 *image, VIEW view, U16 center, U16 bot, U16 color1, U16 base) {
    _line_t line;
    int state;

    CHECK_INIT(view);
    state = linear_regression1(image, center, bot, color1, &line);
    if (state != 1) {
        return state;
    }

    return set_straight_long(line, center, view) ? set_center_long(line, base, view) : 0;
}

int default_set_straight_and_center1(U16 *image, VIEW view, U16 center, U16 bot, U16 color1, U16 base) {
    _line_t line;
    int state;

    CHECK_INIT(view);
    state = linear_regression1(image, center, bot, color1, &line);
    if (state != 1) {
        return state;
    }

    return set_straight(line, center, view) ? set_center(line, base, view) : 0;
}

int default_set_straight_and_center2(U16 *image, VIEW view, U16 center, U16 bot, U16 color1, U16 color2, U16 base) {
    _line_t line;
    int state;

    CHECK_INIT(view);
    state = linear_regression2(image, center, bot, color1, color2, &line);
    if (state != 1) {
        return state;
    }

    return set_straight(line, center, view) ? set_center(line, center, view) : 0;
}

int default_set_straight1(U16 *image, VIEW view, U16 center, U16 bot, U16 color1, U16 base) {
    _line_t line;
    int state;

    CHECK_INIT(view);
    state = linear_regression1(image, center, bot, color1, &line);
    if (state != 1) {
        return state;
    }

    return set_straight(line, center, view);
}

int default_set_center1(U16 *image, VIEW view, U16 center, U16 bot, U16 color1, U16 base) {
    _line_t line;
    int state;

    CHECK_INIT(view);
    state = linear_regression1(image, center, bot, color1, &line);
    if (state != 1) {
        return state;
    }

    return set_center(line, base, view);
}

int default_set_center1_long(U16 *image, VIEW view, U16 center, U16 bot, U16 color1, U16 base) {
    _line_t line;
    int state;

    CHECK_INIT(view);
    state = linear_regression1(image, center, bot, color1, &line);
    if (state != 1) {
        return state;
    }

    return set_center_long(line, base, view);
}

int set_straight(_line_t line, U16 center, VIEW view) {
    double angle = atan(line.slope) * 180.0 / M_PI + (view == LEFT ? 9.0 : -11.0);
    DIRECTION turn_dir = (DIRECTION) (angle > 0);
    angle = fabs(angle);

    if (angle > 25,0) {
        ACTION_TURN(LONG, turn_dir, view, 1);
        RobotSleep(2);
        return 0;
    }
    if (angle > 15.0) {
        ACTION_TURN(MIDDLE, turn_dir, view, 2);
        RobotSleep(2);
        return 0;
    }
    if (angle > 8.0) {
        ACTION_TURN(MIDDLE, turn_dir, view, 1);
        RobotSleep(2);
        return 0;
    }
    if (angle > 3.0) {
        ACTION_TURN(SHORT, turn_dir, view, 1);
        RobotSleep(3);
        return 0;
    }

    return 1;
}

int set_straight_long(_line_t line, U16 center, VIEW view) {
    double angle = atan(line.slope) * 180.0 / M_PI + (view == LEFT ? 9.0 : -11.0);
    DIRECTION turn_dir = (DIRECTION) (angle > 0);
    angle = fabs(angle);

    if (angle > 25,0) {
        ACTION_TURN(LONG, turn_dir, view, 1);
        RobotSleep(2);
        return 0;
    }
    if (angle > 15.0) {
        ACTION_TURN(MIDDLE, turn_dir, view, 2);
        RobotSleep(2);
        return 0;
    }
    if (angle > 8.0) {
        ACTION_TURN(MIDDLE, turn_dir, view, 1);
        RobotSleep(2);
        return 0;
    }

    return 1;
}

int set_center_long(_line_t line, U16 base, VIEW view) {
    double dist_err = base - (line.slope * WIDTH_CENTER + line.intercept);
    DIRECTION move_dir = (DIRECTION) ((view - LEFT) == (dist_err > 0.0));
    dist_err = fabs(dist_err);

    if (dist_err > DEFAULT_CENTER_THRES_LONG - 2) {
        ACTION_MOVE(LONG, move_dir, view, MIN(2, (int)dist_err / DEFAULT_CENTER_THRES_LONG));
        RobotSleep(4);
        return 0;
    }

    return 1;
}

int set_center(_line_t line, U16 base, VIEW view) {
    double dist_err = base - (line.slope * WIDTH_CENTER + line.intercept);
    DIRECTION move_dir = (DIRECTION) ((view - LEFT) == (dist_err > 0));
    dist_err = fabs(dist_err);

    if (dist_err > DEFAULT_CENTER_THRES_LONG) {
        ACTION_MOVE(LONG, move_dir, view, MIN(2, (int)dist_err / DEFAULT_CENTER_THRES_LONG));
        RobotSleep(6);
        return 0;
    }
    if (dist_err > DEFAULT_CENTER_THRES_SHORT) {
        ACTION_MOVE(SHORT, move_dir, view, MIN(5, (int)dist_err / 4 + 1));
        RobotSleep(2);
        return 0;
    }

    return 1;
}

int default_set_not_black(U16 *image) {
    int dist = getDistance1(image, WIDTH_CENTER, HEIGHT - 1, BLACK);

    if (dist > 10) {
        ACTION_TURN(LONG, DIR_LEFT, UP, 1);
        RobotSleep(1);
        setFPGAVideoData(image);
    }
    return dist < 10;
}


int point_t_cmp_y(const void *a, const void *b) {
    return (((_point_t *) a)->y - ((_point_t *) b)->y);
}
