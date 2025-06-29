/*
 * TW_MSP430G2xx_ESP.c
 *
 *  Created on: May 25, 2020
 *      Author: Turjasu
 */
#include <TW_MSP430G2xx_ESP_lite.h>


/*
 *  Variables
 */

volatile                          unsigned char           _MdmBuffer[_bufferMax];
                                  uint8_t                 _MdmCbDataParsed[_parseBufferMax];
                                                          //_MdmStatus[80],        // Global MBuffer to store the modem replies. Define in your required C/CPP files/classes
                                                          //_MdmHTTPBuff[_bufferMax],
                                                          //_MdmIPAddr[16] = {NULL};
//                                  unsigned char           _APpassword[30]="TM#4C1294NCPDTI3",
//                                                          _APname[30]="TurjasuzWorldAP8";//INTIALIZED VALUE, CHECK FOR THIS
volatile                          unsigned int            _MdmBuffCnt = 0;
                                  char                    *temp_mdm = NULL;






/* Configure the ports ONLY FOR THE USCIA0
 * P3.4 = USCIA0TXD
 * P3.5 = USCIA0RXD
 * IF THE BOARD USES OTHER USCIAx MODULE, PLEASE MODIFY THE
 * INSTRUCTIONS ACCORDINGLY
 *
 * TW PRODUCTS WILL ALWAYS USE USCIA0 = P3.4 & 3.5 FOR MODEM
 * COMMUNICATIONS
 *
 */

void (*espReply_callback)(uint8_t*,uint8_t*) = 0;

//Function to register the callback: (void (*callback)())
void register_esp01_callback(void (*callback)(uint8_t*,uint8_t*)) {
        espReply_callback = callback;
}


void    parseEsp01_TCP_Client_Reply(uint8_t* stringToRespond, uint8_t* parseBuff) {

    volatile unsigned char*   pToStr = NULL;
    pToStr = strstr((const char*)_MdmBuffer,"+IPD");
    if(pToStr != NULL) {
                SendDataToESP("AT+CIPSEND=0,12\r\n");
                __delay_cycles(32000000);
                SendDataToESP("Enjoy Life \n");
                __delay_cycles(80000000);
        __no_operation();
    }

}
esp8266StateMachines ESP_PinSetup(void)
{
    esp8266StateMachines esp8266StateMachines;
    _ESP_PORT_DIR |= _ESP_RST;
    _ESP_PORT_OUT &= ~_ESP_RST; // keep ESP in Reset state
    esp8266StateMachines = _E8266_PWR_UP;
    return esp8266StateMachines;
}

/**
 * @brief mdmReplySearch: Searches for a given string in
 * another string and waits for a timeout upto mentioned
 * time in seconds
 *
 * Before this AP: Nothing specific.
 *
 * @param _buff : point to buffer space
 * @param compStr : string to compare
 * @param t_out : string to compare
 *
 * @return char* to where the string is found
 */
char*       mdmReplySearch(const char* _buff, const char* compStr, uint8_t t_out)
{
    char* temp_res = NULL;
    static uint8_t  timeout;
    timeout = t_out;
    while((temp_res == NULL)&&(timeout>0))
    {
        _delay_cycles(16000000);
        temp_res = strstr(_buff, compStr);
        timeout--;
    }
    return temp_res;

}

/**
 * @brief esp_Control_Init: initializes the ESP-01 module with connected PINS
 * over the UART.This api can only be called by the internal apis and send params
 * to simply Turn ON ESP/ Turn OFF or RESET
 *
 * Before this API, you must call the ConfigureEspUART API.
 *
 * @param struct configEspPort_USCI* espCfg --> Pointer to configuration structure.
 * @return esp8266StateMachines state of the module.
 */
    esp_Control_Init(struct configEspPort_USCI* espCfg)
{
    int resendCtr = 0, espPwrUpSuccessRetryCtr = 3;
    char* reply = NULL;
    while((espCfg->currentState != _E8266_PWR_UP_SUCCESS)&&(espPwrUpSuccessRetryCtr > 0))
    {

        switch (espCfg->requestedState) {

        case _E8266_PWR_UP:

            espCfg->currentState = _E8266_PWR_UP;
            *( espCfg->ESP_EN_PSEL) &= ~(espCfg->ESP_EN_PIN + espCfg->ESP_RST_PIN);
            *( espCfg->ESP_EN_PDIR) |= espCfg->ESP_EN_PIN ;
            *( espCfg->ESP_RST_PDIR) |= espCfg->ESP_RST_PIN ;
            *( espCfg->ESP_EN_POUT) |= espCfg->ESP_EN_PIN ;
            *( espCfg->ESP_RST_POUT) |= espCfg->ESP_RST_PIN ;
            __delay_cycles(48000000);
            // Check if module has powered up
            resendCtr = 3;
            do {
                SendDataToESP("AT\r\n");
                reply = mdmReplySearch((const char*)_MdmBuffer, "AT\r\n\r\nOK\r\n", 3);
                resendCtr--;
                if ((resendCtr == 0)&&( NULL == reply )) {
                    espCfg->requestedState = _E8266_PWR_RST;
                }
                if ( 'A' == *reply ){
                    espCfg->currentState = _E8266_PWR_UP_SUCCESS;
                }
            } while (( NULL == reply )&&(resendCtr > 0));

            break;

        case _E8266_PWR_RST:

            espCfg->currentState = _E8266_PWR_RST;
            *( espCfg->ESP_RST_POUT) &= ~(espCfg->ESP_RST_PIN) ;
            __delay_cycles(32000000);
            *( espCfg->ESP_RST_POUT) |= espCfg->ESP_RST_PIN ;
            __delay_cycles(48000000);
            // ESP01 reboot management
            espCfg->requestedState = _E8266_PWR_UP;

            break;

        case _E8266_PWR_DN:

            *( espCfg->ESP_EN_POUT) &= ~(espCfg->ESP_EN_PIN) ;
            __delay_cycles(32000000);
            *( espCfg->ESP_RST_POUT) &= ~(espCfg->ESP_RST_PIN) ;
            __delay_cycles(48000000);
            espCfg->currentState = _E8266_PWR_RST;


            break;

        default:
            break;
        }
        espPwrUpSuccessRetryCtr--;
        if(espPwrUpSuccessRetryCtr == 0)
        {
            espCfg->requestedState = _UNKNOWN;
            espCfg->currentState = _E8266_UNKNOWN_FAILURE;
        }
    }

}

