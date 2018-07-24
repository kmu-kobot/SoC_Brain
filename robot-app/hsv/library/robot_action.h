//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#ifndef SOC_APP_ROBOT_ACTION_H

#include "./robot_protocol.h"

#define SOC_APP_ROBOT_ACTION_H

#define INIT_POSE_COEF 5
#define INIT_VIEW_COEF 1
#define WALK_STEP_COEF 1
#define WALK_SPEED_COEF 15
#define WALK_VIEW_COEF 5
#define TURN_DIR_COEF 1
#define TURN_POSE_COEF 15
#define TURN_VIEW_COEF 3
#define MOVE_LEN_COEF 33
#define MOVE_DIR_COEF 1
#define MOVE_POSE_COEF 11
#define MOVE_VIEW_COEF 2
#define BIT_DIR_COEF 5

#define INIT_MOTION(pose, view) (INIT_LOW_DOWN + INIT_POSE_COEF*pose + INIT_VIEW_COEF*view)
#define WALK_START_MOTION(speed, view) (WALK_FAST_START_DOWN + WALK_SPEED_COEF*speed + WALK_VIEW_COEF*view)
#define WALK_END_MOTION(speed, view) (WALK_FAST_END_DOWN + WALK_SPEED_COEF*speed + WALK_VIEW_COEF*view)
#define WALK_MOTION(step, speed, view) (WALK_FAST_L_DOWN + WALK_STEP_COEF*step + WALK_SPEED_COEF*speed + WALK_VIEW_COEF*view)
#define TURN_MOTION(dir, pose, view) (TURN_LEFT_LOW_DOWN + TURN_DIR_COEF*dir + TURN_POSE_COEF*pose + TURN_VIEW_COEF*view)
#define MOVE_MOTION(len, dir, pose, view) (LONG_MOVE_LEFT_LOW_DOWN + MOVE_LEN_COEF*len + MOVE_DIR_COEF*dir + MOVE_POSE_COEF*pose + MOVE_VIEW_COEF*view)
#define BIT_MOTION(dir) (BIT_FRONT + BIT_DIR_COEF*dir)

