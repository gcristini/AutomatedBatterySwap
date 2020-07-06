/*******************************************************************************
* File Name: pin_NTC.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_pin_NTC_H) /* Pins pin_NTC_H */
#define CY_PINS_pin_NTC_H

#include "cytypes.h"
#include "cyfitter.h"
#include "pin_NTC_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} pin_NTC_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   pin_NTC_Read(void);
void    pin_NTC_Write(uint8 value);
uint8   pin_NTC_ReadDataReg(void);
#if defined(pin_NTC__PC) || (CY_PSOC4_4200L) 
    void    pin_NTC_SetDriveMode(uint8 mode);
#endif
void    pin_NTC_SetInterruptMode(uint16 position, uint16 mode);
uint8   pin_NTC_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void pin_NTC_Sleep(void); 
void pin_NTC_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(pin_NTC__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define pin_NTC_DRIVE_MODE_BITS        (3)
    #define pin_NTC_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - pin_NTC_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the pin_NTC_SetDriveMode() function.
         *  @{
         */
        #define pin_NTC_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define pin_NTC_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define pin_NTC_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define pin_NTC_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define pin_NTC_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define pin_NTC_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define pin_NTC_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define pin_NTC_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define pin_NTC_MASK               pin_NTC__MASK
#define pin_NTC_SHIFT              pin_NTC__SHIFT
#define pin_NTC_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in pin_NTC_SetInterruptMode() function.
     *  @{
     */
        #define pin_NTC_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define pin_NTC_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define pin_NTC_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define pin_NTC_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(pin_NTC__SIO)
    #define pin_NTC_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(pin_NTC__PC) && (CY_PSOC4_4200L)
    #define pin_NTC_USBIO_ENABLE               ((uint32)0x80000000u)
    #define pin_NTC_USBIO_DISABLE              ((uint32)(~pin_NTC_USBIO_ENABLE))
    #define pin_NTC_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define pin_NTC_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define pin_NTC_USBIO_ENTER_SLEEP          ((uint32)((1u << pin_NTC_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << pin_NTC_USBIO_SUSPEND_DEL_SHIFT)))
    #define pin_NTC_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << pin_NTC_USBIO_SUSPEND_SHIFT)))
    #define pin_NTC_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << pin_NTC_USBIO_SUSPEND_DEL_SHIFT)))
    #define pin_NTC_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(pin_NTC__PC)
    /* Port Configuration */
    #define pin_NTC_PC                 (* (reg32 *) pin_NTC__PC)
#endif
/* Pin State */
#define pin_NTC_PS                     (* (reg32 *) pin_NTC__PS)
/* Data Register */
#define pin_NTC_DR                     (* (reg32 *) pin_NTC__DR)
/* Input Buffer Disable Override */
#define pin_NTC_INP_DIS                (* (reg32 *) pin_NTC__PC2)

/* Interrupt configuration Registers */
#define pin_NTC_INTCFG                 (* (reg32 *) pin_NTC__INTCFG)
#define pin_NTC_INTSTAT                (* (reg32 *) pin_NTC__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define pin_NTC_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(pin_NTC__SIO)
    #define pin_NTC_SIO_REG            (* (reg32 *) pin_NTC__SIO)
#endif /* (pin_NTC__SIO_CFG) */

/* USBIO registers */
#if !defined(pin_NTC__PC) && (CY_PSOC4_4200L)
    #define pin_NTC_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define pin_NTC_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define pin_NTC_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define pin_NTC_DRIVE_MODE_SHIFT       (0x00u)
#define pin_NTC_DRIVE_MODE_MASK        (0x07u << pin_NTC_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins pin_NTC_H */


/* [] END OF FILE */
