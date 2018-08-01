//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#ifndef SOC_APP_ROBOT_ACTION_H

#include "./robot_protocol.h"

#define SOC_APP_ROBOT_ACTION_H

#define INIT_POSE_COEF 5
#define INIT_VIEW_COEF 1
#define WALK_STEP_COEF 1
#define WALK_SPEED_COEF 20
#define WALK_VIEW_COEF 5
#define TURN_LEN_COEF 18
#define TURN_DIR_COEF 1
#define TURN_POSE_COEF 9
#define TURN_VIEW_COEF 2
#define MOVE_LEN_COEF 27
#define MOVE_DIR_COEF 1
#define MOVE_POSE_COEF 9
#define MOVE_VIEW_COEF 2
#define BIT_DIR_COEF 2
#define MINE_WALK_REPEAT 7

#define INIT_MOTION(pose, view) (INIT_LOW_DOWN + INIT_POSE_COEF*pose + INIT_VIEW_COEF*view)
#define WALK_START_MOTION(speed, view) (WALK_FAST_START_DOWN + WALK_SPEED_COEF*speed + WALK_VIEW_COEF*view)
#define WALK_END_MOTION(speed, view) (WALK_FAST_END_DOWN + WALK_SPEED_COEF*speed + WALK_VIEW_COEF*view)
#define WALK_MOTION(step, speed, view) (WALK_FAST_L_DOWN + WALK_STEP_COEF*step + WALK_SPEED_COEF*speed + WALK_VIEW_COEF*view)
#define TURN_MOTION(len, dir, pose, view) (LONG_TURN_LEFT_LOW_DOWN + TURN_LEN_COEF*len + TURN_DIR_COEF*dir + TURN_POSE_COEF*pose + TURN_VIEW_COEF*view)
#define MOVE_MOTION(len, dir, pose, view) (LONG_MOVE_LEFT_LOW_DOWN + MOVE_LEN_COEF*len + MOVE_DIR_COEF*dir + MOVE_POSE_COEF*pose + MOVE_VIEW_COEF*view)
#define BIT_MOTION(dir) (BIT_FRONT + BIT_DIR_COEF*dir)