///*
// *  Functions to control ESP8266
// */

esp8266StateMachines moduleInitDiag(esp8266StateMachines currState)
{
    esp8266StateMachines state = currState;

    return state;

}

/*
 * BaaudRate = 115200.
 * Oversampling is off. please read datasheet before pushing values
 * volatile            unsigned char*      ESP_PSEL;
            volatile            unsigned char*      ESP_PSEL2;
            volatile            unsigned char*      ESP_EN_PDIR;
            volatile            unsigned char*      ESP_EN_POUT;
            volatile            unsigned char*      ESP_RST_PDIR;
            volatile            unsigned char*      ESP_RST_POUT;
            const               unsigned short      ESP_RX_PIN;
            const               unsigned short      ESP_TX_PIN;
            const               unsigned short      ESP_EN_PIN;
            const               unsigned short      ESP_RST_PIN;
            const               uint32_t             systemFreq;
            const               uint32_t             ESP_comm_baudRate;
            const               bool                interrupt_enabled;
            char*               _MdmIPAddr;

    Return 0 = Success
    Return 127 = Module Failure
    Return 255 = Power Down
 */
uint8_t ConfigureEspUART(struct configEspPort_USCI* espCfg)
{
    /*
     * Set the Pins for UART config
     * Please note that  espCfg->ESP_PSEL will access the
     * secific memory of the object which is initialized
     * in main with the struct configEspPort_USCI. That
     * will point to some SFR register. So our goal is
     * to write pin's bits values inside that SFR and not
     * the object memory location. That is why we need to
     * again de-reference it as *( espCfg->ESP_PSEL)
     */

   *( espCfg->ESP_PSEL) |= (espCfg->ESP_RX_PIN + espCfg->ESP_TX_PIN);
   *( espCfg->ESP_PSEL2) |= (espCfg->ESP_RX_PIN + espCfg->ESP_TX_PIN);
     UCA0CTL1 |= UCSWRST;                     // **Reset USCI state machine**
//     _ESP_PORT_SEL |= (_ESP_UART_RX + _ESP_UART_TX);                             // P3.4,5 = USCI_A0 TXD/RXD
//     _ESP_PORT_SEL2 |= (_ESP_UART_RX + _ESP_UART_TX);
     UCA0CTL1 |= UCSSEL_2;                     // SMCLK
     // Configure the baudrate values
     volatile float baudRate_N = (float)espCfg->systemFreq/(float)espCfg->ESP_comm_baudRate;
     uint16_t N_wholeNum = (uint16_t)baudRate_N;
     volatile float N_decimal = (baudRate_N - (float)N_wholeNum)*8; // refer userguide
     uint8_t modulation_factor = (uint8_t)N_decimal;
     switch (espCfg->ESP_comm_baudRate)                             // Default BR is 9600, if not mentioned
     {
       case 9600:
           UCA0BR0 = 104;                            // 1MHz 9600 = 104, 115200 = 8
           UCA0BR1 = 0;                              // 1MHz 9600
           UCA0MCTL |= UCBRS_1;              // cpu @ 1mhz, BAUD @ 115200
           break;
       case 115200:
           UCA0BR0 = N_wholeNum%256;                            // 1MHz 9600 = 104, 115200 = 8
           UCA0BR1 = N_wholeNum/256;                              // 1MHz 9600
           UCA0MCTL |= modulation_factor << 1;              // left shifted by 1 due to registor layout, see userguide
           break;

       default:
           UCA0BR0 = 104;                            // 1MHz 9600 = 104, 115200 = 8
           UCA0BR1 = 0;                              // 1MHz 9600
           UCA0MCTL |= UCBRS_1;              // cpu @ 1mhz, BAUD @ 115200
           break;
   }
     UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
//
     if (espCfg->_interrupt_enabled)
     {
         IE2 |= UCA0RXIE;                         // Enable USCI_A0 RX interrupt
     }
     else
     {
         IE2 &= ~(UCA0TXIE + UCA0RXIE);                         // Enable USCI_A0 RX interrupt
     }

     esp_Control_Init(espCfg);

     if (espCfg->currentState == _E8266_PWR_UP_SUCCESS) return 0;
     else if (espCfg->currentState == _E8266_PWR_DN) return 255;
     else if ((espCfg->currentState == _E8266_UNKNOWN_FAILURE)&&(espCfg->requestedState == _UNKNOWN)) return 127;
}


