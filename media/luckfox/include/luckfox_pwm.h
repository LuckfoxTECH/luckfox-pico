/*****************************************************************************
* | File        :   luckfox_pwm.h
* | Author      :   luckfox team
* | Function    :   Pulse width modulation
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2023-08-01
* | Info        :   Basic version
*
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#ifndef __LUCKFOX_PWM_H__
#define __LUCKFOX_PWM_H__

#include <stdbool.h>
#include <stdint.h>

/**********************************************/
enum pwm_polarity {
        PWM_POLARITY_NORMAL,
        PWM_POLARITY_INVERSED,
};

int luckfox_pwm_export(uint32_t pwm);
int luckfox_pwm_unexport(uint32_t pwm);
int luckfox_pwm_set_period(uint32_t pwm, uint32_t period);
int luckfox_pwm_get_period(uint32_t pwm);
int luckfox_pwm_set_duty(uint32_t pwm, uint32_t duty);
int luckfox_pwm_get_duty(uint32_t pwm);
int luckfox_pwm_set_polarity(uint32_t pwm, enum pwm_polarity polarity);
int luckfox_pwm_get_polarity(uint32_t pwm);
int luckfox_pwm_set_enable(uint32_t pwm, bool enabled);
int luckfox_pwm_get_enable(uint32_t pwm);
int luckfox_pwm_init(uint32_t pwm, uint32_t period, uint32_t duty,
		   enum pwm_polarity polarity);
int luckfox_pwm_deinit(uint32_t pwm);
#endif //__LUCKFOX_PWM_H__
