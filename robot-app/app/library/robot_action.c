//
// Created by Gihyeon Yang on 2018. 7. 7..
//

#include "./robot_action.h"

void prev_check(MOTION init_motion, PREV_CHECK_MOD mod) {
    static MOTION prev = NIL;

    if (mod == CHECK) {
        if (init_motion != prev) {
            RobotAction(init_motion);
        }
    } else if (mod == SET) {
    } else if (mod == HEAD) {
        if (init_motion != prev) {
            if (GET_INIT_VIEW(prev) == DOWN &&
                (GET_INIT_VIEW(init_motion) == LEFT || GET_INIT_VIEW(init_motion) == RIGHT)) {
                RobotAction(HEAD_DOWN_TO_LEFT + GET_INIT_VIEW(init_motion) - LEFT);
            } else if (GET_INIT_VIEW(init_motion) == DOWN &&
                       (GET_INIT_VIEW(prev) == LEFT || GET_INIT_VIEW(prev) == RIGHT)) {
                RobotAction(HEAD_SIDE_TO_DOWN);
            } else {
                RobotAction(HEAD_DOWN + GET_INIT_VIEW(init_motion));
            }
        }
    }
    prev = init_motion;
}

void *checker(void *args)
{
    _args_t data = *(_args_t *)args;

    while(*data.destroy == 0)
    {
        setFPGAVideoData(data.p_image);
        *data.p_state = data.check(data.p_image);
    }

    return (void *)data.p_state;
}

int ACTION_WALK_CHECK(VIEW view, U16 *image,  int (*check)(U16 *), int finish, int repeat) {
    int i;
    int state = 0, destroy = 0;

    pthread_t p_thread;
    int thread_id;
    int result;

    _args_t args = {image, check, &state, finish, &destroy};

    thread_id = pthread_create(&p_thread, NULL, checker, (void *)&args);

    if (thread_id < 0)
    {
        printf("thread create error\n");
        return 0;
    }

    action(INIT_MOTION(view), WALK_START_MOTION(SLOW, view));

    for (i = 0; i >> 1 < repeat; ++i) {
        if (state == finish) {
            break;
        } else {
            RobotAction(WALK_MOTION((i & 1), SLOW, view));
        }
    }

    destroy = 1;

    if (i & 1) {
        RobotAction(WALK_MOTION(DIR_RIGHT, SLOW, view));
    }

    RobotAction(WALK_END_MOTION(SLOW, view));

    pthread_join(p_thread, (void *)&result);

    return *(int *)result;
}
