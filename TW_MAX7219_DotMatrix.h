/*
 * TW_MAX7219_DotMatrix.h
 *
 *  Created on: 20-Jul-2024
 *      Author: admin
 */

#ifndef TW_MAX7219_DOTMATRIX_H_
#define TW_MAX7219_DOTMATRIX_H_

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#define     __MAX_NO_DISPLAYS__     4

extern const unsigned char disp1[37][8];
extern unsigned short MatrixSetup[7];
extern const unsigned char* dispx[__MAX_NO_DISPLAYS__];

//Functions
extern  void    dotMatrixSendRowWise(const unsigned char* value[__MAX_NO_DISPLAYS__], uint8_t maxDisplays);
extern  void dotMatrixSend(const unsigned char* value, uint8_t displayNumber);
extern  void sendData(unsigned short *data, uint8_t dispNo);
#endif /* TW_MAX7219_DOTMATRIX_H_ */
