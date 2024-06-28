

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MCAL_STD_TYPES_H
#define	MCAL_STD_TYPES_H
/*section : includes*/
#include "std_libraries.h"
#include "compiler.h"

/*section : macro declarations*/
#define STD_HIGH 0X01
#define STD_LOW 0X00

#define config_enable 0x01
#define config_disable 0x00
#define STD_ON 0X01
#define STD_OFF 0X00

#define STD_ACTIVE 0X01
#define STD_IDLE 0X00
#define E_OK         (std_returntype)0X01
#define E_NOT_OK         (std_returntype)0X00
/*section : macro functions*/

/*section : data type declarartion*/
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef signed char sint8;
typedef signed short sint16;
typedef signed long sint32;

 typedef uint8 std_returntype;

/*section :functions declaration*/
#endif	/* MCAL_STD_TYPES_H */

