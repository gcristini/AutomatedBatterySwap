/***************************************************************************//**
* \file UartPC_UART.c
* \version 4.0
*
* \brief
*  This file provides the source code to the API for the SCB Component in
*  UART mode.
*
* Note:
*
*******************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "UartPC_PVT.h"
#include "UartPC_SPI_UART_PVT.h"
#include "cyapicallbacks.h"

#if (UartPC_UART_WAKE_ENABLE_CONST && UartPC_UART_RX_WAKEUP_IRQ)
    /**
    * \addtogroup group_globals
    * \{
    */
    /** This global variable determines whether to enable Skip Start
    * functionality when UartPC_Sleep() function is called:
    * 0 – disable, other values – enable. Default value is 1.
    * It is only available when Enable wakeup from Deep Sleep Mode is enabled.
    */
    uint8 UartPC_skipStart = 1u;
    /** \} globals */
#endif /* (UartPC_UART_WAKE_ENABLE_CONST && UartPC_UART_RX_WAKEUP_IRQ) */

#if(UartPC_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Configuration Structure Initialization
    ***************************************/

    const UartPC_UART_INIT_STRUCT UartPC_configUart =
    {
        UartPC_UART_SUB_MODE,
        UartPC_UART_DIRECTION,
        UartPC_UART_DATA_BITS_NUM,
        UartPC_UART_PARITY_TYPE,
        UartPC_UART_STOP_BITS_NUM,
        UartPC_UART_OVS_FACTOR,
        UartPC_UART_IRDA_LOW_POWER,
        UartPC_UART_MEDIAN_FILTER_ENABLE,
        UartPC_UART_RETRY_ON_NACK,
        UartPC_UART_IRDA_POLARITY,
        UartPC_UART_DROP_ON_PARITY_ERR,
        UartPC_UART_DROP_ON_FRAME_ERR,
        UartPC_UART_WAKE_ENABLE,
        0u,
        NULL,
        0u,
        NULL,
        UartPC_UART_MP_MODE_ENABLE,
        UartPC_UART_MP_ACCEPT_ADDRESS,
        UartPC_UART_MP_RX_ADDRESS,
        UartPC_UART_MP_RX_ADDRESS_MASK,
        (uint32) UartPC_SCB_IRQ_INTERNAL,
        UartPC_UART_INTR_RX_MASK,
        UartPC_UART_RX_TRIGGER_LEVEL,
        UartPC_UART_INTR_TX_MASK,
        UartPC_UART_TX_TRIGGER_LEVEL,
        (uint8) UartPC_UART_BYTE_MODE_ENABLE,
        (uint8) UartPC_UART_CTS_ENABLE,
        (uint8) UartPC_UART_CTS_POLARITY,
        (uint8) UartPC_UART_RTS_POLARITY,
        (uint8) UartPC_UART_RTS_FIFO_LEVEL,
        (uint8) UartPC_UART_RX_BREAK_WIDTH
    };


    /*******************************************************************************
    * Function Name: UartPC_UartInit
    ****************************************************************************//**
    *
    *  Configures the UartPC for UART operation.
    *
    *  This function is intended specifically to be used when the UartPC
    *  configuration is set to “Unconfigured UartPC” in the customizer.
    *  After initializing the UartPC in UART mode using this function,
    *  the component can be enabled using the UartPC_Start() or
    * UartPC_Enable() function.
    *  This function uses a pointer to a structure that provides the configuration
    *  settings. This structure contains the same information that would otherwise
    *  be provided by the customizer settings.
    *
    *  \param config: pointer to a structure that contains the following list of
    *   fields. These fields match the selections available in the customizer.
    *   Refer to the customizer for further description of the settings.
    *
    *******************************************************************************/
    void UartPC_UartInit(const UartPC_UART_INIT_STRUCT *config)
    {
        uint32 pinsConfig;

        if (NULL == config)
        {
            CYASSERT(0u != 0u); /* Halt execution due to bad function parameter */
        }
        else
        {
            /* Get direction to configure UART pins: TX, RX or TX+RX */
            pinsConfig  = config->direction;

        #if !(UartPC_CY_SCBIP_V0 || UartPC_CY_SCBIP_V1)
            /* Add RTS and CTS pins to configure */
            pinsConfig |= (0u != config->rtsRxFifoLevel) ? (UartPC_UART_RTS_PIN_ENABLE) : (0u);
            pinsConfig |= (0u != config->enableCts)      ? (UartPC_UART_CTS_PIN_ENABLE) : (0u);
        #endif /* !(UartPC_CY_SCBIP_V0 || UartPC_CY_SCBIP_V1) */

            /* Configure pins */
            UartPC_SetPins(UartPC_SCB_MODE_UART, config->mode, pinsConfig);

            /* Store internal configuration */
            UartPC_scbMode       = (uint8) UartPC_SCB_MODE_UART;
            UartPC_scbEnableWake = (uint8) config->enableWake;
            UartPC_scbEnableIntr = (uint8) config->enableInterrupt;

            /* Set RX direction internal variables */
            UartPC_rxBuffer      =         config->rxBuffer;
            UartPC_rxDataBits    = (uint8) config->dataBits;
            UartPC_rxBufferSize  =         config->rxBufferSize;

            /* Set TX direction internal variables */
            UartPC_txBuffer      =         config->txBuffer;
            UartPC_txDataBits    = (uint8) config->dataBits;
            UartPC_txBufferSize  =         config->txBufferSize;

            /* Configure UART interface */
            if(UartPC_UART_MODE_IRDA == config->mode)
            {
                /* OVS settings: IrDA */
                UartPC_CTRL_REG  = ((0u != config->enableIrdaLowPower) ?
                                                (UartPC_UART_GET_CTRL_OVS_IRDA_LP(config->oversample)) :
                                                (UartPC_CTRL_OVS_IRDA_OVS16));
            }
            else
            {
                /* OVS settings: UART and SmartCard */
                UartPC_CTRL_REG  = UartPC_GET_CTRL_OVS(config->oversample);
            }

            UartPC_CTRL_REG     |= UartPC_GET_CTRL_BYTE_MODE  (config->enableByteMode)      |
                                             UartPC_GET_CTRL_ADDR_ACCEPT(config->multiprocAcceptAddr) |
                                             UartPC_CTRL_UART;

            /* Configure sub-mode: UART, SmartCard or IrDA */
            UartPC_UART_CTRL_REG = UartPC_GET_UART_CTRL_MODE(config->mode);

            /* Configure RX direction */
            UartPC_UART_RX_CTRL_REG = UartPC_GET_UART_RX_CTRL_MODE(config->stopBits)              |
                                        UartPC_GET_UART_RX_CTRL_POLARITY(config->enableInvertedRx)          |
                                        UartPC_GET_UART_RX_CTRL_MP_MODE(config->enableMultiproc)            |
                                        UartPC_GET_UART_RX_CTRL_DROP_ON_PARITY_ERR(config->dropOnParityErr) |
                                        UartPC_GET_UART_RX_CTRL_DROP_ON_FRAME_ERR(config->dropOnFrameErr)   |
                                        UartPC_GET_UART_RX_CTRL_BREAK_WIDTH(config->breakWidth);

            if(UartPC_UART_PARITY_NONE != config->parity)
            {
               UartPC_UART_RX_CTRL_REG |= UartPC_GET_UART_RX_CTRL_PARITY(config->parity) |
                                                    UartPC_UART_RX_CTRL_PARITY_ENABLED;
            }

            UartPC_RX_CTRL_REG      = UartPC_GET_RX_CTRL_DATA_WIDTH(config->dataBits)       |
                                                UartPC_GET_RX_CTRL_MEDIAN(config->enableMedianFilter) |
                                                UartPC_GET_UART_RX_CTRL_ENABLED(config->direction);

            UartPC_RX_FIFO_CTRL_REG = UartPC_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(config->rxTriggerLevel);

            /* Configure MP address */
            UartPC_RX_MATCH_REG     = UartPC_GET_RX_MATCH_ADDR(config->multiprocAddr) |
                                                UartPC_GET_RX_MATCH_MASK(config->multiprocAddrMask);

            /* Configure RX direction */
            UartPC_UART_TX_CTRL_REG = UartPC_GET_UART_TX_CTRL_MODE(config->stopBits) |
                                                UartPC_GET_UART_TX_CTRL_RETRY_NACK(config->enableRetryNack);

            if(UartPC_UART_PARITY_NONE != config->parity)
            {
               UartPC_UART_TX_CTRL_REG |= UartPC_GET_UART_TX_CTRL_PARITY(config->parity) |
                                                    UartPC_UART_TX_CTRL_PARITY_ENABLED;
            }

            UartPC_TX_CTRL_REG      = UartPC_GET_TX_CTRL_DATA_WIDTH(config->dataBits)    |
                                                UartPC_GET_UART_TX_CTRL_ENABLED(config->direction);

            UartPC_TX_FIFO_CTRL_REG = UartPC_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(config->txTriggerLevel);

        #if !(UartPC_CY_SCBIP_V0 || UartPC_CY_SCBIP_V1)
            UartPC_UART_FLOW_CTRL_REG = UartPC_GET_UART_FLOW_CTRL_CTS_ENABLE(config->enableCts) | \
                                            UartPC_GET_UART_FLOW_CTRL_CTS_POLARITY (config->ctsPolarity)  | \
                                            UartPC_GET_UART_FLOW_CTRL_RTS_POLARITY (config->rtsPolarity)  | \
                                            UartPC_GET_UART_FLOW_CTRL_TRIGGER_LEVEL(config->rtsRxFifoLevel);
        #endif /* !(UartPC_CY_SCBIP_V0 || UartPC_CY_SCBIP_V1) */

            /* Configure interrupt with UART handler but do not enable it */
            CyIntDisable    (UartPC_ISR_NUMBER);
            CyIntSetPriority(UartPC_ISR_NUMBER, UartPC_ISR_PRIORITY);
            (void) CyIntSetVector(UartPC_ISR_NUMBER, &UartPC_SPI_UART_ISR);

            /* Configure WAKE interrupt */
        #if(UartPC_UART_RX_WAKEUP_IRQ)
            CyIntDisable    (UartPC_RX_WAKE_ISR_NUMBER);
            CyIntSetPriority(UartPC_RX_WAKE_ISR_NUMBER, UartPC_RX_WAKE_ISR_PRIORITY);
            (void) CyIntSetVector(UartPC_RX_WAKE_ISR_NUMBER, &UartPC_UART_WAKEUP_ISR);
        #endif /* (UartPC_UART_RX_WAKEUP_IRQ) */

            /* Configure interrupt sources */
            UartPC_INTR_I2C_EC_MASK_REG = UartPC_NO_INTR_SOURCES;
            UartPC_INTR_SPI_EC_MASK_REG = UartPC_NO_INTR_SOURCES;
            UartPC_INTR_SLAVE_MASK_REG  = UartPC_NO_INTR_SOURCES;
            UartPC_INTR_MASTER_MASK_REG = UartPC_NO_INTR_SOURCES;
            UartPC_INTR_RX_MASK_REG     = config->rxInterruptMask;
            UartPC_INTR_TX_MASK_REG     = config->txInterruptMask;

            /* Configure TX interrupt sources to restore. */
            UartPC_IntrTxMask = LO16(UartPC_INTR_TX_MASK_REG);

            /* Clear RX buffer indexes */
            UartPC_rxBufferHead     = 0u;
            UartPC_rxBufferTail     = 0u;
            UartPC_rxBufferOverflow = 0u;

            /* Clear TX buffer indexes */
            UartPC_txBufferHead = 0u;
            UartPC_txBufferTail = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: UartPC_UartInit
    ****************************************************************************//**
    *
    *  Configures the SCB for the UART operation.
    *
    *******************************************************************************/
    void UartPC_UartInit(void)
    {
        /* Configure UART interface */
        UartPC_CTRL_REG = UartPC_UART_DEFAULT_CTRL;

        /* Configure sub-mode: UART, SmartCard or IrDA */
        UartPC_UART_CTRL_REG = UartPC_UART_DEFAULT_UART_CTRL;

        /* Configure RX direction */
        UartPC_UART_RX_CTRL_REG = UartPC_UART_DEFAULT_UART_RX_CTRL;
        UartPC_RX_CTRL_REG      = UartPC_UART_DEFAULT_RX_CTRL;
        UartPC_RX_FIFO_CTRL_REG = UartPC_UART_DEFAULT_RX_FIFO_CTRL;
        UartPC_RX_MATCH_REG     = UartPC_UART_DEFAULT_RX_MATCH_REG;

        /* Configure TX direction */
        UartPC_UART_TX_CTRL_REG = UartPC_UART_DEFAULT_UART_TX_CTRL;
        UartPC_TX_CTRL_REG      = UartPC_UART_DEFAULT_TX_CTRL;
        UartPC_TX_FIFO_CTRL_REG = UartPC_UART_DEFAULT_TX_FIFO_CTRL;

    #if !(UartPC_CY_SCBIP_V0 || UartPC_CY_SCBIP_V1)
        UartPC_UART_FLOW_CTRL_REG = UartPC_UART_DEFAULT_FLOW_CTRL;
    #endif /* !(UartPC_CY_SCBIP_V0 || UartPC_CY_SCBIP_V1) */

        /* Configure interrupt with UART handler but do not enable it */
    #if(UartPC_SCB_IRQ_INTERNAL)
        CyIntDisable    (UartPC_ISR_NUMBER);
        CyIntSetPriority(UartPC_ISR_NUMBER, UartPC_ISR_PRIORITY);
        (void) CyIntSetVector(UartPC_ISR_NUMBER, &UartPC_SPI_UART_ISR);
    #endif /* (UartPC_SCB_IRQ_INTERNAL) */

        /* Configure WAKE interrupt */
    #if(UartPC_UART_RX_WAKEUP_IRQ)
        CyIntDisable    (UartPC_RX_WAKE_ISR_NUMBER);
        CyIntSetPriority(UartPC_RX_WAKE_ISR_NUMBER, UartPC_RX_WAKE_ISR_PRIORITY);
        (void) CyIntSetVector(UartPC_RX_WAKE_ISR_NUMBER, &UartPC_UART_WAKEUP_ISR);
    #endif /* (UartPC_UART_RX_WAKEUP_IRQ) */

        /* Configure interrupt sources */
        UartPC_INTR_I2C_EC_MASK_REG = UartPC_UART_DEFAULT_INTR_I2C_EC_MASK;
        UartPC_INTR_SPI_EC_MASK_REG = UartPC_UART_DEFAULT_INTR_SPI_EC_MASK;
        UartPC_INTR_SLAVE_MASK_REG  = UartPC_UART_DEFAULT_INTR_SLAVE_MASK;
        UartPC_INTR_MASTER_MASK_REG = UartPC_UART_DEFAULT_INTR_MASTER_MASK;
        UartPC_INTR_RX_MASK_REG     = UartPC_UART_DEFAULT_INTR_RX_MASK;
        UartPC_INTR_TX_MASK_REG     = UartPC_UART_DEFAULT_INTR_TX_MASK;

        /* Configure TX interrupt sources to restore. */
        UartPC_IntrTxMask = LO16(UartPC_INTR_TX_MASK_REG);

    #if(UartPC_INTERNAL_RX_SW_BUFFER_CONST)
        UartPC_rxBufferHead     = 0u;
        UartPC_rxBufferTail     = 0u;
        UartPC_rxBufferOverflow = 0u;
    #endif /* (UartPC_INTERNAL_RX_SW_BUFFER_CONST) */

    #if(UartPC_INTERNAL_TX_SW_BUFFER_CONST)
        UartPC_txBufferHead = 0u;
        UartPC_txBufferTail = 0u;
    #endif /* (UartPC_INTERNAL_TX_SW_BUFFER_CONST) */
    }
#endif /* (UartPC_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: UartPC_UartPostEnable
****************************************************************************//**
*
*  Restores HSIOM settings for the UART output pins (TX and/or RTS) to be
*  controlled by the SCB UART.
*
*******************************************************************************/
void UartPC_UartPostEnable(void)
{
#if (UartPC_SCB_MODE_UNCONFIG_CONST_CFG)
    #if (UartPC_TX_SDA_MISO_PIN)
        if (UartPC_CHECK_TX_SDA_MISO_PIN_USED)
        {
            /* Set SCB UART to drive the output pin */
            UartPC_SET_HSIOM_SEL(UartPC_TX_SDA_MISO_HSIOM_REG, UartPC_TX_SDA_MISO_HSIOM_MASK,
                                           UartPC_TX_SDA_MISO_HSIOM_POS, UartPC_TX_SDA_MISO_HSIOM_SEL_UART);
        }
    #endif /* (UartPC_TX_SDA_MISO_PIN_PIN) */

    #if !(UartPC_CY_SCBIP_V0 || UartPC_CY_SCBIP_V1)
        #if (UartPC_RTS_SS0_PIN)
            if (UartPC_CHECK_RTS_SS0_PIN_USED)
            {
                /* Set SCB UART to drive the output pin */
                UartPC_SET_HSIOM_SEL(UartPC_RTS_SS0_HSIOM_REG, UartPC_RTS_SS0_HSIOM_MASK,
                                               UartPC_RTS_SS0_HSIOM_POS, UartPC_RTS_SS0_HSIOM_SEL_UART);
            }
        #endif /* (UartPC_RTS_SS0_PIN) */
    #endif /* !(UartPC_CY_SCBIP_V0 || UartPC_CY_SCBIP_V1) */

#else
    #if (UartPC_UART_TX_PIN)
         /* Set SCB UART to drive the output pin */
        UartPC_SET_HSIOM_SEL(UartPC_TX_HSIOM_REG, UartPC_TX_HSIOM_MASK,
                                       UartPC_TX_HSIOM_POS, UartPC_TX_HSIOM_SEL_UART);
    #endif /* (UartPC_UART_TX_PIN) */

    #if (UartPC_UART_RTS_PIN)
        /* Set SCB UART to drive the output pin */
        UartPC_SET_HSIOM_SEL(UartPC_RTS_HSIOM_REG, UartPC_RTS_HSIOM_MASK,
                                       UartPC_RTS_HSIOM_POS, UartPC_RTS_HSIOM_SEL_UART);
    #endif /* (UartPC_UART_RTS_PIN) */
#endif /* (UartPC_SCB_MODE_UNCONFIG_CONST_CFG) */

    /* Restore TX interrupt sources. */
    UartPC_SetTxInterruptMode(UartPC_IntrTxMask);
}


/*******************************************************************************
* Function Name: UartPC_UartStop
****************************************************************************//**
*
*  Changes the HSIOM settings for the UART output pins (TX and/or RTS) to keep
*  them inactive after the block is disabled. The output pins are controlled by
*  the GPIO data register. Also, the function disables the skip start feature
*  to not cause it to trigger after the component is enabled.
*
*******************************************************************************/
void UartPC_UartStop(void)
{
#if(UartPC_SCB_MODE_UNCONFIG_CONST_CFG)
    #if (UartPC_TX_SDA_MISO_PIN)
        if (UartPC_CHECK_TX_SDA_MISO_PIN_USED)
        {
            /* Set GPIO to drive output pin */
            UartPC_SET_HSIOM_SEL(UartPC_TX_SDA_MISO_HSIOM_REG, UartPC_TX_SDA_MISO_HSIOM_MASK,
                                           UartPC_TX_SDA_MISO_HSIOM_POS, UartPC_TX_SDA_MISO_HSIOM_SEL_GPIO);
        }
    #endif /* (UartPC_TX_SDA_MISO_PIN_PIN) */

    #if !(UartPC_CY_SCBIP_V0 || UartPC_CY_SCBIP_V1)
        #if (UartPC_RTS_SS0_PIN)
            if (UartPC_CHECK_RTS_SS0_PIN_USED)
            {
                /* Set output pin state after block is disabled */
                UartPC_uart_rts_spi_ss0_Write(UartPC_GET_UART_RTS_INACTIVE);

                /* Set GPIO to drive output pin */
                UartPC_SET_HSIOM_SEL(UartPC_RTS_SS0_HSIOM_REG, UartPC_RTS_SS0_HSIOM_MASK,
                                               UartPC_RTS_SS0_HSIOM_POS, UartPC_RTS_SS0_HSIOM_SEL_GPIO);
            }
        #endif /* (UartPC_RTS_SS0_PIN) */
    #endif /* !(UartPC_CY_SCBIP_V0 || UartPC_CY_SCBIP_V1) */

#else
    #if (UartPC_UART_TX_PIN)
        /* Set GPIO to drive output pin */
        UartPC_SET_HSIOM_SEL(UartPC_TX_HSIOM_REG, UartPC_TX_HSIOM_MASK,
                                       UartPC_TX_HSIOM_POS, UartPC_TX_HSIOM_SEL_GPIO);
    #endif /* (UartPC_UART_TX_PIN) */

    #if (UartPC_UART_RTS_PIN)
        /* Set output pin state after block is disabled */
        UartPC_rts_Write(UartPC_GET_UART_RTS_INACTIVE);

        /* Set GPIO to drive output pin */
        UartPC_SET_HSIOM_SEL(UartPC_RTS_HSIOM_REG, UartPC_RTS_HSIOM_MASK,
                                       UartPC_RTS_HSIOM_POS, UartPC_RTS_HSIOM_SEL_GPIO);
    #endif /* (UartPC_UART_RTS_PIN) */

#endif /* (UartPC_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (UartPC_UART_WAKE_ENABLE_CONST)
    /* Disable skip start feature used for wakeup */
    UartPC_UART_RX_CTRL_REG &= (uint32) ~UartPC_UART_RX_CTRL_SKIP_START;
#endif /* (UartPC_UART_WAKE_ENABLE_CONST) */

    /* Store TX interrupt sources (exclude level triggered). */
    UartPC_IntrTxMask = LO16(UartPC_GetTxInterruptMode() & UartPC_INTR_UART_TX_RESTORE);
}


/*******************************************************************************
* Function Name: UartPC_UartSetRxAddress
****************************************************************************//**
*
*  Sets the hardware detectable receiver address for the UART in the
*  Multiprocessor mode.
*
*  \param address: Address for hardware address detection.
*
*******************************************************************************/
void UartPC_UartSetRxAddress(uint32 address)
{
     uint32 matchReg;

    matchReg = UartPC_RX_MATCH_REG;

    matchReg &= ((uint32) ~UartPC_RX_MATCH_ADDR_MASK); /* Clear address bits */
    matchReg |= ((uint32)  (address & UartPC_RX_MATCH_ADDR_MASK)); /* Set address  */

    UartPC_RX_MATCH_REG = matchReg;
}


/*******************************************************************************
* Function Name: UartPC_UartSetRxAddressMask
****************************************************************************//**
*
*  Sets the hardware address mask for the UART in the Multiprocessor mode.
*
*  \param addressMask: Address mask.
*   - Bit value 0 – excludes bit from address comparison.
*   - Bit value 1 – the bit needs to match with the corresponding bit
*     of the address.
*
*******************************************************************************/
void UartPC_UartSetRxAddressMask(uint32 addressMask)
{
    uint32 matchReg;

    matchReg = UartPC_RX_MATCH_REG;

    matchReg &= ((uint32) ~UartPC_RX_MATCH_MASK_MASK); /* Clear address mask bits */
    matchReg |= ((uint32) (addressMask << UartPC_RX_MATCH_MASK_POS));

    UartPC_RX_MATCH_REG = matchReg;
}


#if(UartPC_UART_RX_DIRECTION)
    /*******************************************************************************
    * Function Name: UartPC_UartGetChar
    ****************************************************************************//**
    *
    *  Retrieves next data element from receive buffer.
    *  This function is designed for ASCII characters and returns a char where
    *  1 to 255 are valid characters and 0 indicates an error occurred or no data
    *  is present.
    *  - RX software buffer is disabled: Returns data element retrieved from RX
    *    FIFO.
    *  - RX software buffer is enabled: Returns data element from the software
    *    receive buffer.
    *
    *  \return
    *   Next data element from the receive buffer. ASCII character values from
    *   1 to 255 are valid. A returned zero signifies an error condition or no
    *   data available.
    *
    *  \sideeffect
    *   The errors bits may not correspond with reading characters due to
    *   RX FIFO and software buffer usage.
    *   RX software buffer is enabled: The internal software buffer overflow
    *   is not treated as an error condition.
    *   Check UartPC_rxBufferOverflow to capture that error condition.
    *
    *******************************************************************************/
    uint32 UartPC_UartGetChar(void)
    {
        uint32 rxData = 0u;

        /* Reads data only if there is data to read */
        if (0u != UartPC_SpiUartGetRxBufferSize())
        {
            rxData = UartPC_SpiUartReadRxData();
        }

        if (UartPC_CHECK_INTR_RX(UartPC_INTR_RX_ERR))
        {
            rxData = 0u; /* Error occurred: returns zero */
            UartPC_ClearRxInterruptSource(UartPC_INTR_RX_ERR);
        }

        return (rxData);
    }


    /*******************************************************************************
    * Function Name: UartPC_UartGetByte
    ****************************************************************************//**
    *
    *  Retrieves the next data element from the receive buffer, returns the
    *  received byte and error condition.
    *   - The RX software buffer is disabled: returns the data element retrieved
    *     from the RX FIFO. Undefined data will be returned if the RX FIFO is
    *     empty.
    *   - The RX software buffer is enabled: returns data element from the
    *     software receive buffer.
    *
    *  \return
    *   Bits 7-0 contain the next data element from the receive buffer and
    *   other bits contain the error condition.
    *   - UartPC_UART_RX_OVERFLOW - Attempt to write to a full
    *     receiver FIFO.
    *   - UartPC_UART_RX_UNDERFLOW    Attempt to read from an empty
    *     receiver FIFO.
    *   - UartPC_UART_RX_FRAME_ERROR - UART framing error detected.
    *   - UartPC_UART_RX_PARITY_ERROR - UART parity error detected.
    *
    *  \sideeffect
    *   The errors bits may not correspond with reading characters due to
    *   RX FIFO and software buffer usage.
    *   RX software buffer is enabled: The internal software buffer overflow
    *   is not treated as an error condition.
    *   Check UartPC_rxBufferOverflow to capture that error condition.
    *
    *******************************************************************************/
    uint32 UartPC_UartGetByte(void)
    {
        uint32 rxData;
        uint32 tmpStatus;

        #if (UartPC_CHECK_RX_SW_BUFFER)
        {
            UartPC_DisableInt();
        }
        #endif

        if (0u != UartPC_SpiUartGetRxBufferSize())
        {
            /* Enables interrupt to receive more bytes: at least one byte is in
            * buffer.
            */
            #if (UartPC_CHECK_RX_SW_BUFFER)
            {
                UartPC_EnableInt();
            }
            #endif

            /* Get received byte */
            rxData = UartPC_SpiUartReadRxData();
        }
        else
        {
            /* Reads a byte directly from RX FIFO: underflow is raised in the
            * case of empty. Otherwise the first received byte will be read.
            */
            rxData = UartPC_RX_FIFO_RD_REG;


            /* Enables interrupt to receive more bytes. */
            #if (UartPC_CHECK_RX_SW_BUFFER)
            {

                /* The byte has been read from RX FIFO. Clear RX interrupt to
                * not involve interrupt handler when RX FIFO is empty.
                */
                UartPC_ClearRxInterruptSource(UartPC_INTR_RX_NOT_EMPTY);

                UartPC_EnableInt();
            }
            #endif
        }

        /* Get and clear RX error mask */
        tmpStatus = (UartPC_GetRxInterruptSource() & UartPC_INTR_RX_ERR);
        UartPC_ClearRxInterruptSource(UartPC_INTR_RX_ERR);

        /* Puts together data and error status:
        * MP mode and accept address: 9th bit is set to notify mark.
        */
        rxData |= ((uint32) (tmpStatus << 8u));

        return (rxData);
    }


    #if !(UartPC_CY_SCBIP_V0 || UartPC_CY_SCBIP_V1)
        /*******************************************************************************
        * Function Name: UartPC_UartSetRtsPolarity
        ****************************************************************************//**
        *
        *  Sets active polarity of RTS output signal.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *  \param polarity: Active polarity of RTS output signal.
        *   - UartPC_UART_RTS_ACTIVE_LOW  - RTS signal is active low.
        *   - UartPC_UART_RTS_ACTIVE_HIGH - RTS signal is active high.
        *
        *******************************************************************************/
        void UartPC_UartSetRtsPolarity(uint32 polarity)
        {
            if(0u != polarity)
            {
                UartPC_UART_FLOW_CTRL_REG |= (uint32)  UartPC_UART_FLOW_CTRL_RTS_POLARITY;
            }
            else
            {
                UartPC_UART_FLOW_CTRL_REG &= (uint32) ~UartPC_UART_FLOW_CTRL_RTS_POLARITY;
            }
        }


        /*******************************************************************************
        * Function Name: UartPC_UartSetRtsFifoLevel
        ****************************************************************************//**
        *
        *  Sets level in the RX FIFO for RTS signal activation.
        *  While the RX FIFO has fewer entries than the RX FIFO level the RTS signal
        *  remains active, otherwise the RTS signal becomes inactive.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *  \param level: Level in the RX FIFO for RTS signal activation.
        *   The range of valid level values is between 0 and RX FIFO depth - 1.
        *   Setting level value to 0 disables RTS signal activation.
        *
        *******************************************************************************/
        void UartPC_UartSetRtsFifoLevel(uint32 level)
        {
            uint32 uartFlowCtrl;

            uartFlowCtrl = UartPC_UART_FLOW_CTRL_REG;

            uartFlowCtrl &= ((uint32) ~UartPC_UART_FLOW_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
            uartFlowCtrl |= ((uint32) (UartPC_UART_FLOW_CTRL_TRIGGER_LEVEL_MASK & level));

            UartPC_UART_FLOW_CTRL_REG = uartFlowCtrl;
        }
    #endif /* !(UartPC_CY_SCBIP_V0 || UartPC_CY_SCBIP_V1) */

#endif /* (UartPC_UART_RX_DIRECTION) */


#if(UartPC_UART_TX_DIRECTION)
    /*******************************************************************************
    * Function Name: UartPC_UartPutString
    ****************************************************************************//**
    *
    *  Places a NULL terminated string in the transmit buffer to be sent at the
    *  next available bus time.
    *  This function is blocking and waits until there is a space available to put
    *  requested data in transmit buffer.
    *
    *  \param string: pointer to the null terminated string array to be placed in the
    *   transmit buffer.
    *
    *******************************************************************************/
    void UartPC_UartPutString(const char8 string[])
    {
        uint32 bufIndex;

        bufIndex = 0u;

        /* Blocks the control flow until all data has been sent */
        while(string[bufIndex] != ((char8) 0))
        {
            UartPC_UartPutChar((uint32) string[bufIndex]);
            bufIndex++;
        }
    }


    /*******************************************************************************
    * Function Name: UartPC_UartPutCRLF
    ****************************************************************************//**
    *
    *  Places byte of data followed by a carriage return (0x0D) and line feed
    *  (0x0A) in the transmit buffer.
    *  This function is blocking and waits until there is a space available to put
    *  all requested data in transmit buffer.
    *
    *  \param txDataByte: the data to be transmitted.
    *
    *******************************************************************************/
    void UartPC_UartPutCRLF(uint32 txDataByte)
    {
        UartPC_UartPutChar(txDataByte);  /* Blocks control flow until all data has been sent */
        UartPC_UartPutChar(0x0Du);       /* Blocks control flow until all data has been sent */
        UartPC_UartPutChar(0x0Au);       /* Blocks control flow until all data has been sent */
    }


    #if !(UartPC_CY_SCBIP_V0 || UartPC_CY_SCBIP_V1)
        /*******************************************************************************
        * Function Name: UartPCSCB_UartEnableCts
        ****************************************************************************//**
        *
        *  Enables usage of CTS input signal by the UART transmitter.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *******************************************************************************/
        void UartPC_UartEnableCts(void)
        {
            UartPC_UART_FLOW_CTRL_REG |= (uint32)  UartPC_UART_FLOW_CTRL_CTS_ENABLE;
        }


        /*******************************************************************************
        * Function Name: UartPC_UartDisableCts
        ****************************************************************************//**
        *
        *  Disables usage of CTS input signal by the UART transmitter.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        *******************************************************************************/
        void UartPC_UartDisableCts(void)
        {
            UartPC_UART_FLOW_CTRL_REG &= (uint32) ~UartPC_UART_FLOW_CTRL_CTS_ENABLE;
        }


        /*******************************************************************************
        * Function Name: UartPC_UartSetCtsPolarity
        ****************************************************************************//**
        *
        *  Sets active polarity of CTS input signal.
        *  Only available for PSoC 4100 BLE / PSoC 4200 BLE / PSoC 4100M / PSoC 4200M /
        *  PSoC 4200L / PSoC 4000S / PSoC 4100S / PSoC Analog Coprocessor devices.
        *
        * \param
        * polarity: Active polarity of CTS output signal.
        *   - UartPC_UART_CTS_ACTIVE_LOW  - CTS signal is active low.
        *   - UartPC_UART_CTS_ACTIVE_HIGH - CTS signal is active high.
        *
        *******************************************************************************/
        void UartPC_UartSetCtsPolarity(uint32 polarity)
        {
            if (0u != polarity)
            {
                UartPC_UART_FLOW_CTRL_REG |= (uint32)  UartPC_UART_FLOW_CTRL_CTS_POLARITY;
            }
            else
            {
                UartPC_UART_FLOW_CTRL_REG &= (uint32) ~UartPC_UART_FLOW_CTRL_CTS_POLARITY;
            }
        }
    #endif /* !(UartPC_CY_SCBIP_V0 || UartPC_CY_SCBIP_V1) */


    /*******************************************************************************
    * Function Name: UartPC_UartSendBreakBlocking
    ****************************************************************************//**
    *
    * Sends a break condition (logic low) of specified width on UART TX line.
    * Blocks until break is completed. Only call this function when UART TX FIFO
    * and shifter are empty.
    *
    * \param breakWidth
    * Width of break condition. Valid range is 4 to 16 bits.
    *
    * \note
    * Before sending break all UART TX interrupt sources are disabled. The state
    * of UART TX interrupt sources is restored before function returns.
    *
    * \sideeffect
    * If this function is called while there is data in the TX FIFO or shifter that
    * data will be shifted out in packets the size of breakWidth.
    *
    *******************************************************************************/
    void UartPC_UartSendBreakBlocking(uint32 breakWidth)
    {
        uint32 txCtrlReg;
        uint32 txIntrReg;

        /* Disable all UART TX interrupt source and clear UART TX Done history */
        txIntrReg = UartPC_GetTxInterruptMode();
        UartPC_SetTxInterruptMode(0u);
        UartPC_ClearTxInterruptSource(UartPC_INTR_TX_UART_DONE);

        /* Store TX CTRL configuration */
        txCtrlReg = UartPC_TX_CTRL_REG;

        /* Set break width */
        UartPC_TX_CTRL_REG = (UartPC_TX_CTRL_REG & (uint32) ~UartPC_TX_CTRL_DATA_WIDTH_MASK) |
                                        UartPC_GET_TX_CTRL_DATA_WIDTH(breakWidth);

        /* Generate break */
        UartPC_TX_FIFO_WR_REG = 0u;

        /* Wait for break completion */
        while (0u == (UartPC_GetTxInterruptSource() & UartPC_INTR_TX_UART_DONE))
        {
        }

        /* Clear all UART TX interrupt sources to  */
        UartPC_ClearTxInterruptSource(UartPC_INTR_TX_ALL);

        /* Restore TX interrupt sources and data width */
        UartPC_TX_CTRL_REG = txCtrlReg;
        UartPC_SetTxInterruptMode(txIntrReg);
    }
#endif /* (UartPC_UART_TX_DIRECTION) */


#if (UartPC_UART_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: UartPC_UartSaveConfig
    ****************************************************************************//**
    *
    *  Clears and enables an interrupt on a falling edge of the Rx input. The GPIO
    *  interrupt does not track in the active mode, therefore requires to be
    *  cleared by this API.
    *
    *******************************************************************************/
    void UartPC_UartSaveConfig(void)
    {
    #if (UartPC_UART_RX_WAKEUP_IRQ)
        /* Set SKIP_START if requested (set by default). */
        if (0u != UartPC_skipStart)
        {
            UartPC_UART_RX_CTRL_REG |= (uint32)  UartPC_UART_RX_CTRL_SKIP_START;
        }
        else
        {
            UartPC_UART_RX_CTRL_REG &= (uint32) ~UartPC_UART_RX_CTRL_SKIP_START;
        }

        /* Clear RX GPIO interrupt status and pending interrupt in NVIC because
        * falling edge on RX line occurs while UART communication in active mode.
        * Enable interrupt: next interrupt trigger should wakeup device.
        */
        UartPC_CLEAR_UART_RX_WAKE_INTR;
        UartPC_RxWakeClearPendingInt();
        UartPC_RxWakeEnableInt();
    #endif /* (UartPC_UART_RX_WAKEUP_IRQ) */
    }


    /*******************************************************************************
    * Function Name: UartPC_UartRestoreConfig
    ****************************************************************************//**
    *
    *  Disables the RX GPIO interrupt. Until this function is called the interrupt
    *  remains active and triggers on every falling edge of the UART RX line.
    *
    *******************************************************************************/
    void UartPC_UartRestoreConfig(void)
    {
    #if (UartPC_UART_RX_WAKEUP_IRQ)
        /* Disable interrupt: no more triggers in active mode */
        UartPC_RxWakeDisableInt();
    #endif /* (UartPC_UART_RX_WAKEUP_IRQ) */
    }


    #if (UartPC_UART_RX_WAKEUP_IRQ)
        /*******************************************************************************
        * Function Name: UartPC_UART_WAKEUP_ISR
        ****************************************************************************//**
        *
        *  Handles the Interrupt Service Routine for the SCB UART mode GPIO wakeup
        *  event. This event is configured to trigger on a falling edge of the RX line.
        *
        *******************************************************************************/
        CY_ISR(UartPC_UART_WAKEUP_ISR)
        {
        #ifdef UartPC_UART_WAKEUP_ISR_ENTRY_CALLBACK
            UartPC_UART_WAKEUP_ISR_EntryCallback();
        #endif /* UartPC_UART_WAKEUP_ISR_ENTRY_CALLBACK */

            UartPC_CLEAR_UART_RX_WAKE_INTR;

        #ifdef UartPC_UART_WAKEUP_ISR_EXIT_CALLBACK
            UartPC_UART_WAKEUP_ISR_ExitCallback();
        #endif /* UartPC_UART_WAKEUP_ISR_EXIT_CALLBACK */
        }
    #endif /* (UartPC_UART_RX_WAKEUP_IRQ) */
#endif /* (UartPC_UART_RX_WAKEUP_IRQ) */


/* [] END OF FILE */
