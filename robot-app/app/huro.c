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

    int step = 0;

    while (!missionFinished) {

        setFPGAVideoData(fpga_videodata);

        printf("[START]\tMISSION[%d]: STEP [%d]\n", mission, step);

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
                        mission_1_3_escape_yellow_barricade(8);
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
                        mission_2_1_watch_below(8);
                        setFPGAVideoData(fpga_videodata);
                        step += mission_2_1_wait_front_of_red_bridge(fpga_videodata, 5);
                        break;
                    case 1:
                        mission_2_2_watch_side();
                        setFPGAVideoData(fpga_videodata);
                        step += mission_2_2_before_bridge_set_center(fpga_videodata);
                        break;
                    case 2:
                        step += mission_2_3_escape_red_bridge();
                        break;
                    case 3:
                        mission_2_2_watch_side();
                        setFPGAVideoData(fpga_videodata);
                        step += mission_2_4_after_bridge_set_straight(fpga_videodata); // 직선 맞추기

                        if (step == 3) {
                            mission_2_2_watch_side();
                            setFPGAVideoData(fpga_videodata);
                        }
                        step += mission_2_5_after_bridge_set_center(fpga_videodata);// 길이 맞추기

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
                mission += 1;
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
                        mission_4_6_watch_side();
                        setFPGAVideoData(fpga_videodata);
                        step += mission_4_6_set_center(fpga_videodata);

                        if (step == 5) {
                            mission_4_6_watch_side();
                            setFPGAVideoData(fpga_videodata);
                        }

                        step += mission_4_6_set_straight(fpga_videodata);

                        step = (step == 7) ? 6 : 5;
                        break;
                    case 6:
                        mission = 10;
                        step = 0;
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
                        mission_5_1_watch_below(5);
                        setFPGAVideoData(fpga_videodata);
                        step += mission_5_1_check_black_line(fpga_videodata, 3);
                        break;
                    case 1:
                        // 맨 처음 다리 중심 맞추기
                        mission_5_2_watch_side();
                        setFPGAVideoData(fpga_videodata);
                        step += mission_2_2_before_bridge_set_center(fpga_videodata);
                        break;
                    case 2:
                        // 계단 오르기
                        step += mission_5_3_climb_up_stairs();
                        break;
                    case 3:
                        mission_5_2_watch_side();
                        setFPGAVideoData(fpga_videodata);
                        step += mission_5_4_set_center_before_green_bridge(fpga_videodata);
                        break;
                    case 4:
                        mission_5_5_watch_below();
                        setFPGAVideoData(fpga_videodata);

                        if (mission_5_5_check_finish_black_line(fpga_videodata)) {
                            step += 1;
                            break;
                        }

                        step += mission_5_5_check_green_bridge_center(fpga_videodata);

                        if (step == 4) {
                            mission_5_5_watch_below();
                            setFPGAVideoData(fpga_videodata);
                        }

                        step += mission_5_5_check_green_bridge_straight(fpga_videodata);

                        if (step == 6) {
                            mission_5_5_short_walk_on_green_bridge(4);
                        }

                        step = 4;
                        break;
                    case 5:
                        // TODO: 내려가기 위해 선 맞추기
                        break;
                    case 6:
                        step += mission_5_7_climb_down_stairs();
                        break;
                    case 7:
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
                        // TODO: 1. 공을 중심으로 맞출때 까지 회전
                        break;
                    case 1:
                        // TODO: 2. 앞으로 걸으면서 row(0~60) 지점에 공이 잡히는지 확인
                        break;
                    case 2:
                        // TODO 3: 생각이 안난다..
                        break;
                    default:
                        mission = 10;
                        step = 0;
                        nextMission = 7;
                        break;
                }
                break;
            case 7: // MISSION 7: YELLOW HOLE
                switch (step) {
                    case 0:
                        mission_7_1_watch_below(8);
                        setFPGAVideoData(fpga_videodata);
                        step += mission_7_1_wait_front_of_yellow_hole_bridge(fpga_videodata, 5);
                        break;
                    case 1:
                        mission_7_2_watch_side();
                        setFPGAVideoData(fpga_videodata);
                        step += mission_7_2_before_bridge_set_center(fpga_videodata);
                        break;
                    case 2:
                        step += mission_7_3_climb_yellow_hole_bridge();
                        break;
                    case 3:
                        mission_7_2_watch_side();
                        setFPGAVideoData(fpga_videodata);
                        step += mission_7_4_set_center_on_yellow_bridge(fpga_videodata);

                        // TODO: 이전 결과에 따라서 fpga set 여부 결정
                        mission_7_2_watch_side();
                        setFPGAVideoData(fpga_videodata);
                        step += mission_7_4_set_straight_on_yellow_bridge(fpga_videodata);

                        if (step == 5) {
                            mission_7_5_watch_below();
                            setFPGAVideoData(fpga_videodata);
                            // TODO: 앞으로 걸어가면서 바로앞에 검은선인지 확인
                            step += mission_7_5_walk_until_black_line(fpga_videodata);
                            if (step == 5) {
                                Action_WALK_FRONT_SHORT(2);
                            }
                        }
                        step = (step == 6) ? 4 : 3;
                        break;
                    case 4:
                        mission_7_2_watch_side();
                        setFPGAVideoData(fpga_videodata);
                        step += mission_7_4_set_center_on_yellow_bridge(fpga_videodata);

                        mission_7_2_watch_side();
                        setFPGAVideoData(fpga_videodata);
                        step += mission_7_4_set_straight_on_yellow_bridge(fpga_videodata);

                        if (step == 6) {
                            step = 4;
                            step += mission_7_6_jump_hole();
                        } else {
                            step = 4;
                        }

                        break;
                    case 5:
                        // TODO: 각도랑 중심 체크
                        mission_7_2_watch_side();
                        setFPGAVideoData(fpga_videodata);
                        step += mission_7_7_after_yellow_bridge_set_center(fpga_videodata);// 길이 맞추기

                        mission_2_2_watch_side();
                        setFPGAVideoData(fpga_videodata);
                        step += mission_7_7_after_yellow_bridge_set_straight(fpga_videodata); // 직선 맞추기

                        step = (step == 7) ? 6 : 5;
                        break;
                    case 6:
                        mission += 1;
                        step = 0;
                        break;
                    default:
                        break;
                }
                break;
            case 8: // MISSION 8: AVOID BOMB
                mission += 1;
                step = 0;
                break;
            case 9: // MISSION 9: LAST BARRICADE
                switch (step) {
                    case 0:
                        step += mission_9_1_wait_yellow_barricade(fpga_videodata);
                        break;
                    case 1:
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
                mission = nextMission;
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

    Action_INIT_ROBOT();

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
    // clear_screen();
    read_fpga_video_data(buf);
    draw_fpga_video_data_full(buf);
    flip();
}
