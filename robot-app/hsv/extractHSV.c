#include "extractHSV.h"


U8 top, bot, left, right;


void extractHSV(void)
{
    U8 motion;
    U32 H, S, V;
    U16 cnt;
    U16 pos;
    U16 *fpga_videodata;
    U8 i, j;

    help();

    fpga_videodata = (U16 *) malloc(WIDTH * HEIGHT * 2);

    init_extract();

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
                        H += (fpga_videodata[pos] & BIT_H) SHIFT_H;
                        S += (fpga_videodata[pos] & BIT_S) SHIFT_S;
                        V += (fpga_videodata[pos] & BIT_V) SHIFT_V;
                        ++cnt;
                    }
                }
                H /= cnt;
                S /= cnt;
                V /= cnt;

                printf("cnt : %d\tH : %d\tS : %d\tV : %d\n", cnt, H, S, V);
                break;
            case 's':
                printf("top : \n");
                scanf("%d", &top);
                printf("bot : \n");
                scanf("%d", &bot);
                printf("left : \n");
                scanf("%d", &left);
                printf("right : \n");
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
            case 'h':
                help();
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
    for (j = left - 1; j <= right + 1; ++j)
    {
        pos = (top + 1) * 180 + j;
        buf[pos] = 0xffff;
        pos = (bot + 1) * 180 + j;
        buf[pos] = 0xffff;
    }

    for (i = top - 1; i <= bot + 1; ++i)
    {
        pos = i * 180 + left - 1;
        buf[pos] = 0xffff;

        pos = i * 180 + right + 1;
        buf[pos] = 0xffff;
    }

    draw_fpga_video_data_full(buf);
    flip();
}

void help(void)
{
    printf("----------------------------------------------------------------\n");
    printf("e : extract HSV\n");
    printf("s : set box\n");
    printf("i : Action_INIT_ROBOT\n");
    printf("b : Action_WATCH_BELOW_LONG\n");
    printf("l : Action_LEFT_TURN_HEAD_LONG\n");
    printf("r : Action_RIGHT_TURN_HEAD_LONG\n");
    printf("h : help\n");
    printf("q : exit\n");
    printf("----------------------------------------------------------------\n");
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

    top = 54;
    bot = 65;
    left = 84;
    right = 95;

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
