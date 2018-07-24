//
// Created by Gihyeon Yang on 2018. 7. 25..
//

#ifndef SET_CONSTANT_FUNC_H

#include "./library/amazon2_sdk.h"
#include "./library/robot_action.h"
#include "stdio.h"

#define SET_CONSTANT_FUNC_H

#define CASE_0_DEFAULT_LEFT_RANGE 60
#define CASE_0_DEFAULT_RIGHT_RANGE 60
#define CASE_0_DEFAULT_RANGE_ERROR 5
#define CASE_0_DEFAULT_LEFT_SLOPE 60
#define CASE_0_DEFAULT_RIGHT_SLOPE 60

void CASE_0_SET_CENTER(U16 *image, int dir);

void CASE_0_SET_STRAIGHT(U16 *image, int dir);

#define CASE_1_UPPER 0
#define CASE_1_LOWER 110
#define CASE_1_RATIO 10

void CASE_1_CHECK_FRONT_OF_BARRICADE(U16 *image);

#define CASE_2_0_DETECTION 15
#define CASE_2_1_CENTER 5

void CASE_2_CHECK_RED_BRIDGE(U16 *image);

void CASE_2_SET_CENTER(U16 *image);

#define CASE_4_0_LENGTH 13
#define CASE_4_0_LENGTH_ERROR 7
#define CASE_4_1_NON_BLACK_LINE 3

void CASE_4_CHECK_LENGTH(U16 *image);

void CASE_4_CHECK_NON_BLACK(U16 *image);

#define CASE_5_0_DETECTION 15
#define CASE_5_1_CENTER 5
#define CASE_5_2_POINT_Y_1 30
#define CASE_5_2_POINT_Y_2 50
#define CASE_5_2_SLOPE_RANGE 7
#define CASE_5_2_SLOPE 1
#define CASE_5_4_POINT_X_1 40
#define CASE_5_4_POINT_X_2 140
#define CASE_5_4_RANGE 10
#define CASE_5_4_SLOPE 4
#define CASE_5_5_POINT_X_1 40
#define CASE_5_5_POINT_X_2 140
#define CASE_5_5_THRESHOLDS 60
#define CASE_5_5_LENGTH 76

void CASE_5_CHECK_BLACK_BLOCK(U16 *image);

void CASE_5_CHECK_CENTER_ON_GREEN_BRIDGE(U16 *image);

void CASE_5_CHECK_STRAIGHT_ON_GREEN_BRIDGE(U16 *image);

void CASE_5_CHECK_BLACK_LINE_ONLY_ONE(U16 *image);

void CASE_5_CHECK_STRAIGHT_ON_BLACK_LINE(U16 *image);

void CASE_5_CHECK_LENGTH_ON_BLACK_LINE(U16 *image);


#define CASE_7_0_CENTER 5
#define CASE_7_1_POINT_X_1 100
#define CASE_7_1_POINT_X_2 140
#define CASE_7_1_CENTER 5
#define CASE_7_2_POINT_X_1 100
#define CASE_7_2_POINT_X_2 140
#define CASE_7_2_SLOPE 5
#define CASE_7_3_POINT_X_1 70
#define CASE_7_3_POINT_X_2 110
#define CASE_7_3_LENGTH 43
#define CASE_7_4_DETETCH_RATIO 15


void CASE_7_SET_CENTER(U16 *image);

void CASE_7_SET_CENTER_ON_YELLOW_BRIDGE(U16 *image);

void CASE_7_SET_STRAIGHT_ON_YELLOW_BRIDGE(U16 *image);

void CASE_7_CHECK_RANGE(U16 *image);

void CASE_7_CHECK_YELLOW(U16 *image);

#define CASE_8_RATIO 4

void CASE_8_CATCH_BLUE_GATE(U16 *image);


#endif //SET_CONSTANT_FUNC_H
