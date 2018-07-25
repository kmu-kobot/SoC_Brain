//
// Created by Gihyeon Yang on 2018. 7. 25..
//

#include "set_constant.h"

void set_constant(void) {
    init_huro();
    U16 *fpga_videodata = (U16 *) malloc(WIDTH * HEIGHT * 2);
    U8 mission = 0, step = 0, user_command;

    hint(-1);

    int i = 1;

    do {
        user_command = (U8) getchar();

        setFPGAVideoData(fpga_videodata);

        switch (user_command) {
            case 'h':
                hint(-1);
                break;
            case 'm':
                hint(-2);

                mission = (U8) (getchar() - '0');

                hint(mission);

                step = (U8) (getchar() - '0');

                if (
                        (mission == 0 && (step == 0 || step == 2)) ||
                        (mission == 7 && (step == 1 || step == 2))
                        ) {
                    ACTION_INIT(MIDDLE, LEFT);
                } else if (
                        (mission == 0 && (step == 1 || step == 3)) ||
                        (mission == 8)
                        ) {
                    ACTION_INIT(MIDDLE, RIGHT);
                } else if (
                        (mission == 1 && step == 0) ||
                        (mission == 2 && step == 1) ||
                        (mission == 4) ||
                        (mission == 7 && step == 0)
                        ) {
                    ACTION_INIT(MIDDLE, OBLIQUE);
                } else if (
                        (mission == 2 && step == 0) ||
                        (mission == 5) ||
                        (mission == 7 && step == 3 || step == 4)
                        ) {
                    ACTION_INIT(MIDDLE, DOWN);
                }

                break;
            case 'v':
                switch (mission) {
                    case 0:
                        switch (step) {
                            case 0:
                                printf("\t0- TURN LEFT / SET CENTER\n");
                                CASE_0_SET_CENTER(fpga_videodata, 0);
                                break;
                            case 1:
                                printf("\t1- TURN RIGHT / SET CENTER\n");
                                CASE_0_SET_CENTER(fpga_videodata, 1);
                                break;
                            case 2:
                                printf("\t2- TURN LEFT / SET STRAIGHT\n");
                                CASE_0_SET_STRAIGHT(fpga_videodata, 0);
                                break;
                            case 3:
                                printf("\t3- TURN RIGHT / SET STRAIGHT\n");
                                CASE_0_SET_STRAIGHT(fpga_videodata, 1);
                                break;
                            default:;
                        }
                        break;
                    case 1:
                        printf("\t0- OBLIQUE / CHECK FRONT OF BARRICADE\n");
                        CASE_1_CHECK_FRONT_OF_BARRICADE(fpga_videodata);
                        break;
                    case 2:
                        switch (step) {
                            case 0:
                                printf("\t0- DOWN / CHECK RED BRIDGE\n");
                                CASE_2_CHECK_RED_BRIDGE(fpga_videodata);
                                break;
                            case 1:
                                printf("\t1- OBLIQUE / SET CENTER\n");
                                CASE_2_SET_CENTER(fpga_videodata);
                                break;
                            default:;
                        }
                        break;
                    case 3:
                        break;
                    case 4:
                        switch (step) {
                            case 0:
                                printf("\t0- OBLIQUE / CHECK LENGTH\n");
                                CASE_4_CHECK_LENGTH(fpga_videodata);
                                break;
                            case 1:
                                printf("\t1- OBLIQUE / CHECK FRONT OF NON BLACK LINE\n");
                                CASE_4_CHECK_NON_BLACK(fpga_videodata);
                                break;
                            default:;
                        }
                        break;
                    case 5:
                        switch (step) {
                            case 0:
                                printf("\t0- DOWN / CHECK BLACK BLOCK\n");
                                CASE_5_CHECK_BLACK_BLOCK(fpga_videodata);
                                break;
                            case 1:
                                printf("\t1- DOWN / CHECK CENTER ON GREEN BRIDGE\n");
                                CASE_5_CHECK_CENTER_ON_GREEN_BRIDGE(fpga_videodata);
                                break;
                            case 2:
                                printf("\t2- DOWN / CHECK STRAIGHT ON GREEN BRIDGE\n");
                                CASE_5_CHECK_STRAIGHT_ON_GREEN_BRIDGE(fpga_videodata);
                                break;
                            case 3:
                                printf("\t3- DOWN / CHECK BLACK LINE ONLY ONE\n");
                                CASE_5_CHECK_BLACK_LINE_ONLY_ONE(fpga_videodata);
                                break;
                            case 4:
                                printf("\t4- DOWN / CHECK STRAIGHT ON BLACK LINE\n");
                                CASE_5_CHECK_STRAIGHT_ON_BLACK_LINE(fpga_videodata);
                                break;
                            case 5:
                                printf("\t5- DOWN / CHECK LENGTH ON BLACK LINE\n");
                                CASE_5_CHECK_LENGTH_ON_BLACK_LINE(fpga_videodata);
                                break;
                            default:;
                        }
                        break;
                    case 6:
                        break;
                    case 7:
                        switch (step) {
                            case 0:
                                printf("\t0- OBLIQUE / SET CENTER\n");
                                CASE_7_SET_CENTER(fpga_videodata);
                                break;
                            case 1:
                                printf("\t1- LEFT / SET CENTER ON YELLOW BRIDGE\n");
                                CASE_7_SET_CENTER_ON_YELLOW_BRIDGE(fpga_videodata);
                                break;
                            case 2:
                                printf("\t2- LEFT / SET STRAIGHT ON YELLOW BRIDGE\n");
                                CASE_7_SET_STRAIGHT_ON_YELLOW_BRIDGE(fpga_videodata);
                                break;
                            case 3:
                                printf("\t3- DOWN / CHECK RANGE\n");
                                CASE_7_CHECK_RANGE(fpga_videodata);
                                break;
                            case 4:
                                printf("\t4- DOWN / CHECK YELLOW\n");
                                CASE_7_CHECK_YELLOW(fpga_videodata);
                                break;
                            default:;
                        }
                        break;
                    case 8:
                        printf("\t0- RIGHT / CATCH BLUE GATE\n");
                        CASE_8_CATCH_BLUE_GATE(fpga_videodata);
                        break;
                    default:;
                }
                break;
            case 'p':
                printf("%d %d", mission, step);
                break;
            default:;
        }

        i++;
    } while (user_command != 'q');

    destroy_huro(fpga_videodata);
}

