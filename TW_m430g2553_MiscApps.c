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

int calculate_light_intensity(uint16_t adc_value) {
#ifdef _TI_MATH_H_
    // ADC reference voltage
    float Vref = 3.3;
    // Known resistor value in ohms
    float R_fixed = 10000.0;

    // Convert ADC value to voltage
    float voltage = (adc_value / 1023.0) * Vref;

    // Calculate LDR resistance using voltage divider formula
    float R_ldr = (voltage * R_fixed) / (Vref - voltage + 0.0001); // small offset to prevent divide-by-zero

    // Approximate formula to convert LDR resistance to light intensity in lux
    // This formula is based on typical LDR characteristics (e.g., at 10k ohms ≈ 100 lux)
    // lux = A * (R_ldr)^B; using typical values A ≈ 500, B ≈ -1.4
    float lux = 500 * pow(R_ldr, -1.4);

    // Return as integer
    return (int)lux;
#else
    const uint16_t adc_to_lux[16] = {5, 7, 10, 15, 25, 40, 60, 90,130, 180, 250, 350, 500, 650, 800, 1000};
    uint8_t index = adc_value / 64; // 0 to 16 range
    if (index > 15) index = 15;
    return adc_to_lux[index];

#endif
}

#endif /* TW_M430G2553_MISCAPPS_H_*/
