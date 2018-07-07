//
// Created by Gihyeon Yang on 2018. 7. 7..
//

#include "./robot_action.h"

void Action_INIT_ROBOT(void) {
    RobotAction(INIT_ROBOT);
}

void Action_WATCH_BELOW_LONG(void) {
    RobotAction(WATCH_BELOW_LONG);
}

void Action_WATCH_BELOW_SHORT(void) {
    RobotAction(WATCH_BELOW_SHORT);
}

void Action_WALK_FRONT_LONG(int repeat) {
    RobotAction(FRONT_WALK_START_LONG);

    for (; repeat > 0; --repeat) {
        RobotAction(FRONT_RIGHT_TO_LEFT_LONG);
        RobotAction(FRONT_LEFT_TO_RIGHT_LONG);
    }

    RobotAction(FRONT_WALK_END_LONG);
}

void Action_WALK_FRONT_SHORT(int repeat) {
    RobotAction(FRONT_WALK_START_SHORT);

    for (; repeat > 0; --repeat) {
        RobotAction(FRONT_RIGHT_TO_LEFT_SHORT);
        RobotAction(FRONT_LEFT_TO_RIGHT_SHORT);
    }

    RobotAction(FRONT_WALK_END_SHORT);
}

void Action_LEFT_TURN_HEAD_LONG(void) {
    RobotAction(TURN_LEFT_HEAD_LONG);
}

void Action_RIGHT_TURN_HEAD_LONG(void) {
    RobotAction(TURN_RIGHT_HEAD_LONG);
}

void Action_LEFT_TURN_HEAD_SHORT(void) {
    RobotAction(TURN_LEFT_HEAD_SHORT);
}

void Action_RIGHT_TURN_HEAD_SHORT(void) {
    RobotAction(TURN_RIGHT_HEAD_SHORT);
}

void Action_LEFT_MOVE_SHORT(int repeat) {
    for (; repeat > 0; --repeat) {
        RobotAction(LEFT_MOVE_SHORT);
    }
}

void Action_RIGHT_MOVE_SHORT(int repeat) {
    for (; repeat > 0; --repeat) {
        RobotAction(RIGHT_MOVE_SHORT);
    }
}

void Action_LEFT_MOVE_LONG(int repeat) {
    for (; repeat > 0; --repeat) {
        RobotAction(LEFT_MOVE_LONG);
    }
}

void Action_RIGHT_MOVE_LONG(int repeat) {
    for (; repeat > 0; --repeat) {
        RobotAction(RIGHT_MOVE_LONG);
    }
}

void Action_ESCAPE_RED_BRIDGE(void) {
    RobotAction(MISSION_2_RED_BRIDGE_DUMBLING);
}

void Action_LEFT_TURN_BODY_LONG(int repeat) {
    for (; repeat > 0; --repeat) {
        RobotAction(TURN_LEFT_BODY_LONG);
    }
}

void Action_RIGHT_TURN_BODY_LONG(int repeat) {
    for (; repeat > 0; --repeat) {
        RobotAction(TURN_RIGHT_BODY_LONG);
    }
}

void Action_LEFT_TURN_BODY_SHORT(int repeat) {
    for (; repeat > 0; --repeat) {
        RobotAction(TURN_LEFT_BODY_SHORT);
    }
}

void Action_RIGHT_TURN_BODY_SHORT(int repeat) {
    for (; repeat > 0; --repeat) {
        RobotAction(TURN_RIGHT_BODY_SHORT);
    }
}