void hint(int m) {
    switch (m) {
        case -2:
            printf("0: (default) CENTER, STRAIGHT\n");
            printf("1: (0) READY PLAY, (1) YELLOW BARRICADE, (9) LAST BARRICADE\n");
            printf("2: (2) RED BRIDGE\n");
            printf("3: (3) AVOID BOMB\n");
            printf("4: (4) JUMP HURDLE\n");
            printf("5: (5) GREEN BRIDGE\n");
            printf("6: (7) KICK BALL\n");
            printf("7: (8) YELLOW HOLE BRIDGE\n");
            printf("8: (10) BLUE GATE\n");
            break;
        case -1:
            printf("v: validation\n");
            printf("m: mode step change\n");
            printf("h: hint\n");
            printf("p: print state\n");
            break;
        case 0:
            printf("\t0- TURN LEFT / SET CENTER\n");
            printf("\t1- TURN RIGHT / SET CENTER\n");
            printf("\t2- TURN LEFT / SET STRAIGHT\n");
            printf("\t3- TURN RIGHT / SET STRAIGHT\n");
            break;
        case 1:
            printf("\t0- OBLIQUE / CHECK FRONT OF BARRICADE\n");
            break;
        case 2:
            printf("\t0- DOWN / CHECK RED BRIDGE\n");
            printf("\t1- OBLIQUE / SET CENTER\n");
            break;
        case 3:
            break;
        case 4:
            printf("\t0- OBLIQUE / CHECK LENGTH\n");
            printf("\t1- OBLIQUE / CHECK FRONT OF NON BLACK LINE\n");
            break;
        case 5:
            printf("\t0- DOWN / CHECK BLACK BLOCK\n");
            printf("\t1- DOWN / CHECK CENTER ON GREEN BRIDGE\n");
            printf("\t2- DOWN / CHECK STRAIGHT ON GREEN BRIDGE\n");
            printf("\t3- DOWN / CHECK BLACK LINE ONLY ONE\n");
            printf("\t4- DOWN / CHECK STRAIGHT ON BLACK LINE\n");
            printf("\t5- DOWN / CHECK LENGTH ON BLACK LINE\n");
            break;
        case 6:
            break;
        case 7:
            printf("\t0- OBLIQUE / SET CENTER\n");
            printf("\t1- LEFT / SET CENTER ON YELLOW BRIDGE\n");
            printf("\t2- LEFT / SET STRAIGHT ON YELLOW BRIDGE\n");
            printf("\t3- DOWN / CHECK RANGE\n");
            printf("\t4- DOWN / CHECK YELLOW\n");
            break;
        case 8:
            printf("\t0- RIGHT / CATCH BLUE GATE\n");
            break;
        default:
            break;
    }
    printf("\n\n");
}