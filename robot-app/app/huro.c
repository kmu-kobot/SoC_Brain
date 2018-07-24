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
    int nextMission = 0;
    int flag = 0;

    int step = 0;

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
                        break;
                    default:
                        break;
                }
                break;
            case 2: // MISSION 2: RED BRIDGE
                switch (step) {
                    case 0:
                        mission_2_1_watch_below(10);
                        setFPGAVideoData(fpga_videodata);
                        step += mission_2_1_wait_front_of_red_bridge(fpga_videodata, 5);
                        break;
                    case 1:
                        mission_2_2_watch_front();
                        setFPGAVideoData(fpga_videodata);
                        step += mission_2_2_before_bridge_set_center_version2(fpga_videodata);
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
                        mission += 1;
                        step = 0;
                        break;
                    default:
                        break;
                }
                break;
            case 3: // MISSION 3: AVOID BOMB
                switch (step) {
                    // TODO: 끝나고 앞으로 가서 중심 맞추는거 추가해야함
                    case 4:
                        mission_3_4_watch_front();
                        setFPGAVideoData(fpga_videodata);
                        mission += mission_3_4_is_not_front_of_bomb(fpga_videodata);
                        step = (mission == 4) ? 1 : 0;
                        break;
                    default:
                        mission_3_default_watch_below();
                        setFPGAVideoData(fpga_videodata);
                        step += mission_3_default_avoid_bomb(fpga_videodata);
                        break;
                }
                break;
            case 4: // MISSION 4: JUMP HURDLE
                switch (step) {
                    case 3:
                        step += mission_4_4_jump_hurdle();
                        break;
                    case 4:
                        mission_4_5_watch_diagonal_line();
                        setFPGAVideoData(fpga_videodata);
                        step += mission_4_5_set_front_of_not_bk(fpga_videodata);
                        break;
                    case 5:
                        if (flag == 0) {
                            mission_4_6_watch_side();
                            flag++;
                        }
                        setFPGAVideoData(fpga_videodata);
                        step += mission_2_4_after_bridge_set_straight(fpga_videodata, 1);
                        if (step == 6) {
                            step += mission_4_6_set_center(fpga_videodata, 0);
                        }

                        step = (step == 7) ? 6 : 5;
                        break;
                    case 6:
                        mission = 10;
                        step = 0;
                        flag = 0;
                        nextMission = 5;
                        break;
                    default:
                        mission_4_1_watch_front(4);
                        setFPGAVideoData(fpga_videodata);
                        step += mission_4_2_ready_hurdle(fpga_videodata);
                        break;
                }
                break;
            case 5: // MISSION 5: GREEN BRIDGE
                switch (step) {
                    case 0:
                        flag = 0;
                        mission_5_1_watch_below(7);
                        setFPGAVideoData(fpga_videodata);
                        step += mission_5_1_check_black_line(fpga_videodata, 5);
                        break;
                    case 1:
                        // 맨 처음 다리 중심 맞추기
                        if (flag == 0) {
                            mission_5_2_watch_side();
                            flag++;
                        }
                        setFPGAVideoData(fpga_videodata);
                        step += mission_2_2_before_bridge_set_center(fpga_videodata);
                        break;
                    case 2:
                        // 계단 오르기
                        step += mission_5_3_climb_up_stairs();
                        flag = 0;
                        break;
                    case 3:
                        if (flag == 0) {
                            mission_5_5_watch_below();
                            flag++;
                        }

                        setFPGAVideoData(fpga_videodata);

                        if (mission_5_5_check_green_bridge_center(fpga_videodata)) {
                            step += 1;
                            mission_5_5_short_walk_on_green_bridge(4);
                        }
                        break;
                    case 4:
                        setFPGAVideoData(fpga_videodata);

                        if (mission_5_5_check_finish_black_line(fpga_videodata)) {
                            step = 5;
                            mission_5_5_short_walk_on_green_bridge(3);
                            break;
                        }

                        step += mission_5_5_check_green_bridge_straight(fpga_videodata);

                        if (step == 5) {
                            step += mission_5_5_check_green_bridge_center(fpga_videodata);
                        }

                        if (step == 6) {
                            mission_5_5_short_walk_on_green_bridge(5);
                        }

                        step = 4;
                        break;
                    case 5:
                        setFPGAVideoData(fpga_videodata);
                        nextMission = mission_5_6_set_straight(fpga_videodata);
                        step += nextMission;

                        if (nextMission) {
                            step += mission_5_6_set_only_one_bk_bar(fpga_videodata);
                            if (step == 6) { ACTION_WALK(SLOW, DOWN, 1); }
                        }

                        nextMission = 0;
                        step = (step == 7) ? 6 : 5;
                        break;
                    case 6:
                        step += mission_5_7_climb_down_stairs();
                        break;
                    case 7:
                        mission_5_2_watch_side();
                        setFPGAVideoData(fpga_videodata);
                        step += mission_2_5_after_bridge_set_center(fpga_videodata);
                        break;
                    case 8:
                        mission += 1;
                        step = 0;
                        break;
                    default:
                        break;
                }
                break;
            case 6: // MISSION 6: KICK BALL
                switch (step) {
                    case 0:
                        mission_6_1_watch_front();
                        setFPGAVideoData(fpga_videodata);
                        step += mission_6_1_detection_ball(fpga_videodata);
                        break;
                    case 1:
                        mission_6_2_watch_below();
                        setFPGAVideoData(fpga_videodata);
                        step += mission_6_2_set_center_of_ball(fpga_videodata);
                        break;
                    case 2:
                        mission_6_2_watch_below();
                        setFPGAVideoData(fpga_videodata);
                        if (mission_6_2_set_center_of_ball(fpga_videodata)) {
                            mission_6_1_watch_front();
                            setFPGAVideoData(fpga_videodata);
                            step += mission_6_3_find_hole(fpga_videodata);
                        }
                        break;
                    case 3:
                        mission_6_2_watch_below();
                        setFPGAVideoData(fpga_videodata);
                        step += mission_6_4_set_front_of_ball(fpga_videodata);
                        break;
                    case 4:
                        step += mission_6_5_kick_ball();
                        break;
                    case 5:
                        // TODO: 노란색 보는 방향으로 센터랑 직각 맞추기
                        mission_6_6_watch_side();
                        setFPGAVideoData(fpga_videodata);
                        step += mission_6_6_set_straight_black_line(fpga_videodata);

                        if (step == 6) {
                            step += mission_6_6_set_center_black_line(fpga_videodata);
                        }

                        step = (step == 7) ? 6 : 5;
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
                        flag = 0;
                        mission_7_1_watch_below(5);
                        setFPGAVideoData(fpga_videodata);
                        step += mission_7_1_wait_front_of_yellow_hole_bridge(fpga_videodata, 5);
                        break;
                    case 1:
                        if (flag == 0) {
                            flag++;
                            mission_7_2_watch_below();
                        }
                        setFPGAVideoData(fpga_videodata);
                        step += mission_7_2_before_bridge_set_center(fpga_videodata);
                        break;
                    case 2:
                        flag = 0;
                        step += mission_7_3_climb_yellow_hole_bridge();
                        break;
                    case 3:
                        if (flag == 0) {
                            flag++;
                            mission_7_7_watch_side();
                        }
                        setFPGAVideoData(fpga_videodata);
                        step += mission_7_4_set_straight(fpga_videodata);

                        if (step == 4) {
                            step += mission_7_4_set_center(fpga_videodata);
                        }

                        if (step == 5) {
                            mission_7_4_watch_below();
                            setFPGAVideoData(fpga_videodata);
                            step += mission_7_5_walk_until_line_front_of_feet(fpga_videodata);
                        }

                        step = (step == 6) ? 6 : 3;
                        flag = (step == 5) ? 0 : 1;
                        break;
                    case 6:
                        step += mission_7_6_jump_hole();
                        break;
                    case 7:
                        mission_7_7_watch_side();
                        setFPGAVideoData(fpga_videodata);
                        step += mission_7_7_after_yellow_bridge_set_straight(fpga_videodata); // 직선 맞추기

                        if (step == 8) {
                            step += mission_7_7_after_yellow_bridge_set_center(fpga_videodata);// 길이 맞추기
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
                        mission += mission_8_4_is_front_of_finish_line(fpga_videodata);
                        step = 0;
                        break;
                    default:
                        mission_3_default_watch_below();
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
                        // 왼쪽 보기
                        // 파란색이 있는지 없는지 확인
                        // 선이 직선이고 일정 거리인지 확인
                        //오른쪽 보기

                        if (flag == 0) {
                            mission_4_6_watch_side();
                            flag++;
                        }

                        setFPGAVideoData(fpga_videodata);

                        if (mission_10_1_catch_blue_gate(fpga_videodata)) { // 파란색
                            mission_10_1_front_walk(8);
                            step += 1;
                            break;
                        }

                        if (mission_2_4_after_bridge_set_straight(fpga_videodata, 1) &&
                            mission_4_6_set_center(fpga_videodata, ((nextMission == 7) ? 70 : 60))) { // 앞에꺼는
                            mission_10_1_front_walk(4);
                        }

                        step = 0;
                        break;
                    case 1:
                        step = 0;
                        mission = nextMission;
                        nextMission = 0;
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
