/* **********************************************************************/ /**
 \file      \arg  Globals.h
 \brief  
 \date		\arg  Created on: July 24.20
			\arg  Last Edit: July 24.20
 \author	\arg  Gabriele Cristini
			\arg  \b 
 */
/* ***********************************************************************/

#ifndef _GLOBALS_H
#define _GLOBALS_H


/* **********************************************************************/
/* ***                Definition of global macros                     ***/
/* **********************************************************************/
/* Others */
#define PIN_HIGH    1
#define PIN_LOW     0

/* Active High Relay */
#define PACKP_ON    PIN_HIGH
#define PACKP_OFF   PIN_LOW
#define SCL_ON      PIN_HIGH
#define SCL_OFF     PIN_LOW
#define SDA_ON      PIN_HIGH
#define SDA_OFF     PIN_LOW
#define DETECT_ON   PIN_HIGH
#define DETECT_OFF  PIN_LOW 

/* Active Low Relay */
#define NTC_ON      PIN_LOW
#define NTC_OFF     PIN_HIGH



#endif /* GLOBALS */