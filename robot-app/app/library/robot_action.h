//
// Created by Gihyeon Yang on 2018. 7. 5..
//

#ifndef SOC_APP_ROBOT_ACTION_H

#include "./robot_protocol.h"

#define SOC_APP_ROBOT_ACTION_H

#define ROBOT_CENTER_OFFSET 10

/////////////////////////////////
//////// Motion: Default ////////
/////////////////////////////////

#define INIT_ROBOT 1

void Action_INIT_ROBOT(void);

// Motion: Watch below
#define WATCH_BELOW_SHORT 2
#define WATCH_BELOW_MIDDLE 3
#define WATCH_BELOW_LONG 4

void Action_WATCH_BELOW_SHORT(void);

void Action_WATCH_BELOW_MIDDLE(void);

void Action_WATCH_BELOW_LONG(void);

// Motion: Walk
#define FRONT_WALK_START_LONG 5
#define FRONT_RIGHT_TO_LEFT_LONG 6
#define FRONT_LEFT_TO_RIGHT_LONG 7
#define FRONT_WALK_END_LONG 8

void Action_WALK_FRONT_LONG(int repeat);

void Action_WALK_FRONT_SHORT(int repeat);

// Motion: Turn head
#define TURN_LEFT_HEAD_LONG 15
#define TURN_RIGHT_HEAD_LONG 16
#define TURN_LEFT_HEAD_SHORT 17
#define TURN_RIGHT_HEAD_SHORT 18

void Action_LEFT_TURN_HEAD_LONG(void);

void Action_RIGHT_TURN_HEAD_LONG(void);

void Action_LEFT_TURN_HEAD_SHORT(void);

void Action_RIGHT_TURN_HEAD_SHORT(void);

// Motion: Move side
#define LEFT_OR_RIGHT_MOVE_START_LONG 20
#define LEFT_OR_RIGHT_MOVE_END_LONG 21
#define LEFT_MOVE_LONG 22

void Action_LEFT_MOVE_LONG(int repeat);

void Action_RIGHT_MOVE_LONG(int repeat);

#define LEFT_MOVE_SHORT 24
#define RIGHT_MOVE_SHORT 25

void Action_LEFT_MOVE_SHORT(int repeat);

void Action_RIGHT_MOVE_SHORT(int repeat);

// Motion: Turn body
#define TURN_BODY_START 27
#define TURN_BODY_END 28
#define TURN_LEFT_BODY 29
#define TURN_RIGHT_BODY 30

void Action_LEFT_TURN_BODY(int repeat);

void Action_RIGHT_TURN_BODY(int repeat);

////////// Motion: Climb Stairs /////////
#define CLIMB_UP_STAIRS 43
#define CLIMB_DOWN_STAIRS 46

void Action_CLIMB_UP_STAIRS(void);

void Action_CLIMB_DOWN_STAIRS(void);

/////////////////////////////////
//////// Motion: Mission 2 //////
/////////////////////////////////

#define MISSION_2_RED_BRIDGE_DUMBLING 32

void Action_ESCAPE_RED_BRIDGE(void);

/////////////////////////////////
//////// Motion: Mission 4 //////
/////////////////////////////////

#define MISSION_4_HURDLING 36

void Action_MISSION_4_HURDLING(void);


/////////////////////////////////
//////// Motion: Mission 7 //////
/////////////////////////////////

#define MISSION_7_YELLOW_HOLE_BRIDG_DUMBLING 39

void Action_MISSION_7_YELLOW_HOLE_BRIDGE_DUMBLING(void);


/////////////////////////////////
//////// Motion: Mission 7 //////
/////////////////////////////////

#define MISSION_6_KICK_BALL 70

void Action_MISSION_6_KICK_BALL(void);

/////////////////////////////////

#endif //SOC_APP_ROBOT_ACTION_H
