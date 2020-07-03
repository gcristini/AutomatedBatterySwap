/***************************************************************************//**
* \file UartPC_PINS.h
* \version 4.0
*
* \brief
*  This file provides constants and parameter values for the pin components
*  buried into SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PINS_UartPC_H)
#define CY_SCB_PINS_UartPC_H

#include "cydevice_trm.h"
#include "cyfitter.h"
#include "cytypes.h"


/***************************************
*   Conditional Compilation Parameters
****************************************/

/* Unconfigured pins */
#define UartPC_REMOVE_RX_WAKE_SCL_MOSI_PIN  (1u)
#define UartPC_REMOVE_RX_SCL_MOSI_PIN      (1u)
#define UartPC_REMOVE_TX_SDA_MISO_PIN      (1u)
#define UartPC_REMOVE_CTS_SCLK_PIN      (1u)
#define UartPC_REMOVE_RTS_SS0_PIN      (1u)
#define UartPC_REMOVE_SS1_PIN                 (1u)
#define UartPC_REMOVE_SS2_PIN                 (1u)
#define UartPC_REMOVE_SS3_PIN                 (1u)

/* Mode defined pins */
#define UartPC_REMOVE_I2C_PINS                (1u)
#define UartPC_REMOVE_SPI_MASTER_PINS         (1u)
#define UartPC_REMOVE_SPI_MASTER_SCLK_PIN     (1u)
#define UartPC_REMOVE_SPI_MASTER_MOSI_PIN     (1u)
#define UartPC_REMOVE_SPI_MASTER_MISO_PIN     (1u)
#define UartPC_REMOVE_SPI_MASTER_SS0_PIN      (1u)
#define UartPC_REMOVE_SPI_MASTER_SS1_PIN      (1u)
#define UartPC_REMOVE_SPI_MASTER_SS2_PIN      (1u)
#define UartPC_REMOVE_SPI_MASTER_SS3_PIN      (1u)
#define UartPC_REMOVE_SPI_SLAVE_PINS          (1u)
#define UartPC_REMOVE_SPI_SLAVE_MOSI_PIN      (1u)
#define UartPC_REMOVE_SPI_SLAVE_MISO_PIN      (1u)
#define UartPC_REMOVE_UART_TX_PIN             (0u)
#define UartPC_REMOVE_UART_RX_TX_PIN          (1u)
#define UartPC_REMOVE_UART_RX_PIN             (0u)
#define UartPC_REMOVE_UART_RX_WAKE_PIN        (1u)
#define UartPC_REMOVE_UART_RTS_PIN            (1u)
#define UartPC_REMOVE_UART_CTS_PIN            (1u)

/* Unconfigured pins */
#define UartPC_RX_WAKE_SCL_MOSI_PIN (0u == UartPC_REMOVE_RX_WAKE_SCL_MOSI_PIN)
#define UartPC_RX_SCL_MOSI_PIN     (0u == UartPC_REMOVE_RX_SCL_MOSI_PIN)
#define UartPC_TX_SDA_MISO_PIN     (0u == UartPC_REMOVE_TX_SDA_MISO_PIN)
#define UartPC_CTS_SCLK_PIN     (0u == UartPC_REMOVE_CTS_SCLK_PIN)
#define UartPC_RTS_SS0_PIN     (0u == UartPC_REMOVE_RTS_SS0_PIN)
#define UartPC_SS1_PIN                (0u == UartPC_REMOVE_SS1_PIN)
#define UartPC_SS2_PIN                (0u == UartPC_REMOVE_SS2_PIN)
#define UartPC_SS3_PIN                (0u == UartPC_REMOVE_SS3_PIN)

/* Mode defined pins */
#define UartPC_I2C_PINS               (0u == UartPC_REMOVE_I2C_PINS)
#define UartPC_SPI_MASTER_PINS        (0u == UartPC_REMOVE_SPI_MASTER_PINS)
#define UartPC_SPI_MASTER_SCLK_PIN    (0u == UartPC_REMOVE_SPI_MASTER_SCLK_PIN)
#define UartPC_SPI_MASTER_MOSI_PIN    (0u == UartPC_REMOVE_SPI_MASTER_MOSI_PIN)
#define UartPC_SPI_MASTER_MISO_PIN    (0u == UartPC_REMOVE_SPI_MASTER_MISO_PIN)
#define UartPC_SPI_MASTER_SS0_PIN     (0u == UartPC_REMOVE_SPI_MASTER_SS0_PIN)
#define UartPC_SPI_MASTER_SS1_PIN     (0u == UartPC_REMOVE_SPI_MASTER_SS1_PIN)
#define UartPC_SPI_MASTER_SS2_PIN     (0u == UartPC_REMOVE_SPI_MASTER_SS2_PIN)
#define UartPC_SPI_MASTER_SS3_PIN     (0u == UartPC_REMOVE_SPI_MASTER_SS3_PIN)
#define UartPC_SPI_SLAVE_PINS         (0u == UartPC_REMOVE_SPI_SLAVE_PINS)
#define UartPC_SPI_SLAVE_MOSI_PIN     (0u == UartPC_REMOVE_SPI_SLAVE_MOSI_PIN)
#define UartPC_SPI_SLAVE_MISO_PIN     (0u == UartPC_REMOVE_SPI_SLAVE_MISO_PIN)
#define UartPC_UART_TX_PIN            (0u == UartPC_REMOVE_UART_TX_PIN)
#define UartPC_UART_RX_TX_PIN         (0u == UartPC_REMOVE_UART_RX_TX_PIN)
#define UartPC_UART_RX_PIN            (0u == UartPC_REMOVE_UART_RX_PIN)
#define UartPC_UART_RX_WAKE_PIN       (0u == UartPC_REMOVE_UART_RX_WAKE_PIN)
#define UartPC_UART_RTS_PIN           (0u == UartPC_REMOVE_UART_RTS_PIN)
#define UartPC_UART_CTS_PIN           (0u == UartPC_REMOVE_UART_CTS_PIN)


/***************************************
*             Includes
****************************************/

#if (UartPC_RX_WAKE_SCL_MOSI_PIN)
    #include "UartPC_uart_rx_wake_i2c_scl_spi_mosi.h"
#endif /* (UartPC_RX_SCL_MOSI) */

#if (UartPC_RX_SCL_MOSI_PIN)
    #include "UartPC_uart_rx_i2c_scl_spi_mosi.h"
#endif /* (UartPC_RX_SCL_MOSI) */

#if (UartPC_TX_SDA_MISO_PIN)
    #include "UartPC_uart_tx_i2c_sda_spi_miso.h"
#endif /* (UartPC_TX_SDA_MISO) */

#if (UartPC_CTS_SCLK_PIN)
    #include "UartPC_uart_cts_spi_sclk.h"
#endif /* (UartPC_CTS_SCLK) */

#if (UartPC_RTS_SS0_PIN)
    #include "UartPC_uart_rts_spi_ss0.h"
