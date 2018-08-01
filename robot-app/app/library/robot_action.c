//
// Created by Gihyeon Yang on 2018. 7. 7..
//

#include "./robot_action.h"

void prev_check(MOTION_INIT init_motion, PREV_CHECK_MOD mod)
{
    static MOTION_INIT prev = INIT_NIL;

    if (mod == CHECK)
    {
        if (init_motion != prev)
        {
            RobotAction(init_motion);
        }
    }
    else if (mod == SET)
    {
    }
    else if (mod == HEAD)
    {
        if (init_motion != prev) 
        {
            if (GET_INIT_POSE(prev) == GET_INIT_POSE(init_motion))
            {
                if (GET_INIT_VIEW(prev) == DOWN && (GET_INIT_VIEW(init_motion) == LEFT || GET_INIT_VIEW(init_motion) == RIGHT))
                {
                    RobotAction(HEAD_MIDDLE_DOWN_TO_LEFT + GET_INIT_VIEW(init_motion) - LEFT);
                }
                else if (GET_INIT_VIEW(init_motion) == DOWN && (GET_INIT_VIEW(prev) == LEFT || GET_INIT_VIEW(prev) == RIGHT))
                {
                    RobotAction(HEAD_MIDDLE_SIDE_TO_DOWN);
                }
                else
                {
                    RobotAction(HEAD_MIDDLE_DOWN + GET_INIT_VIEW(init_motion));
                }
            }
            else 
            {
                RobotAction(init_motion);
            }

        }
    }
    prev = init_motion;
}