/*
 *  Reset the buffer counter
 */
void    ClrEspBuff(void)
{
    if(_MdmBuffCnt>0)
        {
            unsigned int var;
            for (var = 0; var < _MdmBuffCnt; ++var)
            {
                _MdmBuffer[var]='\0';
            }
            _MdmBuffCnt = 0;
        }
}


/*
 *  Send String of data to modem by calling this Fn.
 */
void     SendDataToESP(const uint8_t* data)
{
//    uint8_t indx=0;

        ClrEspBuff();
        while(*data)
        {
            while (!(IFG2&UCA0TXIFG));             // USCI_A0 TX buffer ready?
            UCA0TXBUF = *data;                  // TX -> RXed character
            data++;
        }



}

/*
 *  Send single character data to modem
 */
void      SendCharToESP(unsigned char data)
{
        while (!(IFG2&UCA0TXIFG));             // USCI_A0 TX buffer ready?
        UCA0TXBUF = data;                  // TX -> RXed character
        //_delay_cycles(SystemFreq/100);

}



//esp8266StateMachines    MdmMakeReady(espOperCommand OperCmd, esp8266StateMachines currState, char* getString, char* setString) // currState will be verified everytime
//{
//    /*                  === GET EXAMPLE =====
//     *
//
//     */
//    esp8266StateMachines RetSts =  _E8266_PWR_UP;
//    static int          RetryTimeout1=0, RetryTimeout2=0;//RetryTimeout3=0,RetryTimeout4=0,RetryTimeout5=0;
////    unsigned char    temp[4];
//    volatile int Charcount=0, res_type = 0;
//    static int apiLength=0, apnValidation=0, temp_count=0, waitCount = 0;
//    volatile char* res = NULL;
////    static  char    _getString[] = getString, _setString[]= setString;
/////////////// Initialization Switch Cases////////////////////
//    switch (OperCmd)
//    {
//        case _Esp_Full_Init:
//            ////SendDataToUCA3("__case _Esp_Full_Init in MdmMakeReady__\r");
//            RetryTimeout1 = 5;
//            while((RetryTimeout1>0)&& (RetSts == _E8266_PWR_UP||
//                                        RetSts == _E8266_AT_RESPNSE_FAIL||
//                                        RetSts == _E8266_DeECHO_FAIL||
//                                        RetSts == _E8266_CIFSR_FAIL||
//                                        RetSts == _E8266_CWJAP_CON_TIMOUT_1))
//            {
//                if(RetSts == _E8266_PWR_UP)
//                {
//                    RetSts = ESP_ON_OFF(TurnOnImmediately);
//                }
//                else
//                {
//                    //Modem_ON_OFF(Restart);
//                    RetSts = ESP_ON_OFF(Restart);// States = unknown / Fail / E8266 rst success
//                }
//
//                switch (RetSts)
//                {
//                    case _UNKNOWN:
//
//                        ESP_ON_OFF(TurnOff); // alternatively we can send module to deep sleep
//                        RetSts = _UNKNOWN;
//
//                        break;
//
//                    case _FAIL:
//
//                        ESP_ON_OFF(TurnOff); // alternatively we can send module to deep sleep
//                        RetSts = _FAIL;
//
//                         break;
//
//                    case _E8266_RST_SUCCESS:
//                        // no handling, Retsts will be returned
//                         break;
//
//                    case _E8266_PWR_UP:
//                        //AT and ATE0 handling to be done here later
//
//                        RetSts = _E8266_PWR_UP_SUCCESS;
//                         break;
//                    default:
//                        break;
//                }
//
//                RetryTimeout1--;
////                RetSts = MdmInitAndDiag();
//                RetSts = moduleInitDiag(RetSts); // Esp8266 initialization and update the state machine state here
//                ////_delay_cycles(SystemFreq);
//            }
//
//            break;
//
//
//
//        case _Esp_Connect_WIFI:
//             RetSts = currState;
//             if((RetSts == _E8266_PWR_UP)||(RetSts == _E8266_RST_SUCCESS)||(RetSts == _E8266_DeECHO_SUCCESS))
//             {
//                 ///////////// send CWMODE = 1  _E8266_STN_MODE_FAIL /  _E8266_STN_MODE_SUCCESS, ////////////
//                 RetryTimeout1 = 5;
//                 do
//                 {
//                     SendDataToESP("AT+CWMODE_CUR=1\r\n"); // station mode set
//                     ////_delay_cycles(SystemFreq*3);
//                     res = NULL;
//                     do
//                     {
//                         res = strstr((const char *)_MdmBuffer,"AT+CWMODE_CUR=1"); //===> error detected here. cannot compare like this as the
//                                                                                     //buffer is filled with other value that remoes the condition of res==NULL
//                         //_delay_cycles(SystemFreq/5);
//                         //res = strstr((const char *)_MdmBuffer,"FAIL");
//                     } while (res == NULL); // Blocking Call
//                     res += 20;
//                     if(*res=='O')
//                     {
//                         res_type = 0; // 0 = success
//                         RetSts = _E8266_STN_MODE_SUCCESS;
//                     }
//                     else if(*res=='F')
//                     {
//                         res_type = -1; // -1 = fail
//                         RetSts = _E8266_STN_MODE_FAIL;
//                     }
//
//                     RetryTimeout1--;
//
//                 } while((RetSts != _E8266_STN_MODE_SUCCESS)&&(RetryTimeout1>0));
//
//                 if (RetryTimeout1 == 0)
//                  {
//                     RetSts = _E8266_STN_MODE_FAIL;
//                  }
//
//                 /////////// Next connect to WIFI Network (this should be fetched from LCD///////////
//                 if(RetSts == _E8266_STN_MODE_SUCCESS)
//                              {
//
//                                  RetryTimeout1 = 3;
//                                  do
//                                  {
//                                      SendDataToESP("AT+CWJAP_CUR=\"");
////                                      SendDataToESP((const uint8_t*)_APname);
//                                      SendDataToESP("\",\"");
////                                      SendDataToESP((const uint8_t*)_APpassword);
//                                      SendDataToESP("\"\r\n"); // later ssid and psswd will be fetched from LCD
//                                      ////_delay_cycles(SystemFreq*3);
//
//                                      do
//                                      {
//                                          //res = strstr((const char *)_MdmBuffer,"OK");
//                                          res = NULL;
//                                          res = strstr((const char *)_MdmBuffer,"OK");
//                                          //_delay_cycles(SystemFreq/5);
//                                          if(*res == 'O')
//                                          {
//                                              break;
//                                          }
//                                          res = NULL;
//                                          res = strstr((const char *)_MdmBuffer,"FAIL");
//                                          //_delay_cycles(SystemFreq/5);
//                                          if(*res == 'F')
//                                          {
//                                              break;
//                                          }
//
//                                      } while (res == NULL); // Blocking Call
//                                      if(res!=NULL)
//                                      {
//                                          if((*res)=='F')
//                                          {
//                                              /* Standard replies when Failure occurs
//                                               * AT+CWJAP_CUR="?",""<\r>
//                                                 <\r>
//                                                    <\n>WIFI DISCONNECT<\r>
//                                                    <\n>+CWJAP:3<\r>
//                                                    <\n><\r>
//                                                    <\n>FAIL<\r>
//                                                    <\n>
//                                               */
//                                              res = NULL;
//                                              res = strstr((const char *)_MdmBuffer,"CWJAP:");
//                                              res = res + 6;// point to <error code> in +CWJAP_CUR:<error code>
//                                              if(*res == '1')
//                                                  {
//                                                      RetSts = _E8266_CWJAP_CON_TIMOUT_1;
//                                                  }
//                                              else if(*res == '2')
//                                                  {
//                                                      RetSts = _E8266_CWJAP_WRNG_PSWRD_2;
//                                                  }
//                                              else if(*res == '3')
//                                                  {
//                                                      RetSts = _E8266_CWJAP_AP_NT_FND_3;
//                                                  }
//                                              else if(*res == '4')
//                                                  {
//                                                      RetSts = _E8266_CWJAP_CON_GENERAL_FAIL_4;
//                                                  }
//                                              else
//                                                  {
//                                                      RetSts = _E8266_CWJAP_UNKNWN_FAIL;
//                                                  }
//
//                                          }
//                                          else if((*res)=='O')
//                                          {
//                                              RetSts = _E8266_WIFI_CONCTD_SUCCESS;
//                                              res = NULL;
//                                              do
//                                              {
//                                                  res = strstr((const char *)_MdmBuffer,"WIFI GOT IP");
//                                              } while (res == NULL); // Blocking Call
//
//                                              if(res!=NULL)
//                                              {
//                                                  RetSts = _E8266_WIFI_RCV_IP_SUCCESS;
//                                              }
//                                          }
//
//                                      }
//                                      else
//                                      {
//                                          // no handling reqd.
//                                      }
//
//
//
//                                      RetryTimeout1--;
//
//                                  } while((RetSts != _E8266_WIFI_RCV_IP_SUCCESS)&&(RetryTimeout1>0));
//
//                                  if (RetryTimeout1 == 0)
//                                   {
//                                      //return the RetSts received
//
//                                   }
//                              }
//
//                 ////////////Ping and check Internet availability ////////////////////////////
//                 if(RetSts == _E8266_WIFI_RCV_IP_SUCCESS)
//                 {
//                     RetryTimeout1 = 2;
//                     do
//                     {
//                         SendDataToESP("AT+CIFSR\r\n");
//                         //_delay_cycles(SystemFreq/2);
//
//                         do
//                         {
//                             res = NULL;
//                             res = strstr((const char *)_MdmBuffer,"\"");//+CIFSR:STAIP,"192.168.1.7"<\r><\n>
//                                                                                     //+CIFSR:STAMACA,"ec:fa:Tbc:5b:57:d9"<\r><\n>
//                             //_delay_cycles(SystemFreq/5);
//                         } while (res == NULL); // Blocking Call
//
//                         res += 1; // points to the first number of the IP
//                         temp_count = 0;
//                         while(*res != '\r')
//                         {
////                             _devIP_Address[temp_count] = *res;
//                             //_MdmIPAddr[temp_count] = *res;
//                             temp_count++;
//                             res++;
//                         }
//                         //_delay_cycles(SystemFreq/5);
//
//                         waitCount = 5;
//                         SendDataToESP("AT+PING=\"www.google.com\"\r\n");
//                         ////_delay_cycles(SystemFreq);
//
//                         do
//                         {
//                             res = NULL;
//                             res = strstr((const char *)_MdmBuffer,"OK");
//                             //_delay_cycles(SystemFreq/2);
//                             if(*res == 'O')
//                             {
//                                 break;
//                             }
//                             res = NULL;
//                             res = strstr((const char *)_MdmBuffer,"ERROR");
//                             //_delay_cycles(SystemFreq/2);
//                             if(*res == 'E')
//                             {
//                                 break;
//                             }
//                             waitCount --;
//                         } while ((res == NULL)&&(waitCount>0)); // Blocking Call but implemented timeout
//                         if(*res == 'O')
//                         {
//                             RetSts = _E8266_PING_SUCCESS;
//                         }
//                         else if(*res == 'E')
//                         {
//                             RetSts = _E8266_PING_FAIL;
//                         }
//                         RetryTimeout1--;
//                     } while((RetSts != _E8266_PING_SUCCESS)&&(RetSts != _E8266_PING_FAIL)&&(RetryTimeout1>0));
//
//                     if (RetryTimeout1 == 0)
//                     {
//                         RetSts = _E8266_PING_TIMEOUT;
//                     }
//                 }
//
//             }
//             else
//             {
//                 // No handling reqd. the last status will be returned
//             }
//
//
//            break;
//        case _Esp_disConnect_WIFI:
////            RetSts = _M95_STOP_CGATT_SUCCESS;
////            ///////////// Check CGATT STATUS ////////////
////            RetryTimeout = 5;
////            do
////            {
////                SendDataToMdm("AT+CGATT?\r"); // CHECK IF GPRS IS ATTACHED ?
////                //_delay_cycles(SystemFreq*3);
////                StrResult = strncmp(_MdmBuffer, CGATT_OFF_Rply, 11);
////                if(StrResult != 0)
////                {
////                    SendDataToMdm("AT+CGATT=0\r"); // if GPRS bearer detached, attach it
////                    //_delay_cycles(SystemFreq*3);
////                }
////                RetryTimeout--;
////
////            } while((StrResult != 0)&&(RetryTimeout>0));
////            if (RetryTimeout == 0)
////             {
////                RetSts = _M95_STOP_CGATT_FAIL;
////             }
//            break;
//        case _Esp_GET_Request:
//
//            ///// Concatenate the GET URL//////////
////            _getString = strcat(_getString, _serviceURI);
////            _getString = strcat(_getString, _xVal);
////            // next send the x value
////            _getString = strcat(_getString, _yVal);
////            //next send the y value
//
//            RetSts = currState;
//            if((RetSts == _E8266_WIFI_RCV_IP_SUCCESS)||(RetSts == _E8266_PING_SUCCESS)||(RetSts == _E8266_SEND_OK_RECVD))
//            {
//
//
//                /////////// Next Send the GET request to the server = CONNECT TO SERVER ///////////
//
//                    RetryTimeout1 = 2;
//                    do
//                    {
//
//                        SendDataToESP("AT+CIPSTART=\"TCP\",\"turjasuzworld.in\",80\r\n"); // station mode set
//                        ////_delay_cycles(SystemFreq*3);
////                        res = NULL;
////                        do
////                        {
////                            res = strstr((const char *)_MdmBuffer,"CONNECT");
////                        } while (res == NULL); // Blocking Call
//
//
//
//                        RetryTimeout2 = 5;
//                        do
//                        {
//                            // Check for CONNECT
//                            res = NULL;
//                            res = strstr((const char *)_MdmBuffer,"CONNECT");
//                            //_delay_cycles(SystemFreq/5);
//                            if(*res == 'C')
//                            {
//                                break;
//                            }
//                            // Second Check for ERROR
//                            res = NULL;
//                            res = strstr((const char *)_MdmBuffer,"FAIL");
//                            //_delay_cycles(SystemFreq/5);
//                            if(*res == 'F')
//                            {
//                                break;
//                            }
//                            // Second Check for ERROR
//                            res = NULL;
//                            res = strstr((const char *)_MdmBuffer,"CLOSED");
//                            //_delay_cycles(SystemFreq/5);
//                            if(*res == 'C')
//                            {
//                                break;
//                            }
//
//                            RetryTimeout2--;
//                        } while ((res == NULL)&&(RetryTimeout2>0));
//                        res += 1;   // Since CLOSED and CLOSE starts with C, so we check with O and L and A for FAIL
//                        if(*res=='O')
//                        {
//                            RetSts = _E8266_CIPSTART_OK;
//                        }
//                        else if(*res=='A')
//                        {
//                            RetSts = _E8266_CIPSTART_ERROR;
//                        }
//                        else
//                        {
//                            // no handling for CLOSED, may add later
//                        }
//
//                        RetryTimeout1--;
//
//                    } while((RetSts != _E8266_CIPSTART_OK)&&(RetryTimeout1>0));
//
//                    if (RetryTimeout1 == 0)
//                    {
//                        RetSts = _E8266_CIPSTART_TIMEOUT;
//                    }
//                    /////////// Next Send the GET request to the server = CIPSEND ///////////
//
//               if(RetSts == _E8266_CIPSTART_OK)
//               {
//                    RetryTimeout1 = 5;
//                    do
//                    {
//                        SendDataToESP("AT+CIPSEND=117\r\n"); // station mode set
//                        //_delay_cycles(SystemFreq/50);
//                        res = NULL;
//                        do
//                        {
//                            res = strstr((const char *)_MdmBuffer,">");
//                            //_delay_cycles(SystemFreq/5);
//                        } while (res == NULL); // Blocking Call
//                        if(*res=='>')
//                        {
//                            RetSts = _E8266_CIPSEND_ARROW_SUCCESS;
//                        }
//                        else
//                        {
//                            // HANDLING TO BE DONE HERE
//                        }
//
//                        RetryTimeout1--;
//
//                    } while((RetSts != _E8266_CIPSEND_ARROW_SUCCESS)&&(RetryTimeout1>0));
//
//                    if (RetryTimeout1 == 0)
//                    {
//                        RetSts = _E8266_CIPSEND_ARROW_TIMEOUT;// => CONTINUE FROM HERE
//                    }
//
//                    //////////////// Next send the GET Request in HTTP1.1 format without Secured channel/////
//                    if(RetSts == _E8266_CIPSEND_ARROW_SUCCESS)
//                    {
//                        RetryTimeout1 = 2;
//                            do
//                            {
//                                SendDataToESP((const uint8_t *)getString); // GET request sent to Server
//                                //_delay_cycles(SystemFreq/2);
//                                SendDataToESP("AT+CIPCLOSE\r\n"); // Close the Connection and wait for SEND OK
//                                ////_delay_cycles(SystemFreq);
//                                /*
//                                res = NULL;
//                                do
//                                {
//                                    res = strstr((const char *)_MdmBuffer,"SEND "); // SEND FAIL handling also has to be done!!
//                                } while (res == NULL); // Blocking Call
//                                res += 5;               // Point to the OK / FAIL
//                                */
//
//                                RetryTimeout2 = 20;
//                                do
//                                {
//                                    res = NULL;
//                                    res = strstr((const char *)_MdmBuffer,"CLOSED");//OK was changed to CLOSED on 2 May 2020
//                                    //_delay_cycles(SystemFreq/5);
//                                    if(*res == 'C') //O was changed to CLOSED on 2 May 2020
//                                    {
//
//                                        break;
//                                    }
//                                    res = NULL;
//                                    res = strstr((const char *)_MdmBuffer,"FAIL");
//                                    //_delay_cycles(SystemFreq/5);
//                                    if(*res == 'F')
//                                    {
//
//                                        break;
//                                    }
//
//                                    RetryTimeout2--;
//                                } while ((res == NULL)&&(RetryTimeout2>0)); // Blocking Call
//
//                                if(res != NULL)
//                                {
//
//                                        if(*res=='C') //O was changed to CLOSED on 2 May 2020
//                                        {
//                                            RetSts = _E8266_SEND_OK_RECVD;
//                                        }
//                                        else if(*res=='F')
//                                        {
//                                            // HANDLING TO BE DONE HERE
//                                            RetSts = _E8266_SEND_FAIL;
//                                        }
//                                }
//                                else if(RetryTimeout2 == 0)
//                                {
//                                    RetSts = _E8266_SEND_TIMEOUT;// => CONTINUE FROM HERE
//                                }
//                                else
//                                {
//                                    RetSts = _E8266_SM_CMD_ERROR;
//                                }
//
//                                RetryTimeout1--;
//                            } while((RetSts == _E8266_SM_CMD_ERROR)&&(RetryTimeout1>0));
//
//                    }
//                    else
//                    {
//                        //RetSts != _E8266_CIPSEND_ARROW_SUCCESS
//                    }
//                    ////////// GET handling completed //////////////////////////////////////
//
//                                if(RetSts == _E8266_SEND_OK_RECVD)
//                                {
//
//                                            ////runHomeDisplay(_UpdateRxDataTransaction); // Indicate on HMI that data has been received from server
//
//                                            /*
//                                             * Fetch the GET DATA into RAM
//                                             */
//
//                                                temp_mdm = strchr((const char*)_MdmBuffer, '{'); //char *strchr(const char *str, int c) searches for the first
//                                                                                            //occurrence of the character c (an unsigned char) in the
//                                                                                            //string pointed to by the argument str //
//                                                temp_mdm+= 2;                        // To make the pointer parse the Battery % val and also the mV val.
//
//                                                while(*temp_mdm != '}')
//                                                {
//
//                                                    _MdmHTTPBuff[Charcount]= *temp_mdm;
//                                                    temp_mdm++;
//                                                    Charcount++;
//                                                }
//
//#if     _espRxDataParsingReqd
//                                                ParseMdmBuffer();
//#endif
//                                }
//                                else
//                                {
//                                   //if  RetSts != _E8266_SEND_OK_RECVD
//                                }
//
//               }
//               else
//               {
//                   //if RetSts != _E8266_CIPSTART_OK
//               }
//
//
//
//            }
//            else
//            {
//                //RetSts = _E8266_SM_CMD_ERROR;
//            }
//
//
//            ////runHomeDisplay(_UpdateIoTRxTxDataTransaction);
//
//            break;
//        case _Esp_SET_Request:
//            ///// Concatenate the SET API ///////////////////
////            _setString = strcat(_setString, _turjasuzworldURI);
////            _setString = strcat(_setString, _serviceURI);
////            _setString = strcat(_setString, _xVal);
////            // next send the x value
////            _setString = strcat(_setString, _yVal);
////            //next send the y value
//
//
//            RetSts = currState;
//            if((RetSts == _E8266_WIFI_RCV_IP_SUCCESS)||(RetSts == _E8266_PING_SUCCESS)||(RetSts == _E8266_TEST_ENUM)||(RetSts == _E8266_SEND_OK_RECVD))
//            {
//
//
//                /////////// Next Send the GET request to the server = CONNECT TO SERVER ///////////
//
//                    RetryTimeout1 = 5;
//                    do
//                    {
//                        SendDataToESP("AT+CIPSTART=\"TCP\",\"turjasuzworld.in\",80\r\n"); // station mode set
//                        ////_delay_cycles(SystemFreq*3);
//                        res = NULL;
//                        do
//                        {
//                            res = strstr((const char *)_MdmBuffer,"CONNECT");
//                        } while (res == NULL); // Blocking Call
//                        if(*res=='C')
//                        {
//                            RetSts = _E8266_CIPSTART_OK;
//                        }
//                        else
//                        {
//                            // NON CONNECT HANDLING TO BE DONE HERE
//                        }
//
//                        RetryTimeout1--;
//
//                    } while((RetSts != _E8266_CIPSTART_OK)&&(RetryTimeout1>0));
//
//                    if (RetryTimeout1 == 0)
//                    {
//                        RetSts = _E8266_CIPSTART_ERROR;
//                    }
//                    /////////// Next Send the GET request to the server = CIPSEND ///////////
//                    RetryTimeout1 = 5;
//                    do
//                    {
//                        apiLength = (int)strlen((const char*)setString);
//                        apiLength += 8;
//                        SendDataToESP("AT+CIPSEND=");
//                        SendCharToESP((apiLength/100) + 48); // hundreds
//                        SendCharToESP(((apiLength%100)/10)+ 48); // tens
//                        SendCharToESP((apiLength%10)+ 48);// units
//                        SendDataToESP("\r\n");// Length of the SET API
//                        ////_delay_cycles(SystemFreq*3);
//                        res = NULL;
//                        do
//                        {
//                            res = strstr((const char *)_MdmBuffer,">");
//                        } while (res == NULL); // Blocking Call
//                        if(*res=='>')
//                        {
//                            RetSts = _E8266_CIPSEND_ARROW_SUCCESS;
//                        }
//                        else
//                        {
//                            // HANDLING TO BE DONE HERE
//                        }
//
//                        RetryTimeout1--;
//
//                    } while((RetSts != _E8266_CIPSEND_ARROW_SUCCESS)&&(RetryTimeout1>0));
//
//                    if (RetryTimeout1 == 0)
//                    {
//                        RetSts = _E8266_CIPSEND_ARROW_TIMEOUT;// => CONTINUE FROM HERE
//                    }
//
//                    //////////////// Next send the GET Request in HTTP1.1 format without Secured channel/////
//                    if(RetSts == _E8266_CIPSEND_ARROW_SUCCESS)
//                    {
//                            RetryTimeout1 = 5;
//                            do
//                            {
//                                SendDataToESP((const uint8_t*)setString); // GET request sent to Server
//                                ////_delay_cycles(SystemFreq);
//                                SendDataToESP("AT+CIPCLOSE\r\n"); // Close the Connection and wait for SEND OK
//                                //_delay_cycles(SystemFreq*5);
//                                res = NULL;
//                                do
//                                {
//                                    res = strstr((const char *)_MdmBuffer,"CLOSED"); // SEND OK changed to  CLOSED ; SEND FAIL handling also has to be done!!
//                                } while (res == NULL); // Blocking Call
//
//                                if(*res=='C')
//                                {
//                                    RetSts = _E8266_SEND_OK_RECVD;
//                                }
//                                else
//                                {
//                                    // HANDLING TO BE DONE HERE
//                                    //RetSts = _E8266_SEND_FAIL;
//                                }
//
//                                RetryTimeout1--;
//
//                            } while((RetSts != _E8266_SEND_OK_RECVD)&&(RetryTimeout1>0));
//
//                            if (RetryTimeout1 == 0)
//                            {
//                                RetSts = _E8266_SEND_TIMEOUT;// => CONTINUE FROM HERE
//                            }
//                    }
//                    ////////// SET handling completed //////////////////////////////////////
//
//                            }
//                            else
//                            {
//                                //RetSts = _E8266_SM_CMD_ERROR;
//                            }
//
//
//            ////runHomeDisplay(_UpdateIoTRxTxDataTransaction);
//
//            break;
//
//
//        default:
//            break;
//    }
//
//
//
//    //SendDataToUCA3("__RetSts: ");
//    //SendCharToUCA3(RetSts);
//    //SendDataToUCA3("\r\n");
//    return RetSts;
//}

