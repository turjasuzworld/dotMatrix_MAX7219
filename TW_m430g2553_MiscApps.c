/*
 * TW_m430g2553_MiscApps.c
 *
 *  Created on: 20-Jul-2024
 *      Author: Turjasu
 */
#ifndef TW_M430G2553_MISCAPPS_H_
#define TW_M430G2553_MISCAPPS_H_
#include <TW_m430g2553_MiscApps.h>

void CalibrateDco(uint8_t freq) {

    switch (freq) {

        case 8:
            if (CALBC1_8MHZ==0xFF)                    // If calibration constant erased
            {
              while(1);                               // do not load, trap CPU!!
            }
            DCOCTL = 0;                               // Select lowest DCOx and MODx settings
            BCSCTL1 = CALBC1_8MHZ;                    // Set range
            DCOCTL = CALDCO_8MHZ;                     // Set DCO step + modulation */

        break;

        case 12:
            if (CALBC1_12MHZ==0xFF)                   // If calibration constant erased
            {
              while(1);                               // do not load, trap CPU!!
            }
            DCOCTL = 0;                               // Select lowest DCOx and MODx settings
            BCSCTL1 = CALBC1_12MHZ;                   // Set range
            DCOCTL = CALDCO_12MHZ;                    // Set DCO step + modulation*/

        break;

        case 16:
            if (CALBC1_16MHZ==0xFF)                   // If calibration constant erased
            {
              while(1);                               // do not load, trap CPU!!
            }
            DCOCTL = 0;                               // Select lowest DCOx and MODx settings
            BCSCTL1 = CALBC1_16MHZ;                   // Set range
            DCOCTL = CALDCO_16MHZ;                    // Set DCO step + modulation*/

        break;

        case 1:
        default:
            DCOCTL = 0;                               // Select lowest DCOx and MODx settings
            BCSCTL1 = CALBC1_1MHZ;                    // Set range
            DCOCTL = CALDCO_1MHZ;                     // Set DCO step + modulation */
            break;
    }

}

/*
 * Function to convert 2's complement to equivalent decimal
 */
int TW_CalcDecFrmTwozComp(unsigned char* regPtr) {
    unsigned char regVal = *regPtr;
    static int retVal;
#ifndef __MSP430G2553
    if(regVal & 0x80) {
        retVal = ((regVal & 0x01)*(pow(2, 0))) +
                 ((regVal & 0x02)*(pow(2, 1))) +
                 ((regVal & 0x04)*(pow(2, 2))) +
                 ((regVal & 0x08)*(pow(2, 3))) +
                 ((regVal & 0x10)*(pow(2, 4))) +
                 ((regVal & 0x20)*(pow(2, 5))) +
                 ((regVal & 0x40)*(pow(2, 6))) -
                 ((regVal & 0x80)*(pow(2, 7))) - 1;
    } else {
        retVal = ((regVal & 0x01)*(pow(2, 0))) +
                 ((regVal & 0x02)*(pow(2, 1))) +
                 ((regVal & 0x04)*(pow(2, 2))) +
                 ((regVal & 0x08)*(pow(2, 3))) +
                 ((regVal & 0x10)*(pow(2, 4))) +
                 ((regVal & 0x20)*(pow(2, 5))) +
                 ((regVal & 0x40)*(pow(2, 6)));
    }
#else
    if(regVal & 0x80) {
        regVal &= 0x7F; // mask out the sign bit
        regVal ^= 0x7F; // invert the bits
        regVal += 1;    // Add 1
        retVal = ((int)(regVal))*(-1);
    } else {
        retVal = (int)(regVal);
    }

#endif
    return retVal;
}

#endif /* TW_M430G2553_MISCAPPS_H_*/
