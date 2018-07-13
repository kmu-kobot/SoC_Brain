#include "extractHSV.h"


U8 top, bot, left, right;


void extractHSV(void)
{
    U8 motion;
    U8 H, S, V;
    U8 cnt;
    U16 pos;
    U16 *fpga_videodata;
    U8 i, j;

    fpga_videodata = (U16 *) malloc(WIDTH * HEIGHT * 2);



    do {
        motion = getchar();
        //동작 수행

        setFPGAVideoData(fpga_videodata);

        switch(motion)
        {
            case 'e':
                H = 0;
                S = 0;
                V = 0;
                cnt = 0;
                for (i = top; i <= bot; ++i)
                {
                    for (j = left; j <= right; ++j)
                    {
                        pos = i * 180 + j;
                        H += fpga_videodata[pos] >> 10;
                        S += fpga_videodata[pos] << 6 >> 11;
                        V += fpga_videodata[pos] << 11 >> 11;
                        ++cnt;
                    }
                }
                H /= cnt;
                S /= cnt;
                V /= cnt;

                printf("H : %d\tS : %d\tV : %d\n", H, S, V);
                break;
            case 's':
                printf("top : ");
                scanf("%d", &top);
                printf("bot : ");
                scanf("%d", &bot);
                printf("left : ");
                scanf("%d", &left);
                printf("right : ");
                scanf("%d", &right);
                break;
            case 'i':
                Action_INIT_ROBOT();
                break;
            case 'b':
                Action_WATCH_BELOW_LONG();
                break;
            case 'l':
                Action_LEFT_TURN_HEAD_LONG();
                break;
            case 'r':
                Action_RIGHT_TURN_HEAD_LONG();
                break;
            default:
            ;
        }
    } while(motion != 'q');

    destroy_extract(fpga_videodata);
}

void setFPGAVideoData(U16 *buf) {
    U8 i, j;
    U16 pos;
    read_fpga_video_data(buf);

    // draw white box
    for (j = left; j <= right; ++j)
    {
        pos = top * 180 + j;
        buf[pos] = 0xffff;

        pos = bot * 180 + j;
        buf[pos] = 0xffff;
    }

    for (i = top; i <= bot; ++i)
    {
        pos = i * 180 + left;
        buf[pos] = 0xffff;

        pos = i * 180 + right;
        buf[pos] = 0xffff;
    }

    draw_fpga_video_data_full(buf);
    flip();
}

int init_extract(void) {

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

    top = 58;
    bot = 61;
    left = 88;
    right = 91;

    return 0;
}

void destroy_extract(U16 *buf) {
    // free fpga video data
    free(buf);

    // graphic close
    close_graphic();

    // UART CLOSE
    uart_close();

}
