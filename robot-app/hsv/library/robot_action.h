//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#ifndef SOC_APP_ROBOT_ACTION_H

#include "./robot_protocol.h"

#define SOC_APP_ROBOT_ACTION_H

#define INIT_POSE_COEF 5
#define INIT_VIEW_COEF 1
#define WALK_SPEED_COEF 15
#define WALK_VIEW_COEF 5
#define TURN_DIR_COEF 1
#define TURN_POSE_COEF 15
#define TURN_VIEW_COEF 3
#define MOVE_DIR_COEF 1
#define MOVE_POSE_COEF 11
#define MOVE_VIEW_COEF 2

#define INIT_MOTION(pose, view) (INIT_LOW_DOWN + INIT_POSE_COEF*pose + INIT_VIEW_COEF*view)
#define WALK_MOTION(step, speed, view) (WALK_FAST_DOWN + WALK_SPEED_COEF*speed + WALK_VIEW_COEF*view)
#define TURN_MOTION(dir, pose, view) (TURN_LEFT_LOW_DOWN + TURN_DIR_COEF*dir + TURN_POSE_COEF*pose + TURN_VIEW_COEF*view)
#define MOVE_MOTION(dir, pose, view) (MOVE_LEFT_LOW_DOWN + MOVE_DIR_COEF*dir + MOVE_POSE_COEF*pose + MOVE_VIEW_COEF*view)

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

    WALK_FAST_DOWN = 16,

    WALK_FAST_OBLIQUE = 21,

    WALK_FAST_UP = 26,

    WALK_SLOW_DOWN = 31,

    WALK_SLOW_OBLIQUE = 36,

    WALK_SLOW_UP = 41,

    TURN_LEFT_LOW_DOWN = 47,
    TURN_RIGHT_LOW_DOWN,

    TURN_LEFT_LOW_OBLIQUE = 50,
    TURN_RIGHT_LOW_OBLIQUE,

    TURN_LEFT_LOW_LEFT = 56,
    TURN_RIGHT_LOW_LEFT,

    TURN_LEFT_LOW_RIGHT = 59,
    TURN_RIGHT_LOW_RIGHT,

    TURN_LEFT_HIGH_DOWN = 62,
    TURN_RIGHT_HIGH_DOWN,

    TURN_LEFT_HIGH_OBLIQUE = 65,
    TURN_RIGHT_HIGH_OBLIQUE,

    TURN_LEFT_HIGH_LEFT = 71,
    TURN_RIGHT_HIGH_LEFT,

    TURN_LEFT_HIGH_RIGHT = 74,
    TURN_RIGHT_HIGH_RIGHT,

    MOVE_LEFT_LOW_DOWN = 78,
    MOVE_RIGHT_LOW_DOWN,
    MOVE_LEFT_LOW_OBLIQUE,
    MOVE_RIGHT_LOW_OBLIQUE,
    MOVE_LEFT_LOW_LEFT = 84,
    MOVE_RIGHT_LOW_LEFT,
    MOVE_LEFT_LOW_RIGHT,
    MOVE_RIGHT_LOW_RIGHT,

    MOVE_LEFT_MIDDLE_DOWN = 89,
    MOVE_RIGHT_MIDDLE_DOWN,
    MOVE_LEFT_MIDDLE_OBLIQUE,
    MOVE_RIGHT_MIDDLE_OBLIQUE,
    MOVE_LEFT_MIDDLE_LEFT = 95,
    MOVE_RIGHT_MIDDLE_LEFT,
    MOVE_LEFT_MIDDLE_RIGHT,
    MOVE_RIGHT_MIDDLE_RIGHT,

    MOVE_LEFT_HIGH_DOWN = 100,
    MOVE_RIGHT_HIGH_DOWN,
    MOVE_LEFT_HIGH_OBLIQUE,
    MOVE_RIGHT_HIGH_OBLIQUE,
    MOVE_LEFT_HIGH_LEFT = 106,
    MOVE_RIGHT_HIGH_LEFT,
    MOVE_LEFT_HIGH_RIGHT,
    MOVE_RIGHT_HIGH_RIGHT,

    MISSION_1_RED_DUMBLING = 162,

    MISSION_2_MINE_WALK = 167,

    MISSION_3_HURDLING = 173,

    MISSION_5_STAIR_UP = 177,

    MISSION_5_STAIR_DOWN = 180,

    MISSION_6_RIGHT_KICK = 185,

    MISSION_7_YELLOW_DUMBLING = 188,

    TAKE_REST = 193,

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
}


//////////////////////////////
//  MOTION WALK             //
//////////////////////////////

static inline void ACTION_WALK(SPEED speed, VIEW view, int repeat)
{
    action(INIT_MOTION(LOW, view), WALK_MOTION(speed, view));

    for(; repeat > 1; --repeat)
    {
        RobotAction(WALK_MOTION(speed, view));
    }
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

static inline void ACTION_MOVE(DIRECTION dir, POSE pose, VIEW view, int repeat)
{
    action(INIT_MOTION(pose, view), MOVE_MOTION(dir, pose, view));

    for(; repeat > 1; --repeat)
    {
        RobotAction(MOVE_MOTION(dir, pose, view));
    }
}

//////////////////////////////
//  MOTION MISSION          //
//////////////////////////////

static inline void ACTION_MISSION(MOTION mission, int repaet = 1)
{
    action(MIDDLE_UP, mission);

    for (; repeat > 1; --repeat)
    {
        RobotAction(mission);
    }
}

#endif
