/*******************************************************************************
* File Name: pinSDA.c  
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
#include "pinSDA.h"

static pinSDA_BACKUP_STRUCT  pinSDA_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: pinSDA_Sleep
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
*  \snippet pinSDA_SUT.c usage_pinSDA_Sleep_Wakeup
*******************************************************************************/
void pinSDA_Sleep(void)
{
    #if defined(pinSDA__PC)
        pinSDA_backup.pcState = pinSDA_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            pinSDA_backup.usbState = pinSDA_CR1_REG;
            pinSDA_USB_POWER_REG |= pinSDA_USBIO_ENTER_SLEEP;
            pinSDA_CR1_REG &= pinSDA_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(pinSDA__SIO)
        pinSDA_backup.sioState = pinSDA_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        pinSDA_SIO_REG &= (uint32)(~pinSDA_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: pinSDA_Wakeup
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
*  Refer to pinSDA_Sleep() for an example usage.
*******************************************************************************/
void pinSDA_Wakeup(void)
{
    #if defined(pinSDA__PC)
        pinSDA_PC = pinSDA_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            pinSDA_USB_POWER_REG &= pinSDA_USBIO_EXIT_SLEEP_PH1;
            pinSDA_CR1_REG = pinSDA_backup.usbState;
            pinSDA_USB_POWER_REG &= pinSDA_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(pinSDA__SIO)
        pinSDA_SIO_REG = pinSDA_backup.sioState;
    #endif
}


/* [] END OF FILE */
