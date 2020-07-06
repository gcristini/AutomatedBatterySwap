/*******************************************************************************
* File Name: pinPackP.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_pinPackP_ALIASES_H) /* Pins pinPackP_ALIASES_H */
#define CY_PINS_pinPackP_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define pinPackP_0			(pinPackP__0__PC)
#define pinPackP_0_PS		(pinPackP__0__PS)
#define pinPackP_0_PC		(pinPackP__0__PC)
#define pinPackP_0_DR		(pinPackP__0__DR)
#define pinPackP_0_SHIFT	(pinPackP__0__SHIFT)
#define pinPackP_0_INTR	((uint16)((uint16)0x0003u << (pinPackP__0__SHIFT*2u)))

#define pinPackP_INTR_ALL	 ((uint16)(pinPackP_0_INTR))


#endif /* End Pins pinPackP_ALIASES_H */


/* [] END OF FILE */
