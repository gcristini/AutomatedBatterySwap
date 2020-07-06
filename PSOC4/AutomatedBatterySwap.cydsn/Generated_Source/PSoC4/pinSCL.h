/*******************************************************************************
* File Name: pinSCL.h  
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

#if !defined(CY_PINS_pinSCL_H) /* Pins pinSCL_H */
#define CY_PINS_pinSCL_H

#include "cytypes.h"
#include "cyfitter.h"
#include "pinSCL_aliases.h"


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
} pinSCL_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   pinSCL_Read(void);
void    pinSCL_Write(uint8 value);
uint8   pinSCL_ReadDataReg(void);
#if defined(pinSCL__PC) || (CY_PSOC4_4200L) 
    void    pinSCL_SetDriveMode(uint8 mode);
#endif
void    pinSCL_SetInterruptMode(uint16 position, uint16 mode);
uint8   pinSCL_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void pinSCL_Sleep(void); 
void pinSCL_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(pinSCL__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define pinSCL_DRIVE_MODE_BITS        (3)
    #define pinSCL_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - pinSCL_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the pinSCL_SetDriveMode() function.
         *  @{
         */
        #define pinSCL_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define pinSCL_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define pinSCL_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define pinSCL_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define pinSCL_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define pinSCL_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define pinSCL_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define pinSCL_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define pinSCL_MASK               pinSCL__MASK
#define pinSCL_SHIFT              pinSCL__SHIFT
#define pinSCL_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in pinSCL_SetInterruptMode() function.
     *  @{
     */
        #define pinSCL_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define pinSCL_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define pinSCL_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define pinSCL_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(pinSCL__SIO)
    #define pinSCL_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(pinSCL__PC) && (CY_PSOC4_4200L)
    #define pinSCL_USBIO_ENABLE               ((uint32)0x80000000u)
    #define pinSCL_USBIO_DISABLE              ((uint32)(~pinSCL_USBIO_ENABLE))
    #define pinSCL_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define pinSCL_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define pinSCL_USBIO_ENTER_SLEEP          ((uint32)((1u << pinSCL_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << pinSCL_USBIO_SUSPEND_DEL_SHIFT)))
    #define pinSCL_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << pinSCL_USBIO_SUSPEND_SHIFT)))
    #define pinSCL_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << pinSCL_USBIO_SUSPEND_DEL_SHIFT)))
    #define pinSCL_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(pinSCL__PC)
    /* Port Configuration */
    #define pinSCL_PC                 (* (reg32 *) pinSCL__PC)
#endif
/* Pin State */
#define pinSCL_PS                     (* (reg32 *) pinSCL__PS)
/* Data Register */
#define pinSCL_DR                     (* (reg32 *) pinSCL__DR)
/* Input Buffer Disable Override */
#define pinSCL_INP_DIS                (* (reg32 *) pinSCL__PC2)

/* Interrupt configuration Registers */
#define pinSCL_INTCFG                 (* (reg32 *) pinSCL__INTCFG)
#define pinSCL_INTSTAT                (* (reg32 *) pinSCL__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define pinSCL_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(pinSCL__SIO)
    #define pinSCL_SIO_REG            (* (reg32 *) pinSCL__SIO)
#endif /* (pinSCL__SIO_CFG) */

/* USBIO registers */
#if !defined(pinSCL__PC) && (CY_PSOC4_4200L)
    #define pinSCL_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define pinSCL_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define pinSCL_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define pinSCL_DRIVE_MODE_SHIFT       (0x00u)
#define pinSCL_DRIVE_MODE_MASK        (0x07u << pinSCL_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins pinSCL_H */


/* [] END OF FILE */
