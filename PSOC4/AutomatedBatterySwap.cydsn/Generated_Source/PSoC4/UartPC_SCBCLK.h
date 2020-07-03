/*******************************************************************************
* File Name: UartPC_SCBCLK.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_UartPC_SCBCLK_H)
#define CY_CLOCK_UartPC_SCBCLK_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void UartPC_SCBCLK_StartEx(uint32 alignClkDiv);
#define UartPC_SCBCLK_Start() \
    UartPC_SCBCLK_StartEx(UartPC_SCBCLK__PA_DIV_ID)

#else

void UartPC_SCBCLK_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void UartPC_SCBCLK_Stop(void);

void UartPC_SCBCLK_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 UartPC_SCBCLK_GetDividerRegister(void);
uint8  UartPC_SCBCLK_GetFractionalDividerRegister(void);

#define UartPC_SCBCLK_Enable()                         UartPC_SCBCLK_Start()
#define UartPC_SCBCLK_Disable()                        UartPC_SCBCLK_Stop()
#define UartPC_SCBCLK_SetDividerRegister(clkDivider, reset)  \
    UartPC_SCBCLK_SetFractionalDividerRegister((clkDivider), 0u)
#define UartPC_SCBCLK_SetDivider(clkDivider)           UartPC_SCBCLK_SetDividerRegister((clkDivider), 1u)
#define UartPC_SCBCLK_SetDividerValue(clkDivider)      UartPC_SCBCLK_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define UartPC_SCBCLK_DIV_ID     UartPC_SCBCLK__DIV_ID

#define UartPC_SCBCLK_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define UartPC_SCBCLK_CTRL_REG   (*(reg32 *)UartPC_SCBCLK__CTRL_REGISTER)
#define UartPC_SCBCLK_DIV_REG    (*(reg32 *)UartPC_SCBCLK__DIV_REGISTER)

#define UartPC_SCBCLK_CMD_DIV_SHIFT          (0u)
#define UartPC_SCBCLK_CMD_PA_DIV_SHIFT       (8u)
#define UartPC_SCBCLK_CMD_DISABLE_SHIFT      (30u)
#define UartPC_SCBCLK_CMD_ENABLE_SHIFT       (31u)

#define UartPC_SCBCLK_CMD_DISABLE_MASK       ((uint32)((uint32)1u << UartPC_SCBCLK_CMD_DISABLE_SHIFT))
#define UartPC_SCBCLK_CMD_ENABLE_MASK        ((uint32)((uint32)1u << UartPC_SCBCLK_CMD_ENABLE_SHIFT))

#define UartPC_SCBCLK_DIV_FRAC_MASK  (0x000000F8u)
#define UartPC_SCBCLK_DIV_FRAC_SHIFT (3u)
#define UartPC_SCBCLK_DIV_INT_MASK   (0xFFFFFF00u)
#define UartPC_SCBCLK_DIV_INT_SHIFT  (8u)

#else 

#define UartPC_SCBCLK_DIV_REG        (*(reg32 *)UartPC_SCBCLK__REGISTER)
#define UartPC_SCBCLK_ENABLE_REG     UartPC_SCBCLK_DIV_REG
#define UartPC_SCBCLK_DIV_FRAC_MASK  UartPC_SCBCLK__FRAC_MASK
#define UartPC_SCBCLK_DIV_FRAC_SHIFT (16u)
#define UartPC_SCBCLK_DIV_INT_MASK   UartPC_SCBCLK__DIVIDER_MASK
#define UartPC_SCBCLK_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_UartPC_SCBCLK_H) */

/* [] END OF FILE */
