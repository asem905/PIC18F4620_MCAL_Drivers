/* 
 * File:   hal_timer2.h
 * Author: Asem
 *
 * Created on March 11, 2024, 9:28 PM
 */

#ifndef HAL_TIMER2_H
#define	HAL_TIMER2_H
/*section : includes*/
#include "../mcal_std_types.h"
#include "../proc/pic18f4620.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "../../MCAL_Layer/interrupt/mcal_internal_interrupt.h"
/*section : macro declarations*/
#define timer2_postscaler_div_by_1 0
#define timer2_postscaler_div_by_2 1
#define timer2_postscaler_div_by_3 2
#define timer2_postscaler_div_by_4 3
#define timer2_postscaler_div_by_5 4
#define timer2_postscaler_div_by_6 5
#define timer2_postscaler_div_by_7 6
#define timer2_postscaler_div_by_8 7
#define timer2_postscaler_div_by_9 8
#define timer2_postscaler_div_by_10 9
#define timer2_postscaler_div_by_11 10
#define timer2_postscaler_div_by_12 11
#define timer2_postscaler_div_by_13 12
#define timer2_postscaler_div_by_14 13
#define timer2_postscaler_div_by_15 14
#define timer2_postscaler_div_by_16 15

#define timer2_prescaler_div_by_1 0
#define timer2_prescaler_div_by_4 1
#define timer2_prescaler_div_by_16 2
/*section : macro functions*/
#define timer2_module_enable() (T2CONbits.TMR2ON=1)
#define timer2_module_disable() (T2CONbits.TMR2ON=0)

#define timer2_prescaler_select(_prescaler_) (T2CONbits.T2CKPS = _prescaler_)
#define timer2_postscaler_select(_postscaler_) (T2CONbits.TOUTPS = _postscaler_)
/*section : data type declarartion*/
typedef struct{
    #if timer2_interrupt_feature_enable ==interrupt_feature_enable
    void (*timer2_InterruptHandler)(void);//to call back to upper layer
    interrupt_priority_cfg priority;
    #endif
    uint8 timer2_preload_value;//not uint16 as before  as it is of 8bit regiter
    uint8 timer2_postscaler_value :4;
    uint8 timer2_prescaler_value :2;
    uint8 timer2_reserved :2;
}timer2_t;
/*section :functions declaration*/
std_returntype timer2_init(const timer2_t* _timer);
std_returntype timer2_deinit(const timer2_t* _timer);
std_returntype timer2_write_value(const timer2_t* _timer,uint8 _value);
std_returntype timer2_read_value(const timer2_t* _timer,uint8 *_value);

#endif	/* HAL_TIMER2_H */

