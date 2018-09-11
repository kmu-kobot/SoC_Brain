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

    int mission = 5;
    int step = 0;

    int nextMission = 0;
    int flag = 0;

    while (!missionFinished) {

        setFPGAVideoData(fpga_videodata);

#ifdef DEBUG
        printf("\n[START]\tMISSION[%d]: STEP [%d]\n", mission, step);
#endif

        switch (mission) {
            case 0: // MISSION 0: READY PLAY   // 빼도 될거같음
                // mission += mission_0_1_wait_play(fpga_videodata);
                // default_set_straight_and_center1(fpga_videodata, LEFT, WIDTH>>1, HEIGHT-11, BLACK);
                // default_get_straight_and_center1(fpga_videodata, RIGHT, WIDTH>>1, HEIGHT-11, BLACK);
                ++mission;
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
                        step += 1;
                        flag = 0;
                        break;
                    case 3:
                        step = 0;
                        mission += 1;
                        flag = 3;
                        break;
                    default:
                        break;
                }
                break;
            case 2: // MISSION 2: RED BRIDGE
                switch (step) {
                    case 0:
                        if (!mission_2_1_wait_front_of_red_bridge(fpga_videodata)) { // 어차피 이 스텝에서 flag 3밖에 안쓰니까 이 if 문 없어도 될거같음
                            // mission_2_1_watch_below(fpga_videodata, 20);
                            ACTION_ATTACH(1);
                        }

                        setFPGAVideoData(fpga_videodata);
                        step += mission_2_1_wait_front_of_red_bridge(fpga_videodata);
                        break;
                    case 1:
                        flag = 0;
                        setFPGAVideoData(fpga_videodata);
                        step += mission_2_1_attach_red_bridge(fpga_videodata);
                        break;
                    case 2:
                        // 1. 왼쪽보고 중심 맞추기
                        if (flag == 0) {
                            default_watch(LEFT);
                            RobotSleep(1);
                            flag++;
                        }

                        step += default_set_center1(fpga_videodata, LEFT, 60, HEIGHT-11, BLACK);

                        if (step == 2)
                        {
                            flag = 2;
                        }
                        break;
                    case 3:
                        if (flag == 2)
                        {
                            ACTION_ATTACH(1);
                        }
                        step += mission_2_3_escape_red_bridge();
                        flag = 0;
                        break;
                    case 4:
                        if (flag == 0) {
                            default_watch(mission_3_4_getMDir());
                            RobotSleep(1);
                            flag++;
                        }

                        step += mission_3_set_straight_and_center1(fpga_videodata);
                        break;
                    case 5:
                        flag = 0;
                        mission += 1;
                        step = 0;
                        default_watch(OBLIQUE);
                        break;
                    default:
                        break;
                }
                break;
            case 3: // MISSION 3: AVOID BOMB
                switch (step) {
                    case 0:
                        default_watch(OBLIQUE);
                        RobotSleep(1);
                        setFPGAVideoData(fpga_videodata);
                        if (!mission_3_default_avoid_bomb(fpga_videodata))
                        {
                            step = mission_3_default_watch_below(fpga_videodata, 10) ? 1 : 2;
                            RobotSleep(2);
                        }
                        else
                        {
                            step = 1;
                        }

                        if (mission_3_isFrontOf_Blue(fpga_videodata, HEIGHT)) { // 얘를 윗줄이랑 합쳐서 a ? 4 : b ? 1 : 2 형태로 만드는게 나을듯
                            step = 4;
                        }

                        flag = 0;
                        break;
                    case 1:
                        if (flag == 0) {
                            flag = 1;
                            default_watch(mission_3_4_getMDir());
                            RobotSleep(2);
                            setFPGAVideoData(fpga_videodata);
                            mission_3_measure_line(fpga_videodata);
                            break;
                        } else if (flag == 1) {
                            flag = 2;
                            default_watch(DOWN);
                        }

                        setFPGAVideoData(fpga_videodata);

                        if (mission_3_isFrontOf_Blue(fpga_videodata, ROBOT_KNEE)){
                            RobotSleep(1);
                            setFPGAVideoData(fpga_videodata);
                            if (mission_3_isFrontOf_Blue(fpga_videodata, ROBOT_KNEE))
                            {
                                step = 4;
                                break;
                            }
                        }

                        if (mission_3_avoid(fpga_videodata)) { // 만약 지뢰를 다 피했으면
                            mission_3_change_mdir();
                            RobotSleep(1);
                            step = 2;
                            flag = 0;
                        }
                        break;
                    case 2:
                        if (flag == 0) {
                            default_watch(mission_3_4_getMDir());
                            RobotSleep(1);
                        }

                        step += mission_3_set_straight(fpga_videodata);

                        flag = (step == 3) ? 0 : 1;
                        break;
                    case 3:
                        default_watch(UP);
                        setFPGAVideoData(fpga_videodata);
                        RobotSleep(1);
                        step = (mission_3_1_ver2(fpga_videodata)) ? 4 : 0;
                        flag = 0;
                        break;
                    case 4:
                        ACTION_ATTACH(3);
                        step += 1;
                        flag = 0;
                        break;
                    case 5:
                        if (flag == 0) {
                            default_watch(mission_3_4_getMDir());
                            RobotSleep(1);
                            flag++;
                        }
                        step += mission_3_set_center(fpga_videodata);
                        if (step == 6)
                        {
                            flag = 2;
                        }
                        break;
                    case 6:
                        if (flag == 2)
                        {
                            ACTION_ATTACH(1);
                        }

                        mission += 1;
                        step = 0;
                        flag = 0;
                        mission_3_init_global();
                    default:
                        break;
                }
                break;
            case 4: // MISSION 4: JUMP HURDLE
                switch (step) {
                    case 0:
                        mission_4_4_jump_hurdle();
                        ++step;
                        break;
                    case 1:
                        // CHECK_INIT(UP);
                        // setFPGAVideoData(fpga_videodata);
                        // step += default_set_not_black(fpga_videodata); // 얘도 6_9 떼버리고 공용함수로 하는게 나을듯
                        ++step;
                        break;
                    case 2:
                        mission = 10;
                        step = 0;
                        flag = 0;
                        nextMission = 5;
                        break;
                    default:
                        break;
                }
                break;
            case 5: // MISSION 5: GREEN BRIDGE
                switch (step) {
                    case 0:
                        if (flag == 0) {
                            default_watch(DOWN);
                            ++flag;
                        }

                        setFPGAVideoData(fpga_videodata);
                        if (!mission_5_1_check_black_line(fpga_videodata))
                        {
                            mission_5_1_watch_below(fpga_videodata, 40);
                        }
                        ACTION_ATTACH(1);

                        ++step;
                        flag = 0;
                        break;
                    case 1:
                        flag = 0;
                        setFPGAVideoData(fpga_videodata);
                        step += mission_5_1_attach(fpga_videodata);
                    case 2:
                        // 맨 처음 다리 중심 맞추기
                        if (flag == 0) {
                            default_watch(LEFT);
                            RobotSleep(3);
                            flag++;
                        }
                        step += default_set_center1(fpga_videodata, LEFT, 60, HEIGHT-11, BLACK);
                        break;
                    case 3:
                        // 계단 오르기
                        ACTION_ATTACH(1);
                        mission_5_3_climb_up_stairs();
                        flag = 0;
                        ++step;
                        break;
                    case 4:
                        if (flag == 0)
                        {
                            default_watch(OBLIQUE);
                            RobotSleep(1);
                            ++flag;
                        }
                        setFPGAVideoData(fpga_videodata);
                        step += mission_5_4_set_straight_and_center(fpga_videodata);
                        break;
                    case 5:
                        mission_5_5_attach_green();
                        ++step;
                        flag = 0;
                        break;
                    case 6:
                        if (flag == 0)
                        {
                            default_watch(OBLIQUE);
                            ++flag;
                        }

                        setFPGAVideoData(fpga_videodata);
                        step += mission_5_6_set_straight_and_center(fpga_videodata);
                        break;
                    case 7:
                        step += mission_5_7_watch_below(fpga_videodata);
                        break;
                    case 8:
                        setFPGAVideoData(fpga_videodata);
                        step += mission_5_8_attach_black(fpga_videodata);
                        flag = 0;
                        break;
                    case 9:
                        if (flag == 0)
                        {
                            default_watch(DOWN);
                            RobotSleep(1);
                            ++flag;
                        }
                        setFPGAVideoData(fpga_videodata);
                        step += mission_5_9_attach_black(fpga_videodata) && mission_5_9_attach_black(fpga_videodata);
                        break;
                    case 10:
                        mission_5_10_climb_down_stairs();
                        ++step;
                        flag = 0;
                        break;
                    case 11:
                        if (flag == 0)
                        {
                            default_watch(LEFT);
                            RobotSleep(3);
                            ++flag;
                        }
                        step += default_set_straight_and_center1(fpga_videodata, LEFT, 120, HEIGHT-11, BLACK);
                        break;
                    case 12:
                        ACTION_WALK(FAST, OBLIQUE, 5);
                        ++step;
                        break;
                    case 13:
                        ++mission;
                        flag = 0;
                        step = 0;
                        break;
                    default :
                        break;
                }
                break;
            case 6: // MISSION 6: KICK BALL
                switch (step) {
                    case 0:
                        flag = 0;
                        default_watch(UP);
                        RobotSleep(1);
                        setFPGAVideoData(fpga_videodata);
                        step += mission_6_1_detection_ball(fpga_videodata);
                        break;
                    case 1:
                        if (flag == 0)
                        {
                            BALL_INIT(DOWN);
                            ++flag;
                        }

                        setFPGAVideoData(fpga_videodata);
                        step += mission_6_4_set_center_of_ball(fpga_videodata);

                        if (step == 2) {
                            flag = 0;
                        }
                        break;
                    case 2:
                        if (flag == 0)
                        {
                            BALL_HEAD(UP);
                            ++flag;
                        }

                        setFPGAVideoData(fpga_videodata);
                        if (mission_6_3_find_hole_interpolation(fpga_videodata, 0, HEIGHT, 0, WIDTH, 0) == 0) {
                            mission_6_3_find_side_hole(fpga_videodata, 0);
                        } else {
                            ++step;
                            flag = 0;
                        }
                        break;
                    case 3:
                        ++step;
                        break;
                    case 4:
                        ++step;
                        break;
                    case 5:
                        if (!flag) {
                            flag = 1;
                            BALL_HEAD(UP);
                        }

                        setFPGAVideoData(fpga_videodata);
                        step += mission_6_3_locate_hole_on_center(fpga_videodata);

                        if (step == 6) {
                            flag = 0;
                            BALL_HEAD(DOWN);
                            while (TRUE) {
                                setFPGAVideoData(fpga_videodata);
                                step += mission_6_4_set_center_of_ball(fpga_videodata);
                                if (step == 7) {
                                    break;
                                }
                            }
                        }

                        if (step == 7) {
                            BALL_HEAD(UP);
                            setFPGAVideoData(fpga_videodata);
                            step += mission_6_3_locate_hole_on_center(fpga_videodata);
                        }

                        if (step == 8) {
                            BALL_HEAD(DOWN);
                            setFPGAVideoData(fpga_videodata);
                            step += mission_6_4_set_center_of_ball(fpga_videodata);
                        }

                        step = (step == 9) ? 6 : 5;

                        break;
                    case 6:
                        flag = 0;
                        step += mission_6_5_kick_ball();
                        break;
                    case 7:
                        setFPGAVideoData(fpga_videodata);
                        step += default_set_not_black(fpga_videodata);
                        break;
                    case 8:
                        mission_6_9_walk_front();
                        ++step;
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
                        if (flag == 0) {
                            setFPGAVideoData(fpga_videodata);
                            mission_7_1_watch_below(fpga_videodata, 20);
                            ACTION_ATTACH(1); // 4개 너무 많음
                        }

                        setFPGAVideoData(fpga_videodata);
                        step += mission_7_1_wait_front_of_yellow_hole_bridge(fpga_videodata);
                        flag = (step == 1);
                        break;
                    case 1:
                        flag = 0;
                        setFPGAVideoData(fpga_videodata);
                        step += mission_7_1_attach_yellow_bridge(fpga_videodata);
                        break;
                    case 2:
                        if (flag == 0) {
                            default_watch(LEFT);
                            RobotSleep(1);
                            flag++;
                        }

                        step += default_set_center1(fpga_videodata, LEFT, 60, HEIGHT-11, BLACK);
                        break;
                    case 3:
                        step = 6;
                        break;
                    case 6:
                        step += mission_7_6_jump_hole();
                        flag = 0;
                        break;
                    case 7:
                        if (flag == 0) {
                            default_watch(LEFT);
                            RobotSleep(1);
                            flag++;
                        }
                        step += default_set_straight_and_center1(fpga_videodata, LEFT, 90, HEIGHT-11, BLACK);
                        break;
                    case 8:
                        mission += 1;
                        step = 0;
                        flag = 0;
                        break;
                    default:
                        break;
                }
                break;
            case 8: // MISSION 8: CREVASSE
                switch (step) {
                    case 0:
                        if (flag == 0)
                        {
                            default_watch(DOWN);
                            RobotSleep(1);
                            ++flag;
                        }
                        mission_8_1_watch_below(fpga_videodata, 10);

                        setFPGAVideoData(fpga_videodata);
                        step += mission_8_1_wait_front_of_crevasse(fpga_videodata);
                        break;
                    case 1:
                        if (flag == 1)
                        {
                            default_watch(LEFT);
                            RobotSleep(1);
                            ++flag;
                        }
                        step += default_set_straight_and_center1(fpga_videodata, LEFT, 60, HEIGHT-11, BLACK);
                        break;
                    case 2:
                        step += mission_8_2_escape_crevasse();
                        flag = 0;
                        break;
                    case 3:
                        if (flag == 0)
                        {
                            default_watch(LEFT);
                            RobotSleep(1);
                        }
                        step += default_set_straight_and_center1(fpga_videodata, LEFT, WIDTH>>1, HEIGHT-11, BLACK);
                        break;
                    default:
                        default_watch(UP);
                        flag = 0;
                        ++mission;
                        break;
                }
                break;
            case 9: // MISSION 9: LAST BARRICADE
                switch (step) {
                    case 0:
                        step += 1;
//                        setFPGAVideoData(fpga_videodata);
//                        mission_9_1_go_front(fpga_videodata);
//                        step += 1;
                        break;
                    case 1:
                        CHECK_INIT(UP);
                        RobotSleep(1);
                        step += 1;
                        break;
                    case 2:
                        setFPGAVideoData(fpga_videodata);
                        step += mission_9_1_wait_yellow_barricade(fpga_videodata);
                        break;
                    case 3:
                        setFPGAVideoData(fpga_videodata);
                        step += mission_9_2_end_yellow_barricade(fpga_videodata);
                        break;
                    case 4:
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
                    case 1:
                        if (flag++ == 0) {
                            default_watch(RIGHT);
                            RobotSleep(1);
                            flag++;
                        }

                        if (default_set_straight_and_center1(fpga_videodata, RIGHT, WIDTH>>1, HEIGHT-11, BLACK) &&
                            default_set_straight_and_center1(fpga_videodata, RIGHT, WIDTH>>1, HEIGHT-11, BLACK)) {
                            // 앞에꺼는 다음이 옐로우 뒤에는 다음이 초록다리

                            flag = 0;
                            step += 1;

                            CHECK_INIT(DOWN);
                            ACTION_WALK(FAST, DOWN, 6);
                            RobotSleep(1);
                            break;
                        }
                        break;
                    case 2:
                        step = 0;
                        mission = nextMission;
                        nextMission = 0;
                        flag = 0;
                        break;
                    default:
                        break;
                }
                break;
            default:
                missionFinished = 1;
                break;
        }
#ifdef DEBUG
        printf("[END]\tMISSION[%d]: STEP [%d]\n", mission, step);
#endif

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

    RobotSleep(7);

    CHECK_INIT(UP);

    return 0;
}

void destroy_huro(U16 *buf) {
    // free fpga video data
    free(buf);

    // graphic close
    close_graphic();

    // UART
    uart_close();

}