#endif /* (UartPC_RTS_SS0_PIN) */

#if (UartPC_SS1_PIN)
    #include "UartPC_spi_ss1.h"
#endif /* (UartPC_SS1_PIN) */

#if (UartPC_SS2_PIN)
    #include "UartPC_spi_ss2.h"
#endif /* (UartPC_SS2_PIN) */

#if (UartPC_SS3_PIN)
    #include "UartPC_spi_ss3.h"
#endif /* (UartPC_SS3_PIN) */

#if (UartPC_I2C_PINS)
    #include "UartPC_scl.h"
    #include "UartPC_sda.h"
#endif /* (UartPC_I2C_PINS) */

#if (UartPC_SPI_MASTER_PINS)
#if (UartPC_SPI_MASTER_SCLK_PIN)
    #include "UartPC_sclk_m.h"
#endif /* (UartPC_SPI_MASTER_SCLK_PIN) */

#if (UartPC_SPI_MASTER_MOSI_PIN)
    #include "UartPC_mosi_m.h"
#endif /* (UartPC_SPI_MASTER_MOSI_PIN) */

#if (UartPC_SPI_MASTER_MISO_PIN)
    #include "UartPC_miso_m.h"
#endif /*(UartPC_SPI_MASTER_MISO_PIN) */
#endif /* (UartPC_SPI_MASTER_PINS) */

#if (UartPC_SPI_SLAVE_PINS)
    #include "UartPC_sclk_s.h"
    #include "UartPC_ss_s.h"

#if (UartPC_SPI_SLAVE_MOSI_PIN)
    #include "UartPC_mosi_s.h"
#endif /* (UartPC_SPI_SLAVE_MOSI_PIN) */

#if (UartPC_SPI_SLAVE_MISO_PIN)
    #include "UartPC_miso_s.h"
#endif /*(UartPC_SPI_SLAVE_MISO_PIN) */
#endif /* (UartPC_SPI_SLAVE_PINS) */

#if (UartPC_SPI_MASTER_SS0_PIN)
    #include "UartPC_ss0_m.h"
#endif /* (UartPC_SPI_MASTER_SS0_PIN) */

#if (UartPC_SPI_MASTER_SS1_PIN)
    #include "UartPC_ss1_m.h"
#endif /* (UartPC_SPI_MASTER_SS1_PIN) */

#if (UartPC_SPI_MASTER_SS2_PIN)
    #include "UartPC_ss2_m.h"
#endif /* (UartPC_SPI_MASTER_SS2_PIN) */

#if (UartPC_SPI_MASTER_SS3_PIN)
    #include "UartPC_ss3_m.h"
#endif /* (UartPC_SPI_MASTER_SS3_PIN) */

#if (UartPC_UART_TX_PIN)
    #include "UartPC_tx.h"
#endif /* (UartPC_UART_TX_PIN) */

#if (UartPC_UART_RX_TX_PIN)
    #include "UartPC_rx_tx.h"
#endif /* (UartPC_UART_RX_TX_PIN) */

#if (UartPC_UART_RX_PIN)
    #include "UartPC_rx.h"
#endif /* (UartPC_UART_RX_PIN) */

#if (UartPC_UART_RX_WAKE_PIN)
    #include "UartPC_rx_wake.h"
#endif /* (UartPC_UART_RX_WAKE_PIN) */

#if (UartPC_UART_RTS_PIN)
    #include "UartPC_rts.h"
#endif /* (UartPC_UART_RTS_PIN) */

#if (UartPC_UART_CTS_PIN)
    #include "UartPC_cts.h"
#endif /* (UartPC_UART_CTS_PIN) */


/***************************************
*              Registers
***************************************/

#if (UartPC_RX_SCL_MOSI_PIN)
    #define UartPC_RX_SCL_MOSI_HSIOM_REG   (*(reg32 *) UartPC_uart_rx_i2c_scl_spi_mosi__0__HSIOM)
    #define UartPC_RX_SCL_MOSI_HSIOM_PTR   ( (reg32 *) UartPC_uart_rx_i2c_scl_spi_mosi__0__HSIOM)
    
    #define UartPC_RX_SCL_MOSI_HSIOM_MASK      (UartPC_uart_rx_i2c_scl_spi_mosi__0__HSIOM_MASK)
    #define UartPC_RX_SCL_MOSI_HSIOM_POS       (UartPC_uart_rx_i2c_scl_spi_mosi__0__HSIOM_SHIFT)
    #define UartPC_RX_SCL_MOSI_HSIOM_SEL_GPIO  (UartPC_uart_rx_i2c_scl_spi_mosi__0__HSIOM_GPIO)
    #define UartPC_RX_SCL_MOSI_HSIOM_SEL_I2C   (UartPC_uart_rx_i2c_scl_spi_mosi__0__HSIOM_I2C)
    #define UartPC_RX_SCL_MOSI_HSIOM_SEL_SPI   (UartPC_uart_rx_i2c_scl_spi_mosi__0__HSIOM_SPI)
    #define UartPC_RX_SCL_MOSI_HSIOM_SEL_UART  (UartPC_uart_rx_i2c_scl_spi_mosi__0__HSIOM_UART)
    
#elif (UartPC_RX_WAKE_SCL_MOSI_PIN)
    #define UartPC_RX_WAKE_SCL_MOSI_HSIOM_REG   (*(reg32 *) UartPC_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM)
    #define UartPC_RX_WAKE_SCL_MOSI_HSIOM_PTR   ( (reg32 *) UartPC_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM)
    
    #define UartPC_RX_WAKE_SCL_MOSI_HSIOM_MASK      (UartPC_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_MASK)
    #define UartPC_RX_WAKE_SCL_MOSI_HSIOM_POS       (UartPC_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_SHIFT)
    #define UartPC_RX_WAKE_SCL_MOSI_HSIOM_SEL_GPIO  (UartPC_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_GPIO)
    #define UartPC_RX_WAKE_SCL_MOSI_HSIOM_SEL_I2C   (UartPC_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_I2C)
    #define UartPC_RX_WAKE_SCL_MOSI_HSIOM_SEL_SPI   (UartPC_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_SPI)
    #define UartPC_RX_WAKE_SCL_MOSI_HSIOM_SEL_UART  (UartPC_uart_rx_wake_i2c_scl_spi_mosi__0__HSIOM_UART)    
   
    #define UartPC_RX_WAKE_SCL_MOSI_INTCFG_REG (*(reg32 *) UartPC_uart_rx_wake_i2c_scl_spi_mosi__0__INTCFG)
    #define UartPC_RX_WAKE_SCL_MOSI_INTCFG_PTR ( (reg32 *) UartPC_uart_rx_wake_i2c_scl_spi_mosi__0__INTCFG)
    #define UartPC_RX_WAKE_SCL_MOSI_INTCFG_TYPE_POS  (UartPC_uart_rx_wake_i2c_scl_spi_mosi__SHIFT)
    #define UartPC_RX_WAKE_SCL_MOSI_INTCFG_TYPE_MASK ((uint32) UartPC_INTCFG_TYPE_MASK << \
                                                                           UartPC_RX_WAKE_SCL_MOSI_INTCFG_TYPE_POS)
