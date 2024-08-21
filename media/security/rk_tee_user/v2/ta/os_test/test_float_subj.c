// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2015, Linaro Limited
 * All rights reserved.
 */

#include "test_float_subj.h"

double test_float_dadd(double a, double b)
{
	return a + b;
}

double test_float_ddiv(double n, double d)
{
	return n / d;
}

double test_float_dmul(double a, double b)
{
	return a * b;
}

double test_float_drsub(double a, double b)
{
	return b - a;
}

double test_float_dsub(double a, double b)
{
	return a - b;
}

int test_float_dcmpeq(double a, double b)
{
	return a == b;
}

int test_float_dcmplt(double a, double b)
{
	return a < b;
}

int test_float_dcmple(double a, double b)
{
	return a <= b;
}

int test_float_dcmpge(double a, double b)
{
	return a >= b;
}

int test_float_dcmpgt(double a, double b)
{
	return a > b;
}

float test_float_fadd(float a, float b)
{
	return a + b;
}

float test_float_fdiv(float n, float d)
{
	return n / d;
}

float test_float_fmul(float a, float b)
{
	return a * b;
}

float test_float_frsub(float a, float b)
{
	return b - a;
}

float test_float_fsub(float a, float b)
{
	return a - b;
}

int test_float_fcmpeq(float a, float b)
{
	return a == b;
}

int test_float_fcmplt(float a, float b)
{
	return a < b;
}

int test_float_fcmple(float a, float b)
{
	return a <= b;
}

int test_float_fcmpge(float a, float b)
{
	return a >= b;
}

int test_float_fcmpgt(float a, float b)
{
	return a > b;
}

int test_float_d2iz(double a)
{
	return a;
}

unsigned test_float_d2uiz(double a)
{
	return a;
}

long long test_float_d2lz(double a)
{
	return a;
}

unsigned long long test_float_d2ulz(double a)
{
	return a;
}

int test_float_f2iz(float a)
{
	return a;
}

unsigned test_float_f2uiz(float a)
{
	return a;
}

long long test_float_f2lz(float a)
{
	return a;
}

unsigned long long test_float_f2ulz(float a)
{
	return a;
}

float test_float_d2f(double a)
{
	return a;
}

double test_float_f2d(float a)
{
	return a;
}

double test_float_i2d(int a)
{
	return a;
}

double test_float_ui2d(unsigned a)
{
	return a;
}

double test_float_l2d(long long a)
{
	return a;
}

double test_float_ul2d(unsigned long long a)
{
	return a;
}

float test_float_i2f(int a)
{
	return a;
}

float test_float_ui2f(unsigned a)
{
	return a;
}

float test_float_l2f(long long a)
{
	return a;
}

float test_float_ul2f(unsigned long long a)
{
	return a;
}
