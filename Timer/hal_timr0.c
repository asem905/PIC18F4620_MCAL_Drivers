/* 
 * File:   hal_timr0.c
 * Author: Asem
 *
 * Created on March 5, 2024, 8:15 PM
 */

#include "hal_timr0.h"
 #if timer0_interrupt_feature_enable ==interrupt_feature_enable
    static interrupthandler tmr0_interrupthandler=NULL;
#endif
static uint8 timer0_preload=0;
static inline void timer0_prescaler_config(const timer0_t* _timer);   
static inline void timer0_register_size_config(const timer0_t* _timer);
static inline void timer0_mode_select(const timer0_t* _timer);
std_returntype timer0_init(const timer0_t* _timer){
    std_returntype ret=E_OK;
    if((NULL==_timer)){
        ret=E_NOT_OK;
    }else{
        timer0_module_disable();
        timer0_prescaler_config(_timer);
        timer0_mode_select(_timer);
        timer0_register_size_config(_timer);
        TMR0H=(uint8)((_timer->timer0_preload_value)>>8);
        TMR0L=(uint8)(_timer->timer0_preload_value);
        timer0_preload=_timer->timer0_preload_value;
#if timer0_interrupt_feature_enable ==interrupt_feature_enable
        timer0_interruptenable();
        timer0_interruptflagclear();
        tmr0_interrupthandler=_timer->timer0_InterruptHandler;
        #if interrupt_priority_levels_enable == interrupt_feature_enable
        interrupt_prioritylevelsenable();
        if(interrupt_high_priority==_timer->priority){
            timer0_highpriorityset();
            interrupt_globalinterrupthighenable();//in interrupt module if we didnot enable our priority interrupt we need to enable gieh but if we enabled priority we need to enable gieh too 
        }else if(interrupt_low_priority==_timer->priority){
            timer0_lowpriorityset();
            interrupt_globalinterruptlowenable();
        }
        #else
        interrupt_globalinterruptenable();//these happens if priority is not enabled 
        interrupt_peripheralinterruptenable();
        #endif
#endif
        timer0_module_enable();
        ret=E_OK;
    }
    return ret;
    
}
std_returntype timer0_deinit(const timer0_t* _timer){
    std_returntype ret=E_OK;
    if((NULL==_timer)){
        ret=E_NOT_OK;
    }else{
        timer0_module_disable();
        #if timer0_interrupt_feature_enable ==interrupt_feature_enable
        timer0_interruptdisable();
        #endif
        ret=E_OK;
    }
    return ret;
   
}
std_returntype timer0_write_value(const timer0_t* _timer,uint16 _value){
    std_returntype ret=E_OK;
    if((NULL==_timer)){
        ret=E_NOT_OK;
    }else{
        TMR0H=(uint8)((_value)>>8);
        TMR0L=(uint8)(_value);
        ret=E_OK;
    }
    return ret;
}
std_returntype timer0_read_value(const timer0_t* _timer,uint16 *_value){
    std_returntype ret=E_OK;
    uint8 l_tmr0l=0,l_tmr0h=0;
    if((NULL==_timer) && (NULL==_value)){
        ret=E_NOT_OK;
    }else{
        /*we read from low bits to high oppossite to write*/
        l_tmr0l=TMR0L;
        l_tmr0h=TMR0H;
        *_value=(uint16)(l_tmr0l+(l_tmr0h<<8));
        ret=E_OK;
    }
    return ret;
}

void tmr0_ISR(void){//this interrupt happens when overflow happens
    timer0_interruptflagclear();
    TMR0H=(uint8)((timer0_preload)>>8);
    TMR0L=(uint8)(timer0_preload);
    if(tmr0_interrupthandler){
        tmr0_interrupthandler();
    }
}
static inline void timer0_prescaler_config(const timer0_t* _timer){
    if(timer0_prescaler_enable_cfg==_timer->prescaler_enable){
        timer0_prescaler_enable();
        T0CONbits.T0PS=_timer->prescaler_value;
    }else if(timer0_prescaler_disable_cfg==_timer->prescaler_enable){
        timer0_prescaler_disable();
    }else{
        /*nothing*/
    }
}
static inline void timer0_mode_select(const timer0_t* _timer){
    if(timer0_timer_mode==_timer->timer0_mode){
        timer0_timer_mode_enable();
    }else if(timer0_counter_mode==_timer->timer0_mode){
        timer0_counter_mode_enable();
        if(timer0_rising_edge_cfg==_timer->timer0_counter_edge){
            timer0_rising_edge_enable();
        }else if(timer0_falling_edge_cfg==_timer->timer0_counter_edge){
            timer0_falling_edge_enable();
        }else{
            
        }
    }else{
        /*nothing*/
    }
}
static inline void timer0_register_size_config(const timer0_t* _timer){
    if(timer0_8bit_register_mode==_timer->timer0_register_size){
        timer0_8bit_register_mode_enable();
    }else if(timer0_16bit_register_mode==_timer->timer0_register_size){
        timer0_16bit_register_mode_enable();
        
    }else{
        /*nothing*/
    }
}