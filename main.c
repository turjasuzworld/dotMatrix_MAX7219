#include <msp430.h> 
#include <TW_MAX7219_DotMatrix.h>
#include <TW_m430g2553_MiscApps.h>
#include <TW_I2C.h>
#include <TW_ADC10.h>
#include <TW_TimerA_v2.h>
#include <TW_MSP430G2xx_ESP_lite.h>

#define     __NUM_OF_LDR_READS_TO_AVG__     (20)        // 20 reads/ sec. So 3 sec will have 60 reads// Adjust accordingly
//#define     _ESP01_MODULE_INSTALLED_
#define     _configure_EN_and_RST_Pins_For_ESP01  P2SEL &= ~(BIT6 | BIT7)
#define     _setRstEnPortDir                      P2DIR |= (BIT6 | BIT7)

typedef enum    {
    ENTRY_STATE,
    SENSE_AMB_LGHT_STATE,
    CLOCK_TEMP_DISPLAY_STATE,
    CHECK_FINGER_TOUCH_LDR,
    CONNECT_TO_WIFI,// NOT IMPLEMENTED
} gDeviceStateMachine;

volatile float     temp_integer, temp_decimal;
int shiftCalc=0;
unsigned char ReadData[19] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};//ss mm hh DoW DD MM YY
unsigned char SetRtcData[7] = {0x00, 0x48, 0x18, 0x04, 0x22, 0x05, 0x25};//ss mm hh DoW DD MM YY
const   unsigned  char  dow[7][3] = {"MON","TUE","WED","THU","FRI","SAT","SUN"};
const   unsigned  char  months[12][3] = {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
volatile unsigned char tempData[3][8];
char    systemIP[16] = "123.456.789.012";
static uint8_t gFlag = 0;
float gAdcValAverager;
uint8_t gAdcValAverager_count;
bool    pEspParsePatternFound = NULL;

/*
 * Other Fxns..
 */
char*  find_pattern_in_buff(char* buff, uint8_t buffLen, const char* pattern)
{
    uint8_t pattern_len = 0;
    int i;
    pattern_len = strlen(pattern);
    if((pattern_len == 0)||(buffLen < pattern_len))
    {
        return NULL;
    }
    for(i=0; i <= buffLen-pattern_len; ++i)
    {
        if(memcmp(&buff[i], pattern, pattern_len)==0)
        {
            return &buff[i];
        }
    }
    return NULL;
}

void initSend_MAX7219(unsigned short* ptrToMax7219InitMatrix, uint_fast8_t size) {
    int var = 0, dispCtr;
    while (var < size) {
        dispCtr = 1;
        while (dispCtr <= __MAX_NO_DISPLAYS__) {
            sendData(&ptrToMax7219InitMatrix[var], dispCtr);
            dispCtr++;
        }
        var++;
        __delay_cycles(160);
    }
}

void    ClockTempDisplay(uint_least16_t delayInCycles) {
    int var=0;
    //I2CSLV_RRead_Burst(0x68, 0, 7, ReadData);
    ReadFromDS1307MultiBytes(0, 19, ReadData);

    switch (delayInCycles)
    {
        case 10:
        case 11:
        {
            //Hour-Min
            interDisplayBlanking(__SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);
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
              // __delay_cycles(16000000*5);

            break;
        }

        case 52:
        case 53:
            //Seconds
            interDisplayBlanking(__SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);
            dispx[3] = &disp1[ReadData[0] >> 4][0];
            dispx[2] = &disp1[ReadData[0] & 0x0F][0];

            dotMatrixSendRowWise(dispx, 2, __SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);
              // __delay_cycles(8000000);
            break;

        case 68:
        case 69:
        {
            //Temperature (Sensor) - I2C Chip
            // Calculate the temperature from 2's complement
            interDisplayBlanking(__SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);
            unsigned char mock = ReadData[18] >> 6;
            volatile int helperVar = 0;
            temp_integer = TW_CalcDecFrmTwozComp(&ReadData[17]);
            temp_integer += 0.25*TW_CalcDecFrmTwozComp(&mock);
            temp_integer *= 100.00;
            helperVar = (int)temp_integer;
            dispx[3] = &disp1[helperVar/1000][0];

            for (var = 0; var < 8; ++var) {
                tempData[2][var] = disp1[(helperVar%1000)/100][var];
            }
            tempData[2][7] |= 1; // creating the dot

            dispx[2] = (const unsigned char*)&tempData[2][0];
            dispx[1] = &disp1[(helperVar%100)/10][0];
            dispx[0] = &disp1[helperVar%10][0];

            dotMatrixSendRowWise(dispx, 2, __SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);

            break;
        }
        case  84:
        case  85:
            //Date
            interDisplayBlanking(__SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);
            dispx[3] = &disp1[36][0];
            dispx[2] = &disp1[ReadData[4] >> 4][0];
            dispx[1] = &disp1[ReadData[4] & 0x0F][0];
            dispx[0] = &disp1[36][0];

            dotMatrixSendRowWise(dispx, 4, __SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);

            break;

        case   100:
        case   101:
            // MONTH //
            interDisplayBlanking(__SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);
            dispx[3] = &disp1[36][0];
            dispx[2] = &disp1[months[(10*(ReadData[5]>>4)+(ReadData[5]& 0x0F))-(unsigned int)1][0] - (unsigned int)55][0];
            dispx[1] = &disp1[months[(10*(ReadData[5]>>4)+(ReadData[5]& 0x0F))-(unsigned int)1][1] - (unsigned int)55][0];
            dispx[0] = &disp1[months[(10*(ReadData[5]>>4)+(ReadData[5]& 0x0F))-(unsigned int)1][2] - (unsigned int)55][0];

            dotMatrixSendRowWise(dispx, 4, __SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);

            break;

        case  116:
        case  117:
            // Day of Week //
            interDisplayBlanking(__SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);
            dispx[3] = &disp1[36][0];
            dispx[2] = &disp1[dow[(ReadData[3]& 0x0F)-(unsigned int)1][0] - (unsigned int)55][0];
            dispx[1] = &disp1[dow[(ReadData[3]& 0x0F)-(unsigned int)1][1] - (unsigned int)55][0];
            dispx[0] = &disp1[dow[(ReadData[3]& 0x0F)-(unsigned int)1][2] - (unsigned int)55][0];

            dotMatrixSendRowWise(dispx, 4, __SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);

            break;

        case  132:
        case  133:
            //Year
            interDisplayBlanking(__SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);
            dispx[3] = &disp1[2][0];
            dispx[2] = &disp1[0][0];
            dispx[1] = &disp1[ReadData[6] >> 4][0];
            dispx[0] = &disp1[ReadData[6] & 0x0F][0];

            dotMatrixSendRowWise(dispx, 2, __SIMPLE_SCROLL_DOWN__, __SCROLL_FAST__);

            break;

        default:
            break;
    }



}
/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

   struct adc10Params ADC10_InitTypeDef = {
        .refVoltage = ADC10_REF_VOLTAGE_VCC_VSS,
        .inputChannel = ADC10_INPUT_CH_0,
        .startAdcNow = true,
        .enableInterrupt = ADC10_ENABLE_INTRPT,
        .sampleHoldTime = ADC10_SHT_64_CYCLES,
        .clockSource = ADC10_CLK_SRC_ADC10CLK,
        .convertDone = 0,
    };
   struct configEspPort_USCI    ESP_InitTypeDef = {
        .ESP_EN_PSEL = &P2SEL,
        .ESP_RST_PSEL = &P2SEL,
        .ESP_PSEL = &P1SEL,
        .ESP_PSEL2 = &P1SEL2,
        .ESP_EN_PDIR = &P2DIR,
        .ESP_RST_PDIR = &P2DIR,
        .ESP_EN_POUT = &P2OUT,
        .ESP_RST_POUT = &P2OUT,
        .ESP_comm_baudRate = 115200,
        .systemFreq = 16000000,
        .ESP_RX_PIN = BIT1,
        .ESP_TX_PIN = BIT2,
        .ESP_EN_PIN = BIT6,
        .ESP_RST_PIN = BIT7,
        ._interrupt_enabled = true,
        ._MdmIPAddr = systemIP,
        ._cbStringPatternFound = &pEspParsePatternFound,
        .pEspBuff = &_MdmBuffer[0],
        .pESPBuffParsedData = &_MdmCbDataParsed[0],

   };
   volatile gDeviceStateMachine DeviceStateMachineControlHandle = ENTRY_STATE;
   volatile uint8_t configEspRplyCode = 127, espInitRetry = 3; // 127 = error, 0 = ok, 255 = pw dn

	_setClockPortDir;
	_setDataPortDir;
	_resetDisplaySelectPortOut;
	_setDisplaySelectPortDir;


	CalibrateDco(16);
	BasicInitUSCI(1); // Master mode I2C
	InitADC10_new(&ADC10_InitTypeDef);
	register_adc10_callback(calculate_light_intensity);
	register_esp01_callback(parseEsp01_TCP_Client_Reply);
	_ElapseTimeCCR0_A0(TA0_USE_SMCLK, _DIV_CLK_BY_8, CONT, 25000, 4,&gFlag);//This setting assumes 16mhz clock/8=2mhz=0.5uS*50000=25mS every interrupt.So 4 overflows=100mS

    initSend_MAX7219(MatrixSetup,sizeof(MatrixSetup));
    __bis_SR_register(GIE);

    /*
     * Setup ESP-01 as the TCP Server
     * With AP: BRANON_V6, Pass: 1234567890
     */
    while((configEspRplyCode > 0)&&(espInitRetry > 0)) {
        ESP_InitTypeDef.currentState = _UNKNOWN;
        ESP_InitTypeDef.requestedState = _E8266_PWR_UP;
        configEspRplyCode = ConfigureEspUART(&ESP_InitTypeDef);
        espInitRetry--;
    }
    SendDataToESP("AT+CWMODE=2\r\n");
    __delay_cycles(80000000);
    SendDataToESP("AT+CWSAP=\"BRANON_V6\",\"1234567890\",6,3\r\n");
    __delay_cycles(80000000);
    SendDataToESP("AT+CIPMUX=1\r\n");
    __delay_cycles(80000000);
    SendDataToESP("AT+CIPSERVER=1,8989\r\n");// default port 333
    __delay_cycles(80000000);
    SendDataToESP("");// Just calls function clearbuff and resets the buffer pointer
//    SendDataToESP("AT+CIPSEND=0,45\r\n");
//    __delay_cycles(80000000);
//    SendDataToESP("You're Connected to BRANON_V6 Over TCP: 8989\n");
//    __delay_cycles(80000000);
     //For ESP01
#ifdef  _ESP01_MODULE_INSTALLED_
     _configure_EN_and_RST_Pins_For_ESP01;
     _setRstEnPortDir;
     P2OUT |= (BIT6 + BIT7);

     //while(1) P2OUT ^= BIT6 + BIT7;
#endif
//    __delay_cycles(80000000);
//    SendDataToESP("AT\r\n");
//    __delay_cycles(80000000);

//    SendDataToESP("AT+CWJAP=\"Hel Secure\",\"helsite987\"\r\n");
//    __delay_cycles(80000000);
//    SendDataToESP("AT+CIFSR\r\n");
//    __delay_cycles(80000000);
//    __delay_cycles(80000000);

//    SendDataToESP("AT+CIPSTART=4,\"UDP\",\"192.168.137.1\",8989,8989,0\r\n");//AT+CIPSTART=<"type">,<"remote host">,<remote port>[,<local port>,<mode>,<"local IP">]
//    __delay_cycles(80000000);

//    while(1){
//        SendDataToESP("AT+CIPSEND=0,12\r\n");
//        __delay_cycles(32000000);
//        SendDataToESP("Enjoy Life \n");
//        __delay_cycles(80000000);
//    }

//	I2CSLV_Save_Burst(0x68, 0, SetRtcData, 7);
//	while(1);



    volatile uint_fast16_t   secondCountInLoops = 0;


	while(1) {
	    /*
	     * The entire functionality has been divided into some states:
	     *
	     * ENTRY_STATE: First time timer shoot, first entry to the system
	     *
	     * SENSE_AMB_LGHT_STATE: Entry to this is only from Entry_state and
	     * the system mainly reads the ADC input of the LDR to understand
	     * the ambient light conditions. Appropriately sets the Dot Matrix
	     * Contrast/Brightness
	     *
	     * CLOCK_TEMP_DISPLAY_STATE: Check the LDR light and then with that
	     * brightness keep showiing the cyclic clock and temp display. Entry
	     * can be from any state
	     *
	     * CHECK_FINGER_TOUCH_LDR: If the user follows a certain sequence
	     * of touching the finger, then the LDR will move to the user
	     * input mode. In this case the user can tap the LDR to enter the
	     * settings, connect to WiFi and other seetings of the device. Entry
	     * can be from any state
	     *
	     */
	    __bis_SR_register(GIE);
	    unsigned char* pFoundParse = NULL;
	    pFoundParse = find_pattern_in_buff(ESP_InitTypeDef.pEspBuff, 150, "+IPD");
	    if(NULL != pFoundParse)
	    {
	        __no_operation();
	        pFoundParse += 10;
	        strncpy(ESP_InitTypeDef.pESPBuffParsedData, pFoundParse, 20);
	        SendDataToESP("");

	    }
//	    if(strstr("+IPD", ))
//	    {
//	        __no_operation();
//	    }
	    if(gFlag)
	    {
	        switch (DeviceStateMachineControlHandle) {
	        case ENTRY_STATE:
	            //controlConversion(ADC10_START_CONVERSION);
	            DeviceStateMachineControlHandle = SENSE_AMB_LGHT_STATE;
	            break;
	        case SENSE_AMB_LGHT_STATE:
	            if(ADC10_InitTypeDef.convertDone == 0)
	            {
	                controlConversion(ADC10_START_CONVERSION);
	            }
	            if(ADC10_InitTypeDef.convertDone == 2)
	            {
	                // Smoothen out ADC Value by simple averaging
	                gAdcValAverager += (float)gAmbientLux;
	                if(++gAdcValAverager_count > __NUM_OF_LDR_READS_TO_AVG__)
	                {
	                    gAdcValAverager_count = 0;
	                    gAdcValAverager /= __NUM_OF_LDR_READS_TO_AVG__;
	                    gAmbientLux = (int)gAdcValAverager;
	                    //calculate_light_intensity(gAdcRawVal,&gAmbientLux);
	                    gAmbientLux /= 64; // 64 used as the func: calculate_light_intensity returns value from 5 - 1000 in 16 stepps.
	                    //But for MAX7219 we need only 16 steps with 0x0A
	                    gAmbientLux += 2560;// 0x0Ayy = 2560 + yy
	                    initSend_MAX7219((unsigned short*)&gAmbientLux,1);
	                }

	                ADC10_InitTypeDef.convertDone = 0;
	            }

	            DeviceStateMachineControlHandle = CLOCK_TEMP_DISPLAY_STATE;
	            break;
	        case CLOCK_TEMP_DISPLAY_STATE:
	            ClockTempDisplay(secondCountInLoops);
	            DeviceStateMachineControlHandle = SENSE_AMB_LGHT_STATE;
	            break;
	        default:
	            break;
	        }
	        if(++secondCountInLoops > 133) secondCountInLoops = 0;
	    }
	    gFlag = 0; // Clear for next hit
	    __bis_SR_register(LPM0_bits + GIE);
	}
	return 0;
}