#else
    /* None of pins UartPC_RX_SCL_MOSI_PIN or UartPC_RX_WAKE_SCL_MOSI_PIN present.*/
#endif /* (UartPC_RX_SCL_MOSI_PIN) */

#if (UartPC_TX_SDA_MISO_PIN)
    #define UartPC_TX_SDA_MISO_HSIOM_REG   (*(reg32 *) UartPC_uart_tx_i2c_sda_spi_miso__0__HSIOM)
    #define UartPC_TX_SDA_MISO_HSIOM_PTR   ( (reg32 *) UartPC_uart_tx_i2c_sda_spi_miso__0__HSIOM)
    
    #define UartPC_TX_SDA_MISO_HSIOM_MASK      (UartPC_uart_tx_i2c_sda_spi_miso__0__HSIOM_MASK)
    #define UartPC_TX_SDA_MISO_HSIOM_POS       (UartPC_uart_tx_i2c_sda_spi_miso__0__HSIOM_SHIFT)
    #define UartPC_TX_SDA_MISO_HSIOM_SEL_GPIO  (UartPC_uart_tx_i2c_sda_spi_miso__0__HSIOM_GPIO)
    #define UartPC_TX_SDA_MISO_HSIOM_SEL_I2C   (UartPC_uart_tx_i2c_sda_spi_miso__0__HSIOM_I2C)
    #define UartPC_TX_SDA_MISO_HSIOM_SEL_SPI   (UartPC_uart_tx_i2c_sda_spi_miso__0__HSIOM_SPI)
    #define UartPC_TX_SDA_MISO_HSIOM_SEL_UART  (UartPC_uart_tx_i2c_sda_spi_miso__0__HSIOM_UART)
#endif /* (UartPC_TX_SDA_MISO_PIN) */

#if (UartPC_CTS_SCLK_PIN)
    #define UartPC_CTS_SCLK_HSIOM_REG   (*(reg32 *) UartPC_uart_cts_spi_sclk__0__HSIOM)
    #define UartPC_CTS_SCLK_HSIOM_PTR   ( (reg32 *) UartPC_uart_cts_spi_sclk__0__HSIOM)
    
    #define UartPC_CTS_SCLK_HSIOM_MASK      (UartPC_uart_cts_spi_sclk__0__HSIOM_MASK)
    #define UartPC_CTS_SCLK_HSIOM_POS       (UartPC_uart_cts_spi_sclk__0__HSIOM_SHIFT)
    #define UartPC_CTS_SCLK_HSIOM_SEL_GPIO  (UartPC_uart_cts_spi_sclk__0__HSIOM_GPIO)
    #define UartPC_CTS_SCLK_HSIOM_SEL_I2C   (UartPC_uart_cts_spi_sclk__0__HSIOM_I2C)
    #define UartPC_CTS_SCLK_HSIOM_SEL_SPI   (UartPC_uart_cts_spi_sclk__0__HSIOM_SPI)
    #define UartPC_CTS_SCLK_HSIOM_SEL_UART  (UartPC_uart_cts_spi_sclk__0__HSIOM_UART)
#endif /* (UartPC_CTS_SCLK_PIN) */

#if (UartPC_RTS_SS0_PIN)
    #define UartPC_RTS_SS0_HSIOM_REG   (*(reg32 *) UartPC_uart_rts_spi_ss0__0__HSIOM)
    #define UartPC_RTS_SS0_HSIOM_PTR   ( (reg32 *) UartPC_uart_rts_spi_ss0__0__HSIOM)
    
    #define UartPC_RTS_SS0_HSIOM_MASK      (UartPC_uart_rts_spi_ss0__0__HSIOM_MASK)
    #define UartPC_RTS_SS0_HSIOM_POS       (UartPC_uart_rts_spi_ss0__0__HSIOM_SHIFT)
    #define UartPC_RTS_SS0_HSIOM_SEL_GPIO  (UartPC_uart_rts_spi_ss0__0__HSIOM_GPIO)
    #define UartPC_RTS_SS0_HSIOM_SEL_I2C   (UartPC_uart_rts_spi_ss0__0__HSIOM_I2C)
    #define UartPC_RTS_SS0_HSIOM_SEL_SPI   (UartPC_uart_rts_spi_ss0__0__HSIOM_SPI)
#if !(UartPC_CY_SCBIP_V0 || UartPC_CY_SCBIP_V1)
    #define UartPC_RTS_SS0_HSIOM_SEL_UART  (UartPC_uart_rts_spi_ss0__0__HSIOM_UART)
#endif /* !(UartPC_CY_SCBIP_V0 || UartPC_CY_SCBIP_V1) */
#endif /* (UartPC_RTS_SS0_PIN) */

#if (UartPC_SS1_PIN)
    #define UartPC_SS1_HSIOM_REG  (*(reg32 *) UartPC_spi_ss1__0__HSIOM)
    #define UartPC_SS1_HSIOM_PTR  ( (reg32 *) UartPC_spi_ss1__0__HSIOM)
    
    #define UartPC_SS1_HSIOM_MASK     (UartPC_spi_ss1__0__HSIOM_MASK)
    #define UartPC_SS1_HSIOM_POS      (UartPC_spi_ss1__0__HSIOM_SHIFT)
    #define UartPC_SS1_HSIOM_SEL_GPIO (UartPC_spi_ss1__0__HSIOM_GPIO)
    #define UartPC_SS1_HSIOM_SEL_I2C  (UartPC_spi_ss1__0__HSIOM_I2C)
    #define UartPC_SS1_HSIOM_SEL_SPI  (UartPC_spi_ss1__0__HSIOM_SPI)
#endif /* (UartPC_SS1_PIN) */

#if (UartPC_SS2_PIN)
    #define UartPC_SS2_HSIOM_REG     (*(reg32 *) UartPC_spi_ss2__0__HSIOM)
    #define UartPC_SS2_HSIOM_PTR     ( (reg32 *) UartPC_spi_ss2__0__HSIOM)
    
    #define UartPC_SS2_HSIOM_MASK     (UartPC_spi_ss2__0__HSIOM_MASK)
    #define UartPC_SS2_HSIOM_POS      (UartPC_spi_ss2__0__HSIOM_SHIFT)
    #define UartPC_SS2_HSIOM_SEL_GPIO (UartPC_spi_ss2__0__HSIOM_GPIO)
    #define UartPC_SS2_HSIOM_SEL_I2C  (UartPC_spi_ss2__0__HSIOM_I2C)
    #define UartPC_SS2_HSIOM_SEL_SPI  (UartPC_spi_ss2__0__HSIOM_SPI)
#endif /* (UartPC_SS2_PIN) */

