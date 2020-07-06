/*******************************************************************************
* File Name: pinDetect.h  
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

#if !defined(CY_PINS_pinDetect_H) /* Pins pinDetect_H */
#define CY_PINS_pinDetect_H

#include "cytypes.h"
#include "cyfitter.h"
#include "pinDetect_aliases.h"


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
} pinDetect_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   pinDetect_Read(void);
void    pinDetect_Write(uint8 value);
uint8   pinDetect_ReadDataReg(void);
#if defined(pinDetect__PC) || (CY_PSOC4_4200L) 
    void    pinDetect_SetDriveMode(uint8 mode);
#endif
void    pinDetect_SetInterruptMode(uint16 position, uint16 mode);
uint8   pinDetect_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void pinDetect_Sleep(void); 
void pinDetect_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(pinDetect__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define pinDetect_DRIVE_MODE_BITS        (3)
    #define pinDetect_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - pinDetect_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the pinDetect_SetDriveMode() function.
         *  @{
         */
        #define pinDetect_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define pinDetect_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define pinDetect_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define pinDetect_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define pinDetect_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define pinDetect_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define pinDetect_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define pinDetect_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define pinDetect_MASK               pinDetect__MASK
#define pinDetect_SHIFT              pinDetect__SHIFT
#define pinDetect_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in pinDetect_SetInterruptMode() function.
     *  @{
     */
        #define pinDetect_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define pinDetect_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define pinDetect_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define pinDetect_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(pinDetect__SIO)
    #define pinDetect_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(pinDetect__PC) && (CY_PSOC4_4200L)
    #define pinDetect_USBIO_ENABLE               ((uint32)0x80000000u)
    #define pinDetect_USBIO_DISABLE              ((uint32)(~pinDetect_USBIO_ENABLE))
    #define pinDetect_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define pinDetect_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define pinDetect_USBIO_ENTER_SLEEP          ((uint32)((1u << pinDetect_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << pinDetect_USBIO_SUSPEND_DEL_SHIFT)))
    #define pinDetect_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << pinDetect_USBIO_SUSPEND_SHIFT)))
    #define pinDetect_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << pinDetect_USBIO_SUSPEND_DEL_SHIFT)))
    #define pinDetect_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(pinDetect__PC)
    /* Port Configuration */
    #define pinDetect_PC                 (* (reg32 *) pinDetect__PC)
#endif
/* Pin State */
#define pinDetect_PS                     (* (reg32 *) pinDetect__PS)
/* Data Register */
#define pinDetect_DR                     (* (reg32 *) pinDetect__DR)
/* Input Buffer Disable Override */
#define pinDetect_INP_DIS                (* (reg32 *) pinDetect__PC2)

/* Interrupt configuration Registers */
#define pinDetect_INTCFG                 (* (reg32 *) pinDetect__INTCFG)
#define pinDetect_INTSTAT                (* (reg32 *) pinDetect__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define pinDetect_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(pinDetect__SIO)
    #define pinDetect_SIO_REG            (* (reg32 *) pinDetect__SIO)
#endif /* (pinDetect__SIO_CFG) */

/* USBIO registers */
#if !defined(pinDetect__PC) && (CY_PSOC4_4200L)
    #define pinDetect_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define pinDetect_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define pinDetect_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define pinDetect_DRIVE_MODE_SHIFT       (0x00u)
#define pinDetect_DRIVE_MODE_MASK        (0x07u << pinDetect_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins pinDetect_H */


/* [] END OF FILE */
