/*
 * This header is for multiple general purpose functions that
 *  are needed for low memory, cheap MSP430G2xx series
 */

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
//#include <math.h>
#ifndef __MSP430G2553
#include <math.h>
#endif
// Macros

// Global Vars
extern  int gAmbientLux;

//Global Functions
extern void CalibrateDco(uint8_t freq);
extern  int TW_CalcDecFrmTwozComp(unsigned char*);
extern void calculate_light_intensity(uint16_t , int* );

//Others