#if (UartPC_SS3_PIN)
    #define UartPC_SS3_HSIOM_REG     (*(reg32 *) UartPC_spi_ss3__0__HSIOM)
    #define UartPC_SS3_HSIOM_PTR     ( (reg32 *) UartPC_spi_ss3__0__HSIOM)
    
    #define UartPC_SS3_HSIOM_MASK     (UartPC_spi_ss3__0__HSIOM_MASK)
    #define UartPC_SS3_HSIOM_POS      (UartPC_spi_ss3__0__HSIOM_SHIFT)
    #define UartPC_SS3_HSIOM_SEL_GPIO (UartPC_spi_ss3__0__HSIOM_GPIO)
    #define UartPC_SS3_HSIOM_SEL_I2C  (UartPC_spi_ss3__0__HSIOM_I2C)
    #define UartPC_SS3_HSIOM_SEL_SPI  (UartPC_spi_ss3__0__HSIOM_SPI)
#endif /* (UartPC_SS3_PIN) */

#if (UartPC_I2C_PINS)
    #define UartPC_SCL_HSIOM_REG  (*(reg32 *) UartPC_scl__0__HSIOM)
    #define UartPC_SCL_HSIOM_PTR  ( (reg32 *) UartPC_scl__0__HSIOM)
    
    #define UartPC_SCL_HSIOM_MASK     (UartPC_scl__0__HSIOM_MASK)
    #define UartPC_SCL_HSIOM_POS      (UartPC_scl__0__HSIOM_SHIFT)
    #define UartPC_SCL_HSIOM_SEL_GPIO (UartPC_sda__0__HSIOM_GPIO)
    #define UartPC_SCL_HSIOM_SEL_I2C  (UartPC_sda__0__HSIOM_I2C)
    
    #define UartPC_SDA_HSIOM_REG  (*(reg32 *) UartPC_sda__0__HSIOM)
    #define UartPC_SDA_HSIOM_PTR  ( (reg32 *) UartPC_sda__0__HSIOM)
    
    #define UartPC_SDA_HSIOM_MASK     (UartPC_sda__0__HSIOM_MASK)
    #define UartPC_SDA_HSIOM_POS      (UartPC_sda__0__HSIOM_SHIFT)
    #define UartPC_SDA_HSIOM_SEL_GPIO (UartPC_sda__0__HSIOM_GPIO)
    #define UartPC_SDA_HSIOM_SEL_I2C  (UartPC_sda__0__HSIOM_I2C)
#endif /* (UartPC_I2C_PINS) */

#if (UartPC_SPI_SLAVE_PINS)
    #define UartPC_SCLK_S_HSIOM_REG   (*(reg32 *) UartPC_sclk_s__0__HSIOM)
    #define UartPC_SCLK_S_HSIOM_PTR   ( (reg32 *) UartPC_sclk_s__0__HSIOM)
    
    #define UartPC_SCLK_S_HSIOM_MASK      (UartPC_sclk_s__0__HSIOM_MASK)
    #define UartPC_SCLK_S_HSIOM_POS       (UartPC_sclk_s__0__HSIOM_SHIFT)
    #define UartPC_SCLK_S_HSIOM_SEL_GPIO  (UartPC_sclk_s__0__HSIOM_GPIO)
    #define UartPC_SCLK_S_HSIOM_SEL_SPI   (UartPC_sclk_s__0__HSIOM_SPI)
    
    #define UartPC_SS0_S_HSIOM_REG    (*(reg32 *) UartPC_ss0_s__0__HSIOM)
    #define UartPC_SS0_S_HSIOM_PTR    ( (reg32 *) UartPC_ss0_s__0__HSIOM)
    
    #define UartPC_SS0_S_HSIOM_MASK       (UartPC_ss0_s__0__HSIOM_MASK)
    #define UartPC_SS0_S_HSIOM_POS        (UartPC_ss0_s__0__HSIOM_SHIFT)
    #define UartPC_SS0_S_HSIOM_SEL_GPIO   (UartPC_ss0_s__0__HSIOM_GPIO)  
    #define UartPC_SS0_S_HSIOM_SEL_SPI    (UartPC_ss0_s__0__HSIOM_SPI)
#endif /* (UartPC_SPI_SLAVE_PINS) */

#if (UartPC_SPI_SLAVE_MOSI_PIN)
    #define UartPC_MOSI_S_HSIOM_REG   (*(reg32 *) UartPC_mosi_s__0__HSIOM)
    #define UartPC_MOSI_S_HSIOM_PTR   ( (reg32 *) UartPC_mosi_s__0__HSIOM)
    
    #define UartPC_MOSI_S_HSIOM_MASK      (UartPC_mosi_s__0__HSIOM_MASK)
    #define UartPC_MOSI_S_HSIOM_POS       (UartPC_mosi_s__0__HSIOM_SHIFT)
    #define UartPC_MOSI_S_HSIOM_SEL_GPIO  (UartPC_mosi_s__0__HSIOM_GPIO)
    #define UartPC_MOSI_S_HSIOM_SEL_SPI   (UartPC_mosi_s__0__HSIOM_SPI)
#endif /* (UartPC_SPI_SLAVE_MOSI_PIN) */

#if (UartPC_SPI_SLAVE_MISO_PIN)
    #define UartPC_MISO_S_HSIOM_REG   (*(reg32 *) UartPC_miso_s__0__HSIOM)
    #define UartPC_MISO_S_HSIOM_PTR   ( (reg32 *) UartPC_miso_s__0__HSIOM)
    
    #define UartPC_MISO_S_HSIOM_MASK      (UartPC_miso_s__0__HSIOM_MASK)
    #define UartPC_MISO_S_HSIOM_POS       (UartPC_miso_s__0__HSIOM_SHIFT)
    #define UartPC_MISO_S_HSIOM_SEL_GPIO  (UartPC_miso_s__0__HSIOM_GPIO)
    #define UartPC_MISO_S_HSIOM_SEL_SPI   (UartPC_miso_s__0__HSIOM_SPI)
#endif /* (UartPC_SPI_SLAVE_MISO_PIN) */

#if (UartPC_SPI_MASTER_MISO_PIN)
    #define UartPC_MISO_M_HSIOM_REG   (*(reg32 *) UartPC_miso_m__0__HSIOM)
    #define UartPC_MISO_M_HSIOM_PTR   ( (reg32 *) UartPC_miso_m__0__HSIOM)
    
    #define UartPC_MISO_M_HSIOM_MASK      (UartPC_miso_m__0__HSIOM_MASK)
    #define UartPC_MISO_M_HSIOM_POS       (UartPC_miso_m__0__HSIOM_SHIFT)
    #define UartPC_MISO_M_HSIOM_SEL_GPIO  (UartPC_miso_m__0__HSIOM_GPIO)
    #define UartPC_MISO_M_HSIOM_SEL_SPI   (UartPC_miso_m__0__HSIOM_SPI)
#endif /* (UartPC_SPI_MASTER_MISO_PIN) */

