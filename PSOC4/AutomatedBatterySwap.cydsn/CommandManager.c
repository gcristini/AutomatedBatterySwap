/* **********************************************************************/ /**
 \file      \arg  CommandManager.C
 \brief  
 \date		\arg  Created on: July 24.20
			\arg  Last Edit: July 24.20
 \author	\arg  Gabriele Cristini
			\arg  \b 
 */
/* ***********************************************************************/

/* **********************************************************************/
/* ***              System and library files included                 ***/
/* **********************************************************************/
#include <stdio.h>
#include <math.h>


/* **********************************************************************/
/* ***                 Other components included                      ***/
/* **********************************************************************/
#include "CommandManager.h"
#include "CyLib.h"
#include "Globals.h"


/* **********************************************************************/
/* ***                Definition of local macros                      ***/
/* **********************************************************************/
#define CMD_RELAYS_SEPARATOR    "_"
#define CMD_STATUS_SEPARATOR    " "


/* **********************************************************************/
/* ***                Definition of local types                       ***/
/* **********************************************************************/
typedef enum
{
    EN_RELAY_FUN_PACKP      = 0,
    EN_RELAY_FUN_SCL        = 1,
    EN_RELAY_FUN_SDA        = 2,
    EN_RELAY_FUN_DETECT     = 3,
    EN_RELAY_FUN_NTC        = 4,
    EN_RELAY_FUN_ALL        = 5,
    EN_RELAY_FUN_MAX_ENUM   = 6
}EN_RELAY_FUNCTION_ENUM_TYPE;

typedef struct
{
    uint8   *pu8_header;
    void    (*pv_function)(uint8*);   
}ST_PARSER_FUNCTION_STRUCT_TYPE;


/* **********************************************************************/
/* ***                     Local variables                            ***/
/* **********************************************************************/


/* **********************************************************************/
/* ***             Declaration of local variables                     ***/
/* **********************************************************************/
ST_PARSER_FUNCTION_STRUCT_TYPE st_parserFunction[EN_RELAY_FUN_MAX_ENUM];

/* **********************************************************************/
/* ***             Declaration of local functions                     ***/
/* **********************************************************************/
void    CMD_v_PackpRelayFun    (uint8 *pu8_PinState);
void    CMD_v_SclRelayFun      (uint8 *pu8_PinState);
void    CMD_v_SdaRelayFun      (uint8 *pu8_PinState);
void    CMD_v_DetectRelayFun   (uint8 *pu8_PinState);
void    CMD_v_NtcRelayFun      (uint8 *pu8_PinState);
void    CMD_v_AllRelayFun      (uint8 *pu8_PinState);

/* **********************************************************************/
/* ***             Definition of local functions                      ***/
/* **********************************************************************/
/* *********************************************************************************/ /**
   \fn      	void CMD_v_PackpRelayFun(uint8 *pu8_PinState)
   \brief   	\n
            	Scope: Local
   \return  	void
   \author		\arg Gabriele Cristini
   \date		\arg Creation:  July 24.20
				\arg Last Edit: July 24.20
 */
/* **********************************************************************************/
void CMD_v_PackpRelayFun(uint8 *pu8_PinState)
{   
    if (!strcmp((const char*)pu8_PinState, "on"))
    {
        pin_PackP_Write(PACKP_ON);
        #ifdef PRINT_DEBUG
        UartPC_UartPutString("PACKP_ON\r\n");
        #endif
    }
    else if(!strcmp((const char*)pu8_PinState, "off"))
    {
        pin_PackP_Write(PACKP_OFF);
        #ifdef PRINT_DEBUG
        UartPC_UartPutString("PACKP_OFF\r\n");
        #endif
    }
    else
    {
        /* MISRA */
    }    
    
    return;
}

/* *********************************************************************************/ /**
   \fn      	void CMD_v_SclRelayFun(uint8 *pu8_PinState)
   \brief   	\n
            	Scope: Local
   \return  	void
   \author		\arg Gabriele Cristini
   \date		\arg Creation:  July 24.20
				\arg Last Edit: July 24.20
 */
