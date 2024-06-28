/* 
 * File:   hal_timer1.c
 * Author: Asem
 *
 * Created on March 9, 2024, 10:49 PM
 */
#include "hal_timer1.h"
#if timer1_interrupt_feature_enable ==interrupt_feature_enable
    static interrupthandler tmr1_interrupthandler=NULL;
#endif
static inline void timer1_mode_select(const timer1_t* _timer);   
static uint8 timer1_preload=0;
std_returntype timer1_init(const timer1_t* _timer){
    std_returntype ret=E_OK;
    if((NULL==_timer)){
        ret=E_NOT_OK;
    }else{
        timer1_module_disable();
        timer1_prescaler_select(_timer->timer1_prescaler_value);
        timer1_mode_select(_timer);
        TMR1H=(uint8)((_timer->timer1_preload_value)>>8);
        TMR1L=(uint8)(_timer->timer1_preload_value);
        timer1_preload=_timer->timer1_preload_value;//as after each over flow we return from this preloaded value
        #if timer1_interrupt_feature_enable ==interrupt_feature_enable
        timer1_interruptenable();
        timer1_interruptflagclear();
        tmr1_interrupthandler=_timer->timer1_InterruptHandler;
        #if interrupt_priority_levels_enable == interrupt_feature_enable
        interrupt_prioritylevelsenable();
        if(interrupt_high_priority==_timer->priority){
            timer1_highpriorityset();
            interrupt_globalinterrupthighenable();//in interrupt module if we didnot enable our priority interrupt we need to enable gieh but if we enabled priority we need to enable gieh too 
        }else if(interrupt_low_priority==_timer->priority){
            timer1_lowpriorityset();
            interrupt_globalinterruptlowenable();
        }
        #else
        interrupt_globalinterruptenable();//these happens if priority is not enabled 
        interrupt_peripheralinterruptenable();
        #endif
        #endif
        timer1_module_enable();
    }
    return ret;
}
std_returntype timer1_deinit(const timer1_t* _timer){
    std_returntype ret=E_OK;
    if((NULL==_timer)){
        ret=E_NOT_OK;
    }else{
        timer1_module_disable();
        #if timer1_interrupt_feature_enable ==interrupt_feature_enable
        timer1_interruptdisable();
        #endif
       
        ret=E_OK;
    }
    return ret;
}
std_returntype timer1_write_value(const timer1_t* _timer,uint16 _value){
    std_returntype ret=E_OK;
    if((NULL==_timer)){
        ret=E_NOT_OK;
    }else{
        TMR1H=(uint8)((_value)>>8);
        TMR1L=(uint8)(_value);
       
        ret=E_OK;
    }
    return ret;
}
std_returntype timer1_read_value(const timer1_t* _timer,uint16 *_value){
    std_returntype ret=E_OK;
    uint8 l_tmr1l=0,l_tmr1h=0;
    if((NULL==_timer)||(NULL==_value)){
        ret=E_NOT_OK;
    }else{
        l_tmr1l=TMR1L;
        l_tmr1h=TMR1H;
        *_value=(uint16)(l_tmr1l+(l_tmr1h<<8));
        ret=E_OK;
    }
    return ret;
}

void tmr1_ISR(void){//this interrupt happens when overflow happens
    timer1_interruptflagclear();
    TMR1H=(uint8)((timer1_preload)>>8);
    TMR1L=(uint8)(timer1_preload);
    if(tmr1_interrupthandler){
        tmr1_interrupthandler();
    }
}
static inline void timer1_mode_select(const timer1_t* _timer){
    if(timer1_timer_mode==_timer->timer1_mode){
        timer1_timer_mode_enable();
    }else if(timer1_counter_mode==_timer->timer1_mode){
        timer1_counter_mode_enable();
        if(timer1_async_counter_mode==_timer->timer1_counter_mode_select){
            timer1_async_counter_mode_enable();
        }else if(timer1_sync_counter_mode==_timer->timer1_counter_mode_select){
            timer1_sync_counter_mode_enable();
        }else{
            
        }
    }else{
        /*nothing*/
    }
}