typedef enum {
    INIT_LOW_DOWN = 1,
    INIT_LOW_OBLIQUE,
    INIT_LOW_LEFT,
    INIT_LOW_RIGHT,
    INIT_LOW_UP,

    INIT_MIDDLE_DOWN,
    INIT_MIDDLE_OBLIQUE,
    INIT_MIDDLE_LEFT,
    INIT_MIDDLE_RIGHT,
    INIT_MIDDLE_UP,

    INIT_HIGH_DOWN,
    INIT_HIGH_OBLIQUE,
    INIT_HIGH_LEFT,
    INIT_HIGH_RIGHT,
    INIT_HIGH_UP,

    WALK_FAST_START_DOWN = 18,
    WALK_FAST_END_DOWN,
    WALK_FAST_L_DOWN,
    WALK_FAST_R_DOWN,

    WALK_FAST_START_OBLIQUE = 23,
    WALK_FAST_END_OBLIQUE,
    WALK_FAST_L_OBLIQUE,
    WALK_FAST_R_OBLIQUE,

    WALK_FAST_START_RIGHT = 33,
    WALK_FAST_END_RIGHT,
    WALK_FAST_L_RIGHT,
    WALK_FAST_R_RIGHT,

    WALK_SLOW_START_DOWN = 38,
    WALK_SLOW_END_DOWN,
    WALK_SLOW_L_DOWN,
    WALK_SLOW_R_DOWN,

    WALK_SLOW_START_OBLIQUE = 43,
    WALK_SLOW_END_OBLIQUE,
    WALK_SLOW_L_OBLIQUE,
    WALK_SLOW_R_OBLIQUE,

    WALK_SLOW_START_RIGHT = 53,
    WALK_SLOW_END_RIGHT,
    WALK_SLOW_L_RIGHT,
    WALK_SLOW_R_RIGHT,

    WALK_CLOSE_START_DOWN = 58,
    WALK_CLOSE_END_DOWN,
    WALK_CLOSE_L_DOWN,
    WALK_CLOSE_R_DOWN,

    WALK_CLOSE_START_OBLIQUE = 63,
    WALK_CLOSE_END_OBLIQUE,
    WALK_CLOSE_L_OBLIQUE,
    WALK_CLOSE_R_OBLIQUE,

    WALK_CLOSE_START_LEFT = 68,
    WALK_CLOSE_END_LEFT,
    WALK_CLOSE_L_LEFT,
    WALK_CLOSE_R_LEFT,

    WALK_CLOSE_START_RIGHT = 73,
    WALK_CLOSE_END_RIGHT,
    WALK_CLOSE_L_RIGHT,
    WALK_CLOSE_R_RIGHT,

    LONG_TURN_LEFT_LOW_DOWN = 79,
    LONG_TURN_RIGHT_LOW_DOWN,

    LONG_TURN_LEFT_LOW_OBLIQUE,
    LONG_TURN_RIGHT_LOW_OBLIQUE,

    LONG_TURN_LEFT_LOW_LEFT,
    LONG_TURN_RIGHT_LOW_LEFT,

    LONG_TURN_LEFT_LOW_RIGHT,
    LONG_TURN_RIGHT_LOW_RIGHT,

    LONG_TURN_LEFT_MIDDLE_DOWN = 88,
    LONG_TURN_RIGHT_MIDDLE_DOWN,

    LONG_TURN_LEFT_MIDDLE_OBLIQUE,
    LONG_TURN_RIGHT_MIDDLE_OBLIQUE,

    LONG_TURN_LEFT_MIDDLE_LEFT,
    LONG_TURN_RIGHT_MIDDLE_LEFT,

    LONG_TURN_LEFT_MIDDLE_RIGHT,
    LONG_TURN_RIGHT_MIDDLE_RIGHT,

    SHORT_TURN_LEFT_LOW_DOWN = 97,
    SHORT_TURN_RIGHT_LOW_DOWN,

    SHORT_TURN_LEFT_LOW_OBLIQUE,
    SHORT_TURN_RIGHT_LOW_OBLIQUE,

    SHORT_TURN_LEFT_LOW_LEFT,
    SHORT_TURN_RIGHT_LOW_LEFT,

    SHORT_TURN_LEFT_LOW_RIGHT,
    SHORT_TURN_RIGHT_LOW_RIGHT,

    SHORT_TURN_LEFT_MIDDLE_DOWN = 106,
    SHORT_TURN_RIGHT_MIDDLE_DOWN,

    SHORT_TURN_LEFT_MIDDLE_OBLIQUE,
    SHORT_TURN_RIGHT_MIDDLE_OBLIQUE,

    SHORT_TURN_LEFT_MIDDLE_LEFT,
    SHORT_TURN_RIGHT_MIDDLE_LEFT,

    SHORT_TURN_LEFT_MIDDLE_RIGHT,
    SHORT_TURN_RIGHT_MIDDLE_RIGHT,

    LONG_MOVE_LEFT_LOW_DOWN = 116,
    LONG_MOVE_RIGHT_LOW_DOWN,
    LONG_MOVE_LEFT_LOW_OBLIQUE,
    LONG_MOVE_RIGHT_LOW_OBLIQUE,
    LONG_MOVE_LEFT_LOW_LEFT,
    LONG_MOVE_RIGHT_LOW_LEFT,
    LONG_MOVE_LEFT_LOW_RIGHT,
    LONG_MOVE_RIGHT_LOW_RIGHT,

    LONG_MOVE_LEFT_MIDDLE_DOWN = 125,
    LONG_MOVE_RIGHT_MIDDLE_DOWN,
    LONG_MOVE_LEFT_MIDDLE_OBLIQUE,
    LONG_MOVE_RIGHT_MIDDLE_OBLIQUE,
    LONG_MOVE_LEFT_MIDDLE_LEFT,
    LONG_MOVE_RIGHT_MIDDLE_LEFT,
    LONG_MOVE_LEFT_MIDDLE_RIGHT,
    LONG_MOVE_RIGHT_MIDDLE_RIGHT,

    LONG_MOVE_LEFT_HIGH_DOWN = 134,
    LONG_MOVE_RIGHT_HIGH_DOWN,
    LONG_MOVE_LEFT_HIGH_OBLIQUE,
    LONG_MOVE_RIGHT_HIGH_OBLIQUE,
    LONG_MOVE_LEFT_HIGH_LEFT,
    LONG_MOVE_RIGHT_HIGH_LEFT,
    LONG_MOVE_LEFT_HIGH_RIGHT,
    LONG_MOVE_RIGHT_HIGH_RIGHT,

    SHORT_MOVE_LEFT_LOW_DOWN = 143,
    SHORT_MOVE_RIGHT_LOW_DOWN,
    SHORT_MOVE_LEFT_LOW_OBLIQUE,
    SHORT_MOVE_RIGHT_LOW_OBLIQUE,
    SHORT_MOVE_LEFT_LOW_LEFT,
    SHORT_MOVE_RIGHT_LOW_LEFT,
    SHORT_MOVE_LEFT_LOW_RIGHT,
    SHORT_MOVE_RIGHT_LOW_RIGHT,

    SHORT_MOVE_LEFT_MIDDLE_DOWN = 152,
    SHORT_MOVE_RIGHT_MIDDLE_DOWN,
    SHORT_MOVE_LEFT_MIDDLE_OBLIQUE,
    SHORT_MOVE_RIGHT_MIDDLE_OBLIQUE,
    SHORT_MOVE_LEFT_MIDDLE_LEFT,
    SHORT_MOVE_RIGHT_MIDDLE_LEFT,
    SHORT_MOVE_LEFT_MIDDLE_RIGHT,
    SHORT_MOVE_RIGHT_MIDDLE_RIGHT,

    SHORT_MOVE_LEFT_HIGH_DOWN = 161,
    SHORT_MOVE_RIGHT_HIGH_DOWN,
    SHORT_MOVE_LEFT_HIGH_OBLIQUE,
    SHORT_MOVE_RIGHT_HIGH_OBLIQUE,
    SHORT_MOVE_LEFT_HIGH_LEFT,
    SHORT_MOVE_RIGHT_HIGH_LEFT,
    SHORT_MOVE_LEFT_HIGH_RIGHT,
    SHORT_MOVE_RIGHT_HIGH_RIGHT,

    BIT_FRONT = 171,

    BIT_BACK = 173,

    MISSION_2_RED_DUMBLING = 176,

    MISSION_3_MINE_WALK = 181,

    MISSION_4_HURDLING = 187,

    MISSION_5_STAIR_UP = 191,

    MISSION_5_STAIR_DOWN = 195,

    MISSION_6_RIGHT_KICK = 200,

    MISSION_7_YELLOW_DUMBLING = 203,

    HEAD_MIDDLE_DOWN = 210,
    HEAD_MIDDLE_OBLIQUE,
    HAED_MIDDLE_LEFT,
    HEAD_MIDDLE_RIGHT,
    HEAD_MIDDLE_UP,

    HEAD_MIDDLE_SIDE_TO_DOWN = 216,
    HEAD_MIDDLE_DOWN_TO_LEFT,
    HEAD_MIDDLE_DOWN_TO_RIGHT,

    NIL = 0xff
} MOTION;

