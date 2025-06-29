/*
 * TW_MSP430G2xx_ESP.h
 *
 *  Created on: May 25, 2020
 *      Author: Turjasu
 */

#ifndef TW_MSP430G2XX_ESP_LITE_H_
#define TW_MSP430G2XX_ESP_LITE_H_
#include <msp430.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>



//extern   volatile       unsigned char                                           _MdmAPN[];           // APN name
/*
 *  Definitions for Port and Pins
 */
#define                 WifiCred_SSID                 "Hel Secure"
#define                 WifiCred_Pwd                  "helsite987"
//#define                 ESP_UART_CB_NEEDED

#define                 _BAUD_115200_                   1
#define                 _BAUD_9600_                     2

#define                 TurnOnImmediately               1
#define                 TurnOnAfter1Sec                 2
#define                 TurnOff                         3
#define                 Restart                         4

#define                 _ESP_PORT_DIR                    P1DIR
#define                 _ESP_RST_PORT_DIR                P2DIR
#define                 _ESP_PORT_OUT                    P1OUT
#define                 _ESP_RST_PORT_OUT                P2OUT
#define                 _ESP_RST                         BIT4
#define                 WIFI_CONNECTED_LED               BIT6   // P2.6
// USCIA0 comm. bits
#define                 _ESP_PORT_SEL                 P1SEL
#define                 _ESP_PORT_SEL2                 P1SEL2
#define                 _ESP_UART_RX                  BIT1      // RX - MCU side
#define                 _ESP_UART_TX                  BIT2


#define                 _bufferMax                       150
#define                 _parseBufferMax                  50
#define                 _espRxDataParsingReqd            false


//extern      volatile            unsigned char                                   _MdmBuffer[_bufferMax],
//                                                                                _MdmStatus[80],        // Global MBuffer to store the modem replies. Define in your required C/CPP files/classes
//                                                                                _MdmHTTPBuff[_bufferMax],     //HTTP Get Buffer
//                                                                                _MdmIPAddr[16];
           volatile             unsigned char                                   _MdmBuffer[_bufferMax];
extern     volatile             unsigned int                                    _MdmBuffCnt;
extern                          uint8_t                                         _MdmCbDataParsed[_parseBufferMax];

/*
 *  Enums
 */
typedef     enum    {
        _Esp_Full_Init,
        _Esp_Connect_WIFI,
        _Esp_disConnect_WIFI,
        _Esp_GET_Request,
        _Esp_SET_Request,
        _Esp_POST_Request,
}espOperCommand;


