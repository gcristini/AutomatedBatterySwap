/* **********************************************************************/ /**
 \file      \arg  Main.c
 \brief  
 \date		\arg  Created on: July 24.20
			\arg  Last Edit: July 24.20
 \author	\arg  Gabriele Cristini
			\arg  \b 
 */

/* ***********************************************************************/
/* **********************************************************************/
/* ***              System and library files included                 ***/
/* **********************************************************************/
#include <string.h>

/* **********************************************************************/
/* ***                 Other components included                      ***/
/* **********************************************************************/
#include "project.h"
#include "CommandManager.h"

/* **********************************************************************/
/* ***                Definition of local macros                      ***/
/* **********************************************************************/
#define UART_RX_BUFFER_SIZE 100 
#define forever for (;;) /* Enjoy :) */

/* **********************************************************************/
/* ***             Declaration of local functions                     ***/
/* **********************************************************************/

void    MAIN_v_PollUart     (void);
void    MAIN_v_Init         (void);
void    MAIN_v_ClearBuffer  (uint8 *pu8_buffer, uint8 u8_bufferSize);

/* **********************************************************************/
/* ***             Definition of local functions                      ***/
/* **********************************************************************/
/* *********************************************************************************/ /**
   \fn      	void MAIN_v_PollUart(void)
   \brief   	Poll Uart \n
            	Scope: Local
   \return  	void
   \author		\arg Gabriele Cristini
   \date		\arg Creation:  July 24.20
				\arg Last Edit: July 24.20
 */
/* **********************************************************************************/
void MAIN_v_PollUart(void)
{
    static uint8    u8_ch;
    static uint16   u16_rxDataIndex = 0u;
    static uint8    u8_rxData[UART_RX_BUFFER_SIZE];
    static uint8*   pu8_rxCommand;

    /* Get received character or zero if nothing has been received yet */
    u8_ch = UartPC_UartGetChar();
    if (0u != u8_ch)
    {
        /* Store the last received char */
        u8_rxData[u16_rxDataIndex] = u8_ch;

        if (u8_ch == '\r')
        {
            /* Insert the string terminator character */
            u8_rxData[u16_rxDataIndex] = '\0';

            /* Point to buffer */
            pu8_rxCommand = u8_rxData;
            //UartPC_UartPutString((const char *)rxCommand);
            CMD_v_ParseCommand(pu8_rxCommand);

            /* Clear buffer and index */
            MAIN_v_ClearBuffer(u8_rxData, UART_RX_BUFFER_SIZE);

            u16_rxDataIndex = -1;
        }

        u16_rxDataIndex++;
    }

    return;
}

/* *********************************************************************************/ /**
   \fn      	void MAIN_v_Init(void)
   \brief   	Main Initialization \n
            	Scope: Local
   \return  	void
   \author		\arg Gabriele Cristini
   \date		\arg Creation:  July 24.20
				\arg Last Edit: July 24.20
 */
/* **********************************************************************************/
void MAIN_v_Init(void)
{
    /* User Led On */
    pin_UserLed_Write(1);
    
    /* Initialize Command Manager Module */
    CMD_v_Init();

    /* Start Uart PC*/
    UartPC_Start();

    return;    
}

/* *********************************************************************************/ /**
   \fn      	void MAIN_v_Init(void)
   \brief   	Main Initialization \n
            	Scope: Local
   \return  	void
   \author		\arg Gabriele Cristini
   \date		\arg Creation:  July 24.20
				\arg Last Edit: July 24.20
 */
/* **********************************************************************************/
void MAIN_v_ClearBuffer(uint8 *pu8_buffer, uint8 u8_bufferSize)
{
    uint8 i;
    for (i = 0; i < u8_bufferSize; i++)
    {
        pu8_buffer[i] = 0u;
    }

    return;
}

/* *********************************************************************************/ /**
   \fn      	void main(void)
   \brief   	Main Program \n
            	Scope: Local
   \return  	void
   \author		\arg Gabriele Cristini
   \date		\arg Creation:  July 24.20
				\arg Last Edit: July 24.20
 */
/* **********************************************************************************/
int main(void)
{
    /* Initialization */
    MAIN_v_Init();
                
    /* Main Loop */
    forever
    {
        MAIN_v_PollUart();
    }
    return 0;
}