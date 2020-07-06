/*******************************************************************************
* File Name: pinNTC.h  
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

#if !defined(CY_PINS_pinNTC_ALIASES_H) /* Pins pinNTC_ALIASES_H */
#define CY_PINS_pinNTC_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define pinNTC_0			(pinNTC__0__PC)
#define pinNTC_0_PS		(pinNTC__0__PS)
#define pinNTC_0_PC		(pinNTC__0__PC)
#define pinNTC_0_DR		(pinNTC__0__DR)
#define pinNTC_0_SHIFT	(pinNTC__0__SHIFT)
#define pinNTC_0_INTR	((uint16)((uint16)0x0003u << (pinNTC__0__SHIFT*2u)))

#define pinNTC_INTR_ALL	 ((uint16)(pinNTC_0_INTR))


#endif /* End Pins pinNTC_ALIASES_H */


/* [] END OF FILE */
