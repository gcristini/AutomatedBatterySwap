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
#define forever for (;;)

#define RELAY_ON 1
#define RELAY_OFF 0

void pollUart(void);
void init(void);
void parseCommand(char *command);
void clearBuffer(uint8 *buffer, uint8 bufferSize);

int main(void)
{

    /* Initializtion */
    init();

    UartPC_UartPutString("Welcome\n");

    /* Main Loop */
    forever
    {
        pollUart();
    }
}

void init(void)
{
    UartPC_Start();
}

void parseCommand(char *command)
{
    /* PINS ON */
    if (!strcmp(command, "pack_p_on"))
    {
        pin_PackP_Write(RELAY_ON);
    }
    else if (!strcmp(command, "sda_on"))
    {
        pin_SDA_Write(RELAY_ON);
    }
    else if (!strcmp(command, "scl_on"))
    {
        pin_SCL_Write(RELAY_ON);
    }
    else if (!strcmp(command, "detect_on"))
    {
        pin_Detect_Write(RELAY_ON);
    }
    else if (!strcmp(command, "ntc_on"))
    {
        pin_NTC_Write(RELAY_ON);
    }
    else if (!strcmp (command, "all_on"))
    {
        pin_PackP_Write(RELAY_ON);
        pin_SDA_Write(RELAY_ON);
        pin_SCL_Write(RELAY_ON);
        pin_Detect_Write(RELAY_ON);
        pin_NTC_Write(RELAY_ON);
    }
    /* PINS OFF */
    else if (!strcmp(command, "pack_p_off"))
    {
        pin_PackP_Write(RELAY_OFF);
    }
    else if (!strcmp(command, "sda_off"))
    {
        pin_SDA_Write(RELAY_OFF);
    }
    else if (!strcmp(command, "scl_off"))
    {
        pin_SCL_Write(RELAY_OFF);
    }
    else if (!strcmp(command, "detect_off"))
    {
        pin_Detect_Write(RELAY_OFF);
    }
    else if (!strcmp(command, "ntc_off"))
    {
        pin_NTC_Write(RELAY_OFF);
    }
    else if (!strcmp (command, "all_off"))
    {
        pin_PackP_Write(RELAY_OFF);
        pin_SDA_Write(RELAY_OFF);
        pin_SCL_Write(RELAY_OFF);
        pin_Detect_Write(RELAY_OFF);
        pin_NTC_Write(RELAY_OFF);
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
