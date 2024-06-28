/* 
 * File:   hal_timr0.h
 * Author: Asem
 *
 * Created on March 5, 2024, 8:15 PM
 */

#ifndef HAL_TIMR0_H
#define	HAL_TIMR0_H

/*section : includes*/
#include "../mcal_std_types.h"
#include "../proc/pic18f4620.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "../../MCAL_Layer/interrupt/mcal_internal_interrupt.h"
/*section : macro declarations*/

#define timer0_prescaler_enable_cfg 1
#define timer0_prescaler_disable_cfg 0

#define timer0_rising_edge_cfg 1
#define timer0_falling_edge_cfg 0

#define timer0_timer_mode 1
#define timer0_counter_mode 0

#define timer0_8bit_register_mode 1
#define timer0_16bit_register_mode 0
/*section : macro functions*/

#define timer0_prescaler_enable() (T0CONbits.PSA=0)
#define timer0_prescaler_disable() (T0CONbits.PSA=1)

#define timer0_rising_edge_enable() (T0CONbits.T0SE=0)
#define timer0_falling_edge_enable() (T0CONbits.T0SE=1)

#define timer0_timer_mode_enable() (T0CONbits.T0CS=0)
#define timer0_counter_mode_enable() (T0CONbits.T0CS=1)//by selecting what clock source

#define timer0_8bit_register_mode_enable() (T0CONbits.T08BIT=1)
#define timer0_16bit_register_mode_enable() (T0CONbits.T08BIT=0)

#define timer0_module_enable() (T0CONbits.TMR0ON=1)
#define timer0_module_disable() (T0CONbits.TMR0ON=0)
/*section : data type declarartion*/
typedef enum{
    timer0_prescaler_div_by_2=0,
    timer0_prescaler_div_by_4,   
    timer0_prescaler_div_by_8,
    timer0_prescaler_div_by_16, 
    timer0_prescaler_div_by_32,
    timer0_prescaler_div_by_64, 
    timer0_prescaler_div_by_128,
    timer0_prescaler_div_by_256 
}timer0_prescaler_select_t;

typedef struct{
    #if timer0_interrupt_feature_enable ==interrupt_feature_enable
    void (*timer0_InterruptHandler)(void);//to call back to upper layer
    interrupt_priority_cfg priority;
    #endif
    timer0_prescaler_select_t prescaler_value;
    uint16 timer0_preload_value;
    uint8 prescaler_enable :1;
    uint8 timer0_counter_edge :1;
    uint8 timer0_mode :1; 
    uint8 timer0_register_size :1;
    uint8 timer0_reserved :4;
}timer0_t;
/*section :functions declaration*/
std_returntype timer0_init(const timer0_t* _timer);
std_returntype timer0_deinit(const timer0_t* _timer);
std_returntype timer0_write_value(const timer0_t* _timer,uint16 _value);
std_returntype timer0_read_value(const timer0_t* _timer,uint16 *_value);
#endif	/* HAL_TIMR0_H */

