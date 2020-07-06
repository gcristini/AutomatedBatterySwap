/*******************************************************************************
* File Name: pinSDA.h  
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

#if !defined(CY_PINS_pinSDA_ALIASES_H) /* Pins pinSDA_ALIASES_H */
#define CY_PINS_pinSDA_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define pinSDA_0			(pinSDA__0__PC)
#define pinSDA_0_PS		(pinSDA__0__PS)
#define pinSDA_0_PC		(pinSDA__0__PC)
#define pinSDA_0_DR		(pinSDA__0__DR)
#define pinSDA_0_SHIFT	(pinSDA__0__SHIFT)
#define pinSDA_0_INTR	((uint16)((uint16)0x0003u << (pinSDA__0__SHIFT*2u)))

#define pinSDA_INTR_ALL	 ((uint16)(pinSDA_0_INTR))


#endif /* End Pins pinSDA_ALIASES_H */


/* [] END OF FILE */