#if (UartPC_SPI_MASTER_MOSI_PIN)
    #define UartPC_MOSI_M_HSIOM_REG   (*(reg32 *) UartPC_mosi_m__0__HSIOM)
    #define UartPC_MOSI_M_HSIOM_PTR   ( (reg32 *) UartPC_mosi_m__0__HSIOM)
    
    #define UartPC_MOSI_M_HSIOM_MASK      (UartPC_mosi_m__0__HSIOM_MASK)
    #define UartPC_MOSI_M_HSIOM_POS       (UartPC_mosi_m__0__HSIOM_SHIFT)
    #define UartPC_MOSI_M_HSIOM_SEL_GPIO  (UartPC_mosi_m__0__HSIOM_GPIO)
    #define UartPC_MOSI_M_HSIOM_SEL_SPI   (UartPC_mosi_m__0__HSIOM_SPI)
#endif /* (UartPC_SPI_MASTER_MOSI_PIN) */

#if (UartPC_SPI_MASTER_SCLK_PIN)
    #define UartPC_SCLK_M_HSIOM_REG   (*(reg32 *) UartPC_sclk_m__0__HSIOM)
    #define UartPC_SCLK_M_HSIOM_PTR   ( (reg32 *) UartPC_sclk_m__0__HSIOM)
    
    #define UartPC_SCLK_M_HSIOM_MASK      (UartPC_sclk_m__0__HSIOM_MASK)
    #define UartPC_SCLK_M_HSIOM_POS       (UartPC_sclk_m__0__HSIOM_SHIFT)
    #define UartPC_SCLK_M_HSIOM_SEL_GPIO  (UartPC_sclk_m__0__HSIOM_GPIO)
    #define UartPC_SCLK_M_HSIOM_SEL_SPI   (UartPC_sclk_m__0__HSIOM_SPI)
#endif /* (UartPC_SPI_MASTER_SCLK_PIN) */

#if (UartPC_SPI_MASTER_SS0_PIN)
    #define UartPC_SS0_M_HSIOM_REG    (*(reg32 *) UartPC_ss0_m__0__HSIOM)
    #define UartPC_SS0_M_HSIOM_PTR    ( (reg32 *) UartPC_ss0_m__0__HSIOM)
    
    #define UartPC_SS0_M_HSIOM_MASK       (UartPC_ss0_m__0__HSIOM_MASK)
    #define UartPC_SS0_M_HSIOM_POS        (UartPC_ss0_m__0__HSIOM_SHIFT)
    #define UartPC_SS0_M_HSIOM_SEL_GPIO   (UartPC_ss0_m__0__HSIOM_GPIO)
    #define UartPC_SS0_M_HSIOM_SEL_SPI    (UartPC_ss0_m__0__HSIOM_SPI)
#endif /* (UartPC_SPI_MASTER_SS0_PIN) */

#if (UartPC_SPI_MASTER_SS1_PIN)
    #define UartPC_SS1_M_HSIOM_REG    (*(reg32 *) UartPC_ss1_m__0__HSIOM)
    #define UartPC_SS1_M_HSIOM_PTR    ( (reg32 *) UartPC_ss1_m__0__HSIOM)
    
    #define UartPC_SS1_M_HSIOM_MASK       (UartPC_ss1_m__0__HSIOM_MASK)
    #define UartPC_SS1_M_HSIOM_POS        (UartPC_ss1_m__0__HSIOM_SHIFT)
    #define UartPC_SS1_M_HSIOM_SEL_GPIO   (UartPC_ss1_m__0__HSIOM_GPIO)
    #define UartPC_SS1_M_HSIOM_SEL_SPI    (UartPC_ss1_m__0__HSIOM_SPI)
#endif /* (UartPC_SPI_MASTER_SS1_PIN) */

#if (UartPC_SPI_MASTER_SS2_PIN)
    #define UartPC_SS2_M_HSIOM_REG    (*(reg32 *) UartPC_ss2_m__0__HSIOM)
    #define UartPC_SS2_M_HSIOM_PTR    ( (reg32 *) UartPC_ss2_m__0__HSIOM)
    
    #define UartPC_SS2_M_HSIOM_MASK       (UartPC_ss2_m__0__HSIOM_MASK)
    #define UartPC_SS2_M_HSIOM_POS        (UartPC_ss2_m__0__HSIOM_SHIFT)
    #define UartPC_SS2_M_HSIOM_SEL_GPIO   (UartPC_ss2_m__0__HSIOM_GPIO)
    #define UartPC_SS2_M_HSIOM_SEL_SPI    (UartPC_ss2_m__0__HSIOM_SPI)
#endif /* (UartPC_SPI_MASTER_SS2_PIN) */

#if (UartPC_SPI_MASTER_SS3_PIN)
    #define UartPC_SS3_M_HSIOM_REG    (*(reg32 *) UartPC_ss3_m__0__HSIOM)
    #define UartPC_SS3_M_HSIOM_PTR    ( (reg32 *) UartPC_ss3_m__0__HSIOM)
    
    #define UartPC_SS3_M_HSIOM_MASK      (UartPC_ss3_m__0__HSIOM_MASK)
    #define UartPC_SS3_M_HSIOM_POS       (UartPC_ss3_m__0__HSIOM_SHIFT)
    #define UartPC_SS3_M_HSIOM_SEL_GPIO  (UartPC_ss3_m__0__HSIOM_GPIO)
    #define UartPC_SS3_M_HSIOM_SEL_SPI   (UartPC_ss3_m__0__HSIOM_SPI)
#endif /* (UartPC_SPI_MASTER_SS3_PIN) */

#if (UartPC_UART_RX_PIN)
    #define UartPC_RX_HSIOM_REG   (*(reg32 *) UartPC_rx__0__HSIOM)
    #define UartPC_RX_HSIOM_PTR   ( (reg32 *) UartPC_rx__0__HSIOM)
    
    #define UartPC_RX_HSIOM_MASK      (UartPC_rx__0__HSIOM_MASK)
    #define UartPC_RX_HSIOM_POS       (UartPC_rx__0__HSIOM_SHIFT)
    #define UartPC_RX_HSIOM_SEL_GPIO  (UartPC_rx__0__HSIOM_GPIO)
    #define UartPC_RX_HSIOM_SEL_UART  (UartPC_rx__0__HSIOM_UART)
#endif /* (UartPC_UART_RX_PIN) */

#if (UartPC_UART_RX_WAKE_PIN)
    #define UartPC_RX_WAKE_HSIOM_REG   (*(reg32 *) UartPC_rx_wake__0__HSIOM)
    #define UartPC_RX_WAKE_HSIOM_PTR   ( (reg32 *) UartPC_rx_wake__0__HSIOM)
    
    #define UartPC_RX_WAKE_HSIOM_MASK      (UartPC_rx_wake__0__HSIOM_MASK)
    #define UartPC_RX_WAKE_HSIOM_POS       (UartPC_rx_wake__0__HSIOM_SHIFT)
    #define UartPC_RX_WAKE_HSIOM_SEL_GPIO  (UartPC_rx_wake__0__HSIOM_GPIO)
    #define UartPC_RX_WAKE_HSIOM_SEL_UART  (UartPC_rx_wake__0__HSIOM_UART)
