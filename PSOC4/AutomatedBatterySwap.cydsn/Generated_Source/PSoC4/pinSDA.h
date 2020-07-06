/*******************************************************************************
* File Name: pinSDA.h  
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

#if !defined(CY_PINS_pinSDA_H) /* Pins pinSDA_H */
#define CY_PINS_pinSDA_H

#include "cytypes.h"
#include "cyfitter.h"
#include "pinSDA_aliases.h"


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
} pinSDA_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   pinSDA_Read(void);
void    pinSDA_Write(uint8 value);
uint8   pinSDA_ReadDataReg(void);
#if defined(pinSDA__PC) || (CY_PSOC4_4200L) 
    void    pinSDA_SetDriveMode(uint8 mode);
#endif
void    pinSDA_SetInterruptMode(uint16 position, uint16 mode);
uint8   pinSDA_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void pinSDA_Sleep(void); 
void pinSDA_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(pinSDA__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define pinSDA_DRIVE_MODE_BITS        (3)
    #define pinSDA_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - pinSDA_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the pinSDA_SetDriveMode() function.
         *  @{
         */
        #define pinSDA_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define pinSDA_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define pinSDA_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define pinSDA_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define pinSDA_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define pinSDA_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define pinSDA_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define pinSDA_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define pinSDA_MASK               pinSDA__MASK
#define pinSDA_SHIFT              pinSDA__SHIFT
#define pinSDA_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in pinSDA_SetInterruptMode() function.
     *  @{
     */
        #define pinSDA_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define pinSDA_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define pinSDA_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define pinSDA_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(pinSDA__SIO)
    #define pinSDA_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(pinSDA__PC) && (CY_PSOC4_4200L)
    #define pinSDA_USBIO_ENABLE               ((uint32)0x80000000u)
    #define pinSDA_USBIO_DISABLE              ((uint32)(~pinSDA_USBIO_ENABLE))
    #define pinSDA_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define pinSDA_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define pinSDA_USBIO_ENTER_SLEEP          ((uint32)((1u << pinSDA_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << pinSDA_USBIO_SUSPEND_DEL_SHIFT)))
    #define pinSDA_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << pinSDA_USBIO_SUSPEND_SHIFT)))
    #define pinSDA_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << pinSDA_USBIO_SUSPEND_DEL_SHIFT)))
    #define pinSDA_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(pinSDA__PC)
    /* Port Configuration */
    #define pinSDA_PC                 (* (reg32 *) pinSDA__PC)
#endif
/* Pin State */
#define pinSDA_PS                     (* (reg32 *) pinSDA__PS)
/* Data Register */
#define pinSDA_DR                     (* (reg32 *) pinSDA__DR)
/* Input Buffer Disable Override */
#define pinSDA_INP_DIS                (* (reg32 *) pinSDA__PC2)

/* Interrupt configuration Registers */
#define pinSDA_INTCFG                 (* (reg32 *) pinSDA__INTCFG)
#define pinSDA_INTSTAT                (* (reg32 *) pinSDA__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define pinSDA_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(pinSDA__SIO)
    #define pinSDA_SIO_REG            (* (reg32 *) pinSDA__SIO)
#endif /* (pinSDA__SIO_CFG) */

/* USBIO registers */
#if !defined(pinSDA__PC) && (CY_PSOC4_4200L)
    #define pinSDA_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define pinSDA_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define pinSDA_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define pinSDA_DRIVE_MODE_SHIFT       (0x00u)
#define pinSDA_DRIVE_MODE_MASK        (0x07u << pinSDA_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins pinSDA_H */


/* [] END OF FILE */
