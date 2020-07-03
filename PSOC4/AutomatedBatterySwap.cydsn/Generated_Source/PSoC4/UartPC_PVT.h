/***************************************************************************//**
* \file .h
* \version 4.0
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component.
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

#if !defined(CY_SCB_PVT_UartPC_H)
#define CY_SCB_PVT_UartPC_H

#include "UartPC.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define UartPC_SetI2CExtClkInterruptMode(interruptMask) UartPC_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define UartPC_ClearI2CExtClkInterruptSource(interruptMask) UartPC_CLEAR_INTR_I2C_EC(interruptMask)
#define UartPC_GetI2CExtClkInterruptSource()                (UartPC_INTR_I2C_EC_REG)
#define UartPC_GetI2CExtClkInterruptMode()                  (UartPC_INTR_I2C_EC_MASK_REG)
#define UartPC_GetI2CExtClkInterruptSourceMasked()          (UartPC_INTR_I2C_EC_MASKED_REG)

#if (!UartPC_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define UartPC_SetSpiExtClkInterruptMode(interruptMask) \
                                                                UartPC_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define UartPC_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                UartPC_CLEAR_INTR_SPI_EC(interruptMask)
    #define UartPC_GetExtSpiClkInterruptSource()                 (UartPC_INTR_SPI_EC_REG)
    #define UartPC_GetExtSpiClkInterruptMode()                   (UartPC_INTR_SPI_EC_MASK_REG)
    #define UartPC_GetExtSpiClkInterruptSourceMasked()           (UartPC_INTR_SPI_EC_MASKED_REG)
#endif /* (!UartPC_CY_SCBIP_V1) */

#if(UartPC_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void UartPC_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (UartPC_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (UartPC_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_UartPC_CUSTOM_INTR_HANDLER)
    extern cyisraddress UartPC_customIntrHandler;
#endif /* !defined (CY_REMOVE_UartPC_CUSTOM_INTR_HANDLER) */
#endif /* (UartPC_SCB_IRQ_INTERNAL) */

extern UartPC_BACKUP_STRUCT UartPC_backup;

#if(UartPC_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 UartPC_scbMode;
    extern uint8 UartPC_scbEnableWake;
    extern uint8 UartPC_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 UartPC_mode;
    extern uint8 UartPC_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * UartPC_rxBuffer;
    extern uint8   UartPC_rxDataBits;
    extern uint32  UartPC_rxBufferSize;

    extern volatile uint8 * UartPC_txBuffer;
    extern uint8   UartPC_txDataBits;
    extern uint32  UartPC_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 UartPC_numberOfAddr;
    extern uint8 UartPC_subAddrSize;
#endif /* (UartPC_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (UartPC_SCB_MODE_I2C_CONST_CFG || \
        UartPC_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 UartPC_IntrTxMask;
#endif /* (! (UartPC_SCB_MODE_I2C_CONST_CFG || \
              UartPC_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(UartPC_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define UartPC_SCB_MODE_I2C_RUNTM_CFG     (UartPC_SCB_MODE_I2C      == UartPC_scbMode)
    #define UartPC_SCB_MODE_SPI_RUNTM_CFG     (UartPC_SCB_MODE_SPI      == UartPC_scbMode)
    #define UartPC_SCB_MODE_UART_RUNTM_CFG    (UartPC_SCB_MODE_UART     == UartPC_scbMode)
    #define UartPC_SCB_MODE_EZI2C_RUNTM_CFG   (UartPC_SCB_MODE_EZI2C    == UartPC_scbMode)
    #define UartPC_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (UartPC_SCB_MODE_UNCONFIG == UartPC_scbMode)

    /* Defines wakeup enable */
    #define UartPC_SCB_WAKE_ENABLE_CHECK       (0u != UartPC_scbEnableWake)
#endif /* (UartPC_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!UartPC_CY_SCBIP_V1)
    #define UartPC_SCB_PINS_NUMBER    (7u)
#else
    #define UartPC_SCB_PINS_NUMBER    (2u)
#endif /* (!UartPC_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_UartPC_H) */


/* [] END OF FILE */
