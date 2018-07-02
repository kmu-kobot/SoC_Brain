cmake_minimum_required(VERSION 3.9)
project(SoC_App C)

set(CMAKE_C_STANDARD 99)

add_executable(
        SoC_App
        main.c
        huro.c
        huro.h
        library/amazon2_sdk.h
        library/graphic_api.c
        library/graphic_api.h
        library/robot_protocol.c
        library/robot_protocol.h
        library/uart_api.c
        library/uart_api.h
)