typedef enum
{
    INIT_LOW_DOWN = 1,
    INIT_LOW_OBLIQUE,
    INIT_LOW_UP,
    INIT_LOW_LEFT,
    INIT_LOW_RIGHT,

    INIT_MIDDLE_DOWN,
    INIT_MIDDLE_OBLIQUE,
    INIT_MIDDLE_UP,
    INIT_MIDDLE_LEFT,
    INIT_MIDDLE_RIGHT,

    INIT_HIGH_DOWN,
    INIT_HIGH_OBLIQUE,
    INIT_HIGH_UP,
    INIT_HIGH_LEFT,
    INIT_HIGH_RIGHT,

    WALK_FAST_START_DOWN = 16,
    WALK_FAST_END_DOWN,
    WALK_FAST_L_DOWN,
    WALK_FAST_R_DOWN,

    WALK_FAST_START_OBLIQUE = 21,
    WALK_FAST_END_OBLIQUE,
    WALK_FAST_L_OBLIQUE,
    WALK_FAST_R_OBLIQUE,

    WALK_FAST_START_UP = 26,
    WALK_FAST_END_UP,
    WALK_FAST_L_UP,
    WALK_FAST_R_UP,

    WALK_SLOW_START_DOWN = 31,
    WALK_SLOW_END_DOWN,
    WALK_SLOW_L_DOWN,
    WALK_SLOW_R_DOWN,

    WALK_SLOW_START_OBLIQUE = 36,
    WALK_SLOW_END_OBLIQUE,
    WALK_SLOW_L_OBLIQUE,
    WALK_SLOW_R_OBLIQUE,

    WALK_SLOW_START_UP = 41,
    WALK_SLOW_END_UP,
    WALK_SLOW_L_UP,
    WALK_SLOW_R_UP,

    TURN_LEFT_LOW_DOWN = 47,
    TURN_RIGHT_LOW_DOWN,

    TURN_LEFT_LOW_OBLIQUE = 50,
    TURN_RIGHT_LOW_OBLIQUE,

    TURN_LEFT_LOW_LEFT = 56,
    TURN_RIGHT_LOW_LEFT,

    TURN_LEFT_LOW_RIGHT = 59,
    TURN_RIGHT_LOW_RIGHT,

    TURN_LEFT_HIGH_DOWN = 77,
    TURN_RIGHT_HIGH_DOWN,

    TURN_LEFT_HIGH_OBLIQUE = 80,
    TURN_RIGHT_HIGH_OBLIQUE,

    TURN_LEFT_HIGH_LEFT = 86,
    TURN_RIGHT_HIGH_LEFT,

    TURN_LEFT_HIGH_RIGHT = 89,
    TURN_RIGHT_HIGH_RIGHT,

    LONG_MOVE_LEFT_LOW_DOWN = 93,
    LONG_MOVE_RIGHT_LOW_DOWN,
    LONG_MOVE_LEFT_LOW_OBLIQUE,
    LONG_MOVE_RIGHT_LOW_OBLIQUE,
    LONG_MOVE_LEFT_LOW_LEFT = 99,
    LONG_MOVE_RIGHT_LOW_LEFT,
    LONG_MOVE_LEFT_LOW_RIGHT,
    LONG_MOVE_RIGHT_LOW_RIGHT,

    LONG_MOVE_LEFT_MIDDLE_DOWN = 104,
    LONG_MOVE_RIGHT_MIDDLE_DOWN,
    LONG_MOVE_LEFT_MIDDLE_OBLIQUE,
    LONG_MOVE_RIGHT_MIDDLE_OBLIQUE,
    LONG_MOVE_LEFT_MIDDLE_LEFT = 110,
    LONG_MOVE_RIGHT_MIDDLE_LEFT,
    LONG_MOVE_LEFT_MIDDLE_RIGHT,
    LONG_MOVE_RIGHT_MIDDLE_RIGHT,

    LONG_MOVE_LEFT_HIGH_DOWN = 115,
    LONG_MOVE_RIGHT_HIGH_DOWN,
    LONG_MOVE_LEFT_HIGH_OBLIQUE,
    LONG_MOVE_RIGHT_HIGH_OBLIQUE,
    LONG_MOVE_LEFT_HIGH_LEFT = 121,
    LONG_MOVE_RIGHT_HIGH_LEFT,
    LONG_MOVE_LEFT_HIGH_RIGHT,
    LONG_MOVE_RIGHT_HIGH_RIGHT,

    SHORT_MOVE_LEFT_LOW_DOWN = 126,
    SHORT_MOVE_RIGHT_LOW_DOWN,
    SHORT_MOVE_LEFT_LOW_OBLIQUE,
    SHORT_MOVE_RIGHT_LOW_OBLIQUE,
    SHORT_MOVE_LEFT_LOW_LEFT = 132,
    SHORT_MOVE_RIGHT_LOW_LEFT,
    SHORT_MOVE_LEFT_LOW_RIGHT,
    SHORT_MOVE_RIGHT_LOW_RIGHT,

    SHORT_MOVE_LEFT_MIDDLE_DOWN = 137,
    SHORT_MOVE_RIGHT_MIDDLE_DOWN,
    SHORT_MOVE_LEFT_MIDDLE_OBLIQUE,
    SHORT_MOVE_RIGHT_MIDDLE_OBLIQUE,
    SHORT_MOVE_LEFT_MIDDLE_LEFT = 143,
    SHORT_MOVE_RIGHT_MIDDLE_LEFT,
    SHORT_MOVE_LEFT_MIDDLE_RIGHT,
    SHORT_MOVE_RIGHT_MIDDLE_RIGHT,

    SHORT_MOVE_LEFT_HIGH_DOWN = 148,
    SHORT_MOVE_RIGHT_HIGH_DOWN,
    SHORT_MOVE_LEFT_HIGH_OBLIQUE,
    SHORT_MOVE_RIGHT_HIGH_OBLIQUE,
    SHORT_MOVE_LEFT_HIGH_LEFT = 154,
    SHORT_MOVE_RIGHT_HIGH_LEFT,
    SHORT_MOVE_LEFT_HIGH_RIGHT,
    SHORT_MOVE_RIGHT_HIGH_RIGHT,

    BIT_FRONT = 159,

    BIT_BACK = 164,

    MISSION_2_RED_DUMBLING = 177,

    MISSION_3_MINE_WALK = 182,

    MISSION_4_HURDLING = 188,

    MISSION_5_STAIR_UP = 192,

    MISSION_5_STAIR_DOWN = 195,

    MISSION_6_RIGHT_KICK = 200,

    MISSION_7_YELLOW_DUMBLING = 203,

    TAKE_REST = 208,

    NIL = 0xff
} MOTION;

