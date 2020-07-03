/***************************************************************************//**
* \file UartPC_BOOT.h
* \version 4.0
*
* \brief
*  This file provides constants and parameter values of the bootloader
*  communication APIs for the SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2014-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_BOOT_UartPC_H)
#define CY_SCB_BOOT_UartPC_H

#include "UartPC_PVT.h"

#if (UartPC_SCB_MODE_I2C_INC)
    #include "UartPC_I2C.h"
#endif /* (UartPC_SCB_MODE_I2C_INC) */

#if (UartPC_SCB_MODE_EZI2C_INC)
    #include "UartPC_EZI2C.h"
#endif /* (UartPC_SCB_MODE_EZI2C_INC) */

#if (UartPC_SCB_MODE_SPI_INC || UartPC_SCB_MODE_UART_INC)
    #include "UartPC_SPI_UART.h"
#endif /* (UartPC_SCB_MODE_SPI_INC || UartPC_SCB_MODE_UART_INC) */


/***************************************
*  Conditional Compilation Parameters
****************************************/

/* Bootloader communication interface enable */
#define UartPC_BTLDR_COMM_ENABLED ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_UartPC) || \
                                             (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))

/* Enable I2C bootloader communication */
#if (UartPC_SCB_MODE_I2C_INC)
    #define UartPC_I2C_BTLDR_COMM_ENABLED     (UartPC_BTLDR_COMM_ENABLED && \
                                                            (UartPC_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             UartPC_I2C_SLAVE_CONST))
#else
     #define UartPC_I2C_BTLDR_COMM_ENABLED    (0u)
#endif /* (UartPC_SCB_MODE_I2C_INC) */

/* EZI2C does not support bootloader communication. Provide empty APIs */
#if (UartPC_SCB_MODE_EZI2C_INC)
    #define UartPC_EZI2C_BTLDR_COMM_ENABLED   (UartPC_BTLDR_COMM_ENABLED && \
                                                         UartPC_SCB_MODE_UNCONFIG_CONST_CFG)
#else
    #define UartPC_EZI2C_BTLDR_COMM_ENABLED   (0u)
#endif /* (UartPC_EZI2C_BTLDR_COMM_ENABLED) */

/* Enable SPI bootloader communication */
#if (UartPC_SCB_MODE_SPI_INC)
    #define UartPC_SPI_BTLDR_COMM_ENABLED     (UartPC_BTLDR_COMM_ENABLED && \
                                                            (UartPC_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             UartPC_SPI_SLAVE_CONST))
#else
        #define UartPC_SPI_BTLDR_COMM_ENABLED (0u)
#endif /* (UartPC_SPI_BTLDR_COMM_ENABLED) */

/* Enable UART bootloader communication */
#if (UartPC_SCB_MODE_UART_INC)
       #define UartPC_UART_BTLDR_COMM_ENABLED    (UartPC_BTLDR_COMM_ENABLED && \
                                                            (UartPC_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             (UartPC_UART_RX_DIRECTION && \
                                                              UartPC_UART_TX_DIRECTION)))
#else
     #define UartPC_UART_BTLDR_COMM_ENABLED   (0u)
#endif /* (UartPC_UART_BTLDR_COMM_ENABLED) */

/* Enable bootloader communication */
#define UartPC_BTLDR_COMM_MODE_ENABLED    (UartPC_I2C_BTLDR_COMM_ENABLED   || \
                                                     UartPC_SPI_BTLDR_COMM_ENABLED   || \
                                                     UartPC_EZI2C_BTLDR_COMM_ENABLED || \
                                                     UartPC_UART_BTLDR_COMM_ENABLED)


/***************************************
*        Function Prototypes
***************************************/

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (UartPC_I2C_BTLDR_COMM_ENABLED)
    /* I2C Bootloader physical layer functions */
    void UartPC_I2CCyBtldrCommStart(void);
    void UartPC_I2CCyBtldrCommStop (void);
    void UartPC_I2CCyBtldrCommReset(void);
    cystatus UartPC_I2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus UartPC_I2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map I2C specific bootloader communication APIs to SCB specific APIs */
    #if (UartPC_SCB_MODE_I2C_CONST_CFG)
        #define UartPC_CyBtldrCommStart   UartPC_I2CCyBtldrCommStart
        #define UartPC_CyBtldrCommStop    UartPC_I2CCyBtldrCommStop
        #define UartPC_CyBtldrCommReset   UartPC_I2CCyBtldrCommReset
        #define UartPC_CyBtldrCommRead    UartPC_I2CCyBtldrCommRead
        #define UartPC_CyBtldrCommWrite   UartPC_I2CCyBtldrCommWrite
    #endif /* (UartPC_SCB_MODE_I2C_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (UartPC_I2C_BTLDR_COMM_ENABLED) */


