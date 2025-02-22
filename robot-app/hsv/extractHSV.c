#include "extractHSV.h"

int cmp(const void* a, const void* b)
{
  return *(U8*)a - *(U8*)b;
}

U8 top, bot, left, right;
U8 p_x[2], p_y[2];

void extractHSV(void)
{
    U8 key_input, speed_input, len_input, dir_input, pose_input, view_input, mod_input;
    U8 centerX, centerY;
    float dx, dy, slope, distance;
    U8 mod;

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
    mod = 0;

    init_extract();

    do {
        key_input = getchar();
        //동작 수행

        setFPGAVideoData(fpga_videodata);

        switch(key_input)
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
                if (mod == 0)
                {
                    if (top < 1) break;
                    --top;
                    --bot;
                }
                else
                {
                    if (p_y[mod - 1] < 1) break;
                    --p_y[mod - 1];
                }
                break;
            case 's':
                if (mod == 0)
                {
                    if (bot > HEIGHT - 2) break;
                    ++top;
                    ++bot;
                }
                else
                {
                    if (p_y[mod - 1] > HEIGHT - 2) break;
                    ++p_y[mod - 1];
                }
                break;
            case 'a':
                if (mod == 0)
                {
                    if (left < 1) break;
                    --left;
                    --right;
                }
                else
                {
                    if (p_x[mod - 1] < 1) break;
                    --p_x[mod - 1];
                }
                break;
            case 'd':
                if (mod == 0)
                {
                    if (right > WIDTH - 2) break;
                    ++left;
                    ++right;
                }
                else
                {
                    if (p_x[mod - 1] > WIDTH - 2) break;
                    ++p_x[mod - 1];
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
                printf("\t1 : LOW\n");
                printf("\t2 : MIDDLE\n");
                printf("\t3 : HIGH\n");
                pose_input = getchar();

                printf("\t1 : DOWN\n");
                printf("\t2 : OBLIQUE\n");
                printf("\t3 : UP\n");
                printf("\t4 : LEFT\n");
                printf("\t5 : RIGHT\n");
                view_input = getchar();

                printf("INIT\tpose : %d\tview : %d\n", pose_input - '1', view_input - '1');
                ACTION_INIT(pose_input - '1', view_input - '1');
                break;
            case 'r':
                printf("\t1 : FAST\n");
                printf("\t2 : SLOW\n");
                speed_input = getchar();

                printf("\t1 : DOWN\n");
                printf("\t2 : OBLIQUE\n");
                printf("\t3 : UP\n");
                view_input = getchar();

                printf("WALK\tspeed : %d\tview : %d\n", speed_input - '1', view_input - '1');
                ACTION_WALK(speed_input - '1', view_input - '1', 3);
                break;
            case 't':
                printf("\t1 : LEFT\n");
                printf("\t2 : RIGHT\n");
                dir_input = getchar();

                printf("\t1 : LOW\n");
                printf("\t3 : HIGH\n");
                pose_input = getchar();

                printf("\t1 : DOWN\n");
                printf("\t2 : OBLIQUE\n");
                printf("\t4 : LEFT\n");
                printf("\t5 : RIGHT\n");
                view_input = getchar();

                printf("TURN\tdir : %d\tpose : %d\tview : %d\t%d\n", dir_input - '1', pose_input - '1', view_input - '1');
                ACTION_TURN(dir_input - '1', pose_input - '1', view_input - '1', 3);
                break;
            case 'y':
                printf("\t1 : LONG\n");
                printf("\t2 : SHORT\n");
                len_input = getchar();

                printf("\t1 : LEFT\n");
                printf("\t2 : RIGHT\n");
                dir_input = getchar();

                printf("\t1 : LOW\n");
                printf("\t2 : MIDDLE\n");
                printf("\t3 : HIGH\n");
                pose_input = getchar();

                printf("\t1 : DOWN\n");
                printf("\t2 : OBLIQUE\n");
                printf("\t4 : LEFT\n");
                printf("\t5 : RIGHT\n");
                view_input = getchar();

                printf("MOVE\tlen : %d\tdir : %d\tpose : %d\tview : %d\n", len_input - '1', dir_input - '1', pose_input - '1', view_input - '1');
                ACTION_MOVE(len_input - '1', dir_input -  '1', pose_input - '1', view_input - '1', 3);
                break;
            case 'b':
                printf("\t1 : FRONT\n");
                printf("\t2 : BACK\n");
                dir_input = getchar();

                printf("BIT\tdir : %d\n", dir_input - '1');
                ACTION_BIT(dir_input - '1', 3);
                break;
            case 'u':
                printf("\t1 : MISSION 2\n");
                printf("\t2 : MISSION 3\n");
                printf("\t3 : MISSION 4\n");
                printf("\t4 : MISSION 5 UP\n");
                printf("\t5 : MISSION 5 DOWN\n");
                printf("\t6 : MISSION 6\n");
                printf("\t7 : MISSION 7\n");
                key_input = getchar();

                switch(key_input)
                {
                    case '1':
                        ACTION_MOTION(MISSION_2_RED_DUMBLING, MIDDLE, DOWN);
                        break;
                    case '2':
                        ACTION_MOTION(MISSION_3_MINE_WALK, LOW, DOWN);
                        break;
                    case '3':
                        ACTION_MOTION(MISSION_4_HURDLING, MIDDLE, DOWN);
                        break;
                    case '4':
                        ACTION_MOTION(MISSION_5_STAIR_UP, MIDDLE, DOWN);
                        break;
                    case '5':
                        ACTION_MOTION(MISSION_5_STAIR_DOWN, MIDDLE, DOWN);
                        break;
                    case '6':
                        ACTION_MOTION(MISSION_6_RIGHT_KICK, MIDDLE, DOWN);
                        break;
                    case '7':
                        ACTION_MOTION(MISSION_7_YELLOW_DUMBLING, MIDDLE, DOWN);
                        break;
                    default:
                        ;
                }
                break;
            case 'm':
                printf("\t1 : box\n");
                printf("\t2 : point 1\n");
                printf("\t3 : point 2\n");

                mod_input = getchar();
                switch (mod_input)
                {
                    case '1':
                        mod = 0;
                        break;
                    case '2':
                        mod = 1;
                        break;
                    case '3':
                        mod = 2;
                        break;
                    default:
                        ;
                }
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
            case 'h':
                help();
                break;
            default:
            ;
        }
    } while(key_input != 'q');

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
    printf("w : move picked up\n");
    printf("s : move picked down\n");
    printf("a : move picked left\n");
    printf("d : move picked right\n");
    printf("z : box height size down\n");
    printf("x : box height size up\n");
    printf("c : box width size down\n");
    printf("v : box width size up\n");
    printf("i : Action_INIT\n");
    printf("r : Action_WALK\n");
    printf("t : Action_TURN\n");
    printf("y : Action_MOVE\n");
    printf("u : Action_MISSION\n");
    printf("b : Action_BIT\n");
    printf("m : change mod\n");
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

    ACTION_INIT(MIDDLE, UP);

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