typedef enum    { //POWER ON -> UNECHO SHRT RESPNSE -> SET NTWRK TIME SYNC -> CHK NTWRK REG -> CHECK NTWRK PWR -> CHK MODULE SUPPLY VOLTAGE
                          //*   --> CHK GPRS -->EXIT
        _SUCCESS,
        _FAIL,
        _E8266_INIT_SUCCESS,
        _E8266_HW_FLT,
        _E8266_HW_RETRY_TIMEOUT,
        _UNKNOWN,
        _E8266_PWR_UP,
        _E8266_PWR_UP_SUCCESS,
        _E8266_PWR_DN,
        _E8266_PWR_RST,
        _E8266_UNKNOWN_FAILURE,
        _E8266_RST_SUCCESS,
        _E8266_RST_FAIL,
        _E8266_AT_RESPNSE_FAIL,
        _E8266_AT_RESPNSE_SUCCESS,
        _E8266_DeECHO_FAIL,
        _E8266_DeECHO_SUCCESS,
        _E8266_STN_MODE_FAIL,
        _E8266_STN_MODE_SUCCESS,
        _E8266_SoFTAP_MODE_FAIL,
        _E8266_SoFTAP_MODE_SUCCESS,
        _E8266_MIX_MODE_FAIL,
        _E8266_MIX_MODE_SUCCESS,
//        _E8266_SoFTAP_MODE_FAIL,
//        _E8266_SoFTAP_MODE_SUCCESS,
        _E8266_WIFI_CONCTD_SUCCESS,
        _E8266_WIFI_RCV_IP_SUCCESS,
        _E8266_SERVR_CONNECT_SUCCESS,
        _E8266_SERVR_CONNECT_TIMEOUT,
        _E8266_SERVR_DISCONNCT,
        _E8266_CWJAP_UNKNWN_FAIL,
        _E8266_CWJAP_CON_TIMOUT_1,
        _E8266_CWJAP_WRNG_PSWRD_2,
        _E8266_CWJAP_AP_NT_FND_3,
        _E8266_CWJAP_CON_GENERAL_FAIL_4,
        _E8266_CIFSR_FAIL,
        _E8266_PING_SUCCESS,    // THIS ALSO ENSURES INTERNET IS PRESENT = PINGING WWW.GOOGLE.COM
        _E8266_PING_FAIL,       // THIS ALSO ENSURES THAT INTERNET IS NOT PRESENT
        _E8266_PING_TIMEOUT,
        _E8266_RSSI_RCV_SUCCESS,
        _E8266_RSSI_RCV_ERROR,
        _E8266_RSSI_RCV_TIMEOUT,
        _E8266_CIPSTART_OK,
        _E8266_CIPSTART_ERROR,
        _E8266_CIPSTART_TIMEOUT,
        _E8266_CIPSTART_ALREADY_CONNCTD,
        _E8266_CIPSEND_ARROW_SUCCESS,
        _E8266_CIPSEND_ARROW_FAIL,  // ALSO INCLUDES TIMEOUT
        _E8266_CIPSEND_ARROW_TIMEOUT,
        _E8266_CIPCLOSE_FAIL,
        _E8266_CIPCLOSED_SUCCESS,
        _E8266_SEND_OK_RECVD,
        _E8266_SEND_FAIL,
        _E8266_SEND_TIMEOUT,
        _E8266_MODULE_EXIT,
        _E8266_SM_CMD_ERROR,    // State MAchine Command could not be processed as the present state is wrong or not allowed
        _E8266_TEST_ENUM,

} esp8266StateMachines;

/*
 * Variables
 */
//Global pointers to the registers of USCI
struct      configEspPort_USCI  {
            volatile            unsigned char*      ESP_EN_PSEL;
            volatile            unsigned char*      ESP_RST_PSEL;
            volatile            unsigned char*      ESP_PSEL;
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
            const               bool                _interrupt_enabled;
                                bool*                _cbStringPatternFound;
                                unsigned char*      pEspBuff;
                                unsigned char*      pESPBuffParsedData;
            char*               _MdmIPAddr;
                                esp8266StateMachines currentState;
                                esp8266StateMachines requestedState;

};



/*
 *  Source functions required for the control and communications
 *  using ESP8266
 */

//extern                      esp8266StateMachines resetESP8266(void);          // generates a reset to the esp device
//extern                      esp8266StateMachines moduleInitDiag(esp8266StateMachines);
extern                      uint8_t ConfigureEspUART(struct configEspPort_USCI* );
extern                      void                SendDataToESP(const uint8_t* data);
extern                      void                SendCharToESP(unsigned char);

//extern                      esp8266StateMachines ESP_PinSetup(void);
//extern                      esp8266StateMachines ESP_ON_OFF(uint8_t);

//extern                      void                DeEchoShrtRsp(void);
//extern                      esp8266StateMachines    ESPInitAndDiag(void);       // Implements StateMachine for the init and diag for M95
//extern                      esp8266StateMachines    MdmMakeReady(espOperCommand , esp8266StateMachines, char*, char*);
//extern                      void                ParseCallback(void (*ptr)());
//extern                      esp8266StateMachines ReadEspRSSI(unsigned char*);
//extern                      int                 ReadIPAddr(void);
//extern                      void                ClrEspBuff(void);
//extern                      esp8266StateMachines espConnectWiFi(void);
//extern                      esp8266StateMachines espReadFromServer(esp8266StateMachines);

extern                      void     register_esp01_callback(void (*callback)(uint8_t*,uint8_t*)); // first is the string to search in reply, second is buffer to parsed data
extern                      void    parseEsp01_TCP_Client_Reply(uint8_t* , uint8_t* );
#endif /* TW_MSP430G2XX_ESP_LITE_H_ */