#endif /* (UartPC_UART_WAKE_RX_PIN) */

#if (UartPC_UART_CTS_PIN)
    #define UartPC_CTS_HSIOM_REG   (*(reg32 *) UartPC_cts__0__HSIOM)
    #define UartPC_CTS_HSIOM_PTR   ( (reg32 *) UartPC_cts__0__HSIOM)
    
    #define UartPC_CTS_HSIOM_MASK      (UartPC_cts__0__HSIOM_MASK)
    #define UartPC_CTS_HSIOM_POS       (UartPC_cts__0__HSIOM_SHIFT)
    #define UartPC_CTS_HSIOM_SEL_GPIO  (UartPC_cts__0__HSIOM_GPIO)
    #define UartPC_CTS_HSIOM_SEL_UART  (UartPC_cts__0__HSIOM_UART)
#endif /* (UartPC_UART_CTS_PIN) */

#if (UartPC_UART_TX_PIN)
    #define UartPC_TX_HSIOM_REG   (*(reg32 *) UartPC_tx__0__HSIOM)
    #define UartPC_TX_HSIOM_PTR   ( (reg32 *) UartPC_tx__0__HSIOM)
    
    #define UartPC_TX_HSIOM_MASK      (UartPC_tx__0__HSIOM_MASK)
    #define UartPC_TX_HSIOM_POS       (UartPC_tx__0__HSIOM_SHIFT)
    #define UartPC_TX_HSIOM_SEL_GPIO  (UartPC_tx__0__HSIOM_GPIO)
    #define UartPC_TX_HSIOM_SEL_UART  (UartPC_tx__0__HSIOM_UART)
#endif /* (UartPC_UART_TX_PIN) */

#if (UartPC_UART_RX_TX_PIN)
    #define UartPC_RX_TX_HSIOM_REG   (*(reg32 *) UartPC_rx_tx__0__HSIOM)
    #define UartPC_RX_TX_HSIOM_PTR   ( (reg32 *) UartPC_rx_tx__0__HSIOM)
    
    #define UartPC_RX_TX_HSIOM_MASK      (UartPC_rx_tx__0__HSIOM_MASK)
    #define UartPC_RX_TX_HSIOM_POS       (UartPC_rx_tx__0__HSIOM_SHIFT)
    #define UartPC_RX_TX_HSIOM_SEL_GPIO  (UartPC_rx_tx__0__HSIOM_GPIO)
    #define UartPC_RX_TX_HSIOM_SEL_UART  (UartPC_rx_tx__0__HSIOM_UART)
#endif /* (UartPC_UART_RX_TX_PIN) */

#if (UartPC_UART_RTS_PIN)
    #define UartPC_RTS_HSIOM_REG      (*(reg32 *) UartPC_rts__0__HSIOM)
    #define UartPC_RTS_HSIOM_PTR      ( (reg32 *) UartPC_rts__0__HSIOM)
    
    #define UartPC_RTS_HSIOM_MASK     (UartPC_rts__0__HSIOM_MASK)
    #define UartPC_RTS_HSIOM_POS      (UartPC_rts__0__HSIOM_SHIFT)    
    #define UartPC_RTS_HSIOM_SEL_GPIO (UartPC_rts__0__HSIOM_GPIO)
    #define UartPC_RTS_HSIOM_SEL_UART (UartPC_rts__0__HSIOM_UART)    
#endif /* (UartPC_UART_RTS_PIN) */


/***************************************
*        Registers Constants
***************************************/

/* HSIOM switch values. */ 
#define UartPC_HSIOM_DEF_SEL      (0x00u)
#define UartPC_HSIOM_GPIO_SEL     (0x00u)
/* The HSIOM values provided below are valid only for UartPC_CY_SCBIP_V0 
* and UartPC_CY_SCBIP_V1. It is not recommended to use them for 
* UartPC_CY_SCBIP_V2. Use pin name specific HSIOM constants provided 
* above instead for any SCB IP block version.
*/
#define UartPC_HSIOM_UART_SEL     (0x09u)
#define UartPC_HSIOM_I2C_SEL      (0x0Eu)
#define UartPC_HSIOM_SPI_SEL      (0x0Fu)

/* Pins settings index. */
#define UartPC_RX_WAKE_SCL_MOSI_PIN_INDEX   (0u)
#define UartPC_RX_SCL_MOSI_PIN_INDEX       (0u)
#define UartPC_TX_SDA_MISO_PIN_INDEX       (1u)
#define UartPC_CTS_SCLK_PIN_INDEX       (2u)
#define UartPC_RTS_SS0_PIN_INDEX       (3u)
#define UartPC_SS1_PIN_INDEX                  (4u)
#define UartPC_SS2_PIN_INDEX                  (5u)
#define UartPC_SS3_PIN_INDEX                  (6u)

/* Pins settings mask. */
#define UartPC_RX_WAKE_SCL_MOSI_PIN_MASK ((uint32) 0x01u << UartPC_RX_WAKE_SCL_MOSI_PIN_INDEX)
#define UartPC_RX_SCL_MOSI_PIN_MASK     ((uint32) 0x01u << UartPC_RX_SCL_MOSI_PIN_INDEX)
#define UartPC_TX_SDA_MISO_PIN_MASK     ((uint32) 0x01u << UartPC_TX_SDA_MISO_PIN_INDEX)
#define UartPC_CTS_SCLK_PIN_MASK     ((uint32) 0x01u << UartPC_CTS_SCLK_PIN_INDEX)
#define UartPC_RTS_SS0_PIN_MASK     ((uint32) 0x01u << UartPC_RTS_SS0_PIN_INDEX)
#define UartPC_SS1_PIN_MASK                ((uint32) 0x01u << UartPC_SS1_PIN_INDEX)
#define UartPC_SS2_PIN_MASK                ((uint32) 0x01u << UartPC_SS2_PIN_INDEX)
#define UartPC_SS3_PIN_MASK                ((uint32) 0x01u << UartPC_SS3_PIN_INDEX)

/* Pin interrupt constants. */
#define UartPC_INTCFG_TYPE_MASK           (0x03u)
#define UartPC_INTCFG_TYPE_FALLING_EDGE   (0x02u)

/* Pin Drive Mode constants. */
#define UartPC_PIN_DM_ALG_HIZ  (0u)
#define UartPC_PIN_DM_DIG_HIZ  (1u)
#define UartPC_PIN_DM_OD_LO    (4u)
#define UartPC_PIN_DM_STRONG   (6u)


/***************************************
*          Macro Definitions
***************************************/

