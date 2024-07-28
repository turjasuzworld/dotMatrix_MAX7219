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

#endif /* TW_M430G2553_MISCAPPS_H_*/
