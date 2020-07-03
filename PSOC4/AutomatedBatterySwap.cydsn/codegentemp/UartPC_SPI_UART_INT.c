/***************************************************************************//**
* \file UartPC_SPI_UART_INT.c
* \version 4.0
*
* \brief
*  This file provides the source code to the Interrupt Service Routine for
*  the SCB Component in SPI and UART modes.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "UartPC_PVT.h"
#include "UartPC_SPI_UART_PVT.h"
#include "cyapicallbacks.h"

#if (UartPC_SCB_IRQ_INTERNAL)
/*******************************************************************************
* Function Name: UartPC_SPI_UART_ISR
****************************************************************************//**
*
*  Handles the Interrupt Service Routine for the SCB SPI or UART modes.
*
*******************************************************************************/
CY_ISR(UartPC_SPI_UART_ISR)
{
#if (UartPC_INTERNAL_RX_SW_BUFFER_CONST)
    uint32 locHead;
#endif /* (UartPC_INTERNAL_RX_SW_BUFFER_CONST) */

#if (UartPC_INTERNAL_TX_SW_BUFFER_CONST)
    uint32 locTail;
#endif /* (UartPC_INTERNAL_TX_SW_BUFFER_CONST) */

#ifdef UartPC_SPI_UART_ISR_ENTRY_CALLBACK
    UartPC_SPI_UART_ISR_EntryCallback();
#endif /* UartPC_SPI_UART_ISR_ENTRY_CALLBACK */

    if (NULL != UartPC_customIntrHandler)
    {
        UartPC_customIntrHandler();
    }

    #if(UartPC_CHECK_SPI_WAKE_ENABLE)
    {
        /* Clear SPI wakeup source */
        UartPC_ClearSpiExtClkInterruptSource(UartPC_INTR_SPI_EC_WAKE_UP);
    }
    #endif

    #if (UartPC_CHECK_RX_SW_BUFFER)
    {
        if (UartPC_CHECK_INTR_RX_MASKED(UartPC_INTR_RX_NOT_EMPTY))
        {
            do
            {
                /* Move local head index */
                locHead = (UartPC_rxBufferHead + 1u);

                /* Adjust local head index */
                if (UartPC_INTERNAL_RX_BUFFER_SIZE == locHead)
                {
                    locHead = 0u;
                }

                if (locHead == UartPC_rxBufferTail)
                {
                    #if (UartPC_CHECK_UART_RTS_CONTROL_FLOW)
                    {
                        /* There is no space in the software buffer - disable the
                        * RX Not Empty interrupt source. The data elements are
                        * still being received into the RX FIFO until the RTS signal
                        * stops the transmitter. After the data element is read from the
                        * buffer, the RX Not Empty interrupt source is enabled to
                        * move the next data element in the software buffer.
                        */
                        UartPC_INTR_RX_MASK_REG &= ~UartPC_INTR_RX_NOT_EMPTY;
                        break;
                    }
                    #else
                    {
                        /* Overflow: through away received data element */
                        (void) UartPC_RX_FIFO_RD_REG;
                        UartPC_rxBufferOverflow = (uint8) UartPC_INTR_RX_OVERFLOW;
                    }
                    #endif
                }
                else
                {
                    /* Store received data */
                    UartPC_PutWordInRxBuffer(locHead, UartPC_RX_FIFO_RD_REG);

                    /* Move head index */
                    UartPC_rxBufferHead = locHead;
                }
            }
            while(0u != UartPC_GET_RX_FIFO_ENTRIES);

            UartPC_ClearRxInterruptSource(UartPC_INTR_RX_NOT_EMPTY);
        }
    }
    #endif


    #if (UartPC_CHECK_TX_SW_BUFFER)
    {
        if (UartPC_CHECK_INTR_TX_MASKED(UartPC_INTR_TX_NOT_FULL))
        {
            do
            {
                /* Check for room in TX software buffer */
                if (UartPC_txBufferHead != UartPC_txBufferTail)
                {
                    /* Move local tail index */
                    locTail = (UartPC_txBufferTail + 1u);

                    /* Adjust local tail index */
                    if (UartPC_TX_BUFFER_SIZE == locTail)
                    {
                        locTail = 0u;
                    }

                    /* Put data into TX FIFO */
                    UartPC_TX_FIFO_WR_REG = UartPC_GetWordFromTxBuffer(locTail);

                    /* Move tail index */
                    UartPC_txBufferTail = locTail;
                }
                else
                {
                    /* TX software buffer is empty: complete transfer */
                    UartPC_DISABLE_INTR_TX(UartPC_INTR_TX_NOT_FULL);
                    break;
                }
            }
            while (UartPC_SPI_UART_FIFO_SIZE != UartPC_GET_TX_FIFO_ENTRIES);

            UartPC_ClearTxInterruptSource(UartPC_INTR_TX_NOT_FULL);
        }
    }
    #endif

#ifdef UartPC_SPI_UART_ISR_EXIT_CALLBACK
    UartPC_SPI_UART_ISR_ExitCallback();
#endif /* UartPC_SPI_UART_ISR_EXIT_CALLBACK */

}

#endif /* (UartPC_SCB_IRQ_INTERNAL) */


/* [] END OF FILE */
