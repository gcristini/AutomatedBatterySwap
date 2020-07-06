/*******************************************************************************
* File Name: pin_NTC.c  
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
#include "pin_NTC.h"

static pin_NTC_BACKUP_STRUCT  pin_NTC_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: pin_NTC_Sleep
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
*  \snippet pin_NTC_SUT.c usage_pin_NTC_Sleep_Wakeup
*******************************************************************************/
void pin_NTC_Sleep(void)
{
    #if defined(pin_NTC__PC)
        pin_NTC_backup.pcState = pin_NTC_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            pin_NTC_backup.usbState = pin_NTC_CR1_REG;
            pin_NTC_USB_POWER_REG |= pin_NTC_USBIO_ENTER_SLEEP;
            pin_NTC_CR1_REG &= pin_NTC_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(pin_NTC__SIO)
        pin_NTC_backup.sioState = pin_NTC_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        pin_NTC_SIO_REG &= (uint32)(~pin_NTC_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: pin_NTC_Wakeup
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
*  Refer to pin_NTC_Sleep() for an example usage.
*******************************************************************************/
void pin_NTC_Wakeup(void)
{
    #if defined(pin_NTC__PC)
        pin_NTC_PC = pin_NTC_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            pin_NTC_USB_POWER_REG &= pin_NTC_USBIO_EXIT_SLEEP_PH1;
            pin_NTC_CR1_REG = pin_NTC_backup.usbState;
            pin_NTC_USB_POWER_REG &= pin_NTC_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(pin_NTC__SIO)
        pin_NTC_SIO_REG = pin_NTC_backup.sioState;
    #endif
}


/* [] END OF FILE */
