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

    int nextMission = 0;
    int flag = 0;

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
                        step = 0;
                        mission += 1;
                        flag = 2;
                        break;
                    default:
                        break;
                }
                break;
            case 2: // MISSION 2: RED BRIDGE
                switch (step) {
                    case 0:
                        if (flag == 3) {
                            mission_2_1_watch_below(6);
                        }

                        setFPGAVideoData(fpga_videodata);
                        step += mission_2_1_wait_front_of_red_bridge(fpga_videodata);
                        flag = (step == 1) ? 0 : 3;
                        break;
                    case 1:
                        // 1. 왼쪽보고 중심 맞추기
                        if (flag == 0) {
                            mission_5_2_watch_side();
                            flag++;
                        }
                        setFPGAVideoData(fpga_videodata);
                        // TODO: 상수 설정
                        step += mission_2_2_before_bridge_set_center(fpga_videodata, 0, 0);
                        break;
                    case 2:
                        step += mission_2_3_escape_red_bridge();
                        break;
                    case 3:
                        if (flag == 0) {
                            flag++;
                            mission_2_2_watch_side();
                        }
                        setFPGAVideoData(fpga_videodata);
                        step += mission_2_4_after_bridge_set_straight(fpga_videodata, 0); // 직선 맞추기

                        if (step == 4) {
                            step += mission_2_5_after_bridge_set_center(fpga_videodata);// 길이 맞추기
                        }

                        step = (step == 5) ? 4 : 3;
                        break;
                    case 4:
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
                    case 4:
                        mission_3_4_watch_front();
                        setFPGAVideoData(fpga_videodata);
                        step = (mission_3_4_is_not_front_of_bomb(fpga_videodata)) ? 5 : 2;
                        flag = 0;
                        break;
                    case 5:
                        if (flag == 0) {
                            flag++;
                            mission_3_default_watch_below();
                        }

                        setFPGAVideoData(fpga_videodata);
                        step += mission_3_default_avoid_bomb(fpga_videodata);
                        break;
                    case 6:
                        mission_3_7_attach_hurdle(fpga_videodata);
                        step += 1;
                        flag = 0;
                        break;
                    case 7:
                        if (flag == 0) {
                            mission_5_2_watch_side();
                            flag++;
                        }
                        setFPGAVideoData(fpga_videodata);
                        step += mission_2_2_before_bridge_set_center(fpga_videodata, 0, 0);
                        break;
                    case 8:
                        mission = 4;
                        step = 0;
                        flag = 0;
                        mission_3_init_global();
                    default:
                        if (flag == 0) {
                            flag++;
                            mission_3_default_watch_below();
                        }

                        setFPGAVideoData(fpga_videodata);
                        step += mission_3_default_avoid_bomb(fpga_videodata);
                        break;
                }
                break;
            case 4: // MISSION 4: JUMP HURDLE
                switch (step) {
                    case 3:
                        mission_4_4_jump_hurdle();
                        step = 6;
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
                            ACTION_INIT(MIDDLE, OBLIQUE);
                            mission_4_1_watch_front(2);
                        }

                        ACTION_INIT(MIDDLE, UP);

                        setFPGAVideoData(fpga_videodata);
                        step = (mission_4_2_ready_hurdle(fpga_videodata)) ? 3 : 2;
                        break;
                }
                break;
            case 5: // MISSION 5: GREEN BRIDGE
                switch (step) {
                    case 0:
                        if (flag != 0) {
                            mission_5_1_watch_below(6);
                        }
                        setFPGAVideoData(fpga_videodata);
                        step += mission_5_1_check_black_line(fpga_videodata, 5);
                        flag = (step == 1) ? 0 : 1;
                        break;
                    case 1:
                        // 맨 처음 다리 중심 맞추기
                        if (flag == 0) {
                            mission_5_2_watch_side();
                            flag++;
                        }
                        setFPGAVideoData(fpga_videodata);
                        // TODO: 상수 맞추기
                        step += mission_2_2_before_bridge_set_center(fpga_videodata, -1, 0);
                        break;
                    case 2:
                        // 계단 오르기
                        step += mission_5_3_climb_up_stairs();
                        flag = 0;
                        break;
                    case 3:
                        ACTION_WALK(FAST, DOWN, 8);
                        RobotSleep(2);
                        step += 1;
                        flag = 0;
                        break;
                    case 4:
                        setFPGAVideoData(fpga_videodata);

                        if (mission_5_5_check_finish_black_line(fpga_videodata)) {
                            step = 5;
                            int repeat = mission_5_5_get_repeat(fpga_videodata);
                            ACTION_WALK((repeat == 4) ? SLOW : CLOSE, DOWN, repeat);
                            break;
                        }

                        step += mission_5_5_check_green_bridge_straight(fpga_videodata);

                        if (step == 5) {
                            step += mission_5_5_check_green_bridge_center(fpga_videodata);
                        }

                        if (step == 6) {
                            mission_5_5_short_walk_on_green_bridge((flag++ >= 2) ? 4 : 6);
                        }

                        step = 4;
                        break;
                    case 5:
                        setFPGAVideoData(fpga_videodata);
                        nextMission = mission_5_6_set_straight(fpga_videodata);
                        step += nextMission;

                        if (nextMission) {
                            step += mission_5_6_set_only_one_bk_bar(fpga_videodata);
                            if (step == 6) {
                                ACTION_BIT(FRONT, 1);
                                RobotSleep(1);
                            }
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

                        step += mission_2_4_after_bridge_set_straight(fpga_videodata, 1);
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

                        if (flag == 0) {
                            mission_6_2_watch_below();
                            ++flag;
                        }
                        setFPGAVideoData(fpga_videodata);
                        step += mission_6_2_set_center_of_ball(fpga_videodata);

                        if (step == 2) {
                            flag = 0;
                        }
                        break;
                    case 2:
                        // if(flag == 0) {
                        //     mission_6_2_watch_right();
                        //     ++flag;
                        // }
                        // setFPGAVideoData(fpga_videodata);
                        // step += mission_6_2_set_straight_black_line(fpga_videodata, 1);
                        mission_6_1_watch_front();
                        setFPGAVideoData(fpga_videodata);
                        if (mission_6_3_find_hole(fpga_videodata) == 0) {
                            mission_6_2_set_straight_black_line(fpga_videodata, 1);
                        } else {
                            ++step;
                        }
                        break;
                    case 3:
                        mission_6_1_watch_front();
                        if (mission_6_3_find_hole(fpga_videodata) == 0) {
                            mission_6_4_turn_to_detect_hole();
                        } else {
                            step++;
                        }
                        break;
                    case 4:
                        flag = 0;
                        mission_6_1_watch_front();
                        setFPGAVideoData(fpga_videodata);
                        step += mission_6_3_set_straight_hole(fpga_videodata);
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
                                //step += mission_6_2_set_center_of_ball(fpga_videodata);
                                step += mission_6_4_set_front_of_ball(fpga_videodata);

                                printf("step:%d\n", step);
                                if (step == 7) {
                                    break;
                                }
                            }
                        }

                        if (step == 7) {
                            //mission_6_2_watch_below();
                            setFPGAVideoData(fpga_videodata);
                            step += mission_6_4_set_front_of_ball(fpga_videodata);

                            mission_6_1_watch_front();
                            setFPGAVideoData(fpga_videodata);
                            step += mission_6_3_locate_hole_on_center(fpga_videodata);
                        }

                        step = (step == 9) ? 6 : 5;

                        break;
                    case 6:
                        // if(flag == 0) {
                        //     flag++;
                        //     mission_6_2_watch_below();
                        // }
                        // setFPGAVideoData(fpga_videodata);
                        // step += mission_6_4_set_front_of_ball(fpga_videodata);
                        step++;
                        break;
                    case 7:
                        flag = 0;
                        step += mission_6_5_kick_ball();
                        break;
                    case 8:
                        setFPGAVideoData(fpga_videodata);
                        step += mission_6_9_set_front_of_not_bk(fpga_videodata);
                        break;
                    case 9:
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
                        if (flag != 0) {
                            mission_7_1_watch_below(6);
                        }

                        setFPGAVideoData(fpga_videodata);
                        step += mission_7_1_wait_front_of_yellow_hole_bridge(fpga_videodata, 2);
                        flag = (step == 1) ? 0 : 1;
                        break;
                    case 1:
                        if (flag == 0) {
                            flag++;
                            mission_7_2_watch_below();
                        }
                        setFPGAVideoData(fpga_videodata);
                        // TODO: 상수 맞추기
                        step += mission_2_2_before_bridge_set_center(fpga_videodata, 3, 0);
                        break;
                    case 2:
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
                        step += mission_2_4_after_bridge_set_straight(fpga_videodata, 0); // 직선 맞추기

                        if (step == 8) {
                            step += mission_2_5_after_bridge_set_center(fpga_videodata);// 길이 맞추기
                        }

                        step = (step == 9) ? 8 : 7;
                        break;
                    case 8:
                        mission += 1;
                        step = 0;
                        break;
                    default:
                        break;
                }
                break;
            case 8: // MISSION 8: AVOID BOMB
                switch (step) {
                    case 4:
                        mission_3_4_watch_front();
                        setFPGAVideoData(fpga_videodata);
                        step = (mission_3_4_is_not_front_of_bomb(fpga_videodata)) ? 5 : 2;
                        flag = 0;
                        break;
                    case 5:
                        if (flag == 0) {
                            flag++;
                            mission_3_default_watch_below();
                        }

                        setFPGAVideoData(fpga_videodata);
                        step += mission_3_default_avoid_bomb(fpga_videodata);
                        break;
                    case 6:
                        mission_3_7_attach_hurdle(fpga_videodata);
                        step += 1;
                        flag = 0;
                        break;
                    case 7:
                        if (flag == 0) {
                            mission_5_2_watch_side();
                            flag++;
                        }
                        setFPGAVideoData(fpga_videodata);
                        step += mission_2_2_before_bridge_set_center(fpga_videodata, 0, 0);
                        break;
                    case 8:
                        mission = 4;
                        step = 0;
                        flag = 0;
                        mission_3_init_global();
                    default:
                        if (flag == 0) {
                            flag++;
                            mission_3_default_watch_below();
                        }

                        setFPGAVideoData(fpga_videodata);
                        step += mission_3_default_avoid_bomb(fpga_videodata);
                        break;
                }
                break;
            case 9: // MISSION 9: LAST BARRICADE
                switch (step) {
                    case 0:
                        ACTION_INIT(LOW, OBLIQUE);
                        step += mission_9_1_wait_yellow_barricade(fpga_videodata);
                        break;
                    case 1:
                        ACTION_INIT(LOW, OBLIQUE);
                        step += mission_9_2_end_yellow_barricade(fpga_videodata);
                        break;
                    case 2:
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

                        // TODO: 상수 맞추기
                        if (mission_2_4_after_bridge_set_straight(fpga_videodata, 1) &&
                            mission_4_6_set_center(fpga_videodata, ((nextMission == 7) ? 70 : 60))) {
                            // 앞에꺼는 다음이 옐로우 뒤에는 다음이 초록다리

                            flag = 0;
                            step += 1;

                            ACTION_INIT(MIDDLE, DOWN);
                            ACTION_WALK(FAST, DOWN, 7);
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

                        // TODO: 상수 맞추기
                        if (mission_2_4_after_bridge_set_straight(fpga_videodata, 1) &&
                            mission_4_6_set_center(fpga_videodata, ((nextMission == 7) ? 70 : 60))) {
                            // 앞에꺼는 다음이 옐로우 뒤에는 다음이 초록다리

                            flag = 0;
                            step += 1;

                            ACTION_INIT(MIDDLE, DOWN);
                            ACTION_WALK(FAST, DOWN, 7);
                            RobotSleep(1);
                            break;
                        }
                        break;
                    case 2:
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

    ACTION_INIT(MIDDLE, OBLIQUE);

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

void setFPGAVideoData(U16 *buf) {
    read_fpga_video_data(buf);
    draw_fpga_video_data_full(buf);
    flip();
}
