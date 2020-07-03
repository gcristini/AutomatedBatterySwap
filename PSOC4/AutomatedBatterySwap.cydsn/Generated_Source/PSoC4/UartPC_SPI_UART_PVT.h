/***************************************************************************//**
* \file UartPC_SPI_UART_PVT.h
* \version 4.0
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component in SPI and UART modes.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_SPI_UART_PVT_UartPC_H)
#define CY_SCB_SPI_UART_PVT_UartPC_H

#include "UartPC_SPI_UART.h"


/***************************************
*     Internal Global Vars
***************************************/

#if (UartPC_INTERNAL_RX_SW_BUFFER_CONST)
    extern volatile uint32  UartPC_rxBufferHead;
    extern volatile uint32  UartPC_rxBufferTail;
    
    /**
    * \addtogroup group_globals
    * @{
    */
    
    /** Sets when internal software receive buffer overflow
     *  was occurred.
    */  
    extern volatile uint8   UartPC_rxBufferOverflow;
    /** @} globals */
#endif /* (UartPC_INTERNAL_RX_SW_BUFFER_CONST) */

#if (UartPC_INTERNAL_TX_SW_BUFFER_CONST)
    extern volatile uint32  UartPC_txBufferHead;
    extern volatile uint32  UartPC_txBufferTail;
#endif /* (UartPC_INTERNAL_TX_SW_BUFFER_CONST) */

#if (UartPC_INTERNAL_RX_SW_BUFFER)
    extern volatile uint8 UartPC_rxBufferInternal[UartPC_INTERNAL_RX_BUFFER_SIZE];
#endif /* (UartPC_INTERNAL_RX_SW_BUFFER) */

#if (UartPC_INTERNAL_TX_SW_BUFFER)
    extern volatile uint8 UartPC_txBufferInternal[UartPC_TX_BUFFER_SIZE];
#endif /* (UartPC_INTERNAL_TX_SW_BUFFER) */


/***************************************
*     Private Function Prototypes
***************************************/

void UartPC_SpiPostEnable(void);
void UartPC_SpiStop(void);

#if (UartPC_SCB_MODE_SPI_CONST_CFG)
    void UartPC_SpiInit(void);
#endif /* (UartPC_SCB_MODE_SPI_CONST_CFG) */

#if (UartPC_SPI_WAKE_ENABLE_CONST)
    void UartPC_SpiSaveConfig(void);
    void UartPC_SpiRestoreConfig(void);
#endif /* (UartPC_SPI_WAKE_ENABLE_CONST) */

void UartPC_UartPostEnable(void);
void UartPC_UartStop(void);

#if (UartPC_SCB_MODE_UART_CONST_CFG)
    void UartPC_UartInit(void);
#endif /* (UartPC_SCB_MODE_UART_CONST_CFG) */

#if (UartPC_UART_WAKE_ENABLE_CONST)
    void UartPC_UartSaveConfig(void);
    void UartPC_UartRestoreConfig(void);
#endif /* (UartPC_UART_WAKE_ENABLE_CONST) */


/***************************************
*         UART API Constants
***************************************/

/* UART RX and TX position to be used in UartPC_SetPins() */
#define UartPC_UART_RX_PIN_ENABLE    (UartPC_UART_RX)
#define UartPC_UART_TX_PIN_ENABLE    (UartPC_UART_TX)

/* UART RTS and CTS position to be used in  UartPC_SetPins() */
#define UartPC_UART_RTS_PIN_ENABLE    (0x10u)
#define UartPC_UART_CTS_PIN_ENABLE    (0x20u)


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

/* Interrupt processing */
#define UartPC_SpiUartEnableIntRx(intSourceMask)  UartPC_SetRxInterruptMode(intSourceMask)
#define UartPC_SpiUartEnableIntTx(intSourceMask)  UartPC_SetTxInterruptMode(intSourceMask)
uint32  UartPC_SpiUartDisableIntRx(void);
uint32  UartPC_SpiUartDisableIntTx(void);


#endif /* (CY_SCB_SPI_UART_PVT_UartPC_H) */


/* [] END OF FILE */
