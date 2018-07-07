//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#ifndef SOC_APP_ROBOT_ACTION_H

#include "./robot_protocol.h"

#define SOC_APP_ROBOT_ACTION_H

/////////////////////////////////
//////// Motion: Default ////////
/////////////////////////////////

#define INIT_ROBOT 1

// Motion: Watch below
#define WATCH_BELOW_LONG 3
#define WATCH_BELOW_SHORT 2

// Motion: Walk
#define FRONT_WALK_START_LONG 5
#define FRONT_RIGHT_TO_LEFT_LONG 6
#define FRONT_LEFT_TO_RIGHT_LONG 7
#define FRONT_WALK_END_LONG 8

#define FRONT_WALK_START_SHORT 10
#define FRONT_RIGHT_TO_LEFT_SHORT 11
#define FRONT_LEFT_TO_RIGHT_SHORT 12
#define FRONT_WALK_END_SHORT 13

// Motion: Turn head
#define TURN_LEFT_HEAD_LONG 15
#define TURN_RIGHT_HEAD_LONG 16
#define TURN_LEFT_HEAD_SHORT 17
#define TURN_RIGHT_HEAD_SHORT 18

// Motion: Move side
#define LEFT_MOVE_SHORT 22
#define RIGHT_MOVE_SHORT 23
#define LEFT_MOVE_LONG 20
#define RIGHT_MOVE_LONG 21

// Motion: Turn body
#define TURN_LEFT_BODY_LONG 25
#define TURN_RIGHT_BODY_LONG 26
#define TURN_LEFT_BODY_SHORT 27
#define TURN_RIGHT_BODY_SHORT 28


/////////////////////////////////
//////// Motion: Mission 2 //////
/////////////////////////////////

#define MISSION_2_RED_BRIDGE_DUMBLING 30

/////////////////////////////////

void Action_INIT_ROBOT(void);

void Action_WATCH_BELOW_LONG(void);

void Action_WATCH_BELOW_SHORT(void);

void Action_WALK_FRONT_LONG(int repeat);

void Action_WALK_FRONT_SHORT(int repeat);

void Action_LEFT_TURN_HEAD_LONG(void);

void Action_RIGHT_TURN_HEAD_LONG(void);

void Action_LEFT_TURN_HEAD_SHORT(void);

void Action_RIGHT_TURN_HEAD_SHORT(void);

void Action_LEFT_MOVE_SHORT(int repeat);

void Action_RIGHT_MOVE_SHORT(int repeat);

void Action_LEFT_MOVE_LONG(int repeat);

void Action_RIGHT_MOVE_LONG(int repeat);

void Action_ESCAPE_RED_BRIDGE(void);

void Action_LEFT_TURN_BODY_LONG(int repeat);

void Action_RIGHT_TURN_BODY_LONG(int repeat);

void Action_LEFT_TURN_BODY_SHORT(int repeat);

void Action_RIGHT_TURN_BODY_SHORT(int repeat);

#endif //SOC_APP_ROBOT_ACTION_H
