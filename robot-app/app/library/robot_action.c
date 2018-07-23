//
// Created by Gihyeon Yang on 2018. 7. 7..
//

#include "./robot_action.h"

void foo(MOTION_INIT init_motion, FOO_MOD mod)
{
    static MOTION_INIT prev = INIT_NIL;

    if (mod == CHECK)
    {
        if (init_motion != prev)
        {
            RobotAction(init_motion);
            prev = init_motion;
        }
    }
    else if (mod == SET)
    {
        prev = init_motion;
    }
}