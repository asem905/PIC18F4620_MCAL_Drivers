/* 
 * File:   hal_timer3.c
 * Author: Asem
 *
 * Created on March 11, 2024, 9:28 PM
 */
#include "hal_timer3.h"
#if timer3_interrupt_feature_enable ==interrupt_feature_enable
    static interrupthandler tmr3_interrupthandler=NULL;
#endif
static inline void timer3_mode_select(const timer3_t* _timer);   
static uint8 timer3_preload=0;

std_returntype timer3_init(const timer3_t* _timer){
    std_returntype ret=E_OK;
    if((NULL==_timer)){
        ret=E_NOT_OK;
    }else{
        timer3_module_disable();
        timer3_prescaler_select(_timer->timer3_prescaler_value);
        timer3_mode_select(_timer);
        TMR3H=(uint8)((_timer->timer3_preload_value)>>8);
        TMR3L=(uint8)(_timer->timer3_preload_value);
        timer3_preload=_timer->timer3_preload_value;//as after each over flow we return from this preloaded value
        #if timer3_interrupt_feature_enable ==interrupt_feature_enable
        timer3_interruptenable();
        timer3_interruptflagclear();
        tmr3_interrupthandler=_timer->timer3_InterruptHandler;
        #if interrupt_priority_levels_enable == interrupt_feature_enable
        interrupt_prioritylevelsenable();
        if(interrupt_high_priority==_timer->priority){
            timer3_highpriorityset();
            interrupt_globalinterrupthighenable();//in interrupt module if we didnot enable our priority interrupt we need to enable gieh but if we enabled priority we need to enable gieh too 
        }else if(interrupt_low_priority==_timer->priority){
            timer3_lowpriorityset();
            interrupt_globalinterruptlowenable();
        }
        #else
        interrupt_globalinterruptenable();//these happens if priority is not enabled 
        interrupt_peripheralinterruptenable();
        #endif
        #endif
        timer3_module_enable();
    }
    return ret;
}
std_returntype timer3_deinit(const timer3_t* _timer){
    std_returntype ret=E_OK;
    if((NULL==_timer)){
        ret=E_NOT_OK;
    }else{
        timer3_module_disable();
        #if timer3_interrupt_feature_enable ==interrupt_feature_enable
        timer3_interruptdisable();
        #endif
       
        ret=E_OK;
    }
    return ret;
}
std_returntype timer3_write_value(const timer3_t* _timer,uint16 _value){
    std_returntype ret=E_OK;
    if((NULL==_timer)){
        ret=E_NOT_OK;
    }else{
        TMR3H=(uint8)((_value)>>8);
        TMR3L=(uint8)(_value);
       
        ret=E_OK;
    }
    return ret;
}
std_returntype timer3_read_value(const timer3_t* _timer,uint16 *_value){
    std_returntype ret=E_OK;
    uint8 l_tmr3l=0,l_tmr3h=0;
    if((NULL==_timer)||(NULL==_value)){
        ret=E_NOT_OK;
    }else{
        l_tmr3l=TMR3L;
        l_tmr3h=TMR3H;
        *_value=(uint16)(l_tmr3l+(l_tmr3h<<8));
        ret=E_OK;
    }
    return ret;
}

void timer3_mode_select(const timer3_t* _timer){
    if(timer3_timer_mode==_timer->timer3_mode){
        timer3_timer_mode_enable();
    }else if(timer3_counter_mode==_timer->timer3_mode){
        timer3_counter_mode_enable();
        if(timer3_async_counter_mode==_timer->timer3_counter_mode_select){
            timer3_async_counter_mode_enable();
        }else if(timer3_sync_counter_mode==_timer->timer3_counter_mode_select){
            timer3_sync_counter_mode_enable();
        }else{
            
        }
    }else{
        /*nothing*/
    }
}
void tmr3_ISR(void){//this interrupt happens when overflow happens
    timer3_interruptflagclear();
    TMR3H=(uint8)((timer3_preload)>>8);
    TMR3L=(uint8)(timer3_preload);
    if(tmr3_interrupthandler){
        tmr3_interrupthandler();
    }
}