/*******************************************************************************
* File Name: UartPC_tx.h  
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

#if !defined(CY_PINS_UartPC_tx_ALIASES_H) /* Pins UartPC_tx_ALIASES_H */
#define CY_PINS_UartPC_tx_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define UartPC_tx_0			(UartPC_tx__0__PC)
#define UartPC_tx_0_PS		(UartPC_tx__0__PS)
#define UartPC_tx_0_PC		(UartPC_tx__0__PC)
#define UartPC_tx_0_DR		(UartPC_tx__0__DR)
#define UartPC_tx_0_SHIFT	(UartPC_tx__0__SHIFT)
#define UartPC_tx_0_INTR	((uint16)((uint16)0x0003u << (UartPC_tx__0__SHIFT*2u)))

#define UartPC_tx_INTR_ALL	 ((uint16)(UartPC_tx_0_INTR))


#endif /* End Pins UartPC_tx_ALIASES_H */


/* [] END OF FILE */
