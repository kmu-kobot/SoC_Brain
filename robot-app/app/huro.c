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
    int mission = 1;

    int rResult = 1;

    U32 col, row, cntBlue = 0, cntGreen = 0;
    U16 p;

    while (!missionFinished) {

        setFPGAVideoData(fpga_videodata);

        cntBlue = 0;
        cntGreen = 0;

        for(row = 0; row < HEIGHT; row++) {
            for(col = 0; col < WIDTH; col++) {
                p = GetPtr(fpga_videodata, row, col, WIDTH);
                cntGreen += GetValueRGBYOBK(p, GREEN);
                cntBlue += GetValueRGBYOBK(p, BLUE);
            }
        }
        
        printf("BLUE : %d\n", cntBlue);
        printf("GREEN : %d\n", cntGreen);

        if (cntBlue / WIDTH * HEIGHT > 0.4) {
            rResult = 200;
        } else if (cntGreen / WIDTH * HEIGHT > 0.4) {
            rResult = 201;
        } else {
            rResult = 1;
        }

        RobotAction(rResult);

//        switch (mission) {
//            case 1:
//                // TODO : rReuslt 미션 수행을 위한 준비과정 위치 선정 결과 참이냐 거짓이냐
//                if (rResult) {
//                    // TODO : 해당 미션에 대한 동작 실행
//                    mission += 1;
//                }
//                break;
//            case 2:
//                // TODO : ...
//                break;
//            default:
//                missionFinished = 1;
//                break;
//        }

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

    RobotAction(1);

    // graphic port check
    if (open_graphic() < 0) {
        return -1;
    }

    direct_camera_display_off();

    // TODO : Ready motion start!

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