typedef enum {
    LOW_DOWN = INIT_LOW_DOWN,
    LOW_OBLIQUE = INIT_LOW_OBLIQUE,
    LOW_LEFT = INIT_LOW_LEFT,
    LOW_RIGHT = INIT_LOW_RIGHT,
    LOW_UP = INIT_LOW_UP,

    MIDDLE_DOWN = INIT_MIDDLE_DOWN,
    MIDDLE_OBLIQUE = INIT_MIDDLE_OBLIQUE,
    MIDDLE_LEFT = INIT_MIDDLE_LEFT,
    MIDDLE_RIGHT = INIT_MIDDLE_RIGHT,
    MIDDLE_UP = INIT_MIDDLE_UP,

    HIGH_DOWN = INIT_HIGH_DOWN,
    HIGH_OBLIQUE = INIT_HIGH_OBLIQUE,
    HIGH_LEFT = INIT_HIGH_LEFT,
    HIGH_RIGHT = INIT_HIGH_RIGHT,
    HIGH_UP = INIT_HIGH_UP,

    INIT_NIL = NIL
} MOTION_INIT;

typedef enum {
    LOW = 0,
    MIDDLE,
    HIGH
} POSE;

typedef enum {
    DOWN = 0,
    OBLIQUE,
    LEFT,
    RIGHT,
    UP
} VIEW;

typedef enum {
    FAST = 0,
    SLOW,
    CLOSE
} SPEED;

