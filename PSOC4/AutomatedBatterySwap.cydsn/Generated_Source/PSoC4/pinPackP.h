/*******************************************************************************
* File Name: pinPackP.h  
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

#if !defined(CY_PINS_pinPackP_H) /* Pins pinPackP_H */
#define CY_PINS_pinPackP_H

#include "cytypes.h"
#include "cyfitter.h"
#include "pinPackP_aliases.h"


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
} pinPackP_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   pinPackP_Read(void);
void    pinPackP_Write(uint8 value);
uint8   pinPackP_ReadDataReg(void);
#if defined(pinPackP__PC) || (CY_PSOC4_4200L) 
    void    pinPackP_SetDriveMode(uint8 mode);
#endif
void    pinPackP_SetInterruptMode(uint16 position, uint16 mode);
uint8   pinPackP_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void pinPackP_Sleep(void); 
void pinPackP_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(pinPackP__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define pinPackP_DRIVE_MODE_BITS        (3)
    #define pinPackP_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - pinPackP_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the pinPackP_SetDriveMode() function.
         *  @{
         */
        #define pinPackP_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define pinPackP_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define pinPackP_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define pinPackP_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define pinPackP_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define pinPackP_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define pinPackP_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define pinPackP_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define pinPackP_MASK               pinPackP__MASK
#define pinPackP_SHIFT              pinPackP__SHIFT
#define pinPackP_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in pinPackP_SetInterruptMode() function.
     *  @{
     */
        #define pinPackP_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define pinPackP_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define pinPackP_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define pinPackP_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(pinPackP__SIO)
    #define pinPackP_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(pinPackP__PC) && (CY_PSOC4_4200L)
    #define pinPackP_USBIO_ENABLE               ((uint32)0x80000000u)
    #define pinPackP_USBIO_DISABLE              ((uint32)(~pinPackP_USBIO_ENABLE))
    #define pinPackP_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define pinPackP_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define pinPackP_USBIO_ENTER_SLEEP          ((uint32)((1u << pinPackP_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << pinPackP_USBIO_SUSPEND_DEL_SHIFT)))
    #define pinPackP_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << pinPackP_USBIO_SUSPEND_SHIFT)))
    #define pinPackP_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << pinPackP_USBIO_SUSPEND_DEL_SHIFT)))
    #define pinPackP_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(pinPackP__PC)
    /* Port Configuration */
    #define pinPackP_PC                 (* (reg32 *) pinPackP__PC)
#endif
/* Pin State */
#define pinPackP_PS                     (* (reg32 *) pinPackP__PS)
/* Data Register */
#define pinPackP_DR                     (* (reg32 *) pinPackP__DR)
/* Input Buffer Disable Override */
#define pinPackP_INP_DIS                (* (reg32 *) pinPackP__PC2)

/* Interrupt configuration Registers */
#define pinPackP_INTCFG                 (* (reg32 *) pinPackP__INTCFG)
#define pinPackP_INTSTAT                (* (reg32 *) pinPackP__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define pinPackP_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(pinPackP__SIO)
    #define pinPackP_SIO_REG            (* (reg32 *) pinPackP__SIO)
#endif /* (pinPackP__SIO_CFG) */

/* USBIO registers */
#if !defined(pinPackP__PC) && (CY_PSOC4_4200L)
    #define pinPackP_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define pinPackP_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define pinPackP_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define pinPackP_DRIVE_MODE_SHIFT       (0x00u)
#define pinPackP_DRIVE_MODE_MASK        (0x07u << pinPackP_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins pinPackP_H */


/* [] END OF FILE */
