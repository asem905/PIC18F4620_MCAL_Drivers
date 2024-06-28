/* 
 * File:   hal_ccp1.h
 * Author: Asem
 *
 * Created on March 15, 2024, 8:57 PM
 */

#ifndef HAL_CCP1_H
#define	HAL_CCP1_H
/*section : includes*/
#include "ccp1_cfg.h"
#include "../proc/pic18f4620.h"
#include "../mcal_std_types.h"
#include "../interrupt/mcal_internal_interrupt.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
/*section : macro declarations*/
#define ccp_postscaler_div_by_1 1
#define ccp_postscaler_div_by_2 2
#define ccp_postscaler_div_by_3 3
#define ccp_postscaler_div_by_4 4
#define ccp_postscaler_div_by_5 5
#define ccp_postscaler_div_by_6 6
#define ccp_postscaler_div_by_7 7
#define ccp_postscaler_div_by_8 8
#define ccp_postscaler_div_by_9 9
#define ccp_postscaler_div_by_10 10
#define ccp_postscaler_div_by_11 11
#define ccp_postscaler_div_by_12 12
#define ccp_postscaler_div_by_13 13
#define ccp_postscaler_div_by_14 14
#define ccp_postscaler_div_by_15 15
#define ccp_postscaler_div_by_16 16

#define ccp_prescaler_div_by_1 1
#define ccp_prescaler_div_by_4 4
#define ccp_prescaler_div_by_16 16



#define ccp_module_disable ((uint8)0x00)//cast to uint8 as value will take it is ccp1_mode_variant of type uint8


#define ccp_capture_mode_1_falling_edge ((uint8)0x04)//to determine when to capture 
#define ccp_capture_mode_1_rising_edge ((uint8)0x05)
#define ccp_capture_mode_4_rising_edge ((uint8)0x06)
#define ccp_capture_mode_16_rising_edge ((uint8)0x07)
/*all coming is compare mode on compare match*/
#define ccp_compare_mode_set_pin_low ((uint8)0x08)
#define ccp_compare_mode_set_pin_high ((uint8)0x09)
#define ccp_compare_mode_toggle_on_match ((uint8)0x02)
#define ccp_compare_mode_gen_sw_interrupt ((uint8)0x0A)
#define ccp_compare_mode_gen_event ((uint8)0x0B)
#define ccp_pwm_mode ((uint8)0x0C)

/*to define state of capture mode*/
#define ccp_capture_ready 0x01
#define ccp_capture_not_ready 0x00//used i function down of is capture ready as aparameter
/*to define state of compare mode*/
#define ccp_compare_ready 0x01
#define ccp_compare_notready 0x00


/*section : macro functions*/
#define ccp_set_mode(_config_) (CCP1CONbits.CCP1M=_config_)//write bits above in it to define what mode to work at
#define ccp2_set_mode(_config_) (CCP2CONbits.CCP2M=_config_)
/*section : data type declarartion*/
typedef enum{
    ccp1_inst=0,
    ccp2_inst
}ccp_inst_t;
typedef enum{
    ccp_capture_mode_selected=0,
    ccp_compare_mode_selected,
    ccp_pwm_mode_selected
}ccp1_mode_t;


typedef enum{
    ccp1_ccp2_timer3=0,
    ccp1_timer1__ccp2_timer3,
    ccp1_ccp2_timer1
}ccp_captur_timer_t;//to define working with timer1or3 in capture mode

/*to determine how to read your values as 8bits or 16 bits*/
typedef union{
    struct{
        uint8 ccpr_low;
        uint8 ccpr_high;
    };
    struct{
        uint16 ccpr_16bit;
    };
}ccp_reg_t;
typedef struct{
    ccp_inst_t ccp_inst;//to define which ccp to work on
    ccp1_mode_t ccp_mode;
    uint8 ccp_mode_variant;//here to determine what is submode of mode you want as if it is  ccp1_compare_mode_set_pin_low or....
    pin_config pin;//depending on mode we determine state of pin
    ccp_captur_timer_t ccp_capture_timer;
#if (ccp1_cfg_selected_mode==ccp_cfg_pwm_mode_selected) || (ccp2_cfg_selected_mode==ccp_cfg_pwm_mode_selected)
    uint32 pwm_frequency;//it is so big so uint32
    uint8 timer2_postscaler_value :4;
    uint8 timer2_prescaler_value :2;//we use them to get PR2
#endif  
#if ccp1_interrupt_feature_enable ==interrupt_feature_enable
    void (*ccp1_InterruptHandler)(void);//to call back to upper layer
    interrupt_priority_cfg ccp1_priority;
#endif
#if ccp1_interrupt_feature_enable ==interrupt_feature_enable
    void (*ccp2_InterruptHandler)(void);//to call back to upper layer
    interrupt_priority_cfg ccp2_priority;
#endif    
}ccp_t;
/*section :functions declaration*/
std_returntype ccp_init(const ccp_t* ccp_obj);
std_returntype ccp_deinit(const ccp_t* ccp_obj);
#if (ccp1_cfg_selected_mode==ccp_cfg_pwm_mode_selected || ccp2_cfg_selected_mode==ccp_cfg_pwm_mode_selected)
    std_returntype ccp_pwm1_set_duty(const uint8 _duty,const ccp_t* ccp_obj);//we can change duty cycle(intensity of any thing) as we want during run time
    std_returntype ccp_pwm1_start(const ccp_t* ccp_obj);//to make pulses out of my pin
    std_returntype ccp_pwm1_stop(const ccp_t* ccp_obj);//to stop pulses out of my pin
#endif
    
#if (ccp1_cfg_selected_mode==ccp1_cfg_capture_mode_selected || ccp2_cfg_selected_mode==ccp1_cfg_capture_mode_selected)
    std_returntype ccp_iscapturedataready(uint8* _capture_status);
    std_returntype ccp_capture_mode_read_value(uint16* _capture_value);
#endif
    
#if (ccp1_cfg_selected_mode==ccp1_cfg_compare_mode_selected || ccp2_cfg_selected_mode==ccp1_cfg_compare_mode_selected)
    std_returntype ccp_iscomparecomplete(uint8* _compare_status);
    std_returntype ccp_compare_mode_set_value(const ccp_t* ccp_obj,uint16 _compare_value);//to write value you want to compare
#endif
    
   

#endif	/* HAL_CCP1_H */

