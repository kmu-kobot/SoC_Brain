cmake_minimum_required(VERSION 3.9)
project(SoC_App C)

set(CMAKE_C_STANDARD 99)

add_executable(
        hsv
        main.c
        extractHSV.c
        library/amazon2_sdk.h
        library/graphic_api.c
        library/graphic_api.h
        library/robot_protocol.c
        library/robot_protocol.h
        library/uart_api.c
        library/uart_api.h
       mission/MISSION_1_YELLOW_BARRICADE.c mission/MISSION_1_YELLOW_BARRICADE.h mission/MISSION_2_RED_BRIDGE.c mission/MISSION_2_RED_BRIDGE.h library/robot_action.h mission/MISSION_0_READY_PLAY.c mission/MISSION_0_READY_PLAY.h library/robot_action.c mission/MISSION_4_JUMP_HURDLE.c mission/MISSION_4_JUMP_HURDLE.h mission/MISSION_5_GREEN_BRIDGE.c mission/MISSION_5_GREEN_BRIDGE.h)
