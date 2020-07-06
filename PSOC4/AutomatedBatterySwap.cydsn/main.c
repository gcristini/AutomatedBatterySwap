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

#define BUFFER_SIZE 100
#define forever for (;;)

#define RELAY_ON 1
#define RELAY_OFF 0

void pollUart(void);
void init(void);
void parseCommand(const char *command);
void clearBuffer(uint8* buffer, uint8 bufferSize);

int main(void)
{

    /* Initializtion */
    init();

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

void parseCommand(const char *command)
{
    if (!strcmp (command, "on"))
    {
        UartPC_UartPutString("Relay_On");
        
        Pin_1_Write(RELAY_ON);
        Pin_2_Write(RELAY_ON);
        Pin_3_Write(RELAY_ON);
        Pin_4_Write(RELAY_ON);
        Pin_5_Write(RELAY_ON);
    }
    else if (!strcmp (command, "off"))
    {
        UartPC_UartPutString("Relay_Off");

        Pin_1_Write(RELAY_OFF);
        Pin_2_Write(RELAY_OFF);
        Pin_3_Write(RELAY_OFF);
        Pin_4_Write(RELAY_OFF);
        Pin_5_Write(RELAY_OFF); 
    }
    else    
    {
        /* code */
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
            UartPC_UartPutString((const char *)rxCommand);
            parseCommand((const char*) rxCommand);

            /* Clear buffer and index */
            /*for (rxDataIndex = 0; rxDataIndex < BUFFER_SIZE; rxDataIndex++)
            {
                rxData[rxDataIndex] = 0u;
            }*/

            clearBuffer(rxData, BUFFER_SIZE);

            rxDataIndex = -1;
        }

        rxDataIndex++;
    }
}

void clearBuffer(uint8* buffer, uint8 bufferSize)
{
    uint8 i;
    for (i = 0; i < bufferSize; i++)
            {
                buffer[i] = 0u;
            }
}

/* [] END OF FILE */