#if defined(CYDEV_BOOTLOADER_IO_COMP) && (UartPC_EZI2C_BTLDR_COMM_ENABLED)
    /* Bootloader physical layer functions */
    void UartPC_EzI2CCyBtldrCommStart(void);
    void UartPC_EzI2CCyBtldrCommStop (void);
    void UartPC_EzI2CCyBtldrCommReset(void);
    cystatus UartPC_EzI2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus UartPC_EzI2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map EZI2C specific bootloader communication APIs to SCB specific APIs */
    #if (UartPC_SCB_MODE_EZI2C_CONST_CFG)
        #define UartPC_CyBtldrCommStart   UartPC_EzI2CCyBtldrCommStart
        #define UartPC_CyBtldrCommStop    UartPC_EzI2CCyBtldrCommStop
        #define UartPC_CyBtldrCommReset   UartPC_EzI2CCyBtldrCommReset
        #define UartPC_CyBtldrCommRead    UartPC_EzI2CCyBtldrCommRead
        #define UartPC_CyBtldrCommWrite   UartPC_EzI2CCyBtldrCommWrite
    #endif /* (UartPC_SCB_MODE_EZI2C_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (UartPC_EZI2C_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (UartPC_SPI_BTLDR_COMM_ENABLED)
    /* SPI Bootloader physical layer functions */
    void UartPC_SpiCyBtldrCommStart(void);
    void UartPC_SpiCyBtldrCommStop (void);
    void UartPC_SpiCyBtldrCommReset(void);
    cystatus UartPC_SpiCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus UartPC_SpiCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map SPI specific bootloader communication APIs to SCB specific APIs */
    #if (UartPC_SCB_MODE_SPI_CONST_CFG)
        #define UartPC_CyBtldrCommStart   UartPC_SpiCyBtldrCommStart
        #define UartPC_CyBtldrCommStop    UartPC_SpiCyBtldrCommStop
        #define UartPC_CyBtldrCommReset   UartPC_SpiCyBtldrCommReset
        #define UartPC_CyBtldrCommRead    UartPC_SpiCyBtldrCommRead
        #define UartPC_CyBtldrCommWrite   UartPC_SpiCyBtldrCommWrite
    #endif /* (UartPC_SCB_MODE_SPI_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (UartPC_SPI_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (UartPC_UART_BTLDR_COMM_ENABLED)
    /* UART Bootloader physical layer functions */
    void UartPC_UartCyBtldrCommStart(void);
    void UartPC_UartCyBtldrCommStop (void);
    void UartPC_UartCyBtldrCommReset(void);
    cystatus UartPC_UartCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus UartPC_UartCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map UART specific bootloader communication APIs to SCB specific APIs */
    #if (UartPC_SCB_MODE_UART_CONST_CFG)
        #define UartPC_CyBtldrCommStart   UartPC_UartCyBtldrCommStart
        #define UartPC_CyBtldrCommStop    UartPC_UartCyBtldrCommStop
        #define UartPC_CyBtldrCommReset   UartPC_UartCyBtldrCommReset
        #define UartPC_CyBtldrCommRead    UartPC_UartCyBtldrCommRead
        #define UartPC_CyBtldrCommWrite   UartPC_UartCyBtldrCommWrite
    #endif /* (UartPC_SCB_MODE_UART_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (UartPC_UART_BTLDR_COMM_ENABLED) */

/**
* \addtogroup group_bootloader
* @{
*/

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (UartPC_BTLDR_COMM_ENABLED)
    #if (UartPC_SCB_MODE_UNCONFIG_CONST_CFG)
        /* Bootloader physical layer functions */
        void UartPC_CyBtldrCommStart(void);
        void UartPC_CyBtldrCommStop (void);
        void UartPC_CyBtldrCommReset(void);
        cystatus UartPC_CyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
        cystatus UartPC_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    #endif /* (UartPC_SCB_MODE_UNCONFIG_CONST_CFG) */

    /* Map SCB specific bootloader communication APIs to common APIs */
    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_UartPC)
        #define CyBtldrCommStart    UartPC_CyBtldrCommStart
        #define CyBtldrCommStop     UartPC_CyBtldrCommStop
        #define CyBtldrCommReset    UartPC_CyBtldrCommReset
        #define CyBtldrCommWrite    UartPC_CyBtldrCommWrite
        #define CyBtldrCommRead     UartPC_CyBtldrCommRead
    #endif /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_UartPC) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (UartPC_BTLDR_COMM_ENABLED) */

/** @} group_bootloader */

/***************************************
*           API Constants
***************************************/

/* Timeout unit in milliseconds */
#define UartPC_WAIT_1_MS  (1u)

/* Return number of bytes to copy into bootloader buffer */
#define UartPC_BYTES_TO_COPY(actBufSize, bufSize) \
                            ( ((uint32)(actBufSize) < (uint32)(bufSize)) ? \
                                ((uint32) (actBufSize)) : ((uint32) (bufSize)) )

/* Size of Read/Write buffers for I2C bootloader  */
#define UartPC_I2C_BTLDR_SIZEOF_READ_BUFFER   (64u)
#define UartPC_I2C_BTLDR_SIZEOF_WRITE_BUFFER  (64u)

/* Byte to byte time interval: calculated basing on current component
* data rate configuration, can be defined in project if required.
*/
#ifndef UartPC_SPI_BYTE_TO_BYTE
    #define UartPC_SPI_BYTE_TO_BYTE   (160u)
#endif

/* Byte to byte time interval: calculated basing on current component
* baud rate configuration, can be defined in the project if required.
*/
#ifndef UartPC_UART_BYTE_TO_BYTE
    #define UartPC_UART_BYTE_TO_BYTE  (171u)
#endif /* UartPC_UART_BYTE_TO_BYTE */

#endif /* (CY_SCB_BOOT_UartPC_H) */


/* [] END OF FILE */
