//
// Created by Gihyeon Yang on 2018. 7. 7..
//

#include "./robot_action.h"

/////////////////////////////////
//////// Motion: Default ////////
/////////////////////////////////

////////// Motion: INIT /////////
void Action_INIT_ROBOT(void) { RobotAction(INIT_ROBOT); }


////////// Motion: Watch Below /////////
void Action_WATCH_BELOW_SHORT(void) { RobotAction(WATCH_BELOW_SHORT); }

void Action_WATCH_BELOW_MIDDLE(void) { RobotAction(WATCH_BELOW_MIDDLE); }

void Action_WATCH_BELOW_LONG(void) { RobotAction(WATCH_BELOW_LONG); }

////////// Motion: Walk Front /////////
void Action_WALK_FRONT_LONG(int repeat) {
    RobotAction((unsigned char) (FRONT_WALK_START_LONG));

    for (; repeat > 0; --repeat) {
        RobotAction((unsigned char) (FRONT_RIGHT_TO_LEFT_LONG));
        RobotAction((unsigned char) (FRONT_LEFT_TO_RIGHT_LONG));
    }

    RobotAction((unsigned char) (FRONT_WALK_END_LONG));
}

void Action_WALK_FRONT_SHORT(int repeat) {
    RobotAction((unsigned char) (FRONT_WALK_START_LONG + 5));

    for (; repeat > 0; --repeat) {
        RobotAction((unsigned char) (FRONT_RIGHT_TO_LEFT_LONG + 5));
        RobotAction((unsigned char) (FRONT_LEFT_TO_RIGHT_LONG + 5));
    }

    RobotAction((unsigned char) (FRONT_WALK_END_LONG + 5));
}

////////// Motion: Turn Head /////////
void Action_LEFT_TURN_HEAD_LONG(void) { RobotAction(TURN_LEFT_HEAD_LONG); }

void Action_RIGHT_TURN_HEAD_LONG(void) { RobotAction(TURN_RIGHT_HEAD_LONG); }

void Action_LEFT_TURN_HEAD_SHORT(void) { RobotAction(TURN_LEFT_HEAD_SHORT); }

void Action_RIGHT_TURN_HEAD_SHORT(void) { RobotAction(TURN_RIGHT_HEAD_SHORT); }

////////// Motion: Mode Side /////////
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
    RobotAction(LEFT_OR_RIGHT_MOVE_START_LONG);
    for (; repeat > 0; --repeat) {
        RobotAction((unsigned char) (LEFT_MOVE_LONG));
    }
    RobotAction(LEFT_OR_RIGHT_MOVE_END_LONG);
}

void Action_RIGHT_MOVE_LONG(int repeat) {
    RobotAction(LEFT_OR_RIGHT_MOVE_START_LONG);
    for (; repeat > 0; --repeat) {
        RobotAction((unsigned char) (LEFT_MOVE_LONG + 1));
    }
    RobotAction(LEFT_OR_RIGHT_MOVE_END_LONG);
}

////////// Motion: Turn Body /////////
void Action_LEFT_TURN_BODY(int repeat) {
    RobotAction(TURN_BODY_START);
    for (; repeat > 0; --repeat) {
        RobotAction((unsigned char) (TURN_LEFT_BODY));
    }
    RobotAction(TURN_BODY_END);
}

void Action_RIGHT_TURN_BODY(int repeat) {
    RobotAction(TURN_BODY_START);
    for (; repeat > 0; --repeat) {
        RobotAction((unsigned char) (TURN_LEFT_BODY + 1));
    }
    RobotAction(TURN_BODY_END);
}

////////// Motion: Climb Stairs /////////
void Action_CLIMB_UP_STAIRS(void) { RobotAction(CLIMB_UP_STAIRS); }

void Action_CLIMB_DOWN_STAIRS(void) { RobotAction(CLIMB_DOWN_STAIRS); }

/////////////////////////////////
//////// Motion: Mission 2 //////
/////////////////////////////////

void Action_ESCAPE_RED_BRIDGE(void) { RobotAction(MISSION_2_RED_BRIDGE_DUMBLING); }


/////////////////////////////////
//////// Motion: Mission 4 //////
/////////////////////////////////

void Action_MISSION_4_HURDLING(void) { RobotAction(MISSION_4_HURDLING); }


/////////////////////////////////
//////// Motion: Mission 7 //////
/////////////////////////////////

void Action_MISSION_7_YELLOW_HOLE_BRIDG_DUMBLING(void) { RobotAction(MISSION_7_YELLOW_HOLE_BRIDG_DUMBLING); }
