/***************************************************************************//**
* \file UartPC_PM.c
* \version 4.0
*
* \brief
*  This file provides the source code to the Power Management support for
*  the SCB Component.
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

#include "UartPC.h"
#include "UartPC_PVT.h"

#if(UartPC_SCB_MODE_I2C_INC)
    #include "UartPC_I2C_PVT.h"
#endif /* (UartPC_SCB_MODE_I2C_INC) */

#if(UartPC_SCB_MODE_EZI2C_INC)
    #include "UartPC_EZI2C_PVT.h"
#endif /* (UartPC_SCB_MODE_EZI2C_INC) */

#if(UartPC_SCB_MODE_SPI_INC || UartPC_SCB_MODE_UART_INC)
    #include "UartPC_SPI_UART_PVT.h"
#endif /* (UartPC_SCB_MODE_SPI_INC || UartPC_SCB_MODE_UART_INC) */


/***************************************
*   Backup Structure declaration
***************************************/

#if(UartPC_SCB_MODE_UNCONFIG_CONST_CFG || \
   (UartPC_SCB_MODE_I2C_CONST_CFG   && (!UartPC_I2C_WAKE_ENABLE_CONST))   || \
   (UartPC_SCB_MODE_EZI2C_CONST_CFG && (!UartPC_EZI2C_WAKE_ENABLE_CONST)) || \
   (UartPC_SCB_MODE_SPI_CONST_CFG   && (!UartPC_SPI_WAKE_ENABLE_CONST))   || \
   (UartPC_SCB_MODE_UART_CONST_CFG  && (!UartPC_UART_WAKE_ENABLE_CONST)))

    UartPC_BACKUP_STRUCT UartPC_backup =
    {
        0u, /* enableState */
    };
#endif


/*******************************************************************************
* Function Name: UartPC_Sleep
****************************************************************************//**
*
*  Prepares the UartPC component to enter Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has an influence on this 
*  function implementation:
*  - Checked: configures the component to be wakeup source from Deep Sleep.
*  - Unchecked: stores the current component state (enabled or disabled) and 
*    disables the component. See SCB_Stop() function for details about component 
*    disabling.
*
*  Call the UartPC_Sleep() function before calling the 
*  CyPmSysDeepSleep() function. 
*  Refer to the PSoC Creator System Reference Guide for more information about 
*  power management functions and Low power section of this document for the 
*  selected mode.
*
*  This function should not be called before entering Sleep.
*
*******************************************************************************/
void UartPC_Sleep(void)
{
#if(UartPC_SCB_MODE_UNCONFIG_CONST_CFG)

    if(UartPC_SCB_WAKE_ENABLE_CHECK)
    {
        if(UartPC_SCB_MODE_I2C_RUNTM_CFG)
        {
            UartPC_I2CSaveConfig();
        }
        else if(UartPC_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            UartPC_EzI2CSaveConfig();
        }
    #if(!UartPC_CY_SCBIP_V1)
        else if(UartPC_SCB_MODE_SPI_RUNTM_CFG)
        {
            UartPC_SpiSaveConfig();
        }
        else if(UartPC_SCB_MODE_UART_RUNTM_CFG)
        {
            UartPC_UartSaveConfig();
        }
    #endif /* (!UartPC_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        UartPC_backup.enableState = (uint8) UartPC_GET_CTRL_ENABLED;

        if(0u != UartPC_backup.enableState)
        {
            UartPC_Stop();
        }
    }

#else

    #if (UartPC_SCB_MODE_I2C_CONST_CFG && UartPC_I2C_WAKE_ENABLE_CONST)
        UartPC_I2CSaveConfig();

    #elif (UartPC_SCB_MODE_EZI2C_CONST_CFG && UartPC_EZI2C_WAKE_ENABLE_CONST)
        UartPC_EzI2CSaveConfig();

    #elif (UartPC_SCB_MODE_SPI_CONST_CFG && UartPC_SPI_WAKE_ENABLE_CONST)
        UartPC_SpiSaveConfig();

    #elif (UartPC_SCB_MODE_UART_CONST_CFG && UartPC_UART_WAKE_ENABLE_CONST)
        UartPC_UartSaveConfig();

    #else

        UartPC_backup.enableState = (uint8) UartPC_GET_CTRL_ENABLED;

        if(0u != UartPC_backup.enableState)
        {
            UartPC_Stop();
        }

    #endif /* defined (UartPC_SCB_MODE_I2C_CONST_CFG) && (UartPC_I2C_WAKE_ENABLE_CONST) */

#endif /* (UartPC_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: UartPC_Wakeup
****************************************************************************//**
*
*  Prepares the UartPC component for Active mode operation after 
*  Deep Sleep.
*  The “Enable wakeup from Deep Sleep Mode” selection has influence on this 
*  function implementation:
*  - Checked: restores the component Active mode configuration.
*  - Unchecked: enables the component if it was enabled before enter Deep Sleep.
*
*  This function should not be called after exiting Sleep.
*
*  \sideeffect
*   Calling the UartPC_Wakeup() function without first calling the 
*   UartPC_Sleep() function may produce unexpected behavior.
*
*******************************************************************************/
void UartPC_Wakeup(void)
{
#if(UartPC_SCB_MODE_UNCONFIG_CONST_CFG)

    if(UartPC_SCB_WAKE_ENABLE_CHECK)
    {
        if(UartPC_SCB_MODE_I2C_RUNTM_CFG)
        {
            UartPC_I2CRestoreConfig();
        }
        else if(UartPC_SCB_MODE_EZI2C_RUNTM_CFG)
        {
            UartPC_EzI2CRestoreConfig();
        }
    #if(!UartPC_CY_SCBIP_V1)
        else if(UartPC_SCB_MODE_SPI_RUNTM_CFG)
        {
            UartPC_SpiRestoreConfig();
        }
        else if(UartPC_SCB_MODE_UART_RUNTM_CFG)
        {
            UartPC_UartRestoreConfig();
        }
    #endif /* (!UartPC_CY_SCBIP_V1) */
        else
        {
            /* Unknown mode */
        }
    }
    else
    {
        if(0u != UartPC_backup.enableState)
        {
            UartPC_Enable();
        }
    }

#else

    #if (UartPC_SCB_MODE_I2C_CONST_CFG  && UartPC_I2C_WAKE_ENABLE_CONST)
        UartPC_I2CRestoreConfig();

    #elif (UartPC_SCB_MODE_EZI2C_CONST_CFG && UartPC_EZI2C_WAKE_ENABLE_CONST)
        UartPC_EzI2CRestoreConfig();

    #elif (UartPC_SCB_MODE_SPI_CONST_CFG && UartPC_SPI_WAKE_ENABLE_CONST)
        UartPC_SpiRestoreConfig();

    #elif (UartPC_SCB_MODE_UART_CONST_CFG && UartPC_UART_WAKE_ENABLE_CONST)
        UartPC_UartRestoreConfig();

    #else

        if(0u != UartPC_backup.enableState)
        {
            UartPC_Enable();
        }

    #endif /* (UartPC_I2C_WAKE_ENABLE_CONST) */

#endif /* (UartPC_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/* [] END OF FILE */
