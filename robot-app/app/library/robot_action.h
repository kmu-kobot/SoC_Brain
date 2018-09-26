//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#ifndef SOC_APP_ROBOT_ACTION_H

#include "./robot_protocol.h"
#include "./graphic_api.h"
#include <stdio.h>
#include <pthread.h>

#define SOC_APP_ROBOT_ACTION_H

#define WALK_SPEED_COEF 10
#define WALK_VIEW_COEF 5
#define TURN_LENGTH_COEF 11
#define TURN_VIEW_COEF 2
#define MOVE_LENGTH_COEF 11
#define MOVE_VIEW_COEF 2

#define INIT_MOTION(view) (INIT_DOWN + view)
#define HEAD_MOTION(view) (HEAD_DOWN + view)
#define WALK_START_MOTION(speed, view) (WALK_FAST_START_DOWN + WALK_SPEED_COEF*speed + WALK_VIEW_COEF*view)
#define WALK_END_MOTION(dir, speed, view) (WALK_FAST_END_L_DOWN + dir + WALK_SPEED_COEF*speed + WALK_VIEW_COEF*view)
#define WALK_MOTION(step, speed, view) (WALK_FAST_L_DOWN + WALK_SPEED_COEF*speed + WALK_VIEW_COEF*view + step)
#define TURN_MOTION(dir, len, view) (TURN_LEFT_LONG_DOWN + dir + TURN_LENGTH_COEF*len + TURN_VIEW_COEF*view)
#define MOVE_MOTION(dir, len, view) (MOVE_LEFT_LONG_DOWN + dir + MOVE_LENGTH_COEF*len + MOVE_VIEW_COEF*view)
#define BIT_MOTION(dir) (BIT_FRONT + dir)

#define GET_INIT_VIEW(motion) (motion - INIT_DOWN)

