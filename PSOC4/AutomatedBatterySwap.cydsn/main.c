/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <string.h>

#define BUFFER_SIZE 100
#define forever for (;;) /* Enjoy :) */

#define PIN_HIGH 1
#define PIN_LOW 0

/* Active High Relay */
#define PACKP_ON PIN_HIGH
#define PACKP_OFF PIN_LOW
#define SCL_ON PIN_HIGH
#define SCL_OFF PIN_LOW
#define SDA_ON PIN_HIGH
#define SDA_OFF PIN_LOW
#define DETECT_ON PIN_HIGH
#define DETECT_OFF PIN_LOW 

/* Active Low Relay */
#define NTC_ON PIN_LOW
#define NTC_OFF PIN_HIGH

void pollUart(void);
void init(void);
void parseCommand(char *command);
void clearBuffer(uint8 *buffer, uint8 bufferSize);

int main(void)
{
    /* Initialization */
    init();
            
    /* Main Loop */
    forever
    {
        pollUart();
    }
}

void init(void)
{
    /* User Led On */
    pin_UserLed_Write(PIN_HIGH);

    /* Start Uart PC*/
    UartPC_Start();
    
    return;    
}

void parseCommand(char *command)
{
    /* PINS ON */
    if (!strcmp(command, "packp_on"))
    {
        pin_PackP_Write(PACKP_ON);
        UartPC_UartPutString("Pack_P set on");
    }
    else if (!strcmp(command, "sda_on"))
    {
        pin_SDA_Write(SDA_ON);
        UartPC_UartPutString("SDA set on");
    }
    else if (!strcmp(command, "scl_on"))
    {
        pin_SCL_Write(SCL_ON);
        UartPC_UartPutString("SCL set on");
    }
    else if (!strcmp(command, "detect_on"))
    {
        pin_Detect_Write(DETECT_ON);
        UartPC_UartPutString("Detect set on");
    }
    else if (!strcmp(command, "ntc_on"))
    {
        pin_NTC_Write(NTC_ON);
        UartPC_UartPutString("NTC set on");
    }
    else if (!strcmp (command, "all_on"))
    {
        pin_PackP_Write(PACKP_ON);
        pin_SDA_Write(SDA_ON);
        pin_SCL_Write(SCL_ON);
        pin_Detect_Write(DETECT_ON);
        pin_NTC_Write(NTC_ON);
        UartPC_UartPutString("All set on");
    }
    /* PINS OFF */
    else if (!strcmp(command, "packp_off"))
    {
        pin_PackP_Write(PACKP_OFF);
        UartPC_UartPutString("Pack_P set off");
    }
    else if (!strcmp(command, "sda_off"))
    {
        pin_SDA_Write(SDA_OFF);
        UartPC_UartPutString("SDA set off");
    }
    else if (!strcmp(command, "scl_off"))
    {
        pin_SCL_Write(SCL_OFF);
        UartPC_UartPutString("SCL set off");
    }
    else if (!strcmp(command, "detect_off"))
    {
        pin_Detect_Write(DETECT_OFF);
         UartPC_UartPutString("Detect set off");
    }
    else if (!strcmp(command, "ntc_off"))
    {
        pin_NTC_Write(NTC_OFF);
        UartPC_UartPutString("NTC set off");
    }
    else if (!strcmp (command, "all_off"))
    {
        pin_PackP_Write(PACKP_OFF);
        pin_SDA_Write(SDA_OFF);
        pin_SCL_Write(SCL_OFF);
        pin_Detect_Write(DETECT_OFF);
        pin_NTC_Write(NTC_OFF);
        UartPC_UartPutString("All set off");
    }
    else    
    {
        /* MISRA */
    }
    
    return;
}

    void pollUart()
    {
        static uint8 ch;
        static uint16 rxDataIndex = 0u;
        static uint8 rxData[BUFFER_SIZE];
        static uint8 *rxCommand;

        /* Get received character or zero if nothing has been received yet */
        ch = UartPC_UartGetChar();
        if (0u != ch)
        {
            /* Store the last received char */
            rxData[rxDataIndex] = ch;

            if (ch == '\r')
            {
                /* Insert the string terminator character */
                rxData[rxDataIndex] = '\0';

                /* Point to buffer */
                rxCommand = rxData;
                //UartPC_UartPutString((const char *)rxCommand);
                parseCommand((char *)rxCommand);

                /* Clear buffer and index */
                clearBuffer(rxData, BUFFER_SIZE);

                rxDataIndex = -1;
            }

            rxDataIndex++;
        }
    }

    void clearBuffer(uint8 *buffer, uint8 bufferSize)
    {
        uint8 i;
        for (i = 0; i < bufferSize; i++)
        {
            buffer[i] = 0u;
        }
    }

    /* [] END OF FILE */
