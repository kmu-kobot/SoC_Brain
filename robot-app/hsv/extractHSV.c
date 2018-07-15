#include "extractHSV.h"

int cmp(const void* a, const void* b)
{
  return *(U8*)a - *(U8*)b;
}

U8 top, bot, left, right;

void extractHSV(void)
{
    U8 motion;
    U8 H_buff[SIZE], H_R_buff[SIZE], S_buff[SIZE], V_buff[SIZE];
    U32 H, H_R, S, V;
    U16 cnt;
    U16 pos;
    U16 cnt_valid;
    U16 start, end;

    U8 i, j;

    U16 *fpga_videodata;

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
                H_R = 0;
                S = 0;
                V = 0;
                cnt = 0;
                for (i = top; i <= bot; ++i)
                {
                    for (j = left; j <= right; ++j)
                    {
                        pos = i * WIDTH + j;
                        H_buff[cnt] = (fpga_videodata[pos] & BIT_H) SHIFT_H;
                        H_R_buff[cnt] = (((fpga_videodata[pos] & BIT_H) SHIFT_H) + 120) % 240;
                        S_buff[cnt] = (fpga_videodata[pos] & BIT_S) SHIFT_S;
                        V_buff[cnt] = (fpga_videodata[pos] & BIT_V) SHIFT_V;
                        ++cnt;
                    }
                }

                qsort(H_buff, cnt, sizeof(U8), cmp);
                qsort(H_R_buff, cnt, sizeof(U8), cmp);
                qsort(S_buff, cnt, sizeof(U8), cmp);
                qsort(V_buff, cnt, sizeof(U8), cmp);

                start = cnt / 10;
                end = cnt - start;
                cnt_valid = end - start;

                for (i = start; i < end; ++i)
                {
                    H += H_buff[i];
                    H_R += H_R_buff[i];
                    S += S_buff[i];
                    V += V_buff[i];
                }

                H /= cnt_valid;
                H_R /= cnt_valid;
                S /= cnt_valid;
                V /= cnt_valid;

                printf("cnt : %d\tcnt_valid : %d\tH : %d\tH_R : %d\tS : %d\tV : %d\n", cnt, cnt_valid, H, H_R, S, V);
                break;
            case 'w':
                if (top < 1) break;
                top -= 1;
                bot -= 1;
                break;
            case 's':
                if (bot > HEIGHT - 2) break;
                top += 1;
                bot += 1;
                break;
            case 'a':
                if (left < 1) break;
                left -= 1;
                right -= 1;
                break;
            case 'd':
                if (right > WIDTH - 2) break;
                left += 1;
                right += 1;
                break;
            case 'z':
                if (bot - top < 3) break;
                top += 1;
                bot -= 1;
                break;
            case 'x':
                if (top < 1 || bot > HEIGHT - 2) break;
                top -= 1;
                bot += 1;
                break;
            case 'c':
                if (right - left < 3) break;
                left += 1;
                right -= 1;
                break;
            case 'v':
                if (left < 1 || right > WIDTH - 2) break;
                left -= 1;
                right += 1;
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
        pos = (top - 1) * WIDTH + j;
        buf[pos] = 0xffff;
        pos = (bot + 1) * WIDTH + j;
        buf[pos] = 0xffff;
    }

    for (i = top - 1; i <= bot + 1; ++i)
    {
        pos = i * WIDTH + left - 1;
        buf[pos] = 0xffff;

        pos = i * WIDTH + right + 1;
        buf[pos] = 0xffff;
    }

    draw_fpga_video_data_full(buf);
    flip();
}

void help(void)
{
    printf("----------------------------------------------------------------\n");
    printf("e : extract HSV\n");
    printf("w : move box up\n");
    printf("s : move box down\n");
    printf("a : move box left\n");
    printf("d : move box right\n");
    printf("z : box height size down\n");
    printf("x : box height size up\n");
    printf("c : box width size down\n");
    printf("v : box width size up\n");
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