typedef enum {
    INIT_DOWN = 1,
    INIT_OBLIQUE,
    INIT_LEFT,
    INIT_RIGHT,
    INIT_UP,

    HEAD_DOWN = 7,
    HEAD_OBLQUE,
    HEAD_LEFT,
    HEAD_RIGHT,
    HEAD_UP,

    HEAD_SIDE_TO_DOWN = 13,
    HEAD_SIDE_TO_OBLIQUE,
    HEAD_DOWN_TO_LEFT,
    HEAD_DOWN_TO_RIGHT,
    HEAD_HALF_LEFT,
    HEAD_HALF_RIGHT,

    WALK_FAST_START_DOWN = 20,
    WALK_FAST_END_L_DOWN,
    WALK_FAST_END_R_DOWN,
    WALK_FAST_L_DOWN,
    WALK_FAST_R_DOWN,

    WALK_FAST_START_OBLIQUE = 25,
    WALK_FAST_END_L_OBLIQUE,
    WALK_FAST_END_R_OBLIQUE,
    WALK_FAST_L_OBLIQUE,
    WALK_FAST_R_OBLIQUE,

    WALK_SLOW_START_DOWN = 30,
    WALK_SLOW_END_L_DOWN,
    WALK_SLOW_END_R_DOWN,
    WALK_SLOW_L_DOWN,
    WALK_SLOW_R_DOWN,

    WALK_SLOW_START_OBLIQUE = 35,
    WALK_SLOW_END_L_OBLIQUE,
    WALK_SLOW_END_R_OBLIQUE,
    WALK_SLOW_L_OBLIQUE,
    WALK_SLOW_R_OBLIQUE,

    TURN_LEFT_LONG_DOWN = 41,
    TURN_RIGHT_LONG_DOWN,
    TURN_LEFT_LONG_OBLIQUE,
    TURN_RIGHT_LONG_OBLIQUE,
    TURN_LEFT_LONG_LEFT,
    TURN_RIGHT_LONG_LEFT,
    TURN_LEFT_LONG_RIGHT,
    TURN_RIGHT_LONG_RIGHT,
    TURN_LEFT_LONG_UP,
    TURN_RIGHT_LONG_UP,

    TURN_LEFT_MIDDLE_DOWN = 52,
    TURN_RIGHT_MIDDLE_DOWN,
    TURN_LEFT_MIDDLE_OBLIQUE,
    TURN_RIGHT_MIDDLE_OBLIQUE,
    TURN_LEFT_MIDDLE_LEFT,
    TURN_RIGHT_MIDDLE_LEFT,
    TURN_LEFT_MIDDLE_RIGHT,
    TURN_RIGHT_MIDDLE_RIGHT,
    TURN_LEFT_MIDDLE_UP,
    TURN_RIGHT_MIDDLE_UP,

    TURN_LEFT_SHORT_DOWN = 63,
    TURN_RIGHT_SHORT_DOWN,
    TURN_LEFT_SHORT_OBLIQUE,
    TURN_RIGHT_SHORT_OBLIQUE,
    TURN_LEFT_SHORT_LEFT,
    TURN_RIGHT_SHORT_LEFT,
    TURN_LEFT_SHORT_RIGHT,
    TURN_RIGHT_SHORT_RIGHT,
    TURN_LEFT_SHORT_UP,
    TURN_RIGHT_SHORT_UP,

    MOVE_LEFT_LONG_DOWN = 75,
    MOVE_RIGHT_LONG_DOWN,
    MOVE_LEFT_LONG_OBLIQUE,
    MOVE_RIGHT_LONG_OBLIQUE,
    MOVE_LEFT_LONG_LEFT,
    MOVE_RIGHT_LONG_LEFT,
    MOVE_LEFT_LONG_RIGHT,
    MOVE_RIGHT_LONG_RIGHT,
    MOVE_LEFT_LONG_UP,
    MOVE_RIGHT_LONG_UP,

    MOVE_LEFT_MIDDLE_DOWN = 86,
    MOVE_RIGHT_MIDDLE_DOWN,
    MOVE_LEFT_MIDDLE_OBLIQUE,
    MOVE_RIGHT_MIDDLE_OBLIQUE,
    MOVE_LEFT_MIDDLE_LEFT,
    MOVE_RIGHT_MIDDLE_LEFT,
    MOVE_LEFT_MIDDLE_RIGHT,
    MOVE_RIGHT_MIDDLE_RIGHT,
    MOVE_LEFT_MIDDLE_UP,
    MOVE_RIGHT_MIDDLE_UP,

    MOVE_LEFT_SHORT_DOWN = 97,
    MOVE_RIGHT_SHORT_DOWN,
    MOVE_LEFT_SHORT_OBLIQUE,
    MOVE_RIGHT_SHORT_OBLIQUE,
    MOVE_LEFT_SHORT_LEFT,
    MOVE_RIGHT_SHORT_LEFT,
    MOVE_LEFT_SHORT_RIGHT,
    MOVE_RIGHT_SHORT_RIGHT,
    MOVE_LEFT_SHORT_UP,
    MOVE_RIGHT_SHORT_UP,

    ATTACH_LIFT = 108,
    ATTACH = 109,
    ATTACH_SHORT,

    BIT_FRONT = 111,
    BIT_BACK,

    MISSION_2_RED_DUMBLING = 115,

    MISSION_4_HURDLING = 120,

    MISSION_5_STAIR_UP = 125,

    MISSION_5_STAIR_DOWN = 129,

    MISSION_6_RIGHT_KICK = 134,

    MISSION_7_YELLOW_DUMBLING = 138,

    MISSION_8_CREVASSE_DUMBLING = 143,

    MISSION_9_FACE_FINAL = 12,

    STABLE_DOWN = 147,
    STABLE_OBLIQUE,
    STABLE_LEFT,
    STABLE_RIGHT,
    STABLE_UP,

    BALL_INIT_DOWN = 155,
    BALL_INIT_OBLIQUE,
    BALL_INIT_UP,

    BALL_HEAD_DOWN = 159,
    BALL_HEAD_OBLIQUE,
    BALL_HEAD_UP,
    BALL_HEAD_HALF_LEFT,
    BALL_HEAD_HALF_RIGHT,

    BALL_TURN_LEFT_DOWN = 165,
    BALL_TURN_RIGHT_DOWN,
    BALL_TURN_LEFT_OBLIQUE,
    BALL_TURN_RIGHT_OBLIQUE,
    BALL_TURN_LEFT_UP,
    BALL_TURN_RIGHT_UP,

    BALL_MOVE_LEFT_DOWN = 172,
    BALL_MOVE_RIGHT_DOWN,
    BALL_MOVE_LEFT_OBLIQUE,
    BALL_MOVE_RIGHT_OBLIQUE,
    BALL_MOVE_LEFT_UP,
    BALL_MOVE_RIGHT_UP,

    BALL_BIT_FRONT = 179,
    BALL_BIT_BACK,

    KICK = 182,

    BALL_STABLE_DOWN = 185,
    BALL_STABLE_OBLIQUE,
    BALL_STABLE_UP,

    BALL_MOVE_LONG_LEFT_DOWN = 189,
    BALL_MOVE_LONG_RIGHT_DOWN,
    BALL_MOVE_LONG_LEFT_OBLIQUE,
    BALL_MOVE_LONG_RIGHT_OBLIQUE,
    BALL_MOVE_LONG_LEFT_UP,
    BALL_MOVE_LONG_RIGHT_UP,

    NIL = 0xff
} MOTION;

