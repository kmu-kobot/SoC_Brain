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
            if (GET_INIT_VIEW(prev) != UP &&
                (GET_INIT_VIEW(init_motion) == LEFT || GET_INIT_VIEW(init_motion) == RIGHT)) {
                RobotAction(HEAD_DOWN_TO_LEFT + GET_INIT_VIEW(init_motion) - LEFT);
            } else if (GET_INIT_VIEW(init_motion) != UP &&
                       (GET_INIT_VIEW(prev) == LEFT || GET_INIT_VIEW(prev) == RIGHT)) {
                RobotAction(HEAD_SIDE_TO_DOWN + GET_INIT_VIEW(init_motion));
            } else {
                RobotAction(HEAD_DOWN + GET_INIT_VIEW(init_motion));
            }
        }
    }
    prev = init_motion;
}

void move_check(MOTION motion, VIEW view) {
    static MOTION moved = 0;

    if (moved && !IS_MOVE(motion)) {
        RobotAction(STABLE_DOWN + view);
    }
    moved = IS_MOVE(motion);
}

void *checker(void *args) {
    _args_t data = *(_args_t *) args;

    while (*data.destroy == 0) {
        setFPGAVideoData(data.p_image);
        *data.p_state = data.check(data.p_image);
    }

    return (void *) data.p_state;
}

int ACTION_WALK_CHECK(VIEW view, U16 *image, int (*check)(U16 *), int finish, int repeat) {
    int i = 0;
    int state = 0, destroy = 0;

    pthread_t p_thread;
    int thread_id;
    int result;

    _args_t args = {image, check, &state, finish, &destroy};

    repeat <<= 1;
    thread_id = pthread_create(&p_thread, NULL, checker, (void *) &args);

    if (thread_id < 0) {
        printf("thread create error\n");
        return 0;
    }

    action(INIT_MOTION(view), WALK_START_MOTION(SLOW, view));

    while (state != finish && i < repeat) {
        RobotAction(WALK_MOTION((i++ & 1), SLOW, view));
    }

    destroy = 1;

    RobotAction(WALK_END_MOTION((--i & 1), SLOW, view));

    pthread_join(p_thread, (void *) &result);

    return finish == state;
}

int GREEN_WALK_CHECK(U16 *image, int (*check)(U16 *), int repeat) {
    int i = 0;
    int state = 0, destroy = 0;

    pthread_t p_thread;
    int thread_id;
    int result;

    _args_t args = {image, check, &state, 1, &destroy};

    repeat <<= 1;
    thread_id = pthread_create(&p_thread, NULL, checker, (void *) &args);

    if (thread_id < 0) {
        printf("thread create error\n");
        return 0;
    }

    action(INIT_MOTION(OBLIQUE), GREEN_WALK_START);

    while (state != 1 && i < repeat) {
        RobotAction(GREEN_WALK_L + (i++ & 1));
    }

    destroy = 1;

    RobotAction(GREEN_WALK_END_L + (--i & 1));

    pthread_join(p_thread, (void *) &result);

    return 1 == state;
}

int MINE_WALK_CHECK(U16 *image, int (*check)(U16 *), int repeat) {
    int i = 0;
    int state = 0, destroy = 0;

    pthread_t p_thread;
    int thread_id;
    int result;

    _args_t args = {image, check, &state, 1, &destroy};

    repeat <<= 1;
    thread_id = pthread_create(&p_thread, NULL, checker, (void *) &args);

    if (thread_id < 0) {
        printf("thread create error\n");
        return 0;
    }

    action(INIT_MOTION(OBLIQUE), MINE_WALK_START);

    while (state != 1 && i < repeat) {
        RobotAction(MINE_WALK_L + (i++ & 1));
    }

    destroy = 1;

    RobotAction(MINE_WALK_END_L + (--i & 1));

    pthread_join(p_thread, (void *) &result);

    return 1 == state;
}


int STAIR_WALK_CHECK(U16 *image, int (*check)(U16 *), int repeat) {
    int i = 0;
    int state = 0, destroy = 0;

    pthread_t p_thread;
    int thread_id;
    int result;

    _args_t args = {image, check, &state, 1, &destroy};

    repeat <<= 1;
    thread_id = pthread_create(&p_thread, NULL, checker, (void *) &args);

    if (thread_id < 0) {
        printf("thread create error\n");
        return 0;
    }

    action(INIT_MOTION(OBLIQUE), STAIR_WALK_START);

    while (state != 1 && i < repeat) {
        RobotAction(STAIR_WALK_L + (i++ & 1));
    }

    destroy = 1;

    RobotAction(STAIR_WALK_END_L + (--i & 1));

    pthread_join(p_thread, (void *) &result);

    return 1 == state;
}


int YELLOW_WALK_CHECK(U16 *image, int (*check)(U16 *), int repeat) {
    int i = 0;
    int state = 0, destroy = 0;

    pthread_t p_thread;
    int thread_id;
    int result;

    _args_t args = {image, check, &state, 1, &destroy};

    repeat <<= 1;
    thread_id = pthread_create(&p_thread, NULL, checker, (void *) &args);

    if (thread_id < 0) {
        printf("thread create error\n");
        return 0;
    }

    action(INIT_MOTION(OBLIQUE), YELLOW_WALK_START);

    while (state != 1 && i < repeat) {
        RobotAction(YELLOW_WALK_L + (i++ & 1));
    }

    destroy = 1;

    RobotAction(YELLOW_WALK_END_L + (--i & 1));

    pthread_join(p_thread, (void *) &result);

    return 1 == state;
}