/* **********************************************************************************/
void CMD_v_SclRelayFun(uint8 *pu8_PinState)
{
    if (!strcmp((const char*)pu8_PinState, "on"))
    {
        pin_SCL_Write(SCL_ON);
        #ifdef PRINT_DEBUG
        UartPC_UartPutString("SCL_ON\r\n");
        #endif
    }
    else if(!strcmp((const char*)pu8_PinState, "off"))
    {
        pin_SCL_Write(SCL_OFF);
        #ifdef PRINT_DEBUG
        UartPC_UartPutString("SCL_OFF\r\n");
        #endif
        
    }
    else
    {
        /* MISRA */
    }    

    return;
}

/* *********************************************************************************/ /**
   \fn      	void CMD_v_SdaRelayFun(uint8 *pu8_PinState)
   \brief   	\n
            	Scope: Local
   \return  	void
   \author		\arg Gabriele Cristini
   \date		\arg Creation:  July 24.20
				\arg Last Edit: July 24.20
 */
/* **********************************************************************************/
void CMD_v_SdaRelayFun(uint8 *pu8_PinState)
{
    if (!strcmp((const char*)pu8_PinState, "on"))
    {
        pin_SDA_Write(SDA_ON);
        #ifdef PRINT_DEBUG
        UartPC_UartPutString("SDA_ON\r\n");
        #endif
    }
    else if(!strcmp((const char*)pu8_PinState, "off"))
    {
        pin_SDA_Write(SDA_OFF);
        #ifdef PRINT_DEBUG
        UartPC_UartPutString("SDA_OFF\r\n");
        #endif
    }
    else
    {
        /* MISRA */
    }    

    return;
}

/* *********************************************************************************/ /**
   \fn      	void CMD_v_DetectRelayFun(uint8 *pu8_PinState)
   \brief   	\n
            	Scope: Local
   \return  	void
   \author		\arg Gabriele Cristini
   \date		\arg Creation:  July 24.20
				\arg Last Edit: July 24.20
 */
/* **********************************************************************************/
void CMD_v_DetectRelayFun(uint8 *pu8_PinState)
{
    if (!strcmp((const char*)pu8_PinState, "on"))
    {
        pin_Detect_Write(DETECT_ON);
        #ifdef PRINT_DEBUG
        UartPC_UartPutString("DETECT_ON\r\n");
        #endif
    }
    else if(!strcmp((const char*)pu8_PinState, "off"))
    {
        pin_Detect_Write(DETECT_OFF);
        #ifdef PRINT_DEBUG
        UartPC_UartPutString("DETECT_OFF\r\n");
        #endif
    }
    else
    {
        /* MISRA */
    }    
    
    return;
}

/* *********************************************************************************/ /**
   \fn      	void CMD_v_NtcRelayFun(uint8 *pu8_PinState)
   \brief   	\n
            	Scope: Local
   \return  	void
   \author		\arg Gabriele Cristini
   \date		\arg Creation:  July 24.20
				\arg Last Edit: July 24.20
 */
/* **********************************************************************************/
void CMD_v_NtcRelayFun(uint8 *pu8_PinState)
{
    if (!strcmp((const char*)pu8_PinState, "on"))
    {
        pin_NTC_Write(NTC_ON);
        #ifdef PRINT_DEBUG
        UartPC_UartPutString("NTC_ON\r\n");
        #endif
    }
    else if(!strcmp((const char*)pu8_PinState, "off"))
    {
        pin_NTC_Write(NTC_OFF);
        #ifdef PRINT_DEBUG
        UartPC_UartPutString("NTC_OFF\r\n");
        #endif
    }
    else
    {
        /* MISRA */
    }    
    
    return;
}

/* *********************************************************************************/ /**
   \fn      	void CMD_v_AllRelayFun(uint8 *pu8_PinState)
   \brief   	\n
            	Scope: Local
   \return  	void
   \author		\arg Gabriele Cristini
   \date		\arg Creation:  July 24.20
				\arg Last Edit: July 24.20
 */