typedef enum {
    DOWN = 0,
    OBLIQUE,
    LEFT,
    RIGHT,
    UP
} VIEW;

typedef enum {
    FAST = 0,
    SLOW
} SPEED;

typedef enum {
    LONG = 0,
    MIDDLE,
    SHORT
} LENGTH;

typedef enum {
    FRONT = 0,
    BACK,
    DIR_LEFT = 0,
    DIR_RIGHT
} DIRECTION;

typedef enum {
    STEP_LEFT = 0,
    STEP_RIGHT
} STEP;

typedef enum {
    RED_DUMBLING = MISSION_2_RED_DUMBLING,
    HURDLING = MISSION_4_HURDLING,
    STAIR_UP = MISSION_5_STAIR_UP,
    STAIR_DOWN = MISSION_5_STAIR_DOWN,
    RIGHT_KICK = MISSION_6_RIGHT_KICK,
    YELLOW_DUMBLING = MISSION_7_YELLOW_DUMBLING,
    FACE_FINAL = MISSION_9_FACE_FINAL
} MISSION;

typedef enum {
    CHECK = 0,
    SET,
    HEAD,
    STAY
} PREV_CHECK_MOD;

void prev_check(MOTION motion, PREV_CHECK_MOD mod);

void move_check(MOTION motion, VIEW view);

#define IS_MOVE(motion) (MOVE_LEFT_LONG_DOWN <= motion && motion <= MOVE_RIGHT_SHORT_UP)

static inline void action(MOTION init, MOTION motion) {
    prev_check(init, HEAD);
    move_check(motion, GET_INIT_VIEW(init));
    RobotAction(motion);
}

//////////////////////////////
//  MOTION INIT             //
//////////////////////////////

static inline void CHECK_INIT(VIEW view) {
    prev_check(INIT_MOTION(view), HEAD);
}

static inline void ACTION_INIT(VIEW view) {
    RobotAction(INIT_MOTION(view));
    prev_check(INIT_MOTION(view), SET);
}


//////////////////////////////
//  MOTION WALK             //
//////////////////////////////
static inline void ACTION_WALK(SPEED speed, VIEW view, int repeat) {
    action(INIT_MOTION(view), WALK_START_MOTION(speed, view));

    for (; repeat > 1; --repeat) {
        RobotAction(WALK_MOTION(STEP_LEFT, speed, view));
        RobotAction(WALK_MOTION(STEP_RIGHT, speed, view));
    }

    RobotAction(WALK_END_MOTION(DIR_RIGHT, speed, view));
}

typedef struct CHECKER_ARGS {
    U16 *p_image;

    int (*check)(U16 *);

    int *p_state;
    int finish;
    int *destroy;
} _args_t;

void *checker(void *data);

int ACTION_WALK_CHECK(VIEW view, U16 *image, int (*check)(U16 *), int finish, int repeat);


//////////////////////////////
//  MOTION TURN             //
//////////////////////////////

static inline void ACTION_TURN(LENGTH len, DIRECTION dir, VIEW view, int repeat) {
    action(INIT_MOTION(view), TURN_MOTION(dir, len, view));

    for (; repeat > 1; --repeat) {
        RobotAction(TURN_MOTION(dir, len, view));
    }
}


