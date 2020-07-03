/***************************************************************************//**
* \file UartPC.c
* \version 4.0
*
* \brief
*  This file provides the source code to the API for the SCB Component.
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

#if (UartPC_SCB_MODE_I2C_INC)
    #include "UartPC_I2C_PVT.h"
#endif /* (UartPC_SCB_MODE_I2C_INC) */

#if (UartPC_SCB_MODE_EZI2C_INC)
    #include "UartPC_EZI2C_PVT.h"
#endif /* (UartPC_SCB_MODE_EZI2C_INC) */

#if (UartPC_SCB_MODE_SPI_INC || UartPC_SCB_MODE_UART_INC)
    #include "UartPC_SPI_UART_PVT.h"
#endif /* (UartPC_SCB_MODE_SPI_INC || UartPC_SCB_MODE_UART_INC) */


/***************************************
*    Run Time Configuration Vars
***************************************/

/* Stores internal component configuration for Unconfigured mode */
#if (UartPC_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    uint8 UartPC_scbMode = UartPC_SCB_MODE_UNCONFIG;
    uint8 UartPC_scbEnableWake;
    uint8 UartPC_scbEnableIntr;

    /* I2C configuration variables */
    uint8 UartPC_mode;
    uint8 UartPC_acceptAddr;

    /* SPI/UART configuration variables */
    volatile uint8 * UartPC_rxBuffer;
    uint8  UartPC_rxDataBits;
    uint32 UartPC_rxBufferSize;

    volatile uint8 * UartPC_txBuffer;
    uint8  UartPC_txDataBits;
    uint32 UartPC_txBufferSize;

    /* EZI2C configuration variables */
    uint8 UartPC_numberOfAddr;
    uint8 UartPC_subAddrSize;
#endif /* (UartPC_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Common SCB Vars
***************************************/
/**
* \addtogroup group_general
* \{
*/

/** UartPC_initVar indicates whether the UartPC 
*  component has been initialized. The variable is initialized to 0 
*  and set to 1 the first time SCB_Start() is called. This allows 
*  the component to restart without reinitialization after the first 
*  call to the UartPC_Start() routine.
*
*  If re-initialization of the component is required, then the 
*  UartPC_Init() function can be called before the 
*  UartPC_Start() or UartPC_Enable() function.
*/
uint8 UartPC_initVar = 0u;


#if (! (UartPC_SCB_MODE_I2C_CONST_CFG || \
        UartPC_SCB_MODE_EZI2C_CONST_CFG))
    /** This global variable stores TX interrupt sources after 
    * UartPC_Stop() is called. Only these TX interrupt sources 
    * will be restored on a subsequent UartPC_Enable() call.
    */
    uint16 UartPC_IntrTxMask = 0u;
#endif /* (! (UartPC_SCB_MODE_I2C_CONST_CFG || \
              UartPC_SCB_MODE_EZI2C_CONST_CFG)) */
/** \} globals */

#if (UartPC_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_UartPC_CUSTOM_INTR_HANDLER)
    void (*UartPC_customIntrHandler)(void) = NULL;
#endif /* !defined (CY_REMOVE_UartPC_CUSTOM_INTR_HANDLER) */
#endif /* (UartPC_SCB_IRQ_INTERNAL) */


/***************************************
*    Private Function Prototypes
***************************************/

static void UartPC_ScbEnableIntr(void);
static void UartPC_ScbModeStop(void);
static void UartPC_ScbModePostEnable(void);