/* Return drive mode of the pin */
#define UartPC_DM_MASK    (0x7u)
#define UartPC_DM_SIZE    (3u)
#define UartPC_GET_P4_PIN_DM(reg, pos) \
    ( ((reg) & (uint32) ((uint32) UartPC_DM_MASK << (UartPC_DM_SIZE * (pos)))) >> \
                                                              (UartPC_DM_SIZE * (pos)) )

#if (UartPC_TX_SDA_MISO_PIN)
    #define UartPC_CHECK_TX_SDA_MISO_PIN_USED \
                (UartPC_PIN_DM_ALG_HIZ != \
                    UartPC_GET_P4_PIN_DM(UartPC_uart_tx_i2c_sda_spi_miso_PC, \
                                                   UartPC_uart_tx_i2c_sda_spi_miso_SHIFT))
#endif /* (UartPC_TX_SDA_MISO_PIN) */

#if (UartPC_RTS_SS0_PIN)
    #define UartPC_CHECK_RTS_SS0_PIN_USED \
                (UartPC_PIN_DM_ALG_HIZ != \
                    UartPC_GET_P4_PIN_DM(UartPC_uart_rts_spi_ss0_PC, \
                                                   UartPC_uart_rts_spi_ss0_SHIFT))
#endif /* (UartPC_RTS_SS0_PIN) */

/* Set bits-mask in register */
#define UartPC_SET_REGISTER_BITS(reg, mask, pos, mode) \
                    do                                           \
                    {                                            \
                        (reg) = (((reg) & ((uint32) ~(uint32) (mask))) | ((uint32) ((uint32) (mode) << (pos)))); \
                    }while(0)

/* Set bit in the register */
#define UartPC_SET_REGISTER_BIT(reg, mask, val) \
                    ((val) ? ((reg) |= (mask)) : ((reg) &= ((uint32) ~((uint32) (mask)))))

#define UartPC_SET_HSIOM_SEL(reg, mask, pos, sel) UartPC_SET_REGISTER_BITS(reg, mask, pos, sel)
#define UartPC_SET_INCFG_TYPE(reg, mask, pos, intType) \
                                                        UartPC_SET_REGISTER_BITS(reg, mask, pos, intType)
#define UartPC_SET_INP_DIS(reg, mask, val) UartPC_SET_REGISTER_BIT(reg, mask, val)

/* UartPC_SET_I2C_SCL_DR(val) - Sets I2C SCL DR register.
*  UartPC_SET_I2C_SCL_HSIOM_SEL(sel) - Sets I2C SCL HSIOM settings.
*/
/* SCB I2C: scl signal */
#if (UartPC_CY_SCBIP_V0)
#if (UartPC_I2C_PINS)
    #define UartPC_SET_I2C_SCL_DR(val) UartPC_scl_Write(val)

    #define UartPC_SET_I2C_SCL_HSIOM_SEL(sel) \
                          UartPC_SET_HSIOM_SEL(UartPC_SCL_HSIOM_REG,  \
                                                         UartPC_SCL_HSIOM_MASK, \
                                                         UartPC_SCL_HSIOM_POS,  \
                                                         (sel))
    #define UartPC_WAIT_SCL_SET_HIGH  (0u == UartPC_scl_Read())

/* Unconfigured SCB: scl signal */
#elif (UartPC_RX_WAKE_SCL_MOSI_PIN)
    #define UartPC_SET_I2C_SCL_DR(val) \
                            UartPC_uart_rx_wake_i2c_scl_spi_mosi_Write(val)

    #define UartPC_SET_I2C_SCL_HSIOM_SEL(sel) \
                    UartPC_SET_HSIOM_SEL(UartPC_RX_WAKE_SCL_MOSI_HSIOM_REG,  \
                                                   UartPC_RX_WAKE_SCL_MOSI_HSIOM_MASK, \
                                                   UartPC_RX_WAKE_SCL_MOSI_HSIOM_POS,  \
                                                   (sel))

    #define UartPC_WAIT_SCL_SET_HIGH  (0u == UartPC_uart_rx_wake_i2c_scl_spi_mosi_Read())

#elif (UartPC_RX_SCL_MOSI_PIN)
    #define UartPC_SET_I2C_SCL_DR(val) \
                            UartPC_uart_rx_i2c_scl_spi_mosi_Write(val)


    #define UartPC_SET_I2C_SCL_HSIOM_SEL(sel) \
                            UartPC_SET_HSIOM_SEL(UartPC_RX_SCL_MOSI_HSIOM_REG,  \
                                                           UartPC_RX_SCL_MOSI_HSIOM_MASK, \
                                                           UartPC_RX_SCL_MOSI_HSIOM_POS,  \
                                                           (sel))

    #define UartPC_WAIT_SCL_SET_HIGH  (0u == UartPC_uart_rx_i2c_scl_spi_mosi_Read())

#else
    #define UartPC_SET_I2C_SCL_DR(val)        do{ /* Does nothing */ }while(0)
    #define UartPC_SET_I2C_SCL_HSIOM_SEL(sel) do{ /* Does nothing */ }while(0)

    #define UartPC_WAIT_SCL_SET_HIGH  (0u)
#endif /* (UartPC_I2C_PINS) */

/* SCB I2C: sda signal */
#if (UartPC_I2C_PINS)
    #define UartPC_WAIT_SDA_SET_HIGH  (0u == UartPC_sda_Read())
/* Unconfigured SCB: sda signal */
#elif (UartPC_TX_SDA_MISO_PIN)
    #define UartPC_WAIT_SDA_SET_HIGH  (0u == UartPC_uart_tx_i2c_sda_spi_miso_Read())
#else
    #define UartPC_WAIT_SDA_SET_HIGH  (0u)
#endif /* (UartPC_MOSI_SCL_RX_PIN) */
#endif /* (UartPC_CY_SCBIP_V0) */

/* Clear UART wakeup source */
#if (UartPC_RX_SCL_MOSI_PIN)
    #define UartPC_CLEAR_UART_RX_WAKE_INTR        do{ /* Does nothing */ }while(0)
    
#elif (UartPC_RX_WAKE_SCL_MOSI_PIN)
    #define UartPC_CLEAR_UART_RX_WAKE_INTR \
            do{                                      \
                (void) UartPC_uart_rx_wake_i2c_scl_spi_mosi_ClearInterrupt(); \
            }while(0)

#elif(UartPC_UART_RX_WAKE_PIN)
    #define UartPC_CLEAR_UART_RX_WAKE_INTR \
            do{                                      \
                (void) UartPC_rx_wake_ClearInterrupt(); \
            }while(0)
#else
#endif /* (UartPC_RX_SCL_MOSI_PIN) */


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