//////////////////////////////
//  MOTION MOVE             //
//////////////////////////////

static inline void ACTION_MOVE(LENGTH len, DIRECTION dir, VIEW view, int repeat) {
    action(INIT_MOTION(view), MOVE_MOTION(dir, len, view));

    for (; repeat > 1; --repeat) {
        RobotAction(MOVE_MOTION(dir, len, view));
    }
}

//////////////////////////////
//  MOTION ATTACH           //
//////////////////////////////

static inline void ACTION_ATTACH(int repeat) {
    action(INIT_MOTION(DOWN), ATTACH);

    for (; repeat > 1; --repeat) {
        RobotAction(ATTACH);
    }
}

static inline void ACTION_ATTACH_SHORT(int repeat) {
    action(INIT_MOTION(DOWN), ATTACH_SHORT);

    for (; repeat > 1; --repeat) {
        RobotAction(ATTACH_SHORT);
    }
}

static inline void ACTION_ATTACH_LIFT(int repeat) {
    action(INIT_MOTION(DOWN), ATTACH_LIFT);

    for (; repeat > 1; --repeat) {
        RobotAction(ATTACH_LIFT);
    }
}

//////////////////////////////
//  MOTION BIT              //
//////////////////////////////

static inline void ACTION_BIT(DIRECTION dir, int repeat) {
    action(INIT_MOTION(DOWN), BIT_MOTION(dir));

    for (; repeat > 1; --repeat) {
        RobotAction(BIT_MOTION(dir));
    }
}

//////////////////////////////
//  MOTION NUMBER           //
//////////////////////////////

static inline void ACTION_MOTION(MOTION motion, VIEW view) {
    action(INIT_MOTION(view), motion);
}

static inline void ACTION_MOTION_REPEAT(MOTION motion, VIEW view, int repeat) {
    action(INIT_MOTION(view), motion);

    for (; repeat > 1; --repeat) {
        RobotAction(motion);
    }
}

//////////////////////////////
//  MOTION MISSION          //
//////////////////////////////

static inline void ACTION_MISSION(MISSION mission, VIEW view) {
    ACTION_MOTION(mission, view);
    prev_check(INIT_MOTION(UP), SET);
}

//////////////////////////////
//  MOTION BALL             //
//////////////////////////////

static inline void BALL_INIT(VIEW view) {
    if (view == UP) {
        view = OBLIQUE + 1;
    }
    RobotAction(BALL_INIT_DOWN + view);
}

static inline void BALL_HEAD(VIEW view) {
    if (view == UP) {
        view = OBLIQUE + 1;
    }
    RobotAction(BALL_HEAD_DOWN + view);
}

static inline void BALL_HALF_HEAD(MOTION motion) {
    RobotAction(motion);
}

static inline void BALL_TURN(DIRECTION dir, VIEW view, int repeat) {
    if (view == UP) {
        view = OBLIQUE + 1;
    }
    while (repeat--) {
        RobotAction(BALL_TURN_LEFT_DOWN + dir + (view << 1));
    }
}

static inline void BALL_MOVE(DIRECTION dir, VIEW view, int repeat) {
    if (view == UP) {
        view = OBLIQUE + 1;
    }
    while (repeat--) {
        RobotAction(BALL_MOVE_LEFT_DOWN + dir + (view << 1));
    }
}

static inline void BALL_MOVE_LONG(DIRECTION dir, VIEW view, int repeat) {
    if (view == UP) {
        view = OBLIQUE + 1;
    }
    while (repeat--) {
        RobotAction(BALL_MOVE_LONG_LEFT_DOWN + dir + (view << 1));
    }
}

static inline void BALL_BIT(DIRECTION dir, int repeat) {
    while (repeat--) {
        RobotAction(BALL_BIT_FRONT + dir);
    }
}

static inline void BALL_KICK(void) {
    RobotAction(KICK);
    CHECK_INIT(UP);
}

static inline void BALL_STABLE(VIEW view) {
    if (view == UP) {
        view = OBLIQUE + 1;
    }
    RobotAction(BALL_STABLE_DOWN + view);
}

#endif //SOC_APP_ROBOT_ACTION_H
