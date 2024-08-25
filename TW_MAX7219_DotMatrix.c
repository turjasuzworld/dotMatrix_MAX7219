#include <TW_MAX7219_DotMatrix.h>

const unsigned char disp1[37][8]={

{0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//0

{0x10,0x30,0x50,0x10,0x10,0x10,0x10,0x10},//1

{0x7E,0x2,0x2,0x7E,0x40,0x40,0x40,0x7E},//2

{0x3E,0x2,0x2,0x3E,0x2,0x2,0x3E,0x0},//3

{0x8,0x18,0x28,0x48,0xFE,0x8,0x8,0x8},//4

{0x3C,0x20,0x20,0x3C,0x4,0x4,0x3C,0x0},//5

{0x3C,0x20,0x20,0x3C,0x24,0x24,0x3C,0x0},//6

{0x3E,0x22,0x4,0x8,0x8,0x8,0x8,0x8},//7

{0x0,0x3E,0x22,0x22,0x3E,0x22,0x22,0x3E},//8

{0x3E,0x22,0x22,0x3E,0x2,0x2,0x2,0x3E},//9

{0x8,0x14,0x22,0x3E,0x22,0x22,0x22,0x22},//A

{0x3C,0x22,0x22,0x3E,0x22,0x22,0x3C,0x0},//B

{0x3C,0x40,0x40,0x40,0x40,0x40,0x3C,0x0},//C

{0x7C,0x42,0x42,0x42,0x42,0x42,0x7C,0x0},//D

{0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x7C},//E

{0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x40},//F

{0x3C,0x40,0x40,0x40,0x40,0x44,0x44,0x3C},//G

{0x44,0x44,0x44,0x7C,0x44,0x44,0x44,0x44},//H

{0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x7C},//I

{0x3C,0x8,0x8,0x8,0x8,0x8,0x48,0x30},//J

{0x0,0x24,0x28,0x30,0x20,0x30,0x28,0x24},//K

{0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7C},//L

{0x81,0xC3,0xA5,0x99,0x81,0x81,0x81,0x81},//M

{0x0,0x42,0x62,0x52,0x4A,0x46,0x42,0x0},//N

{0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//O

{0x3C,0x22,0x22,0x22,0x3C,0x20,0x20,0x20},//P

{0x1C,0x22,0x22,0x22,0x22,0x26,0x22,0x1D},//Q

{0x3C,0x22,0x22,0x22,0x3C,0x24,0x22,0x21},//R

{0x0,0x1E,0x20,0x20,0x3E,0x2,0x2,0x3C},//S

{0x0,0x3E,0x8,0x8,0x8,0x8,0x8,0x8},//T

{0x42,0x42,0x42,0x42,0x42,0x42,0x22,0x1C},//U

{0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18},//V

{0x0,0x49,0x49,0x49,0x49,0x2A,0x1C,0x0},//W

{0x0,0x41,0x22,0x14,0x8,0x14,0x22,0x41},//X

{0x41,0x22,0x14,0x8,0x8,0x8,0x8,0x8},//Y

{0x0,0x7F,0x2,0x4,0x8,0x10,0x20,0x7F},//Z

{0,0,0,0,0,0,0,0},//space fill

};
unsigned short MatrixSetup[7] = {0x0F01,0x0C01,0x0900,0x0A01,0x0B07,0x0C01,0x0F00};
const unsigned char* dispx[__MAX_NO_DISPLAYS__] = {&disp1[0][0],&disp1[1][0],&disp1[2][0],&disp1[3][0]};

void sendData(unsigned short *data, uint8_t dispNo) {

    uint8_t bitCnt;
    volatile uint8_t dispNumber;
    dispNumber = dispNo - 1;
    P2OUT &= ~(BIT0 + BIT1 + BIT2 + BIT3);
    for (bitCnt = 0; bitCnt < 16; bitCnt++) {
        if((*data)&(0x8000 >> bitCnt)) {
            P1OUT |= BIT2;
        }
        else {
            P1OUT &= ~BIT2;
        }
        P1OUT |= BIT4;
        __delay_cycles(1);
        P1OUT &= ~BIT4;
        __delay_cycles(1);
    }

    P2OUT |= (1<<dispNumber);
    __delay_cycles(1);
    P2OUT &= ~(1<<dispNumber);
    __delay_cycles(1);
}

void dotMatrixSend(const unsigned char* value, uint8_t displayNumber) {
    static unsigned short digitData = 0, zeros = 0;
    int var;
    for (var = 0; var < 8; ++var) {
        digitData = (var + 1) << 8; // create the upper byte
        digitData |= *value;
        value++; // invalidity not checked here
        switch (displayNumber) {
            case 1:
                sendData(&digitData, displayNumber);
                break;
            case 2:
                sendData(&digitData, displayNumber);
                sendData(&zeros, displayNumber);
                break;
            case 3:
                sendData(&digitData, displayNumber);
                sendData(&zeros, displayNumber);
                sendData(&zeros, displayNumber);
                break;
            case 4:
                sendData(&digitData, displayNumber);
                sendData(&zeros, displayNumber);
                sendData(&zeros, displayNumber);
                sendData(&zeros, displayNumber);
                break;
            case 5:
                sendData(&digitData, displayNumber);
                sendData(&zeros, displayNumber);
                sendData(&zeros, displayNumber);
                sendData(&zeros, displayNumber);
                sendData(&zeros, displayNumber);
                break;
            default:
                break;
        }

    }

}

static void loopingDisplay(uint8_t rowVal, const unsigned char* value[__MAX_NO_DISPLAYS__], e__displayStates displayEffect) {
    static unsigned short digitData = 0, zeros = 0;
    static uint8_t rowCnt , displNo, loopCtr;
    rowCnt = rowVal;
    // Using loops
    switch (displayEffect) {
        case __SIMPLE_SCROLL_DOWN__:
            for (loopCtr = 0; loopCtr < (rowCnt+1); ++loopCtr) {
                displNo = 0;
                while(displNo < __MAX_NO_DISPLAYS__) {
                    digitData = ((rowCnt+1)-loopCtr) << 8; // create the upper byte
                    digitData |= *(value[displNo] + (7 - loopCtr));
                    switch (displNo + 1) {
                    case 1:
                        sendData(&digitData, (displNo + 1));
                        break;
                    case 2:
                        sendData(&digitData, (displNo + 1));
                        sendData(&zeros, (displNo + 1));
                        break;
                    case 3:
                        sendData(&digitData, (displNo + 1));
                        sendData(&zeros, (displNo + 1));
                        sendData(&zeros, (displNo + 1));
                        break;
                    case 4:
                        sendData(&digitData, (displNo + 1));
                        sendData(&zeros, (displNo + 1));
                        sendData(&zeros, (displNo + 1));
                        sendData(&zeros, (displNo + 1));
                        break;
                    case 5:
                        sendData(&digitData, (displNo + 1));
                        sendData(&zeros, (displNo + 1));
                        sendData(&zeros, (displNo + 1));
                        sendData(&zeros, (displNo + 1));
                        sendData(&zeros, (displNo + 1));
                        break;
                    default:
                        break;
                    }
                    displNo++;
                }

            }

        break;

        case __SIMPLE_SCROLL_THROUGH_:// NOT IMPLEMENTED CORRECTLY //
            for (loopCtr = 0; loopCtr < rowCnt; ++loopCtr) {
                displNo = 0;
                while(displNo < __MAX_NO_DISPLAYS__) {
                    if(rowCnt > 7) {
                        digitData = (8-loopCtr) << 8; // create the upper byte
                        digitData |= *(value[displNo] + (7 - (loopCtr + 1)));
                    }
                    else {
                        digitData = ((rowCnt+1)-loopCtr) << 8; // create the upper byte
                        digitData |= *(value[displNo] + (7 - loopCtr));
                    }

                    switch (displNo + 1) {
                    case 1:
                        sendData(&digitData, (displNo + 1));
                        break;
                    case 2:
                        sendData(&digitData, (displNo + 1));
                        sendData(&zeros, (displNo + 1));
                        break;
                    case 3:
                        sendData(&digitData, (displNo + 1));
                        sendData(&zeros, (displNo + 1));
                        sendData(&zeros, (displNo + 1));
                        break;
                    case 4:
                        sendData(&digitData, (displNo + 1));
                        sendData(&zeros, (displNo + 1));
                        sendData(&zeros, (displNo + 1));
                        sendData(&zeros, (displNo + 1));
                        break;
                    case 5:
                        sendData(&digitData, (displNo + 1));
                        sendData(&zeros, (displNo + 1));
                        sendData(&zeros, (displNo + 1));
                        sendData(&zeros, (displNo + 1));
                        sendData(&zeros, (displNo + 1));
                        break;
                    default:
                        break;
                    }
                    displNo++;
                }

            }

        break;




        default:
            break;
    }

}

void  interDisplayBlanking(e__displayStates displEffect, e__dispScrollSpeed dispScrlSpd) {
    dispx[3] = &disp1[36][0];
    dispx[2] = &disp1[36][0];
    dispx[1] = &disp1[36][0];
    dispx[0] = &disp1[36][0];

    dotMatrixSendRowWise(dispx, 4, displEffect, dispScrlSpd);
}

void    dotMatrixSendRowWise(const unsigned char* value[__MAX_NO_DISPLAYS__], uint8_t maxDisplays, e__displayStates displEffect, e__dispScrollSpeed dispScrlSpd) {
    //static unsigned short digitData = 0, zeros = 0;
    static uint8_t rowCnt ;
    switch (displEffect) {
        case __SIMPLE_SCROLL_DOWN__:
            for (rowCnt = 0; rowCnt < 8; ++rowCnt) {
                loopingDisplay(rowCnt, value, __SIMPLE_SCROLL_DOWN__);
                switch (dispScrlSpd) {
                    case __SCROLL_SFAST__:
                        __delay_cycles(SysFreq/1000);
                        break;
                    case __SCROLL_FAST__:
                        __delay_cycles(SysFreq/200);
                        break;
                    case __SCROLL_MED__:
                        __delay_cycles(SysFreq/100);
                        break;
                    case __SCROLL_SLOW__:
                        __delay_cycles(SysFreq);
                        break;
                    case __SCROLL_SSLOW__:
                        __delay_cycles(SysFreq*2);
                        break;
                    default:
                        break;
                }

            }

            break;

        case __SIMPLE_SCROLL_THROUGH_:
            for (rowCnt = 0; rowCnt < 16; ++rowCnt) {
                loopingDisplay(rowCnt, value, __SIMPLE_SCROLL_THROUGH_);
                __delay_cycles(16000);
            }
            break;

        default:
            break;
    }

}

void    scrollUpDotMatrixDisplay(void) {

}
