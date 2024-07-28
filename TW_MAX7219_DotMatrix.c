#include <TW_MAX7219_DotMatrix.h>

const unsigned char disp1[36][8]={

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

};

unsigned short MatrixSetup[7] = {0x0F01,0x0C01,0x0900,0x0A01,0x0B07,0x0C01,0x0F00};

void sendData(unsigned short *data, uint8_t dispNo) {

    uint8_t bitCnt;
    volatile uint8_t dispNumber;
    dispNumber = dispNo - 1;
    P2OUT &= ~(BIT0 + BIT1 + BIT2 + BIT3);

//    UCA0TXBUF = (*data)>>8;
//    UCA0TXBUF = MatrixData[count] & 0xFF;
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

void    dotMatrixSendRowWise(const unsigned char* value[__MAX_NO_DISPLAYS__], uint8_t maxDisplays) {
    static unsigned short digitData = 0, zeros = 0;
    static uint8_t rowCnt, displNo , itrNo, scan;
    for (rowCnt = 0; rowCnt < 8; ++rowCnt) {    // Since total no. of rows is 8
        switch (rowCnt) {
            case 0:
                // For ROW 0 :
                displNo = 0;
                while(displNo < maxDisplays) {
                    digitData = 1 << 8; // create the upper byte
                    digitData |= *(value[displNo] + 7);
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
                break;
                //// For Row1 & Row0
            case 1:
                displNo = 0;
                while(displNo < maxDisplays) {
                    digitData = 2 << 8; // create the upper byte
                    digitData |= *(value[displNo] + 7);
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
                displNo = 0;
                while(displNo < maxDisplays) {
                    digitData = 1 << 8; // create the upper byte
                    digitData |= *(value[displNo] + 6);
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
            break;
                ////For Row2,1 and 0
            case 2:
                displNo = 0;
                while(displNo < maxDisplays) {
                    digitData = 3 << 8; // create the upper byte
                    digitData |= *(value[displNo] + 7);
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
                displNo = 0;
                while(displNo < maxDisplays) {
                    digitData = 2 << 8; // create the upper byte
                    digitData |= *(value[displNo] + 6);
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
                displNo = 0;
                while(displNo < maxDisplays) {
                    digitData = 1 << 8; // create the upper byte
                    digitData |= *(value[displNo] + 5);
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
            break;
            ////For Row3,2,1 and 0
            case 3:
                displNo = 0;
                while(displNo < maxDisplays) {
                    digitData = 4 << 8; // create the upper byte
                    digitData |= *(value[displNo] + 7);
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
                displNo = 0;
                while(displNo < maxDisplays) {
                    digitData = 3 << 8; // create the upper byte
                    digitData |= *(value[displNo] + 6);
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
                displNo = 0;
                while(displNo < maxDisplays) {
                    digitData = 2 << 8; // create the upper byte
                    digitData |= *(value[displNo] + 5);
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
                displNo = 0;
                while(displNo < maxDisplays) {
                    digitData = 1 << 8; // create the upper byte
                    digitData |= *(value[displNo] + 4);
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
                break;
                ////For Row4,3,2,1 and 0
                case 4:
                    displNo = 0;
                    while(displNo < maxDisplays) {
                        digitData = 5 << 8; // create the upper byte
                        digitData |= *(value[displNo] + 7);
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
                    displNo = 0;
                    while(displNo < maxDisplays) {
                        digitData = 4 << 8; // create the upper byte
                        digitData |= *(value[displNo] + 6);
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
                    displNo = 0;
                    while(displNo < maxDisplays) {
                        digitData = 3 << 8; // create the upper byte
                        digitData |= *(value[displNo] + 5);
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
                    displNo = 0;
                    while(displNo < maxDisplays) {
                        digitData = 2 << 8; // create the upper byte
                        digitData |= *(value[displNo] + 4);
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
                    displNo = 0;
                     while(displNo < maxDisplays) {
                         digitData = 1 << 8; // create the upper byte
                         digitData |= *(value[displNo] + 3);
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
                break;
                ////For Row5,4,3,2,1 and 0
                case 5:
                    displNo = 0;
                    while(displNo < maxDisplays) {
                        digitData = 6 << 8; // create the upper byte
                        digitData |= *(value[displNo] + 7);
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
                    displNo = 0;
                    while(displNo < maxDisplays) {
                        digitData = 5 << 8; // create the upper byte
                        digitData |= *(value[displNo] + 6);
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
                    displNo = 0;
                    while(displNo < maxDisplays) {
                        digitData = 4 << 8; // create the upper byte
                        digitData |= *(value[displNo] + 5);
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
                    displNo = 0;
                    while(displNo < maxDisplays) {
                        digitData = 3 << 8; // create the upper byte
                        digitData |= *(value[displNo] + 4);
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
                    displNo = 0;
                     while(displNo < maxDisplays) {
                         digitData = 2 << 8; // create the upper byte
                         digitData |= *(value[displNo] + 3);
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
                     displNo = 0;
                      while(displNo < maxDisplays) {
                          digitData = 1 << 8; // create the upper byte
                          digitData |= *(value[displNo] + 2);
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
                break;
                ////For Row6,5,4,3,2,1 and 0
                case 6:
                    displNo = 0;
                    while(displNo < maxDisplays) {
                        digitData = 7 << 8; // create the upper byte
                        digitData |= *(value[displNo] + 7);
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
                    displNo = 0;
                    while(displNo < maxDisplays) {
                        digitData = 6 << 8; // create the upper byte
                        digitData |= *(value[displNo] + 6);
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
                    displNo = 0;
                    while(displNo < maxDisplays) {
                        digitData = 5 << 8; // create the upper byte
                        digitData |= *(value[displNo] + 5);
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
                    displNo = 0;
                    while(displNo < maxDisplays) {
                        digitData = 4 << 8; // create the upper byte
                        digitData |= *(value[displNo] + 4);
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
                    displNo = 0;
                     while(displNo < maxDisplays) {
                         digitData = 3 << 8; // create the upper byte
                         digitData |= *(value[displNo] + 3);
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
                     displNo = 0;
                      while(displNo < maxDisplays) {
                          digitData = 2 << 8; // create the upper byte
                          digitData |= *(value[displNo] + 2);
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
                      displNo = 0;
                       while(displNo < maxDisplays) {
                           digitData = 1 << 8; // create the upper byte
                           digitData |= *(value[displNo] + 1);
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
                break;
                ////For Row7,6,5,4,3,2,1 and 0
                case 7:
                    displNo = 0;
                    while(displNo < maxDisplays) {
                        digitData = 8 << 8; // create the upper byte
                        digitData |= *(value[displNo] + 7);
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
                    displNo = 0;
                    while(displNo < maxDisplays) {
                        digitData = 7 << 8; // create the upper byte
                        digitData |= *(value[displNo] + 6);
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
                    displNo = 0;
                    while(displNo < maxDisplays) {
                        digitData = 6 << 8; // create the upper byte
                        digitData |= *(value[displNo] + 5);
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
                    displNo = 0;
                    while(displNo < maxDisplays) {
                        digitData = 5 << 8; // create the upper byte
                        digitData |= *(value[displNo] + 4);
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
                    displNo = 0;
                     while(displNo < maxDisplays) {
                         digitData = 4 << 8; // create the upper byte
                         digitData |= *(value[displNo] + 3);
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
                     displNo = 0;
                      while(displNo < maxDisplays) {
                          digitData = 3 << 8; // create the upper byte
                          digitData |= *(value[displNo] + 2);
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
                      displNo = 0;
                       while(displNo < maxDisplays) {
                           digitData = 2 << 8; // create the upper byte
                           digitData |= *(value[displNo] + 1);
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
                       displNo = 0;
                        while(displNo < maxDisplays) {
                            digitData = 1 << 8; // create the upper byte
                            digitData |= *(value[displNo] + 0);
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
                break;


            default:
                break;
        }
        __delay_cycles(1600000);
    }
}

void    scrollUpDotMatrixDisplay(void) {

}