typedef enum {
    STEP_LEFT = 0,
    STEP_RIGHT
} STEP;

typedef enum {
    DIR_LEFT = 0,
    DIR_RIGHT,
    FRONT = 0,
    BACK
} DIRECTION;

typedef enum {
    LONG = 0,
    SHORT,
} LENGTH;

typedef enum {
    RED_DUMBLING = MISSION_2_RED_DUMBLING,
    MINE_WALK = MISSION_3_MINE_WALK,
    HURDLING = MISSION_4_HURDLING,
    STAIR_UP = MISSION_5_STAIR_UP,
    STAIR_DOWN = MISSION_5_STAIR_DOWN,
    RIGHT_KICK = MISSION_6_RIGHT_KICK,
    YELLOW_DUMBLING = MISSION_7_YELLOW_DUMBLING
} MISSION;

typedef enum {
    CHECK = 0,
    SET,
    HEAD
} PREV_CHECK_MOD;

void prev_check(MOTION_INIT motion, PREV_CHECK_MOD mod);

#define GET_INIT_POSE(motion) ((motion - 1) / 5)
#define GET_INIT_VIEW(motion) ((motion - 1) % 5)

static inline void action(MOTION_INIT init, MOTION motion) {
    prev_check(init, HEAD);
    RobotAction(motion);
}

//////////////////////////////
//  MOTION INIT             //
//////////////////////////////

static inline void CHECK_INIT(POSE pose, VIEW view) {
    prev_check(INIT_MOTION(pose, view), HEAD);
}

static inline void ACTION_INIT(POSE pose, VIEW view) {
    RobotAction(INIT_MOTION(pose, view));
    prev_check(INIT_MOTION(pose, view), SET);
}


//////////////////////////////
//  MOTION WALK             //
//////////////////////////////

static inline void ACTION_WALK(SPEED speed, VIEW view, int repeat) {
    action(INIT_MOTION(MIDDLE, view), WALK_START_MOTION(speed, view));

    for (; repeat > 1; --repeat) {
        RobotAction(WALK_MOTION(STEP_LEFT, speed, view));
        RobotAction(WALK_MOTION(STEP_RIGHT, speed, view));
    }

    RobotAction(WALK_END_MOTION(speed, view));
}


//////////////////////////////
//  MOTION TURN             //
//////////////////////////////

static inline void ACTION_TURN(LENGTH len, DIRECTION dir, POSE pose, VIEW view, int repeat) {
    action(INIT_MOTION(pose, view), TURN_MOTION(len, dir, pose, view));

    for (; repeat > 1; --repeat) {
        RobotAction(TURN_MOTION(len, dir, pose, view));
    }
}


//////////////////////////////
//  MOTION MOVE             //
//////////////////////////////

static inline void ACTION_MOVE(LENGTH len, DIRECTION dir, POSE pose, VIEW view, int repeat) {
    action(INIT_MOTION(pose, view), MOVE_MOTION(len, dir, pose, view));

    for (; repeat > 1; --repeat) {
        RobotAction(MOVE_MOTION(len, dir, pose, view));
    }
}

//////////////////////////////
//  MOTION BIT              //
//////////////////////////////

static inline void ACTION_BIT(DIRECTION dir, int repeat) {
    action(INIT_MOTION(MIDDLE, DOWN), BIT_MOTION(dir));

    for (; repeat > 1; --repeat) {
        RobotAction(BIT_MOTION(dir));
    }
}

//////////////////////////////
//  MOTION NUMBER           //
//////////////////////////////

static inline void ACTION_MOTION(MOTION motion, POSE pose, VIEW view) {
    action(INIT_MOTION(pose, view), motion);
}

static inline void ACTION_MOTION_REPEAT(MOTION motion, POSE pose, VIEW view, int repeat) {
    action(INIT_MOTION(pose, view), motion);

    for (; repeat > 1; --repeat) {
        RobotAction(motion);
    }
}

//////////////////////////////
//  MOTION MISSION          //
//////////////////////////////

static inline void ACTION_MISSION(MISSION mission, POSE pose, VIEW view) {
    ACTION_MOTION(mission, pose, view);
    prev_check(INIT_MOTION(MIDDLE, OBLIQUE), SET);
}

#endif //SOC_APP_ROBOT_ACTION_H
