#include <msp430.h> 
#include <TW_MAX7219_DotMatrix.h>
#include <TW_m430g2553_MiscApps.h>
#include <TW_I2C.h>

unsigned char ReadData[7];//ss mm hh DoW DD MM YY
unsigned char SetRtcData[7] = {0x00, 0x37, 0x02, 0x01, 0x05, 0x08, 0x24};//ss mm hh DoW DD MM YY
volatile unsigned char tempData[2][8];

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P1OUT &= ~(BIT2 + BIT4);
	P1DIR |= BIT2 + BIT4;
	P2OUT &= ~(BIT0 + BIT1 + BIT2 + BIT3);
	P2DIR |= (BIT0 + BIT1 + BIT2 + BIT3);
	CalibrateDco(16);
	BasicInitUSCI(1); // Master mode I2C

//	I2CSLV_Save_Burst(0x68, 0, SetRtcData, 7);
//	while(1);
	int var=0, shiftCalc=0 ;
	  while(var<sizeof(MatrixSetup)/sizeof(MatrixSetup[0])) {


	      sendData(&MatrixSetup[var],1);
	      sendData(&MatrixSetup[var],2);
	      sendData(&MatrixSetup[var],3);
	      sendData(&MatrixSetup[var],4);

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
	    tempData[0][2] |= 1;
	    tempData[0][5] |= 1;
	    dispx[2] = (const unsigned char*)&tempData[0][0];
	    shiftCalc = ReadData[1] >> 4;
        for (var = 0; var < 8; ++var) {
            tempData[1][var] = disp1[shiftCalc][var];
        }
        tempData[1][2] |= 0x80;
        tempData[1][5] |= 0x80;
        dispx[1] = (const unsigned char*)&tempData[1][0];
        dispx[0] = &disp1[ReadData[1] & 0x0F][0];

	    dotMatrixSendRowWise(dispx, 4, __SIMPLE_SCROLL_DOWN__, __SCROLL_SFAST__);
           __delay_cycles(16000000);

        dispx[3] = &disp1[36][0];
        dispx[2] = &disp1[36][0];
        dispx[1] = &disp1[36][0];
        dispx[0] = &disp1[36][0];

        dotMatrixSendRowWise(dispx, 4, __SIMPLE_SCROLL_DOWN__, __SCROLL_SFAST__);

        dispx[3] = &disp1[ReadData[0] >> 4][0];
        dispx[2] = &disp1[ReadData[0] & 0x0F][0];

        dotMatrixSendRowWise(dispx, 2, __SIMPLE_SCROLL_DOWN__, __SCROLL_SFAST__);
           __delay_cycles(16000000);

        dispx[3] = &disp1[36][0];
        dispx[2] = &disp1[36][0];
        dispx[1] = &disp1[36][0];
        dispx[0] = &disp1[36][0];

        dotMatrixSendRowWise(dispx, 4, __SIMPLE_SCROLL_DOWN__, __SCROLL_SFAST__);

        dispx[3] = &disp1[ReadData[4] >> 4][0];
        dispx[2] = &disp1[ReadData[4] & 0x0F][0];
        dispx[1] = &disp1[ReadData[5] >> 4][0];
        dispx[0] = &disp1[ReadData[5] & 0x0F][0];

        dotMatrixSendRowWise(dispx, 4, __SIMPLE_SCROLL_DOWN__, __SCROLL_SFAST__);
           __delay_cycles(16000000);

        dispx[3] = &disp1[36][0];
        dispx[2] = &disp1[36][0];
        dispx[1] = &disp1[36][0];
        dispx[0] = &disp1[36][0];

        dotMatrixSendRowWise(dispx, 4, __SIMPLE_SCROLL_DOWN__, __SCROLL_SFAST__);

        dispx[3] = &disp1[2][0];
        dispx[2] = &disp1[0][0];
        dispx[1] = &disp1[ReadData[6] >> 4][0];
        dispx[0] = &disp1[ReadData[6] & 0x0F][0];

        dotMatrixSendRowWise(dispx, 2, __SIMPLE_SCROLL_DOWN__, __SCROLL_SFAST__);
           __delay_cycles(16000000);

        dispx[3] = &disp1[36][0];
        dispx[2] = &disp1[36][0];
        dispx[1] = &disp1[36][0];
        dispx[0] = &disp1[36][0];

        dotMatrixSendRowWise(dispx, 4, __SIMPLE_SCROLL_DOWN__, __SCROLL_SFAST__);



//          }
	}
	return 0;
}
