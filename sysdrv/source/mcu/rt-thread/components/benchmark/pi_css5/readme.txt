PI(= 3.1415926535897932384626...) Calculation Program


    ver. LG1.1.2-MP1.5.2a Nov. 1999


Files:
    fftsg_h.c   : FFT Package - split-radix   - use no work areas
    pi_fftcs.c  : PI Calculation Program - memory save version
                  -- use rdft() in "fft*g_h.c"
	readme.txt  : this file
    Makefile    : - for gcc modify for your own compiler.

To Compile:
    Check macros in "pi_fftcs.c" and modify if necessary.
    DBL_ERROR_MARGIN is very impotant parameter.
    If DBL_ERROR_MARGIN is very small then efficiency will be bad.
    If DBL_ERROR_MARGIN >= 0.5 then it may calculate a wrong result.

Example Compilation:
    GNU:
        gcc -O -funroll-loops -fomit-frame-pointer pi_fftcs.c fftsg_h.c -lm -o pi_css5
    SUN:
        cc -fast pi_fftcs.c fftsg_h.c -lm -o pi_css5
    HP:
		aCC -fast pi_fftcs.c fftsg_h.c -lm -o pi_css5
    Microsoft:
        cl -O2 pi_fftcs.c fftsg_h.c -o pi_css5

Relationship between Number of Digits and FFT Length:
    ndigit = nfft*log_10(R), R >= 10000 or 1000
    R is a radix of multiple-precision format.
    R depends on DBL_ERROR_MARGIN and 
    FFT+machine+compiler's tolerance.

Number of Floating Point Operations:
    pi_fftcs.c + fftsg_h.c:
        42*nfft*(log_2(nfft))^2           [Operations]

Memory Use:
    pi_fftcs.c:
        nfft*(6*sizeof(short int)+3*sizeof(double))   [Bytes]

AGM Algorithm:
  ---- a formula based on the AGM (Arithmetic-Geometric Mean) ----
    c = sqrt(0.125);
    a = 1 + 3 * c;
    b = sqrt(a);
    e = b - 0.625;
    b = 2 * b;
    c = e - c;
    a = a + e;
    npow = 4;
    do {
        npow = 2 * npow;
        e = (a + b) / 2;
        b = sqrt(a * b);
        e = e - b;
        b = 2 * b;
        c = c - e;
        a = e + b;
    } while (e > SQRT_SQRT_EPSILON);
    e = e * e / 4;
    a = a + b;
    pi = (a * a - e - e / 2) / (a * c - e) / npow;
  ---- modification ----
    This is a modified version of Gauss-Legendre formula
    (by T.Ooura). It is faster than original version.

Reference:
    1. E.Salamin, 
       Computation of PI Using Arithmetic-Geometric Mean, 
       Mathematics of Computation, Vol.30 1976.
    2. R.P.Brent, 
       Fast Multiple-Precision Evaluation of Elementary Functions, 
       J. ACM 23 1976.
    3. D.Takahasi, Y.Kanada, 
       Calculation of PI to 51.5 Billion Decimal Digits on 
       Distributed Memoriy Parallel Processors, 
       Transactions of Information Processing Society of Japan, 
       Vol.39 No.7 1998.
    4. T.Ooura, 
       Improvement of the PI Calculation Algorithm and 
       Implementation of Fast Multiple-Precision Computation, 
       Information Processing Society of Japan SIG Notes, 
       98-HPC-74, 1998.


Copyright
    source files:
        Copyright(C) 1999 Takuya OOURA
        Email: ooura@mmm.t.u-tokyo.ac.jp
        URL:   http://momonga.t.u-tokyo.ac.jp/~ooura/fft.html
        You may use, copy, modify this code for any purpose and 
        without fee. You may distribute this ORIGINAL package.