typedef enum
{
    LOW_DOWN = INIT_LOW_DOWN,
    LOW_OBLIQUE = INIT_LOW_OBLIQUE,
    LOW_UP = INIT_LOW_UP,
    LOW_LEFT = INIT_LOW_LEFT,
    LOW_RIGHT = INIT_LOW_RIGHT,

    MIDDLE_DOWN = INIT_MIDDLE_DOWN,
    MIDDLE_OBLIQUE = INIT_MIDDLE_OBLIQUE,
    MIDDLE_UP = INIT_MIDDLE_UP,
    MIDDLE_LEFT = INIT_MIDDLE_LEFT,
    MIDDLE_RIGHT = INIT_MIDDLE_RIGHT,

    HIGH_DOWN = INIT_HIGH_DOWN,
    HIGH_OBLIQUE = INIT_HIGH_OBLIQUE,
    HIGH_UP = INIT_HIGH_UP,
    HIGH_LEFT = INIT_HIGH_LEFT,
    HIGH_RIGHT = INIT_HIGH_RIGHT,

    INIT_NIL = NIL
} MOTION_INIT;

typedef enum
{
    LOW = 0,
    MIDDLE,
    HIGH
} POSE;

typedef enum
{
    DOWN = 0,
    OBLIQUE,
    UP,
    LEFT,
    RIGHT
} VIEW;

typedef enum
{
    FAST = 0,
    SLOW
} SPEED;

typedef enum
{
    STEP_LEFT = 0,
    STEP_RIGHT
} STEP;

typedef enum
{
    DIR_LEFT = 0,
    DIR_RIGHT
} DIRECTION;

typedef enum
{
    LONG = 0,
    SHORT,
} LENGTH;

typedef enum
{
    CHECK = 0,
    SET
} FOO_MOD;

void foo(MOTION_INIT motion, FOO_MOD mod);

static inline void action(MOTION_INIT init, MOTION motion)
{
    foo(init, CHECK);
    RobotAction(motion);
}

//////////////////////////////
//  MOTION INIT             //
//////////////////////////////

static inline void ACTION_INIT(POSE pose, VIEW view)
{
    RobotAction(INIT_MOTION(pose, view));
    foo(INIT_MOTION(pose, view), SET);
}


//////////////////////////////
//  MOTION WALK             //
//////////////////////////////

static inline void ACTION_WALK(SPEED speed, VIEW view, int repeat)
{
    action(INIT_MOTION(LOW, view), WALK_START_MOTION(speed, view));

    for(; repeat > 1; --repeat)
    {
        RobotAction(WALK_MOTION(STEP_LEFT, speed, view));
        RobotAction(WALK_MOTION(STEP_RIGHT, speed, view));
    }

    RobotAction(WALK_END_MOTION(speed, view));
}


//////////////////////////////
//  MOTION TURN             //
//////////////////////////////

static inline void ACTION_TURN(DIRECTION dir, POSE pose, VIEW view, int repeat)
{
    action(INIT_MOTION(pose, view), TURN_MOTION(dir, pose, view));

    for(; repeat > 1; --repeat)
    {
        RobotAction(TURN_MOTION(dir, pose, view));
    }
}


//////////////////////////////
//  MOTION MOVE             //
//////////////////////////////

static inline void ACTION_MOVE(LENGTH len, DIRECTION dir, POSE pose, VIEW view, int repeat)
{
    action(INIT_MOTION(pose, view), MOVE_MOTION(len, dir, pose, view));

    for(; repeat > 1; --repeat)
    {
        RobotAction(MOVE_MOTION(len, dir, pose, view));
    }
}

//////////////////////////////
//  MOTION BIT              //
//////////////////////////////

static inline void ACTION_BIT(DIRECTION dir, int repeat)
{
    action(INIT_MOTION(MIDDLE, DOWN), BIT_MOTION(dir));

    for(; repeat > 1; --repeat)
    {
        RobotAction(BIT_MOTION(dir));
    }
}

//////////////////////////////
//  MOTION NUMBER           //
//////////////////////////////

static inline void ACTION_MOTION(MOTION mission, POSE pose, VIEW view)
{
    action(INIT_MOTION(pose, view), mission);
}

static inline void ACTION_MOTION_REPEAT(MOTION mission, POSE pose, VIEW view, int repeat)
{
    action(INIT_MOTION(pose, view), mission);

    for (; repeat > 1; --repeat)
    {
        RobotAction(mission);
    }
}

#endif //SOC_APP_ROBOT_ACTION_H
