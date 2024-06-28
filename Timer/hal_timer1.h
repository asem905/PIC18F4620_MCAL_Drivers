/* 
 * File:   hal_timer1.h
 * Author: Asem
 *
 * Created on March 9, 2024, 10:49 PM
 */

#ifndef HAL_TIMER1_H
#define	HAL_TIMER1_H

/*section : includes*/
#include "../mcal_std_types.h"
#include "../proc/pic18f4620.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "../../MCAL_Layer/interrupt/mcal_internal_interrupt.h"
/*section : macro declarations*/
#define timer1_counter_mode 1
#define timer1_timer_mode 0

#define timer1_async_counter_mode 1
#define timer1_sync_counter_mode 0

#define timer1_osc_enable 1
#define timer1_osc_disable 0

#define timer1_prescaler_div_by_1 0
#define timer1_prescaler_div_by_2 1
#define timer1_prescaler_div_by_4 2
#define timer1_prescaler_div_by_8 3

/*to define how i read my value as two bytes or one 16 bits:*/
#define timer1_RW_reg_8bit_mode 0
#define timer1_RW_reg_16bit_mode 1
/*section : macro functions*/
#define timer1_timer_mode_enable() (T1CONbits.TMR1CS=0)
#define timer1_counter_mode_enable() (T1CONbits.TMR1CS=1)//by selecting what clock source

#define timer1_module_enable() (T1CONbits.TMR1ON=1)
#define timer1_module_disable() (T1CONbits.TMR1ON=0)

#define timer1_prescaler_select(_prescaler_) (T1CONbits.T1CKPS = _prescaler_)/* this bit T1CKPS is of two bits 
so we write in it value given to me in variable _prescaler_ value given to me is one of (timer1_prescaler_div_by_1 or 
timer1_prescaler_div_by_2 or ...)*/


#define timer1_async_counter_mode_enable() (T1CONbits.T1SYNC=1)
#define timer1_sync_counter_mode_enable() (T1CONbits.T1SYNC=0)

#define timer1_osc_hw_enable() (T1CONbits.T1OSCEN=1)//for timer1oscillator 
#define timer1_osc_hw_disable() (T1CONbits.T1OSCEN=0)

#define timer1_system_clk_status() (T1CONbits.T1RUN)/*to define if my system works by timer1oscillator or from other source*/

/*to define how i read my value:*/
#define timer1_RW_reg_8bit_mode_enable (T1CONbits.RD16=0)
#define timer1_RW_reg_16bit_mode_enable (T1CONbits.RD16=1)
/*section : data type declarartion*/
typedef struct{
    #if timer1_interrupt_feature_enable ==interrupt_feature_enable
    void (*timer1_InterruptHandler)(void);//to call back to upper layer
    interrupt_priority_cfg priority;
    #endif
    uint16 timer1_preload_value;
    uint8 timer1_prescaler_value :2;
    uint8 timer1_mode :1; 
    uint8 timer1_counter_mode_select :1; //for defining sync or async
    uint8 timer1_oscillator_mode :1;
    uint8 timer1_reg_wr_mode :1;
    uint8 timer1_reserved :2;
}timer1_t;
/*section :functions declaration*/
std_returntype timer1_init(const timer1_t* _timer);
std_returntype timer1_deinit(const timer1_t* _timer);
std_returntype timer1_write_value(const timer1_t* _timer,uint16 _value);
std_returntype timer1_read_value(const timer1_t* _timer,uint16 *_value);
#endif	/* HAL_TIMER1_H */

