/*
 * TW_TimerA.h
 *
 *  Created on: Jun 27, 2018
 *      Author: Turjasu
 */

#ifndef TW_TIMERA_V2_H_
#define TW_TIMERA_V2_H_

#define         TA0_USE_SMCLK                   0x02
#define         TA0_USE_ACLK                    0x03
#define         TA0_USE_TACLK                   0x04
#define         TA0_USE_INCLK                   0x05
#define         STOP                    0
#define         UP                      1
#define         CONT                    2
#define         UP_DOWN                 3
#define         CLR_TIMER               true
#define         TA_CCR0_IE              true

#define         _DIV_CLK_BY_8           8

extern  uint8_t*    gPtr_TimerExpire;

extern int8_t _ElapseTimeCCR0_A0(unsigned char , unsigned char , unsigned char , uint16_t,  uint16_t,uint8_t* );

#endif /* TW_TIMERA_V2_H_ */
