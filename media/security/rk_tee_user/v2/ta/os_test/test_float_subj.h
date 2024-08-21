/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2015, Linaro Limited
 * All rights reserved.
 */

#ifndef TEST_FLOAT_SUBJ_H
#define TEST_FLOAT_SUBJ_H

double test_float_dadd(double a, double b);
double test_float_ddiv(double a, double b);
double test_float_dmul(double a, double b);
double test_float_drsub(double a, double b);
double test_float_dsub(double a, double b);
int test_float_dcmpeq(double a, double b);
int test_float_dcmplt(double a, double b);
int test_float_dcmple(double a, double b);
int test_float_dcmpge(double a, double b);
int test_float_dcmpgt(double a, double b);
float test_float_fadd(float a, float b);
float test_float_fdiv(float a, float b);
float test_float_fmul(float a, float b);
float test_float_frsub(float a, float b);
float test_float_fsub(float a, float b);
int test_float_fcmpeq(float a, float b);
int test_float_fcmplt(float a, float b);
int test_float_fcmple(float a, float b);
int test_float_fcmpge(float a, float b);
int test_float_fcmpgt(float a, float b);
int test_float_d2iz(double a);
unsigned test_float_d2uiz(double a);
long long test_float_d2lz(double a);
unsigned long long test_float_d2ulz(double a);
int test_float_f2iz(float a);
unsigned test_float_f2uiz(float a);
long long test_float_f2lz(float a);
unsigned long long test_float_f2ulz(float a);
float test_float_d2f(double a);
double test_float_f2d(float a);
double test_float_i2d(int a);
double test_float_ui2d(unsigned a);
double test_float_l2d(long long a);
double test_float_ul2d(unsigned long long a);
float test_float_i2f(int a);
float test_float_ui2f(unsigned a);
float test_float_l2f(long long a);
float test_float_ul2f(unsigned long long a);

#endif /*TEST_FLOAT_SUBJ_H*/
