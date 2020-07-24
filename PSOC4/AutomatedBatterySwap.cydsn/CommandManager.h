/* **********************************************************************/ /**
 \file      \arg  CommandManager.h
 \brief  
 \date		\arg  Created on: July 24.20
			\arg  Last Edit: July 24.20
 \author	\arg  Gabriele Cristini
			\arg  \b 
 */
/* ***********************************************************************/

#ifndef _COMMAND_MANAGER_H
#define _COMMAND_MANAGER_H


/* **********************************************************************/
/* ***              System and library files included                 ***/
/* **********************************************************************/
#include <stdlib.h>
#include "project.h"

/* **********************************************************************/
/* ***                 Other components included                      ***/
/* **********************************************************************/


/* **********************************************************************/
/* ***              Declaration of global functions                   ***/
/* **********************************************************************/
void CMD_v_Init         (void);
void CMD_v_ParseCommand (uint8* pu8_command);



#endif /* _COMMAND_MANAGER_H */