/*******************************************************************************
* Function Name: UartPC_Init
****************************************************************************//**
*
*  Initializes the UartPC component to operate in one of the selected
*  configurations: I2C, SPI, UART or EZI2C.
*  When the configuration is set to "Unconfigured SCB", this function does
*  not do any initialization. Use mode-specific initialization APIs instead:
*  UartPC_I2CInit, UartPC_SpiInit, 
*  UartPC_UartInit or UartPC_EzI2CInit.
*
*******************************************************************************/
void UartPC_Init(void)
{
#if (UartPC_SCB_MODE_UNCONFIG_CONST_CFG)
    if (UartPC_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        UartPC_initVar = 0u;
    }
    else
    {
        /* Initialization was done before this function call */
    }

#elif (UartPC_SCB_MODE_I2C_CONST_CFG)
    UartPC_I2CInit();

#elif (UartPC_SCB_MODE_SPI_CONST_CFG)
    UartPC_SpiInit();

#elif (UartPC_SCB_MODE_UART_CONST_CFG)
    UartPC_UartInit();

#elif (UartPC_SCB_MODE_EZI2C_CONST_CFG)
    UartPC_EzI2CInit();

#endif /* (UartPC_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: UartPC_Enable
****************************************************************************//**
*
*  Enables UartPC component operation: activates the hardware and 
*  internal interrupt. It also restores TX interrupt sources disabled after the 
*  UartPC_Stop() function was called (note that level-triggered TX 
*  interrupt sources remain disabled to not cause code lock-up).
*  For I2C and EZI2C modes the interrupt is internal and mandatory for 
*  operation. For SPI and UART modes the interrupt can be configured as none, 
*  internal or external.
*  The UartPC configuration should be not changed when the component
*  is enabled. Any configuration changes should be made after disabling the 
*  component.
*  When configuration is set to “Unconfigured UartPC”, the component 
*  must first be initialized to operate in one of the following configurations: 
*  I2C, SPI, UART or EZ I2C, using the mode-specific initialization API. 
*  Otherwise this function does not enable the component.
*
*******************************************************************************/
void UartPC_Enable(void)
{
#if (UartPC_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Enable SCB block, only if it is already configured */
    if (!UartPC_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        UartPC_CTRL_REG |= UartPC_CTRL_ENABLED;

        UartPC_ScbEnableIntr();

        /* Call PostEnable function specific to current operation mode */
        UartPC_ScbModePostEnable();
    }
#else
    UartPC_CTRL_REG |= UartPC_CTRL_ENABLED;

    UartPC_ScbEnableIntr();

    /* Call PostEnable function specific to current operation mode */
    UartPC_ScbModePostEnable();
#endif /* (UartPC_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: UartPC_Start
****************************************************************************//**
*
*  Invokes UartPC_Init() and UartPC_Enable().
*  After this function call, the component is enabled and ready for operation.
*  When configuration is set to "Unconfigured SCB", the component must first be
*  initialized to operate in one of the following configurations: I2C, SPI, UART
*  or EZI2C. Otherwise this function does not enable the component.
*
* \globalvars
*  UartPC_initVar - used to check initial configuration, modified
*  on first function call.
*
*******************************************************************************/
void UartPC_Start(void)
{
    if (0u == UartPC_initVar)
    {
        UartPC_Init();
        UartPC_initVar = 1u; /* Component was initialized */
    }

    UartPC_Enable();
}


/*******************************************************************************
* Function Name: UartPC_Stop
****************************************************************************//**
*
*  Disables the UartPC component: disable the hardware and internal 
*  interrupt. It also disables all TX interrupt sources so as not to cause an 
*  unexpected interrupt trigger because after the component is enabled, the 
*  TX FIFO is empty.
*  Refer to the function UartPC_Enable() for the interrupt 
*  configuration details.
*  This function disables the SCB component without checking to see if 
*  communication is in progress. Before calling this function it may be 
*  necessary to check the status of communication to make sure communication 
*  is complete. If this is not done then communication could be stopped mid 
*  byte and corrupted data could result.
*
*******************************************************************************/
void UartPC_Stop(void)
{
#if (UartPC_SCB_IRQ_INTERNAL)
    UartPC_DisableInt();
#endif /* (UartPC_SCB_IRQ_INTERNAL) */

    /* Call Stop function specific to current operation mode */
    UartPC_ScbModeStop();

    /* Disable SCB IP */
    UartPC_CTRL_REG &= (uint32) ~UartPC_CTRL_ENABLED;

    /* Disable all TX interrupt sources so as not to cause an unexpected
    * interrupt trigger after the component will be enabled because the 
    * TX FIFO is empty.
    * For SCB IP v0, it is critical as it does not mask-out interrupt
    * sources when it is disabled. This can cause a code lock-up in the
    * interrupt handler because TX FIFO cannot be loaded after the block
    * is disabled.
    */
    UartPC_SetTxInterruptMode(UartPC_NO_INTR_SOURCES);

#if (UartPC_SCB_IRQ_INTERNAL)
    UartPC_ClearPendingInt();
#endif /* (UartPC_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: UartPC_SetRxFifoLevel
****************************************************************************//**
*
*  Sets level in the RX FIFO to generate a RX level interrupt.
*  When the RX FIFO has more entries than the RX FIFO level an RX level
*  interrupt request is generated.
*
*  \param level: Level in the RX FIFO to generate RX level interrupt.
*   The range of valid level values is between 0 and RX FIFO depth - 1.
*
*******************************************************************************/
void UartPC_SetRxFifoLevel(uint32 level)
{
    uint32 rxFifoCtrl;

    rxFifoCtrl = UartPC_RX_FIFO_CTRL_REG;

    rxFifoCtrl &= ((uint32) ~UartPC_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    rxFifoCtrl |= ((uint32) (UartPC_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    UartPC_RX_FIFO_CTRL_REG = rxFifoCtrl;
}


/*******************************************************************************
* Function Name: UartPC_SetTxFifoLevel
****************************************************************************//**
*
*  Sets level in the TX FIFO to generate a TX level interrupt.
*  When the TX FIFO has less entries than the TX FIFO level an TX level
*  interrupt request is generated.
*
*  \param level: Level in the TX FIFO to generate TX level interrupt.
*   The range of valid level values is between 0 and TX FIFO depth - 1.
*
*******************************************************************************/
void UartPC_SetTxFifoLevel(uint32 level)
{
    uint32 txFifoCtrl;

    txFifoCtrl = UartPC_TX_FIFO_CTRL_REG;

    txFifoCtrl &= ((uint32) ~UartPC_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    txFifoCtrl |= ((uint32) (UartPC_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    UartPC_TX_FIFO_CTRL_REG = txFifoCtrl;
}


#if (UartPC_SCB_IRQ_INTERNAL)
    /*******************************************************************************
    * Function Name: UartPC_SetCustomInterruptHandler
    ****************************************************************************//**
    *
    *  Registers a function to be called by the internal interrupt handler.
    *  First the function that is registered is called, then the internal interrupt
    *  handler performs any operation such as software buffer management functions
    *  before the interrupt returns.  It is the user's responsibility not to break
    *  the software buffer operations. Only one custom handler is supported, which
    *  is the function provided by the most recent call.
    *  At the initialization time no custom handler is registered.
    *
    *  \param func: Pointer to the function to register.
    *        The value NULL indicates to remove the current custom interrupt
    *        handler.
    *
    *******************************************************************************/
    void UartPC_SetCustomInterruptHandler(void (*func)(void))
    {
    #if !defined (CY_REMOVE_UartPC_CUSTOM_INTR_HANDLER)
        UartPC_customIntrHandler = func; /* Register interrupt handler */
    #else
        if (NULL != func)
        {
            /* Suppress compiler warning */
        }
    #endif /* !defined (CY_REMOVE_UartPC_CUSTOM_INTR_HANDLER) */
    }
#endif /* (UartPC_SCB_IRQ_INTERNAL) */


/*******************************************************************************
* Function Name: UartPC_ScbModeEnableIntr
****************************************************************************//**
*
*  Enables an interrupt for a specific mode.
*
*******************************************************************************/
static void UartPC_ScbEnableIntr(void)
{
#if (UartPC_SCB_IRQ_INTERNAL)
    /* Enable interrupt in NVIC */
    #if (UartPC_SCB_MODE_UNCONFIG_CONST_CFG)
        if (0u != UartPC_scbEnableIntr)
        {
            UartPC_EnableInt();
        }

    #else
        UartPC_EnableInt();

    #endif /* (UartPC_SCB_MODE_UNCONFIG_CONST_CFG) */
#endif /* (UartPC_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: UartPC_ScbModePostEnable
****************************************************************************//**
*
*  Calls the PostEnable function for a specific operation mode.
*
*******************************************************************************/
static void UartPC_ScbModePostEnable(void)
{
#if (UartPC_SCB_MODE_UNCONFIG_CONST_CFG)
#if (!UartPC_CY_SCBIP_V1)
    if (UartPC_SCB_MODE_SPI_RUNTM_CFG)
    {
        UartPC_SpiPostEnable();
    }
    else if (UartPC_SCB_MODE_UART_RUNTM_CFG)
    {
        UartPC_UartPostEnable();
    }
    else
    {
        /* Unknown mode: do nothing */
    }
#endif /* (!UartPC_CY_SCBIP_V1) */

#elif (UartPC_SCB_MODE_SPI_CONST_CFG)
    UartPC_SpiPostEnable();

#elif (UartPC_SCB_MODE_UART_CONST_CFG)
    UartPC_UartPostEnable();

#else
    /* Unknown mode: do nothing */
#endif /* (UartPC_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: UartPC_ScbModeStop
****************************************************************************//**
*
*  Calls the Stop function for a specific operation mode.
*
*******************************************************************************/
static void UartPC_ScbModeStop(void)
{
#if (UartPC_SCB_MODE_UNCONFIG_CONST_CFG)
    if (UartPC_SCB_MODE_I2C_RUNTM_CFG)
    {
        UartPC_I2CStop();
    }
    else if (UartPC_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        UartPC_EzI2CStop();
    }
#if (!UartPC_CY_SCBIP_V1)
    else if (UartPC_SCB_MODE_SPI_RUNTM_CFG)
    {
        UartPC_SpiStop();
    }
    else if (UartPC_SCB_MODE_UART_RUNTM_CFG)
    {
        UartPC_UartStop();
    }
#endif /* (!UartPC_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
#elif (UartPC_SCB_MODE_I2C_CONST_CFG)
    UartPC_I2CStop();

#elif (UartPC_SCB_MODE_EZI2C_CONST_CFG)
    UartPC_EzI2CStop();

#elif (UartPC_SCB_MODE_SPI_CONST_CFG)
    UartPC_SpiStop();

#elif (UartPC_SCB_MODE_UART_CONST_CFG)
    UartPC_UartStop();

#else
    /* Unknown mode: do nothing */
#endif /* (UartPC_SCB_MODE_UNCONFIG_CONST_CFG) */
}


#if (UartPC_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: UartPC_SetPins
    ****************************************************************************//**
    *
    *  Sets the pins settings accordingly to the selected operation mode.
    *  Only available in the Unconfigured operation mode. The mode specific
    *  initialization function calls it.
    *  Pins configuration is set by PSoC Creator when a specific mode of operation
    *  is selected in design time.
    *
    *  \param mode:      Mode of SCB operation.
    *  \param subMode:   Sub-mode of SCB operation. It is only required for SPI and UART
    *             modes.
    *  \param uartEnableMask: enables TX or RX direction and RTS and CTS signals.
    *
    *******************************************************************************/
    void UartPC_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask)
    {
        uint32 pinsDm[UartPC_SCB_PINS_NUMBER];
        uint32 i;
        
    #if (!UartPC_CY_SCBIP_V1)
        uint32 pinsInBuf = 0u;
    #endif /* (!UartPC_CY_SCBIP_V1) */
        
        uint32 hsiomSel[UartPC_SCB_PINS_NUMBER] = 
        {
            UartPC_RX_SCL_MOSI_HSIOM_SEL_GPIO,
            UartPC_TX_SDA_MISO_HSIOM_SEL_GPIO,
            0u,
            0u,
            0u,
            0u,
            0u,
        };

    #if (UartPC_CY_SCBIP_V1)
        /* Supress compiler warning. */
        if ((0u == subMode) || (0u == uartEnableMask))
        {
        }
    #endif /* (UartPC_CY_SCBIP_V1) */

        /* Set default HSIOM to GPIO and Drive Mode to Analog Hi-Z */
        for (i = 0u; i < UartPC_SCB_PINS_NUMBER; i++)
        {
            pinsDm[i] = UartPC_PIN_DM_ALG_HIZ;
        }

        if ((UartPC_SCB_MODE_I2C   == mode) ||
            (UartPC_SCB_MODE_EZI2C == mode))
        {
        #if (UartPC_RX_SCL_MOSI_PIN)
            hsiomSel[UartPC_RX_SCL_MOSI_PIN_INDEX] = UartPC_RX_SCL_MOSI_HSIOM_SEL_I2C;
            pinsDm  [UartPC_RX_SCL_MOSI_PIN_INDEX] = UartPC_PIN_DM_OD_LO;
        #elif (UartPC_RX_WAKE_SCL_MOSI_PIN)
            hsiomSel[UartPC_RX_WAKE_SCL_MOSI_PIN_INDEX] = UartPC_RX_WAKE_SCL_MOSI_HSIOM_SEL_I2C;
            pinsDm  [UartPC_RX_WAKE_SCL_MOSI_PIN_INDEX] = UartPC_PIN_DM_OD_LO;
        #else
        #endif /* (UartPC_RX_SCL_MOSI_PIN) */
        
        #if (UartPC_TX_SDA_MISO_PIN)
            hsiomSel[UartPC_TX_SDA_MISO_PIN_INDEX] = UartPC_TX_SDA_MISO_HSIOM_SEL_I2C;
            pinsDm  [UartPC_TX_SDA_MISO_PIN_INDEX] = UartPC_PIN_DM_OD_LO;
        #endif /* (UartPC_TX_SDA_MISO_PIN) */
        }
    #if (!UartPC_CY_SCBIP_V1)
        else if (UartPC_SCB_MODE_SPI == mode)
        {
        #if (UartPC_RX_SCL_MOSI_PIN)
            hsiomSel[UartPC_RX_SCL_MOSI_PIN_INDEX] = UartPC_RX_SCL_MOSI_HSIOM_SEL_SPI;
        #elif (UartPC_RX_WAKE_SCL_MOSI_PIN)
            hsiomSel[UartPC_RX_WAKE_SCL_MOSI_PIN_INDEX] = UartPC_RX_WAKE_SCL_MOSI_HSIOM_SEL_SPI;
        #else
        #endif /* (UartPC_RX_SCL_MOSI_PIN) */
        
        #if (UartPC_TX_SDA_MISO_PIN)
            hsiomSel[UartPC_TX_SDA_MISO_PIN_INDEX] = UartPC_TX_SDA_MISO_HSIOM_SEL_SPI;
        #endif /* (UartPC_TX_SDA_MISO_PIN) */
        
        #if (UartPC_CTS_SCLK_PIN)
            hsiomSel[UartPC_CTS_SCLK_PIN_INDEX] = UartPC_CTS_SCLK_HSIOM_SEL_SPI;
        #endif /* (UartPC_CTS_SCLK_PIN) */

            if (UartPC_SPI_SLAVE == subMode)
            {
                /* Slave */
                pinsDm[UartPC_RX_SCL_MOSI_PIN_INDEX] = UartPC_PIN_DM_DIG_HIZ;
                pinsDm[UartPC_TX_SDA_MISO_PIN_INDEX] = UartPC_PIN_DM_STRONG;
                pinsDm[UartPC_CTS_SCLK_PIN_INDEX] = UartPC_PIN_DM_DIG_HIZ;

            #if (UartPC_RTS_SS0_PIN)
                /* Only SS0 is valid choice for Slave */
                hsiomSel[UartPC_RTS_SS0_PIN_INDEX] = UartPC_RTS_SS0_HSIOM_SEL_SPI;
                pinsDm  [UartPC_RTS_SS0_PIN_INDEX] = UartPC_PIN_DM_DIG_HIZ;
            #endif /* (UartPC_RTS_SS0_PIN) */

            #if (UartPC_TX_SDA_MISO_PIN)
                /* Disable input buffer */
                 pinsInBuf |= UartPC_TX_SDA_MISO_PIN_MASK;
            #endif /* (UartPC_TX_SDA_MISO_PIN) */
            }
            else 
            {
                /* (Master) */
                pinsDm[UartPC_RX_SCL_MOSI_PIN_INDEX] = UartPC_PIN_DM_STRONG;
                pinsDm[UartPC_TX_SDA_MISO_PIN_INDEX] = UartPC_PIN_DM_DIG_HIZ;
                pinsDm[UartPC_CTS_SCLK_PIN_INDEX] = UartPC_PIN_DM_STRONG;

            #if (UartPC_RTS_SS0_PIN)
                hsiomSel [UartPC_RTS_SS0_PIN_INDEX] = UartPC_RTS_SS0_HSIOM_SEL_SPI;
                pinsDm   [UartPC_RTS_SS0_PIN_INDEX] = UartPC_PIN_DM_STRONG;
                pinsInBuf |= UartPC_RTS_SS0_PIN_MASK;
            #endif /* (UartPC_RTS_SS0_PIN) */

            #if (UartPC_SS1_PIN)
                hsiomSel [UartPC_SS1_PIN_INDEX] = UartPC_SS1_HSIOM_SEL_SPI;
                pinsDm   [UartPC_SS1_PIN_INDEX] = UartPC_PIN_DM_STRONG;
                pinsInBuf |= UartPC_SS1_PIN_MASK;
            #endif /* (UartPC_SS1_PIN) */

            #if (UartPC_SS2_PIN)
                hsiomSel [UartPC_SS2_PIN_INDEX] = UartPC_SS2_HSIOM_SEL_SPI;
                pinsDm   [UartPC_SS2_PIN_INDEX] = UartPC_PIN_DM_STRONG;
                pinsInBuf |= UartPC_SS2_PIN_MASK;
            #endif /* (UartPC_SS2_PIN) */

            #if (UartPC_SS3_PIN)
                hsiomSel [UartPC_SS3_PIN_INDEX] = UartPC_SS3_HSIOM_SEL_SPI;
                pinsDm   [UartPC_SS3_PIN_INDEX] = UartPC_PIN_DM_STRONG;
                pinsInBuf |= UartPC_SS3_PIN_MASK;
            #endif /* (UartPC_SS3_PIN) */

                /* Disable input buffers */
            #if (UartPC_RX_SCL_MOSI_PIN)
                pinsInBuf |= UartPC_RX_SCL_MOSI_PIN_MASK;
            #elif (UartPC_RX_WAKE_SCL_MOSI_PIN)
                pinsInBuf |= UartPC_RX_WAKE_SCL_MOSI_PIN_MASK;
            #else
            #endif /* (UartPC_RX_SCL_MOSI_PIN) */

            #if (UartPC_CTS_SCLK_PIN)
                pinsInBuf |= UartPC_CTS_SCLK_PIN_MASK;
            #endif /* (UartPC_CTS_SCLK_PIN) */
            }
        }
        else /* UART */
        {
            if (UartPC_UART_MODE_SMARTCARD == subMode)
            {
                /* SmartCard */
            #if (UartPC_TX_SDA_MISO_PIN)
                hsiomSel[UartPC_TX_SDA_MISO_PIN_INDEX] = UartPC_TX_SDA_MISO_HSIOM_SEL_UART;
                pinsDm  [UartPC_TX_SDA_MISO_PIN_INDEX] = UartPC_PIN_DM_OD_LO;
            #endif /* (UartPC_TX_SDA_MISO_PIN) */
            }
            else /* Standard or IrDA */
            {
                if (0u != (UartPC_UART_RX_PIN_ENABLE & uartEnableMask))
                {
                #if (UartPC_RX_SCL_MOSI_PIN)
                    hsiomSel[UartPC_RX_SCL_MOSI_PIN_INDEX] = UartPC_RX_SCL_MOSI_HSIOM_SEL_UART;
                    pinsDm  [UartPC_RX_SCL_MOSI_PIN_INDEX] = UartPC_PIN_DM_DIG_HIZ;
                #elif (UartPC_RX_WAKE_SCL_MOSI_PIN)
                    hsiomSel[UartPC_RX_WAKE_SCL_MOSI_PIN_INDEX] = UartPC_RX_WAKE_SCL_MOSI_HSIOM_SEL_UART;
                    pinsDm  [UartPC_RX_WAKE_SCL_MOSI_PIN_INDEX] = UartPC_PIN_DM_DIG_HIZ;
                #else
                #endif /* (UartPC_RX_SCL_MOSI_PIN) */
                }

                if (0u != (UartPC_UART_TX_PIN_ENABLE & uartEnableMask))
                {
                #if (UartPC_TX_SDA_MISO_PIN)
                    hsiomSel[UartPC_TX_SDA_MISO_PIN_INDEX] = UartPC_TX_SDA_MISO_HSIOM_SEL_UART;
                    pinsDm  [UartPC_TX_SDA_MISO_PIN_INDEX] = UartPC_PIN_DM_STRONG;
                    
                    /* Disable input buffer */
                    pinsInBuf |= UartPC_TX_SDA_MISO_PIN_MASK;
                #endif /* (UartPC_TX_SDA_MISO_PIN) */
                }

            #if !(UartPC_CY_SCBIP_V0 || UartPC_CY_SCBIP_V1)
                if (UartPC_UART_MODE_STD == subMode)
                {
                    if (0u != (UartPC_UART_CTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* CTS input is multiplexed with SCLK */
                    #if (UartPC_CTS_SCLK_PIN)
                        hsiomSel[UartPC_CTS_SCLK_PIN_INDEX] = UartPC_CTS_SCLK_HSIOM_SEL_UART;
                        pinsDm  [UartPC_CTS_SCLK_PIN_INDEX] = UartPC_PIN_DM_DIG_HIZ;
                    #endif /* (UartPC_CTS_SCLK_PIN) */
                    }

                    if (0u != (UartPC_UART_RTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* RTS output is multiplexed with SS0 */
                    #if (UartPC_RTS_SS0_PIN)
                        hsiomSel[UartPC_RTS_SS0_PIN_INDEX] = UartPC_RTS_SS0_HSIOM_SEL_UART;
                        pinsDm  [UartPC_RTS_SS0_PIN_INDEX] = UartPC_PIN_DM_STRONG;
                        
                        /* Disable input buffer */
                        pinsInBuf |= UartPC_RTS_SS0_PIN_MASK;
                    #endif /* (UartPC_RTS_SS0_PIN) */
                    }
                }
            #endif /* !(UartPC_CY_SCBIP_V0 || UartPC_CY_SCBIP_V1) */
            }
        }
    #endif /* (!UartPC_CY_SCBIP_V1) */

    /* Configure pins: set HSIOM, DM and InputBufEnable */
    /* Note: the DR register settings do not effect the pin output if HSIOM is other than GPIO */

    #if (UartPC_RX_SCL_MOSI_PIN)
        UartPC_SET_HSIOM_SEL(UartPC_RX_SCL_MOSI_HSIOM_REG,
                                       UartPC_RX_SCL_MOSI_HSIOM_MASK,
                                       UartPC_RX_SCL_MOSI_HSIOM_POS,
                                        hsiomSel[UartPC_RX_SCL_MOSI_PIN_INDEX]);

        UartPC_uart_rx_i2c_scl_spi_mosi_SetDriveMode((uint8) pinsDm[UartPC_RX_SCL_MOSI_PIN_INDEX]);

        #if (!UartPC_CY_SCBIP_V1)
            UartPC_SET_INP_DIS(UartPC_uart_rx_i2c_scl_spi_mosi_INP_DIS,
                                         UartPC_uart_rx_i2c_scl_spi_mosi_MASK,
                                         (0u != (pinsInBuf & UartPC_RX_SCL_MOSI_PIN_MASK)));
        #endif /* (!UartPC_CY_SCBIP_V1) */
    
    #elif (UartPC_RX_WAKE_SCL_MOSI_PIN)
        UartPC_SET_HSIOM_SEL(UartPC_RX_WAKE_SCL_MOSI_HSIOM_REG,
                                       UartPC_RX_WAKE_SCL_MOSI_HSIOM_MASK,
                                       UartPC_RX_WAKE_SCL_MOSI_HSIOM_POS,
                                       hsiomSel[UartPC_RX_WAKE_SCL_MOSI_PIN_INDEX]);

        UartPC_uart_rx_wake_i2c_scl_spi_mosi_SetDriveMode((uint8)
                                                               pinsDm[UartPC_RX_WAKE_SCL_MOSI_PIN_INDEX]);

        UartPC_SET_INP_DIS(UartPC_uart_rx_wake_i2c_scl_spi_mosi_INP_DIS,
                                     UartPC_uart_rx_wake_i2c_scl_spi_mosi_MASK,
                                     (0u != (pinsInBuf & UartPC_RX_WAKE_SCL_MOSI_PIN_MASK)));

         /* Set interrupt on falling edge */
        UartPC_SET_INCFG_TYPE(UartPC_RX_WAKE_SCL_MOSI_INTCFG_REG,
                                        UartPC_RX_WAKE_SCL_MOSI_INTCFG_TYPE_MASK,
                                        UartPC_RX_WAKE_SCL_MOSI_INTCFG_TYPE_POS,
                                        UartPC_INTCFG_TYPE_FALLING_EDGE);
    #else
    #endif /* (UartPC_RX_WAKE_SCL_MOSI_PIN) */

    #if (UartPC_TX_SDA_MISO_PIN)
        UartPC_SET_HSIOM_SEL(UartPC_TX_SDA_MISO_HSIOM_REG,
                                       UartPC_TX_SDA_MISO_HSIOM_MASK,
                                       UartPC_TX_SDA_MISO_HSIOM_POS,
                                        hsiomSel[UartPC_TX_SDA_MISO_PIN_INDEX]);

        UartPC_uart_tx_i2c_sda_spi_miso_SetDriveMode((uint8) pinsDm[UartPC_TX_SDA_MISO_PIN_INDEX]);

    #if (!UartPC_CY_SCBIP_V1)
        UartPC_SET_INP_DIS(UartPC_uart_tx_i2c_sda_spi_miso_INP_DIS,
                                     UartPC_uart_tx_i2c_sda_spi_miso_MASK,
                                    (0u != (pinsInBuf & UartPC_TX_SDA_MISO_PIN_MASK)));
    #endif /* (!UartPC_CY_SCBIP_V1) */
    #endif /* (UartPC_RX_SCL_MOSI_PIN) */

    #if (UartPC_CTS_SCLK_PIN)
        UartPC_SET_HSIOM_SEL(UartPC_CTS_SCLK_HSIOM_REG,
                                       UartPC_CTS_SCLK_HSIOM_MASK,
                                       UartPC_CTS_SCLK_HSIOM_POS,
                                       hsiomSel[UartPC_CTS_SCLK_PIN_INDEX]);

        UartPC_uart_cts_spi_sclk_SetDriveMode((uint8) pinsDm[UartPC_CTS_SCLK_PIN_INDEX]);

        UartPC_SET_INP_DIS(UartPC_uart_cts_spi_sclk_INP_DIS,
                                     UartPC_uart_cts_spi_sclk_MASK,
                                     (0u != (pinsInBuf & UartPC_CTS_SCLK_PIN_MASK)));
    #endif /* (UartPC_CTS_SCLK_PIN) */

    #if (UartPC_RTS_SS0_PIN)
        UartPC_SET_HSIOM_SEL(UartPC_RTS_SS0_HSIOM_REG,
                                       UartPC_RTS_SS0_HSIOM_MASK,
                                       UartPC_RTS_SS0_HSIOM_POS,
                                       hsiomSel[UartPC_RTS_SS0_PIN_INDEX]);

        UartPC_uart_rts_spi_ss0_SetDriveMode((uint8) pinsDm[UartPC_RTS_SS0_PIN_INDEX]);

        UartPC_SET_INP_DIS(UartPC_uart_rts_spi_ss0_INP_DIS,
                                     UartPC_uart_rts_spi_ss0_MASK,
                                     (0u != (pinsInBuf & UartPC_RTS_SS0_PIN_MASK)));
    #endif /* (UartPC_RTS_SS0_PIN) */

    #if (UartPC_SS1_PIN)
        UartPC_SET_HSIOM_SEL(UartPC_SS1_HSIOM_REG,
                                       UartPC_SS1_HSIOM_MASK,
                                       UartPC_SS1_HSIOM_POS,
                                       hsiomSel[UartPC_SS1_PIN_INDEX]);

        UartPC_spi_ss1_SetDriveMode((uint8) pinsDm[UartPC_SS1_PIN_INDEX]);

        UartPC_SET_INP_DIS(UartPC_spi_ss1_INP_DIS,
                                     UartPC_spi_ss1_MASK,
                                     (0u != (pinsInBuf & UartPC_SS1_PIN_MASK)));
    #endif /* (UartPC_SS1_PIN) */

    #if (UartPC_SS2_PIN)
        UartPC_SET_HSIOM_SEL(UartPC_SS2_HSIOM_REG,
                                       UartPC_SS2_HSIOM_MASK,
                                       UartPC_SS2_HSIOM_POS,
                                       hsiomSel[UartPC_SS2_PIN_INDEX]);

        UartPC_spi_ss2_SetDriveMode((uint8) pinsDm[UartPC_SS2_PIN_INDEX]);

        UartPC_SET_INP_DIS(UartPC_spi_ss2_INP_DIS,
                                     UartPC_spi_ss2_MASK,
                                     (0u != (pinsInBuf & UartPC_SS2_PIN_MASK)));
    #endif /* (UartPC_SS2_PIN) */

    #if (UartPC_SS3_PIN)
        UartPC_SET_HSIOM_SEL(UartPC_SS3_HSIOM_REG,
                                       UartPC_SS3_HSIOM_MASK,
                                       UartPC_SS3_HSIOM_POS,
                                       hsiomSel[UartPC_SS3_PIN_INDEX]);

        UartPC_spi_ss3_SetDriveMode((uint8) pinsDm[UartPC_SS3_PIN_INDEX]);

        UartPC_SET_INP_DIS(UartPC_spi_ss3_INP_DIS,
                                     UartPC_spi_ss3_MASK,
                                     (0u != (pinsInBuf & UartPC_SS3_PIN_MASK)));
    #endif /* (UartPC_SS3_PIN) */
    }

#endif /* (UartPC_SCB_MODE_UNCONFIG_CONST_CFG) */


#if (UartPC_CY_SCBIP_V0 || UartPC_CY_SCBIP_V1)
    /*******************************************************************************
    * Function Name: UartPC_I2CSlaveNackGeneration
    ****************************************************************************//**
    *
    *  Sets command to generate NACK to the address or data.
    *
    *******************************************************************************/
    void UartPC_I2CSlaveNackGeneration(void)
    {
        /* Check for EC_AM toggle condition: EC_AM and clock stretching for address are enabled */
        if ((0u != (UartPC_CTRL_REG & UartPC_CTRL_EC_AM_MODE)) &&
            (0u == (UartPC_I2C_CTRL_REG & UartPC_I2C_CTRL_S_NOT_READY_ADDR_NACK)))
        {
            /* Toggle EC_AM before NACK generation */
            UartPC_CTRL_REG &= ~UartPC_CTRL_EC_AM_MODE;
            UartPC_CTRL_REG |=  UartPC_CTRL_EC_AM_MODE;
        }

        UartPC_I2C_SLAVE_CMD_REG = UartPC_I2C_SLAVE_CMD_S_NACK;
    }
#endif /* (UartPC_CY_SCBIP_V0 || UartPC_CY_SCBIP_V1) */


/* [] END OF FILE */
