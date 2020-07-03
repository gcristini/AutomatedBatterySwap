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

void pollUart(void);
void init(void);

uint8 ch;
uint8 rxData[BUFFER_SIZE];
uint16 rxDataIndex = 0u;
uint8 *command;

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

void pollUart(void)
{
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
            command = rxData;
            UartPC_UartPutString((const char *)command);

            /* Clear buffer and index */
            for (rxDataIndex = 0; rxDataIndex < BUFFER_SIZE; rxDataIndex++)
            {
                rxData[rxDataIndex] = 0u;
            }

            rxDataIndex = -1;
        }

        rxDataIndex++;
    }
}

/* [] END OF FILE */
