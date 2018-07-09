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
                        step += mission_2_1_wait_front_of_red_bridge(fpga_videodata, 3);
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

                        mission_2_2_watch_side();
                        setFPGAVideoData(fpga_videodata);
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
                break;
            case 4: // MISSION 4: JUMP HURDLE
                switch (step) {
                    case 3:
                        step += mission_4_3_jump_hurdle();
                        break;
                    case 4:
                        // 직각 맞추고 해야댐~~ mission_2_4_after_bridge_set_straight 활용
                        break;
                    case 5:
                        step += mission_4_5_check_bk_line(fpga_videodata);
                        break;
                    case 6:
                        // 중심 맞추기 mission_2_5_after_bridge_set_center 활용
                        break;
                    case 7:
                        mission += 1;
                        step = 0;
                        break;
                    default:
                        mission_4_1_watch_front(4);
                        step += mission_4_2_ready_hurdle(fpga_videodata);
                        break;
                }
                break;
            case 5: // MISSION 5: GREEN BRIDGE
                switch (step) {
                    case 0:
                        // 앞으로 걸어가면서 밑에 검은색 바가 있는지 확인
                        break;
                    case 1:
                        // 맨처음 다리 중심 맞추기
                        break;
                    case 2:
                        // 계단 오르기
                        break;
                    case 3:
                        // 초록색 중심 맞추기
                        break;
                    case 4:
                        // 먼저 각도랑 줌식체크, 맞으면 가기, 그리고 앞에 검은선확인하면서 끝났는지 확인
                        // 1. 검은선확인
                        // if (2. 각도체크) {
                        //    3. 가기
                        // }
                        break;
                    case 5:
                        // 내려가기 위해 선 맞추기
                        break;
                    case 6:
                        // 계단 내려가기
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
                break;
            case 7: // MISSION 7: YELLOW HOLE
                switch (step) {
                    case 0:
                        // 앞으로 걸어가면서 밑에 노란색이 있는지 확인
                        break;
                    case 1:
                        // 중심 맞추기
                        break;
                    case 2:
                        // 계단 오르기
                        break;
                    case 3:
                        // 각도랑 중심 체크
                        break;
                    case 4:
                        break;
                    case 5:
                        // 노란색 선으로 중심 체크
                        break;
                    case 6:
                        // 구르기랑 내려가기
                        break;
                    case 7:
                        // 각도랑 중심 체크
                        break;
                    case 8:
                        mission += 1;
                        step = 0;
                        break;
                    default:
                        break;
                }
                break;
            case 8: // MISSION 8: LAST BARRICADE
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
