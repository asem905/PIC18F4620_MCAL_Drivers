/* 
 * File:   mcal_interrupt_config.h
 * Author: Asem
 *
 * Created on February 8, 2024, 11:19 PM
 */

#ifndef MCAL_INTERRUPT_CONFIG_H
#define	MCAL_INTERRUPT_CONFIG_H

/*section : includes*/
#include "../proc/pic18f4620.h"
#include "../../MCAL_Layer/mcal_std_types.h"
#include "mcal_interrupt_gen_cfg.h"
#include "../GPIO/hal_gpio.h"
/*section : macro declarations*/
#define interrupt_enable 1
#define interrupt_disable 0
#define interrupt_occur 1
#define interrupt_notoccur 0
#define interrupt_priority_enable 1
#define interrupt_priority_disable 0
/*section : macro functions*/
#if interrupt_priority_levels_enable==interrupt_feature_enable

//to enable priority feature
#define interrupt_prioritylevelsenable() (RCONbits.IPEN=1)
#define interrupt_prioritylevelsdisable() (RCONbits.IPEN=0)

//as we said it enables all high priority in intcon register from data sheet we know  
#define interrupt_globalinterrupthighenable() (INTCONbits.GIEH=1)
#define interrupt_globalinterrupthighdisable() (INTCONbits.GIEH=0)
//enable low priority
#define interrupt_globalinterruptlowenable() (INTCONbits.GIEL=1)
#define interrupt_globalinterruptlowdisable() (INTCONbits.GIEL=0)
//previous is important if priority feature is enabled

#else
//as we said it enables global interrupts  
#define interrupt_globalinterruptenable() (INTCONbits.GIE=1)
//as we said it disables global interrupts 
#define interrupt_globalinterruptdisable() (INTCONbits.GIE=0)
//to enable all peripherals interrupt which is Same bit as giel
#define interrupt_peripheralinterruptenable() (INTCONbits.PEIE=1)
//to disable all peripherals interrupt which is Same bit as giel
#define interrupt_peripheralinterruptdisable() (INTCONbits.PEIE=0)
#endif

/*section : data type declarartion*/
typedef enum{
    interrupt_low_priority=0,
    interrupt_high_priority
}interrupt_priority_cfg;
/*section :functions declaration*/

#endif	/* MCAL_INTERRUPT_CONFIG_H */

