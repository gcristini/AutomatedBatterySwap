/*******************************************************************************
* File Name: pinNTC.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "pinNTC.h"

static pinNTC_BACKUP_STRUCT  pinNTC_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: pinNTC_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function applies only to SIO and USBIO pins.
*  It should not be called for GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet pinNTC_SUT.c usage_pinNTC_Sleep_Wakeup
*******************************************************************************/
void pinNTC_Sleep(void)
{
    #if defined(pinNTC__PC)
        pinNTC_backup.pcState = pinNTC_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            pinNTC_backup.usbState = pinNTC_CR1_REG;
            pinNTC_USB_POWER_REG |= pinNTC_USBIO_ENTER_SLEEP;
            pinNTC_CR1_REG &= pinNTC_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(pinNTC__SIO)
        pinNTC_backup.sioState = pinNTC_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        pinNTC_SIO_REG &= (uint32)(~pinNTC_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: pinNTC_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep(). This 
* function applies only to SIO and USBIO pins. It should not be called for
* GPIO or GPIO_OVT pins.
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to pinNTC_Sleep() for an example usage.
*******************************************************************************/
void pinNTC_Wakeup(void)
{
    #if defined(pinNTC__PC)
        pinNTC_PC = pinNTC_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            pinNTC_USB_POWER_REG &= pinNTC_USBIO_EXIT_SLEEP_PH1;
            pinNTC_CR1_REG = pinNTC_backup.usbState;
            pinNTC_USB_POWER_REG &= pinNTC_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(pinNTC__SIO)
        pinNTC_SIO_REG = pinNTC_backup.sioState;
    #endif
}


/* [] END OF FILE */
