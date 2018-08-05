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

    int mission = 5;
    int step = 0;

    int nextMission = 0;
    int flag = 0;
    int ck = 0;
    int tmp = 0;

    // TODO: 상수 설정하기
    int userStaticValue[5] = {
            56, // MISSION 2: RED BRIDGE
            58, // MISSION 5: GREEN BRIDGE
            57, // MISSION 7: YELLOW HOLE
            65, // MISSION 10: BLUE GATE 1 -> GREEN BRIDGE
            65  // MISSION 10: BLUE GATE 1 -> YELLOW HOLE
    };

    while (!missionFinished) {

        setFPGAVideoData(fpga_videodata);

        printf("\n[START]\tMISSION[%d]: STEP [%d]\n", mission, step);

        switch (mission) {
            case 0: // MISSION 0: READY PLAY
                mission += mission_0_1_wait_play(fpga_videodata);
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
                        mission_1_3_escape_yellow_barricade(10);
                        step += 1;
                        flag = 0;
                        break;
                    case 3:
                        if (flag == 0) {
                            flag++;
                            mission_2_2_watch_side();
                        }
                        setFPGAVideoData(fpga_videodata);
                        step += mission_2_4_after_bridge_set_straight(fpga_videodata, 0, 0); // 직선 맞추기

                        if (step == 4) {
                            step += mission_2_5_after_bridge_set_center(fpga_videodata);// 길이 맞추기
                        }

                        step = (step == 5) ? 4 : 3;
                        break;
                    case 4:
                        CHECK_INIT(MIDDLE, DOWN);
                        RobotSleep(1);

//                        ACTION_MOTION(WALK_FAST_SET_7, MIDDLE, DOWN);
                        RobotSleep(1);
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
                        if (flag == 3) {
                            setFPGAVideoData(fpga_videodata);
                            mission_2_1_watch_below(30, fpga_videodata);
                            ACTION_WALK(CLOSE, DOWN, 4);
                        }

                        setFPGAVideoData(fpga_videodata);
                        step += mission_2_1_wait_front_of_red_bridge(fpga_videodata);
                        flag = (step == 1) ? 0 : 3;
                        break;
                    case 1:
                        flag = 0;
                        setFPGAVideoData(fpga_videodata);
                        step += mission_2_1_attach_red_bridge(fpga_videodata);
                        break;
                    case 2:
                        // 1. 왼쪽보고 중심 맞추기
                        if (flag == 0) {
                            mission_5_2_watch_side();
                            flag++;
                        }
                        setFPGAVideoData(fpga_videodata);

                        step += mission_2_2_before_bridge_set_center(fpga_videodata, 5, userStaticValue[0]);
                        break;
                    case 3:
                        step += mission_2_3_escape_red_bridge();
                        break;
                    case 4:
                        if (flag == 0) {
                            flag++;
                            mission_2_2_watch_side();
                        }
                        setFPGAVideoData(fpga_videodata);
                        step += mission_2_4_after_bridge_set_straight(fpga_videodata, 0, 0); // 직선 맞추기

                        if (step == 5) {
                            step += mission_2_5_after_bridge_set_center(fpga_videodata);// 길이 맞추기
                        }

                        step = (step == 6) ? 5 : 4;
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
                switch (step) {
                    case 0:
                        setFPGAVideoData(fpga_videodata);
                        mission_3_default_watch_below(10, fpga_videodata);

                        setFPGAVideoData(fpga_videodata);
                        step = (mission_3_default_avoid_bomb(fpga_videodata)) ? 1 : 2;

                        if (mission_3_isFrontOf_Blue(fpga_videodata)) {
                            step = 4;
                        }

                        flag = 0;
                        break;
                    case 1:
                        if (flag == 0) {
                            flag = 1;
                            mission_3_4_watch_side();
                            setFPGAVideoData(fpga_videodata);
                            mission_3_measure_line(fpga_videodata);
                            break;
                        } else if (flag == 1) {
                            flag = 2;
                            CHECK_INIT(MIDDLE, DOWN);
                            RobotSleep(1);
                        }

                        setFPGAVideoData(fpga_videodata);

                        if (mission_3_avoid(fpga_videodata)) { // 만약 지뢰를 다 피했으면
                            // ACTION_WALK(FAST, DOWN, 4);
                            RobotSleep(1);
                            step = 2;
                            flag = 0;
                        }
                        break;
                    case 2:
                        if (flag == 0) {
                            mission_3_4_watch_side();
                        }

                        setFPGAVideoData(fpga_videodata);
                        step = (mission_2_4_after_bridge_set_straight(fpga_videodata, mission_3_4_getMDir(), 1)) ? 3
                                                                                                                 : 2;
                        flag = (step == 3) ? 0 : 1;
                        break;
                    case 3:
                        mission_3_4_watch_front();
                        setFPGAVideoData(fpga_videodata);
                        step = (mission_3_1_ver2(fpga_videodata) && mission_4_2_ready_hurdle(fpga_videodata)) ? 4 : 0;
                        flag = 0;
                        break;
                    case 4:
                        ACTION_WALK(CLOSE, OBLIQUE, 10);
                        step += 1;
                        flag = 0;
                        break;
                    case 5:
                        if (flag == 0) {
                            mission_5_2_watch_side();
                            flag++;
                        }
                        setFPGAVideoData(fpga_videodata);
                        step += mission_2_2_before_bridge_set_center(fpga_videodata, 0, 50);
                        break;
                    case 6:
                        mission += 1;
                        step = 0;
                        flag = 0;
                        mission_3_init_global();
                    default:
                        break;
                }
                break;
            case 4: // MISSION 4: JUMP HURDLE
                switch (step) {
                    case 3:
                        mission_4_4_jump_hurdle();
                        step = 5;
                        break;
                    case 5:
                        setFPGAVideoData(fpga_videodata);
                        step += mission_6_9_set_front_of_not_bk(fpga_videodata);
                        break;
                    case 6:
                        mission = 10;
                        step = 0;
                        flag = 0;
                        nextMission = 5;
                        break;
                    default:
                        flag = 0;

                        if (step == 2) {
                            CHECK_INIT(MIDDLE, OBLIQUE);
                            // mission_4_1_watch_front(2);
                            step = 3;
                            break;
                        }

                        CHECK_INIT(MIDDLE, UP);
                        RobotSleep(1);

                        setFPGAVideoData(fpga_videodata);
                        step = (mission_4_2_ready_hurdle(fpga_videodata)) ? 3 : 2;
                        break;
                }
                break;
            case 5: // MISSION 5: GREEN BRIDGE
                switch (step) {
                    case 0:
                        if (flag == 0) {
                            setFPGAVideoData(fpga_videodata);
                            mission_5_1_watch_below(40, fpga_videodata);
                            ACTION_WALK(CLOSE, DOWN, 4);
                        }

                        step = 11;
                        flag = 0;
                        break;
                    case 1:
                        // 맨 처음 다리 중심 맞추기
                        if (flag == 0) {
                            mission_5_2_watch_side();
                            flag++;
                        }
                        setFPGAVideoData(fpga_videodata);
                        step += mission_2_2_before_bridge_set_center(fpga_videodata, 5, userStaticValue[1]);
                        break;
                    case 2:
                        // 계단 오르기
                        mission_5_3_climb_up_stairs();
                        flag = 0;
                        step = 12;
                        break;
                    case 3:
                        ACTION_MOTION(MISSION_5_ESCAPE_BLACK_STAIR, MIDDLE, DOWN);
                        RobotSleep(1);
                        step += 1;

                        flag = 0;
                        break;
                    case 4:
                        setFPGAVideoData(fpga_videodata);

                        if (mission_5_5_check_finish_black_line(fpga_videodata)) {
                            step = 14;
                            mission_5_5_set_center(fpga_videodata);
                            // setFPGAVideoData(fpga_videodata);
                            // int repeat = mission_5_5_get_repeat(fpga_videodata);
                            // ACTION_WALK((repeat == 4) ? FAST : CLOSE, DOWN, (repeat == 4) ? 5 : 8);
                            break;
                        }

                        step += mission_5_5_check_green_bridge_straight(fpga_videodata);

                        if (step == 5) {
                            step += mission_5_5_check_green_bridge_center(fpga_videodata);
                        }

                        if (step == 6) {
                            mission_5_5_short_walk_on_green_bridge((flag++ >= 2) ? 6 : 8);
                        }

                        step = 4;
                        break;
                    case 5:
                        setFPGAVideoData(fpga_videodata);
                        nextMission = mission_5_6_set_straight(fpga_videodata);
                        step += nextMission;

                        if (nextMission) {
                            step += mission_5_6_set_only_one_bk_bar(fpga_videodata);
                        }

                        nextMission = 0;
                        step = (step == 7) ? 6 : 5;
                        break;
                    case 6:
                        mission_5_7_climb_down_stairs();
                        flag = 0;
                        step = 9;
                        break;
                    case 9:
                        if (flag == 0) {
                            mission_4_6_watch_side();
                            flag++;
                        }
                        setFPGAVideoData(fpga_videodata);

                        step += mission_2_4_after_bridge_set_straight(fpga_videodata, 1, 0);
                        if (step == 10) {
                            step += mission_4_6_set_center(fpga_videodata, 0);
                        }

                        step = (step == 11) ? 10 : 9;
                        break;
                    case 10:
                        mission += 1;
                        step = 0;
                        flag = 0;
                        break;
                    case 11:
                        flag = 0;
                        setFPGAVideoData(fpga_videodata);
                        step = (mission_5_11_attach(fpga_videodata)) ? 1 : 11;
                        break;
                    case 12:
                        setFPGAVideoData(fpga_videodata);
                        step = (mission_5_12_set_straight(fpga_videodata) &&
                                mission_5_5_check_green_bridge_center(fpga_videodata)) ? 3 : 12;

                        if (step == 3) {
                            ACTION_MOVE(SHORT, DIR_RIGHT, MIDDLE, DOWN, 2);
                            RobotSleep(1);
                        }
                        break;
                    case 13:
                        setFPGAVideoData(fpga_videodata);
                        step = (mission_13_attach_black(fpga_videodata)) ? 5 : 13;
                        break;
                    case 14:
                        setFPGAVideoData(fpga_videodata);
                        step = (mission_14_set_straight(fpga_videodata)) ? 13 : 14;
                        step = 14;
                        break;
                    default:
                        break;
                }
                break;
            case 6: // MISSION 6: KICK BALL
                switch (step) {
                    case 0:
                        flag = 0;
                        mission_6_1_watch_front();
                        setFPGAVideoData(fpga_videodata);
                        step += mission_6_1_detection_ball(fpga_videodata);
                        break;
                    case 1:
                        mission_6_2_watch_below();

                        setFPGAVideoData(fpga_videodata);
                        step += mission_6_2_set_center_of_ball(fpga_videodata);

                        if (step == 2) {
                            flag = 0;
                        }
                        break;
                    case 2:
                        mission_6_1_watch_front();

                        setFPGAVideoData(fpga_videodata);
                        if (mission_6_3_find_hole_interpolation(fpga_videodata, 0, HEIGHT, 0, WIDTH, 0) == 0) {
                            mission_6_3_find_side_hole(fpga_videodata, 0);
                        } else {
                            ++step;
                        }
                        break;
                    case 3:
                        ++step;
                        break;
                    case 4:
                        mission_6_1_watch_front();
                        setFPGAVideoData(fpga_videodata);
                        if (mission_6_3_set_straight_hole(fpga_videodata)) {
                            ++step;
                        }
                        break;
                    case 5:
                        if (!flag) {
                            flag = 1;
                            mission_6_1_watch_front();
                        }

                        setFPGAVideoData(fpga_videodata);
                        step += mission_6_3_locate_hole_on_center(fpga_videodata);

                        if (step == 6) {
                            flag = 0;
                            mission_6_2_watch_below();
                            while (TRUE) {
                                setFPGAVideoData(fpga_videodata);
                                step += mission_6_4_set_front_of_ball(fpga_videodata);

                                printf("step:%d\n", step);
                                if (step == 7) {
                                    break;
                                }
                            }
                        }

                        if (step == 7) {
                            mission_6_1_watch_front();
                            setFPGAVideoData(fpga_videodata);
                            step += mission_6_3_locate_hole_on_center(fpga_videodata);
                        }

                        if (step == 8) {
                            mission_6_2_watch_below();
                            setFPGAVideoData(fpga_videodata);
                            step += mission_6_4_set_front_of_ball(fpga_videodata);
                        }

                        step = (step == 9) ? 6 : 5;

                        break;
                    case 6:
                        flag = 0;
                        step += mission_6_5_kick_ball();
                        break;
                    case 7:
                        setFPGAVideoData(fpga_videodata);
                        step += mission_6_9_set_front_of_not_bk(fpga_videodata);
                        break;
                    case 8:
                        mission_6_9_walk_front();
                        ++step;
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
                            setFPGAVideoData(fpga_videodata);
                            mission_7_1_watch_below(20, fpga_videodata);
                            ACTION_WALK(CLOSE, DOWN, 4);
                        }

                        setFPGAVideoData(fpga_videodata);
                        step += mission_7_1_wait_front_of_yellow_hole_bridge(fpga_videodata);
                        flag = (step == 1) ? 0 : 1;
                        break;
                    case 1:
                        flag = 0;
                        setFPGAVideoData(fpga_videodata);
                        step += mission_7_1_attach_yellow_bridge(fpga_videodata);
                        break;
                    case 2:
                        if (flag == 0) {
                            flag++;
                            mission_2_2_watch_side();
                        }
                        setFPGAVideoData(fpga_videodata);
                        step += mission_2_2_before_bridge_set_center(fpga_videodata, 5, userStaticValue[2]);
                        break;
                    case 3:
                        step = 6;
                        break;
                    case 6:
                        step += mission_7_6_jump_hole();
                        flag = 0;
                        break;
                    case 7:
                        if (flag == 0) {
                            flag++;
                            mission_2_2_watch_side();
                        }
                        setFPGAVideoData(fpga_videodata);
                        step += mission_2_4_after_bridge_set_straight(fpga_videodata, 0, 0); // 직선 맞추기

                        if (step == 8) {
                            step += mission_2_5_after_bridge_set_center(fpga_videodata);// 길이 맞추기
                        }

                        step = (step == 9) ? 8 : 7;
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
            case 8: // MISSION 8: AVOID BOMB
                switch (step) {
                    case 0:
                        setFPGAVideoData(fpga_videodata);
                        mission_3_default_watch_below(10, fpga_videodata);

                        setFPGAVideoData(fpga_videodata);
                        step = (mission_3_default_avoid_bomb(fpga_videodata)) ? 1 : 2;

                        flag = 0;
                        break;
                    case 1:
                        if (flag == 0) {
                            flag = 1;
                            mission_3_4_watch_side();
                            setFPGAVideoData(fpga_videodata);
                            mission_3_measure_line(fpga_videodata);
                            break;
                        } else if (flag == 1) {
                            flag = 2;
                            CHECK_INIT(MIDDLE, DOWN);
                            RobotSleep(1);
                        }

                        setFPGAVideoData(fpga_videodata);

                        if (mission_3_avoid(fpga_videodata)) { // 만약 지뢰를 다 피했으면
                            step = 2;
                            flag = 0;
                        }
                        break;
                    case 2:
                        if (flag == 0) {
                            mission_3_4_watch_side();
                        }

                        setFPGAVideoData(fpga_videodata);
                        step = (mission_2_4_after_bridge_set_straight(fpga_videodata, mission_3_4_getMDir(), 1)) ? 3
                                                                                                                 : 2;

                        flag = (step == 3) ? 0 : 1;
                        break;
                    case 3:
                        CHECK_INIT(MIDDLE, UP);
                        RobotSleep(2);
                        setFPGAVideoData(fpga_videodata);
                        flag += mission_8_1_is_not_front_of_bomb(fpga_videodata);

                        if (flag != 0) {
                            step = 4;
                            flag = 0;
                            break;
                        }

                        ACTION_MOTION(HEAD_MIDDLE_HALF_LEFT, MIDDLE, LEFT);
                        RobotSleep(2);
                        setFPGAVideoData(fpga_videodata);
                        flag += mission_8_4_check_finish_line(fpga_videodata);

                        if (flag != 0) {
                            step = 4;
                        } else {
                            step = 0;
                        }

                        flag = 0;
                        break;
                    case 4:
                        step += 1;
                        flag = 0;
                        break;
                    case 5:
                        if (flag == 0) {
                            flag++;
                            mission_2_2_watch_side();
                            RobotSleep(1);
                        }
                        setFPGAVideoData(fpga_videodata);
                        step += mission_2_4_after_bridge_set_straight(fpga_videodata, 0, 0); // 직선 맞추기

                        if (step == 6) {
                            step += mission_2_5_after_bridge_set_center(fpga_videodata);// 길이 맞추기
                        }

                        step = (step == 7) ? 6 : 5;
                        break;
                    case 6:
                        CHECK_INIT(MIDDLE, OBLIQUE);
                        RobotSleep(2);
                        mission += 1;
                        step = 0;
                        flag = 0;
                        mission_3_init_global();
                    default:
                        break;
                }
                break;
            case 9: // MISSION 9: LAST BARRICADE
                switch (step) {
                    case 0:
                        step += 1;
//                        setFPGAVideoData(fpga_videodata);
//                        mission_9_1_go_front(fpga_videodata);
//                        step += 1;
                        break;
                    case 1:
                        CHECK_INIT(MIDDLE, UP);
                        RobotSleep(1);
                        step += 1;
                        break;
                    case 2:
                        setFPGAVideoData(fpga_videodata);
                        step += mission_9_1_wait_yellow_barricade(fpga_videodata);
                        break;
                    case 3:
                        setFPGAVideoData(fpga_videodata);
                        step += mission_9_2_end_yellow_barricade(fpga_videodata);
                        break;
                    case 4:
                        mission_9_3_escape_yellow_barricade(30);
                        break;
                    default:
                        missionFinished = 1;
                        break;
                }
                break;
            case 10: // MISSION 10: BLUE GATE
                switch (step) {
                    case 0:
                        if (flag++ == 0) {
                            mission_4_6_watch_side();
                            flag++;
                        }

                        setFPGAVideoData(fpga_videodata);

                        if (mission_2_4_after_bridge_set_straight(fpga_videodata, 1, 0) &&
                            mission_4_6_set_center(fpga_videodata,
                                                   ((nextMission == 7) ? userStaticValue[3] : userStaticValue[4]))) {
                            // 앞에꺼는 다음이 옐로우 뒤에는 다음이 초록다리

                            flag = 0;
                            step += 1;

                            CHECK_INIT(MIDDLE, DOWN);
                            ACTION_MOTION(WALK_FAST_SET_9, MIDDLE, DOWN);
                            RobotSleep(1);
                            break;
                        }
                        break;
                    case 1:
                        if (flag++ == 0) {
                            mission_4_6_watch_side();
                            flag++;
                        }

                        setFPGAVideoData(fpga_videodata);

                        if (mission_2_4_after_bridge_set_straight(fpga_videodata, 1, 0) &&
                            mission_4_6_set_center(fpga_videodata,
                                                   ((nextMission == 7) ? userStaticValue[3] : userStaticValue[4]))) {
                            // 앞에꺼는 다음이 옐로우 뒤에는 다음이 초록다리

                            flag = 0;
                            step += 1;

                            CHECK_INIT(MIDDLE, DOWN);
                            ACTION_MOTION(WALK_FAST_SET_9, MIDDLE, DOWN);
                            RobotSleep(1);
                            break;
                        }
                        break;
                    case 2:
                        CHECK_INIT(MIDDLE, DOWN);
                        ACTION_MOTION(WALK_FAST_SET_9, MIDDLE, DOWN);
                        step += 1;
                        break;
                    case 3:
                        step = 0;
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
        printf("[END]\tMISSION[%d]: STEP [%d]\n", mission, step);

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

    RobotSleep(7);

    CHECK_INIT(MIDDLE, OBLIQUE);

    return 0;
}

void destroy_huro(U16 *buf) {
    // free fpga video data
    free(buf);

    // graphic close
    close_graphic();

    // UART CLOSE
    uart_close();

}