/* Unconfigured pins */
#define UartPC_REMOVE_MOSI_SCL_RX_WAKE_PIN    UartPC_REMOVE_RX_WAKE_SCL_MOSI_PIN
#define UartPC_REMOVE_MOSI_SCL_RX_PIN         UartPC_REMOVE_RX_SCL_MOSI_PIN
#define UartPC_REMOVE_MISO_SDA_TX_PIN         UartPC_REMOVE_TX_SDA_MISO_PIN
#ifndef UartPC_REMOVE_SCLK_PIN
#define UartPC_REMOVE_SCLK_PIN                UartPC_REMOVE_CTS_SCLK_PIN
#endif /* UartPC_REMOVE_SCLK_PIN */
#ifndef UartPC_REMOVE_SS0_PIN
#define UartPC_REMOVE_SS0_PIN                 UartPC_REMOVE_RTS_SS0_PIN
#endif /* UartPC_REMOVE_SS0_PIN */

/* Unconfigured pins */
#define UartPC_MOSI_SCL_RX_WAKE_PIN   UartPC_RX_WAKE_SCL_MOSI_PIN
#define UartPC_MOSI_SCL_RX_PIN        UartPC_RX_SCL_MOSI_PIN
#define UartPC_MISO_SDA_TX_PIN        UartPC_TX_SDA_MISO_PIN
#ifndef UartPC_SCLK_PIN
#define UartPC_SCLK_PIN               UartPC_CTS_SCLK_PIN
#endif /* UartPC_SCLK_PIN */
#ifndef UartPC_SS0_PIN
#define UartPC_SS0_PIN                UartPC_RTS_SS0_PIN
#endif /* UartPC_SS0_PIN */

#if (UartPC_MOSI_SCL_RX_WAKE_PIN)
    #define UartPC_MOSI_SCL_RX_WAKE_HSIOM_REG     UartPC_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define UartPC_MOSI_SCL_RX_WAKE_HSIOM_PTR     UartPC_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define UartPC_MOSI_SCL_RX_WAKE_HSIOM_MASK    UartPC_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define UartPC_MOSI_SCL_RX_WAKE_HSIOM_POS     UartPC_RX_WAKE_SCL_MOSI_HSIOM_REG

    #define UartPC_MOSI_SCL_RX_WAKE_INTCFG_REG    UartPC_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define UartPC_MOSI_SCL_RX_WAKE_INTCFG_PTR    UartPC_RX_WAKE_SCL_MOSI_HSIOM_REG

    #define UartPC_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS   UartPC_RX_WAKE_SCL_MOSI_HSIOM_REG
    #define UartPC_MOSI_SCL_RX_WAKE_INTCFG_TYPE_MASK  UartPC_RX_WAKE_SCL_MOSI_HSIOM_REG
#endif /* (UartPC_RX_WAKE_SCL_MOSI_PIN) */

#if (UartPC_MOSI_SCL_RX_PIN)
    #define UartPC_MOSI_SCL_RX_HSIOM_REG      UartPC_RX_SCL_MOSI_HSIOM_REG
    #define UartPC_MOSI_SCL_RX_HSIOM_PTR      UartPC_RX_SCL_MOSI_HSIOM_PTR
    #define UartPC_MOSI_SCL_RX_HSIOM_MASK     UartPC_RX_SCL_MOSI_HSIOM_MASK
    #define UartPC_MOSI_SCL_RX_HSIOM_POS      UartPC_RX_SCL_MOSI_HSIOM_POS
#endif /* (UartPC_MOSI_SCL_RX_PIN) */

#if (UartPC_MISO_SDA_TX_PIN)
    #define UartPC_MISO_SDA_TX_HSIOM_REG      UartPC_TX_SDA_MISO_HSIOM_REG
    #define UartPC_MISO_SDA_TX_HSIOM_PTR      UartPC_TX_SDA_MISO_HSIOM_REG
    #define UartPC_MISO_SDA_TX_HSIOM_MASK     UartPC_TX_SDA_MISO_HSIOM_REG
    #define UartPC_MISO_SDA_TX_HSIOM_POS      UartPC_TX_SDA_MISO_HSIOM_REG
#endif /* (UartPC_MISO_SDA_TX_PIN_PIN) */

#if (UartPC_SCLK_PIN)
    #ifndef UartPC_SCLK_HSIOM_REG
    #define UartPC_SCLK_HSIOM_REG     UartPC_CTS_SCLK_HSIOM_REG
    #define UartPC_SCLK_HSIOM_PTR     UartPC_CTS_SCLK_HSIOM_PTR
    #define UartPC_SCLK_HSIOM_MASK    UartPC_CTS_SCLK_HSIOM_MASK
    #define UartPC_SCLK_HSIOM_POS     UartPC_CTS_SCLK_HSIOM_POS
    #endif /* UartPC_SCLK_HSIOM_REG */
#endif /* (UartPC_SCLK_PIN) */

#if (UartPC_SS0_PIN)
    #ifndef UartPC_SS0_HSIOM_REG
    #define UartPC_SS0_HSIOM_REG      UartPC_RTS_SS0_HSIOM_REG
    #define UartPC_SS0_HSIOM_PTR      UartPC_RTS_SS0_HSIOM_PTR
    #define UartPC_SS0_HSIOM_MASK     UartPC_RTS_SS0_HSIOM_MASK
    #define UartPC_SS0_HSIOM_POS      UartPC_RTS_SS0_HSIOM_POS
    #endif /* UartPC_SS0_HSIOM_REG */
#endif /* (UartPC_SS0_PIN) */

#define UartPC_MOSI_SCL_RX_WAKE_PIN_INDEX UartPC_RX_WAKE_SCL_MOSI_PIN_INDEX
#define UartPC_MOSI_SCL_RX_PIN_INDEX      UartPC_RX_SCL_MOSI_PIN_INDEX
#define UartPC_MISO_SDA_TX_PIN_INDEX      UartPC_TX_SDA_MISO_PIN_INDEX
#ifndef UartPC_SCLK_PIN_INDEX
#define UartPC_SCLK_PIN_INDEX             UartPC_CTS_SCLK_PIN_INDEX
#endif /* UartPC_SCLK_PIN_INDEX */
#ifndef UartPC_SS0_PIN_INDEX
#define UartPC_SS0_PIN_INDEX              UartPC_RTS_SS0_PIN_INDEX
#endif /* UartPC_SS0_PIN_INDEX */

#define UartPC_MOSI_SCL_RX_WAKE_PIN_MASK UartPC_RX_WAKE_SCL_MOSI_PIN_MASK
#define UartPC_MOSI_SCL_RX_PIN_MASK      UartPC_RX_SCL_MOSI_PIN_MASK
#define UartPC_MISO_SDA_TX_PIN_MASK      UartPC_TX_SDA_MISO_PIN_MASK
#ifndef UartPC_SCLK_PIN_MASK
#define UartPC_SCLK_PIN_MASK             UartPC_CTS_SCLK_PIN_MASK
#endif /* UartPC_SCLK_PIN_MASK */
#ifndef UartPC_SS0_PIN_MASK
#define UartPC_SS0_PIN_MASK              UartPC_RTS_SS0_PIN_MASK
#endif /* UartPC_SS0_PIN_MASK */

#endif /* (CY_SCB_PINS_UartPC_H) */


/* [] END OF FILE */
