/* 
 * File:   hal_timer2.c
 * Author: Asem
 *
 * Created on March 11, 2024, 9:28 PM
 */
#include "hal_timer2.h"
#if timer1_interrupt_feature_enable ==interrupt_feature_enable
    static interrupthandler tmr2_interrupthandler=NULL;
#endif
static uint8 timer2_preload=0;    
void tmr2_ISR(void);
std_returntype timer2_init(const timer2_t* _timer){
    std_returntype ret=E_OK;
    if((NULL==_timer)){
        ret=E_NOT_OK;
    }else{
        timer2_module_disable();
        timer2_prescaler_select(_timer->timer2_prescaler_value);
        timer2_postscaler_select(_timer->timer2_postscaler_value);
        TMR2=(_timer->timer2_preload_value);
        timer2_preload=_timer->timer2_preload_value;//as after each over flow we return from this preloaded value
        #if timer2_interrupt_feature_enable ==interrupt_feature_enable
        timer2_interruptenable();
        timer2_interruptflagclear();
        tmr2_interrupthandler=_timer->timer2_InterruptHandler;
        #if interrupt_priority_levels_enable == interrupt_feature_enable
        interrupt_prioritylevelsenable();
        if(interrupt_high_priority==_timer->priority){
            timer2_highpriorityset();
            interrupt_globalinterrupthighenable();//in interrupt module if we didnot enable our priority interrupt we need to enable gieh but if we enabled priority we need to enable gieh too 
        }else if(interrupt_low_priority==_timer->priority){
            timer2_lowpriorityset();
            interrupt_globalinterruptlowenable();
        }
        #else
        interrupt_globalinterruptenable();//these happens if priority is not enabled 
        interrupt_peripheralinterruptenable();
        #endif
        #endif
        timer2_module_enable();
    }
    return ret;
}
std_returntype timer2_deinit(const timer2_t* _timer){
    std_returntype ret=E_OK;
    if((NULL==_timer)){
        ret=E_NOT_OK;
    }else{
        timer2_module_disable();
        #if timer2_interrupt_feature_enable ==interrupt_feature_enable
        timer2_interruptdisable();
        #endif
    }
    return ret;
}
std_returntype timer2_write_value(const timer2_t* _timer,uint8 _value){
    std_returntype ret=E_OK;
    if((NULL==_timer)){
        ret=E_NOT_OK;
    }else{
        TMR2=((_value));
        ret=E_OK;
    }
    return ret;
}
std_returntype timer2_read_value(const timer2_t* _timer,uint8 *_value){
    std_returntype ret=E_OK;
    if((NULL==_timer)||(NULL==_value)){
        ret=E_NOT_OK;
    }else{
        *_value=TMR2;
    }
    return ret;
}
void tmr2_ISR(void){//this interrupt happens when overflow happens
    timer2_interruptflagclear();
    TMR2=((timer2_preload));
    if(tmr2_interrupthandler){
        tmr2_interrupthandler();
    }
}