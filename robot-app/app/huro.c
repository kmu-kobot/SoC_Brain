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
                if (step == 0) {
                    step += mission_1_1_wait_yellow_barricade(fpga_videodata);
                } else if (step == 1) {
                    step += mission_1_2_end_yellow_barricade(fpga_videodata);
                } else if (step == 2) {
                    mission_1_3_escape_yellow_barricade(5);

                    step = 0;
                    mission += 1;
                }
                break;
            case 2: // MISSION 2: RED BRIDGE
                if (step == 0) {
                    step += mission_2_1_wait_front_of_red_bridge(fpga_videodata, 8, 3);
                } else if (step == 1) {
                    step += mission_2_2_before_bridge_set_center(fpga_videodata);
                } else if (step == 2) {
                    step += mission_2_3_escape_red_bridge();
                } else if (step == 3) {
                    step += mission_2_4_after_bridge_set_straight(fpga_videodata); // 직선 맞추기
                } else if (step == 4) {
                    step += mission_2_5_after_bridge_set_center(fpga_videodata);// 길이 맞추기
                } else if (step == 5) {
                    mission += 1;
                    step = 0;
                }
                break;
            case 3: // MISSION 3: AVOID BOMB
                break;
            case 4: // MISSION 4: JUMP HUDDLE
                break;
            case 5: // MISSION 5: GREEN BRIDGE
                break;
            case 6: // MISSION 6: KICK BALL
                break;
            case 7: // MISSION 7: YELLOW HOLE
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

    RobotAction(INIT_ROBOT);

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
