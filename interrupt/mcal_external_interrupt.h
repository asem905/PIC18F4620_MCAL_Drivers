/* 
 * File:   mcal_external_interrupt.h
 * Author: Asem
 *
 * Created on February 8, 2024, 11:20 PM
 */

#ifndef MCAL_EXTERNAL_INTERRUPT_H
#define	MCAL_EXTERNAL_INTERRUPT_H
/*section : includes*/
#include "mcal_interrupt_config.h"

/*section : macro declarations*/
#if external_interrupt_INTx_feature_enable == interrupt_feature_enable
//of INT0:
#define ext_INT0_interruptdisable() (INTCONbits.INT0IE=0)
#define ext_INT0_interruptenable() (INTCONbits.INT0IE=1)
#define ext_INT0_interruptflagclear() (INTCONbits.INT0IF=0)
#define ext_INT0_risingedgeset() (INTCON2bits.INTEDG0=1)
#define ext_INT0_fallingedgeset() (INTCON2bits.INTEDG0=0)

//of INT1:
#define ext_INT1_interruptdisable() (INTCON3bits.INT1IE=0)
#define ext_INT1_interruptenable() (INTCON3bits.INT1IE=1)
#define ext_INT1_interruptflagclear() (INTCON3bits.INT1IF=0)
#define ext_INT1_risingedgeset() (INTCON2bits.INTEDG1=1)
#define ext_INT1_fallingedgeset() (INTCON2bits.INTEDG1=0)

//of INT2:
#define ext_INT2_interruptdisable() (INTCON3bits.INT2IE=0)
#define ext_INT2_interruptenable() (INTCON3bits.INT2IE=1)
#define ext_INT2_interruptflagclear() (INTCON3bits.INT2IF=0)
#define ext_INT2_risingedgeset() (INTCON2bits.INTEDG2=1)
#define ext_INT2_fallingedgeset() (INTCON2bits.INTEDG2=0)

#if interrupt_priority_levels_enable == interrupt_feature_enable
//INT1
#define ext_INT1_highpriorityset() (INTCON3bits.INT1IP=1)
#define ext_INT1_lowpriorityset() (INTCON3bits.INT1IP=0)
//INT2
#define ext_INT2_highpriorityset() (INTCON3bits.INT2IP=1)
#define ext_INT2_lowpriorityset() (INTCON3bits.INT2IP=0)

#endif
#endif

#if external_interrupt_onchange_feature_enable == interrupt_feature_enable
#define ext_RBx_interruptdisable() (INTCONbits.RBIE=0)
#define ext_RBx_interruptenable() (INTCONbits.RBIE=1)
#define ext_RBx_interruptflagclear() (INTCONbits.RBIF=0)

#if interrupt_priority_levels_enable == interrupt_feature_enable

#define ext_RBx_priority_high() (INTCON2bits.RBIP=1)
#define ext_RBx_priority_low() (INTCON2bits.RBIP=0)

#endif

#endif
/*section : macro functions*/

/*section : data type declarartion*/
typedef void (*interrupthandler)(void);//function pointer of interrupt handler we can make from it many other variables
typedef enum{
    interrupt_falling_edge=0,interrupt_rising_edge
}interrupt_INTx_edge;
typedef enum{
    interrupt_external_INT0=0,
    interrupt_external_INT1,
    interrupt_external_INT2
}interrupt_INTx_src;
typedef struct{
    void (* ext_interrupthandler) (void);//function pointer to store address of function that i will jump to if interrupt happens  s it is in upper layer of main application
    pin_config mcu_pin;//on what pin on microcontroller
    interrupt_INTx_edge edge;
    interrupt_INTx_src source;
    interrupt_priority_cfg priority;
}interrupt_INTx_t;

typedef struct{
    void (* ext_interrupthandler_high) (void);
    void (* ext_interrupthandler_low) (void);
    pin_config mcu_pin;
    interrupt_priority_cfg priority;
}interrupt_RBx_t;
/*section :functions declaration*/
std_returntype interrupt_INTx_init(const interrupt_INTx_t *int_obj);
std_returntype interrupt_INTx_deinit(const interrupt_INTx_t *int_obj);
std_returntype interrupt_RBx_init(const interrupt_RBx_t *int_obj);
std_returntype interrupt_RBx_deinit(const interrupt_RBx_t *int_obj);
#endif