//esp8266StateMachines espConnectWiFi(void)
//{
//    esp8266StateMachines RetSts;
//    int var;
//    RetSts = MdmMakeReady(_Esp_Connect_WIFI, _E8266_PWR_UP,NULL,NULL);
////    switch (RetSts)
////    {
////        case _E8266_PING_SUCCESS:
////
////            break;
////        case _E8266_CWJAP_CON_TIMOUT_1:
////
////            break;
////        case _E8266_CWJAP_WRNG_PSWRD_2:
////
////            break;
////        case _E8266_CWJAP_AP_NT_FND_3:
////
////            break;
////        case _E8266_CWJAP_CON_GENERAL_FAIL_4:
////
////            break;
////        default:
////
////            break;
////    }
//
//
//    return RetSts;
//}

//esp8266StateMachines espReadFromServer(esp8266StateMachines _state)                              // Specific Callback handler for Branon only to read IOT status (60Sec)
//{
//      esp8266StateMachines RetSts;
//      RetSts = MdmMakeReady(_Esp_GET_Request,_state,NULL,NULL);
//      return RetSts;
//
//}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *    Interrupt Vectors for the data acquired from modem
 */

/*
 *  UCA0 INTERRUPT
 */
 #if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
 #pragma vector=USCIAB0RX_VECTOR
 __interrupt void USCI0RX_ISR(void)
 #elif defined(__GNUC__)
 void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
 #else
 #error Compiler not supported!
 #endif
 {
//   while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
//   UCA0TXBUF = UCA0RXBUF;                    // TX -> RXed character

     _MdmBuffer[_MdmBuffCnt] = UCA0RXBUF;                  // TX -> RXed character
     _MdmBuffCnt++;
     if(_MdmBuffCnt==_bufferMax) _MdmBuffCnt=0; // Added on 2 May 2020
#ifdef  ESP_UART_CB_NEEDED
     // Callback Processing //
     if((_MdmBuffer[_MdmBuffCnt]=='D')&&(_MdmBuffer[_MdmBuffCnt-1]=='P')&&(_MdmBuffer[_MdmBuffCnt-2]=='I')&&(_MdmBuffer[_MdmBuffCnt-3]=='+'))
        {
         __no_operation();
        }
//     if (espReply_callback) {
//         espReply_callback("+IPD",&_MdmCbDataParsed[0]);
//         //*adcIntrptSts = 2; // sets global flag as 2= done
//         __no_operation();
//     }
#endif
 }

