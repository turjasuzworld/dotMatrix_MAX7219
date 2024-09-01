#include <msp430.h> 
#include <TW_MAX7219_DotMatrix.h>
#include <TW_m430g2553_MiscApps.h>
#include <TW_I2C.h>

unsigned char ReadData[7];//ss mm hh DoW DD MM YY
unsigned char SetRtcData[7] = {0x00, 0x37, 0x02, 0x01, 0x05, 0x08, 0x24};//ss mm hh DoW DD MM YY
const   unsigned  char  dow[7][3] = {"MON","TUE","WED","THU","FRI","SAT","SUN"};
const   unsigned  char  months[12][3] = {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
volatile unsigned char tempData[2][8];

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
//	volatile unsigned char* p;
//	p = (volatile unsigned char*)0x22;//P1DIR
//	*p = 0x01;
//	p = (volatile unsigned char*)0x21;
//	*p = 0x01;
//	*p &= ~0x01;
	P1OUT &= ~(BIT2 + BIT4);
	P1DIR |= BIT2 + BIT4;
	P2OUT &= ~(BIT0 + BIT1 + BIT2 + BIT3);
	P2DIR |= (BIT0 + BIT1 + BIT2 + BIT3);
	CalibrateDco(16);
	BasicInitUSCI(1); // Master mode I2C

//	I2CSLV_Save_Burst(0x68, 0, SetRtcData, 7);
//	while(1);
	int var=0, dispCtr, shiftCalc=0 ;
	  while(var<sizeof(MatrixSetup)/sizeof(MatrixSetup[0])) {
	      dispCtr=1;
	      while(dispCtr <= __MAX_NO_DISPLAYS__) {
	          sendData(&MatrixSetup[var],dispCtr);
	          dispCtr++;
	      }
	      var++;
	      __delay_cycles(160);

	  }
	while(1) {
	    //I2CSLV_RRead_Burst(0x68, 0, 7, ReadData);
	    ReadFromDS1307MultiBytes(0, 7, ReadData);
	    dispx[3] = &disp1[ReadData[2] >> 4][0];
	    for (var = 0; var < 8; ++var) {
	        tempData[0][var] = disp1[ReadData[2] & 0x0F][var];
        }
	    tempData[0][1] |= 1;
	    tempData[0][6] |= 1;
	    dispx[2] = (const unsigned char*)&tempData[0][0];
	    shiftCalc = ReadData[1] >> 4;
        for (var = 0; var < 8; ++var) {
            tempData[1][var] = disp1[shiftCalc][var];
        }
        tempData[1][1] |= 0x80;
        tempData[1][6] |= 0x80;
        dispx[1] = (const unsigned char*)&tempData[1][0];
        dispx[0] = &disp1[ReadData[1] & 0x0F][0];

	    dotMatrixSendRowWise(dispx, 4, __SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);
           __delay_cycles(16000000*5);

           interDisplayBlanking(__SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);

        dispx[3] = &disp1[ReadData[0] >> 4][0];
        dispx[2] = &disp1[ReadData[0] & 0x0F][0];

        dotMatrixSendRowWise(dispx, 2, __SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);
           __delay_cycles(8000000);
        interDisplayBlanking(__SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);

        dispx[3] = &disp1[36][0];
        dispx[2] = &disp1[ReadData[4] >> 4][0];
        dispx[1] = &disp1[ReadData[4] & 0x0F][0];
        dispx[0] = &disp1[36][0];

        dotMatrixSendRowWise(dispx, 4, __SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);
           __delay_cycles(10000000);
        interDisplayBlanking(__SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);

        // MONTH //
        dispx[3] = &disp1[36][0];
        dispx[2] = &disp1[months[(10*(ReadData[5]>>4)+(ReadData[5]& 0x0F))-(unsigned int)1][0] - (unsigned int)55][0];
        dispx[1] = &disp1[months[(10*(ReadData[5]>>4)+(ReadData[5]& 0x0F))-(unsigned int)1][1] - (unsigned int)55][0];
        dispx[0] = &disp1[months[(10*(ReadData[5]>>4)+(ReadData[5]& 0x0F))-(unsigned int)1][2] - (unsigned int)55][0];

        dotMatrixSendRowWise(dispx, 4, __SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);
           __delay_cycles(10000000);

           interDisplayBlanking(__SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);

        // Day of Week //
        dispx[3] = &disp1[36][0];
        dispx[2] = &disp1[dow[(ReadData[3]& 0x0F)-(unsigned int)1][0] - (unsigned int)55][0];
        dispx[1] = &disp1[dow[(ReadData[3]& 0x0F)-(unsigned int)1][1] - (unsigned int)55][0];
        dispx[0] = &disp1[dow[(ReadData[3]& 0x0F)-(unsigned int)1][2] - (unsigned int)55][0];

        dotMatrixSendRowWise(dispx, 4, __SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);
           __delay_cycles(10000000);

           interDisplayBlanking(__SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);


        dispx[3] = &disp1[2][0];
        dispx[2] = &disp1[0][0];
        dispx[1] = &disp1[ReadData[6] >> 4][0];
        dispx[0] = &disp1[ReadData[6] & 0x0F][0];

        dotMatrixSendRowWise(dispx, 2, __SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);
           __delay_cycles(10000000);

           interDisplayBlanking(__SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);
//          }
	}
	return 0;
}
