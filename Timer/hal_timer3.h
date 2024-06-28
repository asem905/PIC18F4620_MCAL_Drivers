/* 
 * File:   hal_timer3.h
 * Author: Asem
 *
 * Created on March 11, 2024, 9:30 PM
 */

#ifndef HAL_TIMER3_H
#define	HAL_TIMER3_H

/*section : includes*/
#include "../mcal_std_types.h"
#include "../proc/pic18f4620.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "../../MCAL_Layer/interrupt/mcal_internal_interrupt.h"
/*section : macro declarations*/
#define timer3_counter_mode 1
#define timer3_timer_mode 0

#define timer3_async_counter_mode 1
#define timer3_sync_counter_mode 0

//#define timer3_osc_enable 1
//#define timer3_osc_enable 0:not found unless in timer1 but it can work with it

#define timer3_prescaler_div_by_1 0
#define timer3_prescaler_div_by_2 1
#define timer3_prescaler_div_by_4 2
#define timer3_prescaler_div_by_8 3
/*to define how i read my value as two bytes or one 16 bits:*/
#define timer3_RW_reg_8bit_mode 0
#define timer3_RW_reg_16bit_mode 1

/*section : macro functions*/
#define timer3_timer_mode_enable() (T3CONbits.TMR3CS=0)
#define timer3_counter_mode_enable() (T3CONbits.TMR3CS=1)//by selecting what clock source

#define timer3_module_enable() (T3CONbits.TMR3ON=1)
#define timer3_module_disable() (T3CONbits.TMR3ON=0)

#define timer3_prescaler_select(_prescaler_) (T3CONbits.T3CKPS = _prescaler_)/* this bit T1CKPS is of two bits 
so we write in it value given to me in variable _prescaler_ value given to me is one of (timer1_prescaler_div_by_1 or 
timer1_prescaler_div_by_2 or ...)*/

#define timer3_async_counter_mode_enable() (T3CONbits.T3SYNC=1)
#define timer3_sync_counter_mode_enable() (T3CONbits.T3SYNC=0)

#define timer3_system_clk_status() (T1CONbits.T1RUN)/*to define if my system works by timer1oscillator or from other source*/

/*to define how i read my value:*/
#define timer3_RW_reg_8bit_mode_enable (T3CONbits.RD16=0)
#define timer3_RW_reg_16bit_mode_enable (T3CONbits.RD16=1)
/*section : data type declarartion*/
typedef struct{
    #if timer3_interrupt_feature_enable ==interrupt_feature_enable
    void (*timer3_InterruptHandler)(void);//to call back to upper layer
    interrupt_priority_cfg priority;
    #endif
    uint16 timer3_preload_value;
    uint8 timer3_prescaler_value :2;
    uint8 timer3_mode :1; 
    uint8 timer3_counter_mode_select :1; //for defining sync or async
    uint8 timer3_reg_wr_mode :1;
    uint8 timer3_reserved :3;
}timer3_t;
/*section :functions declaration*/
std_returntype timer3_init(const timer3_t* _timer);
std_returntype timer3_deinit(const timer3_t* _timer);
std_returntype timer3_write_value(const timer3_t* _timer,uint16 _value);
std_returntype timer3_read_value(const timer3_t* _timer,uint16 *_value);
#endif	/* HAL_TIMER3_H */