/* **********************************************************************************/
void CMD_v_AllRelayFun(uint8 *pu8_PinState)
{
    if (!strcmp((const char*)pu8_PinState, "on"))
    {
        pin_PackP_Write(PACKP_ON);
        pin_SCL_Write(SCL_ON);
        pin_SDA_Write(SDA_ON);
        pin_Detect_Write(DETECT_ON);
        pin_NTC_Write(NTC_ON);
        #ifdef PRINT_DEBUG
        UartPC_UartPutString("ALL_ON\r\n");
        #endif
    }
    else if(!strcmp((const char*)pu8_PinState, "off"))
    {
        pin_PackP_Write(PACKP_OFF);
        pin_SCL_Write(SCL_OFF);
        pin_SDA_Write(SDA_OFF);
        pin_Detect_Write(DETECT_OFF);
        pin_NTC_Write(NTC_OFF);
        #ifdef PRINT_DEBUG
        UartPC_UartPutString("ALL_OFF\r\n");
        #endif
    }
    else
    {
        /* MISRA */
    }    
    
    return;
}

/* **********************************************************************/
/* ***            Definition of global functions                      ***/
/* **********************************************************************/
/* *********************************************************************************/ /**
   \fn      	void CMD_v_Init(void) 
   \brief   	Initialize Command Manager Module\n
            	Scope: Global
   \return  	void
   \author		\arg Gabriele Cristini
   \date		\arg Creation:  July 24.20
				\arg Last Edit: July 24.20
 */
/* **********************************************************************************/
void CMD_v_Init(void)
{   
    /* Initialize parser struct array */
    st_parserFunction[EN_RELAY_FUN_PACKP].pu8_header    = (uint8*)"packp";
    st_parserFunction[EN_RELAY_FUN_PACKP].pv_function   = CMD_v_PackpRelayFun;
    st_parserFunction[EN_RELAY_FUN_SCL].pu8_header      = (uint8*)"scl";
    st_parserFunction[EN_RELAY_FUN_SCL].pv_function     = CMD_v_SclRelayFun;
    st_parserFunction[EN_RELAY_FUN_SDA].pu8_header      = (uint8*)"sda";
    st_parserFunction[EN_RELAY_FUN_SDA].pv_function     = CMD_v_SdaRelayFun;
    st_parserFunction[EN_RELAY_FUN_DETECT].pu8_header   = (uint8*)"detect";
    st_parserFunction[EN_RELAY_FUN_DETECT].pv_function  = CMD_v_DetectRelayFun;
    st_parserFunction[EN_RELAY_FUN_NTC].pu8_header      = (uint8*)"ntc";
    st_parserFunction[EN_RELAY_FUN_NTC].pv_function     = CMD_v_NtcRelayFun;
    st_parserFunction[EN_RELAY_FUN_ALL].pu8_header      = (uint8*)"all";
    st_parserFunction[EN_RELAY_FUN_ALL].pv_function     = CMD_v_AllRelayFun;

    return;
}

/* *********************************************************************************/ /**
   \fn      	void CMD_v_ParseCommand(uint8* pu8_command)
{
   \brief   	Parse command recevied \n
            	Scope: Local
   \return  	void
   \author		\arg Gabriele Cristini
   \date		\arg Creation:  July 24.20
				\arg Last Edit: July 24.20
 */
/* **********************************************************************************/
void CMD_v_ParseCommand(uint8* pu8_command)
{   
    /* Variables */
    uint8                       *pu8_relayStatus;
    uint8                       *pu8_relay;
    EN_RELAY_FUNCTION_ENUM_TYPE en_index;

    /* Extract the Relay Status */
    (void)strtok((char*)pu8_command, (char*)CMD_STATUS_SEPARATOR);
    pu8_relayStatus = (uint8*)strtok(NULL, (char*)CMD_STATUS_SEPARATOR);
              
    if(pu8_relayStatus != NULL)
    {  
        /* Extract the relay(s) to command */ 
        pu8_relay = (uint8*)strtok((char*)pu8_command, (char*)CMD_RELAYS_SEPARATOR);
        
        /* Search for relative function and execute */ 
        do
        {
            for (en_index=0; en_index < EN_RELAY_FUN_MAX_ENUM; en_index++)
            {                
                if (!strcmp((const char*)st_parserFunction[en_index].pu8_header, (const char*)pu8_relay))
                {
                    st_parserFunction[en_index].pv_function(pu8_relayStatus);   
                }                
            }
            pu8_relay = (uint8*)strtok(NULL, CMD_RELAYS_SEPARATOR);

        }
        while (pu8_relay != NULL);
    }

    return;
}
