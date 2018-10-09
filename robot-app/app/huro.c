//
// Created by Gihyeon Yang on 2018. 7. 2..
//

#include "huro.h"

int huro(void) {
    // UART INIT
    init_huro();

    // malloc fpga video data
    U16 *fpga_videodata = (U16 *) malloc(WIDTH * HEIGHT * 2);

    int missionFinished = 0;

    int mission = 0;
    int step = 0;
    int state;
    int dir;

    int nextMission = 0;
    int flag = 0;

    while (!missionFinished) {

        setFPGAVideoData(fpga_videodata);

#ifdef DEBUG
        printf("\n[START]\tMISSION[%d]: STEP [%d]\n", mission, step);
#endif

        switch (mission) {
            case 0: // MISSION 0: READY PLAY   // 빼도 될거같음
                ++mission;
                break;
            case 1: // MISSION 1: YELLOW BARRICADE
                switch (step) {
                    case 0:
                        step += mission_1_1_wait_yellow_barricade(fpga_videodata);
                        break;
                    case 1:
                        step += mission_1_2_end_yellow_barricade(fpga_videodata);
                        break;
                    case 2:
                        mission_1_3_escape_yellow_barricade(8);
                        step += 1;
                        flag = 0;
                        break;
                    case 3:
                        step = 0;
                        mission += 1;
                        flag = 3;
                        break;
                    default:
                        break;
                }
                break;
            case 2: // MISSION 2: RED BRIDGE
                switch (step) {
                    case 0:
                        ++step;
                        flag = 0;
                        break;
                    case 1:
                        ++step;
                        break;
                    case 2:
#if MODE == 3
                        if (flag == 0) {
                            dir = LEFT;
                            default_watch(LEFT, fpga_videodata);
                            flag++;
                        } else if (flag == 3) {
                            dir = RIGHT;
                            default_watch(RIGHT, fpga_videodata);
                            flag++;
                        }

                        setFPGAVideoData(fpga_videodata);
                        state = default_set_center1_long(fpga_videodata, dir, (WIDTH_CENTER) + (dir == LEFT ? -30 : 30), HEIGHT - 4, BLACK, DEFAULT_CENTER_DISTANCE);
                        if (state == 1) {
                            ++step;
                        } else if (state == -1) {
                            if (++flag == 5) {
                                flag = 0;
                            }
                        }
                        // flag = step == 2 ? 2 : flag;
#else
                        ++step;
#endif
                        break;
                    case 3:
                        step += mission_2_3_escape_red_bridge();
                        flag = 0;
                        break;
                    case 4:
                        if (flag == 0) {
                            default_watch((VIEW) mission_3_4_getMDir(), fpga_videodata);
                            // RobotSleep(1);
                            flag++;
                        }

#ifndef MINE
                        step += mission_3_set_straight_and_center1_long(fpga_videodata, WIDTH_CENTER +
                                                                                        (mission_3_4_getMDir() == LEFT
                                                                                         ? 50 : -50));
#else
                        step += mission_3_set_straight(fpga_videodata);
#endif
                        break;
                    case 5:
                        flag = 0;
                        mission += 1;
                        step = 0;
                        break;
                    default:
                        break;
                }
                break;
            case 3: // MISSION 3: AVOID BOMB
                /*
                     * 1. 지뢰 있는지 보면서 걷기
                     * 2. 지뢰 발견하면 옆에보고 어디로 갈지 선택 (이때, 각도가 True 인지도 동시에 검사)
                     * 3. 지뢰 피하기
                     * 4. 만약, 2에서 각도가 False라면 각도 맞추기
                     * */
#ifdef MINE
                switch (step) {
                    case 0:
                        // 앞에 있는 지뢰 감지
                        default_watch(OBLIQUE, fpga_videodata);

                        step = 1;
                        flag = 0;

                        if (!mission_3_default_avoid_bomb(fpga_videodata)) {
                            step = mission_3_default_watch_below(fpga_videodata, 10) ? 1 : 2;
                            flag = step == 2 ? 5 : 0;
                        }

                        setFPGAVideoData(fpga_videodata);
                        step = mission_3_isFrontOf_Blue(fpga_videodata, HEIGHT) ? 4 : step;
                        flag = flag == 5 ? flag : 0;

                        if (step == 1) {
                            default_watch(DOWN, fpga_videodata);
                            setFPGAVideoData(fpga_videodata);
                            mission_3_attach_mine(fpga_videodata);
                        }
                        break;
                    case 1:
                        if (flag == 0) {
                            mission_3_change_mdir(fpga_videodata);
                            ++flag;
                        }
                        if (flag == 1) {
                            ++flag;
                            default_watch(DOWN, fpga_videodata);
                            // RobotSleep(1);
                            setFPGAVideoData(fpga_videodata);
                        }

                        setFPGAVideoData(fpga_videodata);
                        if (mission_3_isFrontOf_Blue(fpga_videodata, ROBOT_KNEE)) {
                            // RobotSleep(1);
                            setFPGAVideoData(fpga_videodata);
                            if (mission_3_isFrontOf_Blue(fpga_videodata, ROBOT_KNEE)) {
                                step = 4;
                                flag = 0;
                                break;
                            }
                        }

                        setFPGAVideoData(fpga_videodata);
                        if (mission_3_avoid(fpga_videodata)) { // 만약 지뢰를 다 피했으면
                            mission_3_inverse_mdir();
                            step = 2;
                            flag = 0;
                        }

                        break;
                    case 2:
                        // 방향 설정
                        if (flag == 5) {
                            mission_3_change_mdir(fpga_videodata);
                            ++flag;

                            if (!mission_3_check_angle()) {
                                step = 3;
                                flag = 0;
                                break;
                            }

                            break;
                        } else if (flag == 0) {
                            // 여기서는 저장된 플래그에 따라 방향을 볼지 말지 결정함
                            // 만약 각도를 맞춰야한다면 해당하는 방향을 봄
                            // 아니면 step 증가 시키도 아웃

                            if (!mission_3_check_angle()) {
                                ++step;
                                break;
                            }

                            flag = 6;
                            flag += mission_3_change_mdir(fpga_videodata);
                            // mission_3_change_mdir_opposite();
                        }

                        // 고개 돌리기
                        if (flag == 6) {
                            default_watch((VIEW) mission_3_4_getMDir(), fpga_videodata);
//                            RobotSleep(1);
                            ++flag;
                            break;
                        }

                        step += mission_3_set_straight(fpga_videodata);
                        flag = (step == 3) ? 0 : 7;
                        break;
                    case 3:
                        default_watch(OBLIQUE, fpga_videodata);
                        setFPGAVideoData(fpga_videodata);
                        step = (mission_3_1_ver2(fpga_videodata)) ? 4 : 0;
                        flag = 0;
                        break;
                    case 4:
                        if (flag == 0) {
                            flag += mission_3_change_mdir(fpga_videodata);
                            ++flag;
                        }
                        if (flag == 1) {
                            ++flag;
                            default_watch((VIEW)mission_3_4_getMDir(), fpga_videodata);
                            setFPGAVideoData(fpga_videodata);
                        }
                        step += mission_3_set_straight_and_center1_long(fpga_videodata, (WIDTH_CENTER) - (mission_3_4_getMDir() == LEFT ? 40 : -40));
                        break;
                    case 5:
                        ACTION_ATTACH(1);
                        ++step;
                        flag = 2;
                        break;
                    case 6:
                        mission += 1;
                        step = 0;
                        flag = 0;
                        mission_3_init_global();
                    default:
                        break;
                }
#else
                switch (step) {
                    case 0:
                        flag = 2;
                    case 1:
                        if (flag == 2) {
                            default_watch(OBLIQUE, fpga_videodata);
                            flag = 0;
                        }

                        step = mission_3_default_watch_below(fpga_videodata, 10) &&
                               mission_3_isFrontOf_Blue(fpga_videodata, HEIGHT) ? 3 : 2;

                        break;
                    case 2:
                        if (flag == 0) {
                            flag += mission_3_change_mdir(fpga_videodata);
                            ++flag;
                        }
                        if (flag == 1) {
                            ++flag;
                            default_watch((VIEW)mission_3_4_getMDir(), fpga_videodata);
                        }
                        setFPGAVideoData(fpga_videodata);
                        step -= mission_3_set_straight(fpga_videodata);
                        break;
                    case 3:
                        if (flag == 0) {
                            flag += mission_3_change_mdir(fpga_videodata);
                            ++flag;
                        }
                        if (flag == 1) {
                            ++flag;
                            default_watch((VIEW)mission_3_4_getMDir(), fpga_videodata);
                            setFPGAVideoData(fpga_videodata);
                        }
                        step += mission_3_set_straight_and_center1_long(fpga_videodata, WIDTH_CENTER);
                        break;
                    case 4:
                        step = 0;
                        flag = 0;
                        ++mission;
                        break;
                }
#endif
                break;
            case 4: // MISSION 4: JUMP HURDLE
                switch (step) {
                    case 0:
#ifndef MINE
                        CHECK_INIT(DOWN);
#endif
                        ACTION_ATTACH(1);
                        mission_4_4_jump_hurdle();
                        ++step;
                        break;
                    case 1:
                        if (flag == 0) {
                            dir = RIGHT;
                            default_watch(RIGHT, fpga_videodata);
                            flag++;
                        } else if (flag == 3) {
                            dir = LEFT;
                            default_watch(LEFT, fpga_videodata);
                            flag++;
                        }

                        setFPGAVideoData(fpga_videodata);
#if MODE == 3
                        state = default_set_straight_and_center1(fpga_videodata, dir, WIDTH_CENTER + (dir == LEFT ? 30 : -30), HEIGHT - 4, BLACK, DEFAULT_CENTER_DISTANCE);
#else
                        state = default_set_straight_and_center1_long(fpga_videodata, dir,
                                                                      WIDTH_CENTER + (dir == LEFT ? 40 : -40),
                                                                      HEIGHT - 4, BLACK, DEFAULT_CENTER_DISTANCE);
#endif

                        if (state == 1) {
                            flag = 0;
                            ++step;

                            default_watch(OBLIQUE, fpga_videodata);
                            RobotSleep(2);
                        } else if (state == -1) {
                            if (++flag == 5) {
                                flag = 0;
                            }
                        }
                        break;
                    case 2:
                        mission = 5;
                        step = 0;
                        flag = 0;
                        break;
                    default:
                        break;
                }
                break;
            case 5: // MISSION 5: GREEN BRIDGE
                switch (step) {
                    case 0:
                        if (flag == 0) {
                            CHECK_INIT(OBLIQUE);
                            ++flag;
                        }

                        setFPGAVideoData(fpga_videodata);
                        if (!mission_5_1_check_black_line(fpga_videodata)) {
                            mission_5_1_watch_below(fpga_videodata, 50);
                        }
//                        ACTION_ATTACH(2);
                        ++step;
                        //                        flag = 0;
                        break;
                    case 1:
                        flag = 0;
                        setFPGAVideoData(fpga_videodata);
                        step += mission_5_1_attach(fpga_videodata);

                        if (step == 2) {
                            ACTION_ATTACH(1);
                        }
                        break;
                    case 2:
                        // 맨 처음 다리 중심 맞추기
                        if (flag == 0) {
                            dir = LEFT;
                            default_watch(LEFT, fpga_videodata);
                            flag++;
                        } else if (flag == 3) {
                            dir = RIGHT;
                            default_watch(RIGHT, fpga_videodata);
                            flag++;
                        }

                        setFPGAVideoData(fpga_videodata);
                        state = default_set_center1_long(fpga_videodata, dir, WIDTH_CENTER + (dir == LEFT ? -30 : 30),
                                                         HEIGHT - 4, BLACK, DEFAULT_CENTER_DISTANCE+3);
                        if (state == -1) {
                            if (++flag == 5) {
                                flag = 0;
                            }
                        } else if (state == 1) {
                            step = 3;

                            if (flag == 99) {
                                ACTION_ATTACH(1);
                            } else {
                                ACTION_ATTACH_SHORT(1);
                            }

                        } else {
                            flag = 99;
                        }

                        break;
                    case 3:
                        // 계단 오르기
                        mission_5_3_climb_up_stairs();
                        flag = 0;
                        ++step;
                        break;
                    case 4:
                        if (flag == 0) {
                            default_watch(OBLIQUE, fpga_videodata);
                            ++flag;
                        }
                        setFPGAVideoData(fpga_videodata);
                        step += mission_5_4_set_straight_and_center(fpga_videodata, 60);
                        break;
                    case 5:
                        mission_5_5_attach_green();
                        ++step;
                        flag = 0;
                        break;
                    case 6:
                        if (flag == 0) {
                            default_watch(OBLIQUE, fpga_videodata);
                            ++flag;
                        }

                        setFPGAVideoData(fpga_videodata);
                        step += mission_5_6_set_straight_and_center(fpga_videodata, 60);
                        break;
                    case 7:
                        step += mission_5_7_watch_below(fpga_videodata);
//                        RobotSleep(2);
                        break;
                    case 8:
                        CHECK_INIT(OBLIQUE);
                        setFPGAVideoData(fpga_videodata);
                        step += mission_5_8_attach_black(fpga_videodata);
                        flag = 0;
                        break;
                    case 9:
                        if (flag == 0) {
                            default_watch(DOWN, fpga_videodata);
//                            RobotSleep(1);
                            ++flag;
                        }
                        step += mission_5_9_attach_black(fpga_videodata) && mission_5_9_attach_black(fpga_videodata);
                        break;
                    case 10:
                        mission_5_10_climb_down_stairs();
                        ++step;
                        flag = 0;
                        break;
                    case 11:
                        ++step;
                        break;
                    case 12:
//                        ACTION_WALK(FAST, OBLIQUE, 5);
                        ++step;
                        break;
                    case 13:
                        ++mission;
                        flag = 0;
                        step = 0;
                        break;
                    default:
                        break;
                }
                break;
            case 6: // MISSION 6: KICK BALL
                switch (step) {
                    case 0:
                        flag = 0;
                        default_watch(UP, fpga_videodata);
                        RobotSleep(1);
                        setFPGAVideoData(fpga_videodata);
                        step += mission_6_1_detection_ball(fpga_videodata);
                        break;
                    case 1:
                        if (flag == 0) {
                            BALL_INIT(DOWN);
                            RobotSleep(1);
                            setFPGAVideoData(fpga_videodata);
                            ++flag;
                        }

                        setFPGAVideoData(fpga_videodata);
                        step += mission_6_4_set_center_of_ball(fpga_videodata);

                        if (step == 2) {
                            flag = 0;
                        }
                        break;
                    case 2:
                        if (flag == 0) {
                            BALL_HEAD(UP);
                            RobotSleep(1);
                            ++flag;
                        }

                        setFPGAVideoData(fpga_videodata);
                        if (mission_6_3_find_hole_interpolation(fpga_videodata, 0, HEIGHT, 0, WIDTH, 0) == 0) {
                            mission_6_3_find_side_hole(fpga_videodata, 0);
                        } else {
                            ++step;
                            flag = 0;
                        }
                        break;
                    case 3:
                        ++step;
                        break;
                    case 4:
                        ++step;
                        break;
                    case 5:
                        if (!flag) {
                            flag = 1;
                            BALL_HEAD(UP);
                            RobotSleep(1);
                        }

                        setFPGAVideoData(fpga_videodata);
                        step += mission_6_3_locate_hole_on_center(fpga_videodata);

                        if (step == 6) {
                            flag = 0;
                            BALL_HEAD(DOWN);
                            RobotSleep(1);
                            while (TRUE) {
                                setFPGAVideoData(fpga_videodata);
                                step += mission_6_4_set_center_of_ball(fpga_videodata);
                                if (step == 7) {
                                    break;
                                }
                            }
                        }

                        if (step == 7) {
                            BALL_HEAD(UP);
                            RobotSleep(1);
                            setFPGAVideoData(fpga_videodata);
                            step += mission_6_3_locate_hole_on_center(fpga_videodata);
                        }

                        if (step == 8) {
                            BALL_HEAD(DOWN);
                            RobotSleep(1);
                            setFPGAVideoData(fpga_videodata);
                            step += mission_6_4_set_center_of_ball(fpga_videodata);
                        }

                        step = (step == 9) ? 6 : 5;

                        break;
                    case 6:
                        flag = 0;
                        step += mission_6_5_kick_ball();
                        break;
                    case 7:
                        setFPGAVideoData(fpga_videodata);
                        step += default_set_not_black(fpga_videodata);
                        break;
                    case 8:
                        // mission_6_9_walk_front();
                        ++step;
                        break;
                    default:
                        mission = 10;
                        step = 0;
                        flag = 0;
                        nextMission = 7;
                        break;
                }
                break;
            case 7: // MISSION 7: YELLOW HOLE
                switch (step) {
                    case 0:
                        if (flag == 0) {
                            CHECK_INIT(OBLIQUE);
//                            RobotSleep(1);
                            setFPGAVideoData(fpga_videodata);
                            mission_7_1_watch_below(fpga_videodata, 30);
                        }
                        setFPGAVideoData(fpga_videodata);
                        step += mission_7_1_wait_front_of_yellow_hole_bridge(fpga_videodata);

                        if (step == 1) {
                            ACTION_ATTACH(2); // 4개 너무 많음
                        }
                        // 위에서 노란다리를 인식한후에 수행하는 라인이므로, 다시 수행하는 것은 무의미
                        //  ^ 걷다가 잘못멈출 수도 있으니까 다시 검사

                        // setFPGAVideoData(fpga_videodata);
                        // step += mission_7_1_wait_front_of_yellow_hole_bridge(fpga_videodata);
                        // flag = (step == 1);
                        break;
                    case 1:
                        ++step;
//                        flag = 0;
//                        setFPGAVideoData(fpga_videodata);
//                        step += mission_7_1_attach_yellow_bridge(fpga_videodata);
                        break;
                    case 2:
                        if (flag == 0) {
                            dir = LEFT;
                            default_watch(LEFT, fpga_videodata);
                            flag++;
                        } else if (flag == 3) {
                            dir = RIGHT;
                            default_watch(RIGHT, fpga_videodata);
                            flag++;
                        }

                        setFPGAVideoData(fpga_videodata);
                        state = default_set_center1(fpga_videodata, dir, WIDTH_CENTER + (dir == LEFT ? -30 : 30),
                                                    HEIGHT - 4, BLACK, DEFAULT_CENTER_DISTANCE+2);
                        // state = default_set_center1_long(fpga_videodata, dir, 60, HEIGHT - 4, BLACK);
                        if (state == 1) {
                            ++step;
                        } else if (state == -1) {
                            if (++flag == 5) {
                                flag = 0;
                            }
                        } else {
                            flag = 99;
                        }
                        break;
                    case 3:
                        // 노란색 앞에서 중심 맞추고 붙이기 한번 더 해아함
                        if (flag == 99) {
                            ACTION_ATTACH(1);
                        } else {
                            ACTION_ATTACH_SHORT(1);
                        }
                        step = 6;
                        break;
                    case 6:
                        step += mission_7_6_jump_hole();
                        flag = 0;
                        break;
                    case 7:
#if MODE == 3
                        if (flag == 0) {
                            dir = RIGHT;
                            default_watch(RIGHT, fpga_videodata);
                            flag++;
                        } else if (flag == 3) {
                            dir = LEFT;
                            default_watch(LEFT, fpga_videodata);
                            flag++;
                        }

                        setFPGAVideoData(fpga_videodata);

                    state = default_set_straight_long_and_center1_long(fpga_videodata, dir, WIDTH_CENTER, HEIGHT - 4,
                                                                       BLACK, DEFAULT_CENTER_DISTANCE);
                        // state = default_set_center1_long(fpga_videodata, dir, WIDTH_CENTER, HEIGHT - 4, BLACK, DEFAULT_CENTER_DISTANCE);
                        state = default_set_straight1(fpga_videodata, dir, WIDTH_CENTER,
                                                      HEIGHT - 4, BLACK, DEFAULT_CENTER_DISTANCE);
                        if (state == 1) {
                            ++step;
                        } else if (state == -1) {
                            if (++flag == 5) {
                                flag = 0;
                            }
                        }
#else
                        ++step;
#endif
                        break;
                    case 8:
                        mission += 1;
                        step = 0;
                        flag = 0;
                        break;
                    default:
                        break;
                }
                break;
            case 8: // MISSION 8: CREVASSE
                switch (step) {
#if MODE == 3
                    case 0:
                        if (flag == 0) {
                            default_watch(OBLIQUE, fpga_videodata);
//                            RobotSleep(1);
                            ++flag;
                        }
                        mission_8_1_watch_below(fpga_videodata, 20);
                        ++step;
                        // setFPGAVideoData(fpga_videodata);
                        // step += mission_8_1_wait_front_of_crevasse(fpga_videodata);
                        break;
                    case 1:
                        if (flag == 0) {
                            dir = LEFT;
                            default_watch(LEFT, fpga_videodata);
                            flag++;
                        } else if (flag == 3) {
                            dir = RIGHT;
                            default_watch(RIGHT, fpga_videodata);
                            flag++;
                        }

                        setFPGAVideoData(fpga_videodata);
//                        state = default_set_straight_and_center1_long(fpga_videodata, dir, 120, HEIGHT - 4, BLACK, DEFAULT_CENTER_DISTANCE);
                        state = default_set_center1_long(fpga_videodata, dir, WIDTH_CENTER + (dir == LEFT ? 30 : -30), HEIGHT - 4, BLACK, DEFAULT_CENTER_DISTANCE);

                        if (state == 1) {
                            ++step;
                        } else if (state == -1) {
                            if (++flag == 5) {
                                flag = 0;
                            }
                        }
                        flag = step == 2 ? 0 : flag;
                        break;
#else
                    case 0: case 1:
                        ++step;
                        break;
#endif
                    case 2:
                        if (flag == 0) {
                            default_watch(DOWN, fpga_videodata);
                            ++flag;
                        }
                        setFPGAVideoData(fpga_videodata);
                        step += mission_8_2_attach_black(fpga_videodata) && mission_8_2_attach_black(fpga_videodata);
                        break;
                    case 3:
                        step += mission_8_3_escape_crevasse();
                        flag = 0;
                        break;
                    case 4:
                        if (flag == 0) {
                            dir = LEFT;
                            default_watch(LEFT, fpga_videodata);
                            flag++;
                        } else if (flag == 3) {
                            dir = RIGHT;
                            default_watch(RIGHT, fpga_videodata);
                            flag++;
                        }

                        setFPGAVideoData(fpga_videodata);
#if MODE == 3
                        state = default_set_straight_and_center1_long(fpga_videodata, dir, 120, HEIGHT - 4, BLACK, DEFAULT_CENTER_DISTANCE);
#else
                        state = default_set_straight_long_and_center1_long(fpga_videodata, dir,
                                                                           WIDTH_CENTER + (dir == LEFT ? 30 : -30),
                                                                           HEIGHT - 4,
                                                                           BLACK, DEFAULT_CENTER_DISTANCE);
#endif
                        // state = default_set_center1_long(fpga_videodata, dir, WIDTH_CENTER + (dir == LEFT ? 30 : -30), HEIGHT - 4, BLACK, DEFAULT_CENTER_DISTANCE);

                        if (state == 1) {
                            CHECK_INIT(OBLIQUE);
                            RobotSleep(1);
                            ++step;
                        } else if (state == -1) {
                            if (++flag == 5) {
                                flag = 0;
                            }
                        }
                        break;
                    default:
                        flag = 0;
                        step = 0;
                        ++mission;
                        break;
                }
                break;
            case 9: // MISSION 9: LAST BARRICADE
                switch (step) {
                    case 0:
                        step += 1;
                        ACTION_WALK(SLOW, OBLIQUE, 4);
                        break;
                    case 1:
//                        CHECK_INIT(UP);
                        ACTION_MOTION(MISSION_9_FACE_FINAL, UP);
//                        RobotSleep(1);
                        flag = 0;
                        step += 1;
                        break;
                    case 2:
                        // TODO: 비율 바꿔야함
                        step += mission_1_1_wait_yellow_barricade(fpga_videodata);
                        break;
                    case 3:
                        step += mission_1_2_end_yellow_barricade(fpga_videodata);
                        break;
                    case 4:
                        mission_9_3_escape_yellow_barricade(30);
                        ++step;
                        break;
                    default:
                        missionFinished = 1;
                        break;
                }
                break;
            case 10: // MISSION 10: BLUE GATE
                switch (step) {
                    case 0:
//                        RobotSleep(2);
                        ++step;
                        flag = 0;
                        break;
                    case 1:
                        if (flag == 0) {
                            dir = RIGHT;
                            default_watch(RIGHT, fpga_videodata);
                            flag++;
                        } else if (flag == 3) {
                            dir = LEFT;
                            default_watch(LEFT, fpga_videodata);
                            flag++;
                        }

                        setFPGAVideoData(fpga_videodata);
#if MODE == 3
                        state = default_set_straight_and_center1(fpga_videodata, dir, WIDTH_CENTER + (dir == LEFT ? 30 : -30), HEIGHT - 4, BLACK, DEFAULT_CENTER_DISTANCE);
#else
                        state = default_set_straight_and_center1_long(fpga_videodata, dir,
                                                                      WIDTH_CENTER + (dir == LEFT ? 30 : -30),
                                                                      HEIGHT - 4, BLACK, DEFAULT_CENTER_DISTANCE);
#endif

                        if (state == 1) {
                            flag = 0;
                            ++step;

                            CHECK_INIT(OBLIQUE);
                            RobotSleep(2);
                            ACTION_WALK(FAST, OBLIQUE, 6);
                            RobotSleep(2);
                        } else if (state == -1) {
                            if (++flag == 5) {
                                flag = 0;
                            }
                        }
                        break;
                    case 2:
#if MODE == 3
                        if (flag == 0) {
                            dir = RIGHT;
                            default_watch(RIGHT, fpga_videodata);
                            flag++;
                        } else if (flag == 3) {
                            dir = LEFT;
                            default_watch(LEFT, fpga_videodata);
                            flag++;
                        }

                        setFPGAVideoData(fpga_videodata);
#if MODE == 3
                        state = default_set_straight_and_center1(fpga_videodata, dir, WIDTH_CENTER + (dir == LEFT ? 30 : -30), HEIGHT - 4, BLACK, DEFAULT_CENTER_DISTANCE);
#else
                        state = default_set_straight_and_center1_long(fpga_videodata, dir, WIDTH_CENTER + (dir == LEFT ? 30 : -30), HEIGHT - 4, BLACK, DEFAULT_CENTER_DISTANCE);
#endif
                        if (state == 1) {
                            flag = 0;
                            ++step;
                            default_watch(OBLIQUE, fpga_videodata);
                        } else if (state == -1) {
                            if (++flag == 5) {
                                flag = 0;
                            }
                        }
#else
                        ++step;
#endif
                        break;
                    case 3:
                        // 일반 걸음으로 걸은 후에, 영상처리 걸음 시작할때 안정화를 위해 슬립
                        step = 0;
                        RobotSleep(1);
                        mission = nextMission;
                        nextMission = 0;
                        flag = 0;
                        break;
                    default:
                        break;
                }
                break;
            default:
                missionFinished = 1;
                break;
        }
#ifdef DEBUG
        printf("[END]\tMISSION[%d]: STEP [%d]\n", mission, step);
#endif

    }

    destroy_huro(fpga_videodata);
    return 0;
}

int init_huro(void) {

    // init uart
    int ret;

    tcgetattr(0, &inittio);
    newtio = inittio;
    newtio.c_lflag &= ~ICANON;
    newtio.c_lflag &= ~ECHO;
    newtio.c_lflag &= ~ISIG;
    newtio.c_cc[VMIN] = 1;
    newtio.c_cc[VTIME] = 0;

    cfsetispeed(&newtio, B115200);

    tcsetattr(0, TCSANOW, &newtio);

    ret = uart_open();

    if (ret < 0) {
        return EXIT_FAILURE;
    }

    uart_config(UART1, 57600, 8, UART_PARNONE, 1);
    //

    // graphic port check
    if (open_graphic() < 0) {
        return -1;
    }

    direct_camera_display_off();

//    RobotSleep(7);

    CHECK_INIT(UP);

    return 0;
}

void destroy_huro(U16 *buf) {
    // free fpga video data
    free(buf);

    // graphic close
    close_graphic();

    // UART
    uart_close();

}
