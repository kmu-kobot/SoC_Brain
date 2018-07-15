#include "extractHSV.h"

int cmp(const void* a, const void* b)
{
  return *(U8*)a - *(U8*)b;
}

U8 top, bot, left, right;
U8 p_x[2], p_y[2];

void extractHSV(void)
{
    U8 motion;
    U8 centerX, centerY;
    float dx, dy, slope, distance;
    U8 picked;

    U8 *H_buff, *H_R_buff, *S_buff, *V_buff;
    U32 H, H_R, S, V;
    U16 cnt;
    U16 pos;
    U16 cnt_valid;
    U16 start, end;

    U16 *fpga_videodata;
    U8 i, j;

    help();

    H_buff = (U8 *) malloc(SIZE * sizeof(U8));
    H_R_buff = (U8 *) malloc(SIZE * sizeof(U8));
    S_buff = (U8 *) malloc(SIZE * sizeof(U8));
    V_buff = (U8 *) malloc(SIZE * sizeof(U8));
    fpga_videodata = (U16 *) malloc(WIDTH * HEIGHT * 2);

    top = 54;
    bot = 65;
    left = 84;
    right = 95;
    p_x[0] = 89;
    p_x[1] = 90;
    p_y[0] = 59;
    p_y[1] = 60;
    picked = 0;

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
                        // H += (fpga_videodata[pos] & BIT_H) SHIFT_H;
                        // H_R += (((fpga_videodata[pos] & BIT_H) SHIFT_H) + 120) % 240;
                        // S += (fpga_videodata[pos] & BIT_S) SHIFT_S;
                        // V += (fpga_videodata[pos] & BIT_V) SHIFT_V;
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
                // H /= cnt;
                // H_R /= cnt;
                // S /= cnt;
                // V /= cnt;

                printf("cnt : %d\tcnt_valid : %d\tH : %d\tH_R : %d\tS : %d\tV : %d\n", cnt, cnt_valid, H, H_R, S, V);
                // printf("cnt : %d\tH : %d\tH_R : %d\tS : %d\tV : %d\n", cnt, H, H_R, S, V);
                break;
            case 'w':
                if (picked == 0)
                {
                    if (top < 1) break;
                    --top;
                    --bot;
                }
                else
                {
                    if (p_y[picked - 1] < 1) break;
                    --p_y[picked - 1];
                }
                break;
            case 's':
                if (picked == 0)
                {
                    if (bot > HEIGHT - 2) break;
                    ++top;
                    ++bot;
                }
                else
                {
                    if (p_y[picked - 1] > HEIGHT - 2) break;
                    ++p_y[picked - 1];
                }
                break;
            case 'a':
                if (picked == 0)
                {
                    if (left < 1) break;
                    --left;
                    --right;
                }
                else
                {
                    if (p_x[picked - 1] < 1) break;
                    --p_x[picked - 1];
                }
                break;
            case 'd':
                if (picked == 0)
                {
                    if (right > WIDTH - 2) break;
                    ++left;
                    ++right;
                }
                else
                {
                    if (p_x[picked - 1] > WIDTH - 2) break;
                    ++p_x[picked - 1];
                }
                break;
            case 'z':
                if (bot - top < 3) break;
                ++top;
                --bot;
                break;
            case 'x':
                if (top < 1 || bot > HEIGHT - 2) break;
                --top;
                ++bot;
                break;
            case 'c':
                if (right - left < 3) break;
                ++left;
                --right;
                break;
            case 'v':
                if (left < 1 || right > WIDTH - 2) break;
                --left;
                ++right;
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
            case '1':
                picked = 0;
                break;
            case '2':
                picked = 1;
                break;
            case '3':
                picked = 2;
                break;
            case 'o':
                centerX = (right + left) >> 1;
                centerY = (bot + top) >> 1;
                printf("center of box : (%d, %d)\n", centerX, centerY);
                printf("top : %d\tbot : %d\tleft : %d\tright : %d\n", top, bot, left, right);
                break;
            case 'p':
                dx = p_x[1] - p_x[0];
                dy = p_y[1] - p_y[0];
                slope = dy / dx;
                distance = sqrt(dx * dx + dy * dy);

                printf("point1 : (%d, %d)\tpoint2 : (%d, %d)\n", p_x[0], p_y[0], p_x[1], p_y[1]);
                printf("slope : %f\tdistance : %f\n", -slope, distance);
                break;
            default:
            ;
        }
    } while(motion != 'q');

    free(H_buff);
    free(H_R_buff);
    free(S_buff);
    free(V_buff);
    destroy_extract(fpga_videodata);
}

void setFPGAVideoData(U16 *buf)
{
    U8 i, j;
    U16 pos;
    read_fpga_video_data(buf);

    // draw white box
    for (j = left - 1; j <= right + 1; ++j)
    {
        pos = (top - 1) * WIDTH + j;
        buf[pos] = COLOR_WHITE;
        pos = (bot + 1) * WIDTH + j;
        buf[pos] = COLOR_WHITE;
    }

    for (i = top - 1; i <= bot + 1; ++i)
    {
        pos = i * WIDTH + left - 1;
        buf[pos] = COLOR_WHITE;

        pos = i * WIDTH + right + 1;
        buf[pos] = COLOR_WHITE;
    }

    // draw points
    pos = p_y[0] * WIDTH + p_x[0];
    buf[pos] = COLOR_CYAN;

    pos = p_y[1] * WIDTH + p_x[1];
    buf[pos] = COLOR_MAGENTA;

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
    printf("1 : pick box\n");
    printf("2 : pick point 1\n");
    printf("3 : pick point 2\n");
    printf("o : print box position\n");
    printf("p : print points position, slope, distance\n");
    printf("h : help\n");
    printf("q : exit\n");
    printf("----------------------------------------------------------------\n");
}

int init_extract(void)
{

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

void destroy_extract(U16 *buf)
{
    // free fpga video data
    free(buf);

    // graphic close
    close_graphic();

    // UART CLOSE
    uart_close();

}
