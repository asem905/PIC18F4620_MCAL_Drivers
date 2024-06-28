/* 
 * File:   hal_ccp1.c
 * Author: Asem
 *
 * Created on March 15, 2024, 8:57 PM
 */
#include "hal_ccp1.h"

#if ccp1_interrupt_feature_enable ==interrupt_feature_enable
    static interrupthandler ccp1_interrupthandler=NULL;
#endif
#if ccp2_interrupt_feature_enable ==interrupt_feature_enable
    static interrupthandler ccp2_interrupthandler=NULL;
#endif

static void ccp_interrupt_config(const ccp_t* ccp_obj);    
static void ccp_pwm_mode_config(const ccp_t* ccp_obj);
static void ccp_mode_timer_select(const ccp_t* ccp_obj);
static std_returntype ccp_capture_mode_config(const ccp_t* ccp_obj);
static std_returntype ccp_compare_mode_config(const ccp_t* ccp_obj);
std_returntype ccp_init(const ccp_t* ccp_obj){
    std_returntype ret=E_OK;
    if((NULL==ccp_obj)){
        ret=E_NOT_OK;
    }else{
        if(ccp1_inst==ccp_obj->ccp_inst){
            ccp_set_mode(ccp_module_disable);//of ccp1
        }else if(ccp2_inst==ccp_obj->ccp_inst){
            ccp2_set_mode(ccp_module_disable);
        }else{
                
        }
               
        if(ccp_capture_mode_selected==ccp_obj->ccp_mode){
            ret=ccp_capture_mode_config(ccp_obj);
            
        }else if(ccp_compare_mode_selected==ccp_obj->ccp_mode){
            ret=ccp_compare_mode_config(ccp_obj);
        }
        #if (ccp1_cfg_selected_mode==ccp_cfg_pwm_mode_selected) || (ccp2_cfg_selected_mode==ccp_cfg_pwm_mode_selected)
        else if(ccp_pwm_mode_selected==ccp_obj->ccp_mode){
            ccp_pwm_mode_config(ccp_obj);

        }else{
            
        }
        #endif 
        /*pin configuration*/
        ret=gpio_pin_initilaize(&(ccp_obj->pin));//we determine pin to be output or input according to mode
        ccp_interrupt_config(ccp_obj);
        ret=E_OK;
        //ccp_set_mode(ccp1_module_enable);
    }
    return ret;
}
std_returntype ccp_deinit(const ccp_t* ccp_obj){
     std_returntype ret=E_OK;
    if((NULL==ccp_obj)){
        ret=E_NOT_OK;
    }else{
        if(ccp1_inst==ccp_obj->ccp_inst){
            ccp_set_mode(ccp_module_disable);//of ccp1
            #if ccp1_interrupt_feature_enable ==interrupt_feature_enable
            ccp1_interruptdisable();
            #endif
        }else if(ccp2_inst==ccp_obj->ccp_inst){
            ccp2_set_mode(ccp_module_disable);
            #if ccp2_interrupt_feature_enable ==interrupt_feature_enable
            ccp2_interruptdisable();
            #endif
        }else{
                
        }
        
        ret=E_OK;
    }
    return ret;
}
#if (ccp1_cfg_selected_mode==ccp_cfg_pwm_mode_selected)
    std_returntype ccp_pwm1_set_duty(const uint8 _duty,const ccp_t* ccp_obj){
        std_returntype ret=E_OK;
        uint16 l_duty_temp=0;
        if((NULL==ccp_obj)){
            ret=E_NOT_OK;
        }else{
            l_duty_temp=(uint16) ((PR2+1)*(_duty/100.0)*4);
            if(ccp1_inst==ccp_obj->ccp_inst){
                CCP1CONbits.DC1B=(uint8)(l_duty_temp & 0x03);//to get bits number 4,5
                CCPR1L=(uint8)(l_duty_temp >> 2);
            }else if(ccp2_inst==ccp_obj->ccp_inst){
                CCP2CONbits.DC2B=(uint8)(l_duty_temp & 0x03);//to get bits number 4,5
                CCPR2L=(uint8)(l_duty_temp >> 2);
            }else{
                
            }

            ret=E_OK;
        }
        

        return ret;
    }
    std_returntype ccp_pwm1_start(const ccp_t* ccp_obj){
        std_returntype ret=E_OK;
        if((NULL==ccp_obj)){
            ret=E_NOT_OK;
        }else{
            if(ccp1_inst==ccp_obj->ccp_inst){
                CCP1CONbits.CCP1M=ccp_pwm_mode;
            }else if(ccp2_inst==ccp_obj->ccp_inst){
                CCP2CONbits.CCP2M=ccp_pwm_mode;
            }else{
                
            }

            ret=E_OK;
        }
        
        return ret;
    }
    std_returntype ccp_pwm1_stop(const ccp_t* ccp_obj){
        std_returntype ret=E_OK;
        if((NULL==ccp_obj)){
            ret=E_NOT_OK;
        }else{
            if(ccp1_inst==ccp_obj->ccp_inst){
                CCP1CONbits.CCP1M=ccp_module_disable;
            }else if(ccp2_inst==ccp_obj->ccp_inst){
                CCP2CONbits.CCP2M=ccp_module_disable;
            }else{
                
            }

            ret=E_OK;
        }
        return ret;
    }
#endif
    
#if (ccp1_cfg_selected_mode==ccp1_cfg_capture_mode_selected)
    std_returntype ccp_iscapturedataready(uint8* _capture_status){
        std_returntype ret=E_OK;
        if((NULL==_capture_status)){
            ret=E_NOT_OK;
        }else{
            if(ccp_capture_ready==PIR1bits.CCP1IF){
                *_capture_status=ccp_capture_ready;
                ccp1_interruptflagclear();
            }else{
                *_capture_status=ccp_capture_not_ready;
            }
            ret=E_OK;
        }
        return ret;
    }
    std_returntype ccp_capture_mode_read_value(uint16* _capture_value){
        std_returntype ret=E_OK;
        ccp1_reg_t capture_temp_value={.ccpr1_low=0,.ccpr1_high=0};//because it is structure
        if((NULL==_capture_value)){
            ret=E_NOT_OK;
        }else{
            capture_temp_value.ccpr1_low=CCPR1L;
            capture_temp_value.ccpr1_high=CCPR1H;
            *_capture_value=(capture_temp_value.ccpr1_high>>8) +capture_temp_value.ccpr1_low;//we did that as in union it is what stored in any thing stored in all

            ret=E_OK;
        }
    return ret;
    }
#endif
    
#if (ccp1_cfg_selected_mode==ccp1_cfg_compare_mode_selected)
    std_returntype ccp_iscomparecomplete(uint8* _compare_status){
        std_returntype ret=E_OK;
        if((NULL==_compare_status)){
            ret=E_NOT_OK;
        }else{
            if(ccp_compare_ready==PIR1bits.CCP1IF){
                *_compare_status=ccp_compare_ready;
                ccp1_interruptflagclear();
            }else{
                *_compare_status=ccp_compare_notready;
            }
            ret=E_OK;
        }
        return ret;
    }
    std_returntype ccp_compare_mode_set_value(const ccp_t* ccp_obj,uint16 _compare_value){
        std_returntype ret=E_OK;
        ccp_reg_t compare_temp_value={.ccpr_low=0,.ccpr_high=0};
        if((NULL==ccp_obj)){
            ret=E_NOT_OK;
        }else{
            compare_temp_value.ccpr_16bit=_compare_value;//we did that as in union it is what stored in any thing stored in all
            if(ccp1_inst==ccp_obj->ccp_inst){
                CCPR1L=compare_temp_value.ccpr_low;//we did that as in union it is what stored in any thing stored in all
                CCPR1H=compare_temp_value.ccpr_high;//we did that as in union it is what stored in any thing stored in all
            }else if(ccp2_inst==ccp_obj->ccp_inst){
                CCPR2L=compare_temp_value.ccpr_low;//we did that as in union it is what stored in any thing stored in all
                CCPR2H=compare_temp_value.ccpr_high;//we did that as in union it is what stored in any thing stored in all
            }else{

            }
            ret=E_OK;
        }
        return ret;
    }
#endif
  
void ccp1_ISR(void){//this interrupt happens when overflow happens
    ccp1_interruptflagclear();
    if(ccp1_interrupthandler){
        ccp1_interrupthandler();
    }else{
        
    }
}
void ccp2_ISR(void){//this interrupt happens when overflow happens
    ccp2_interruptflagclear();
    if(ccp2_interrupthandler){
        ccp2_interrupthandler();
    }else{
        
    }
}
static void ccp_interrupt_config(const ccp_t* ccp_obj){
    #if ccp2_interrupt_feature_enable ==interrupt_feature_enable
    ccp2_interruptenable();
    ccp2_interruptflagclear();
    ccp2_interrupthandler=ccp_obj->ccp2_InterruptHandler;
    #if interrupt_priority_levels_enable == interrupt_feature_enable
    interrupt_prioritylevelsenable();
    if(interrupt_high_priority==ccp_obj->ccp2_priority){
        CCP2_highpriorityset();
        interrupt_globalinterrupthighenable();//in interrupt module if we didnot enable our priority interrupt we need to enable gieh but if we enabled priority we need to enable gieh too 
    }else if(interrupt_low_priority==ccp_obj->ccp2_priority){
        CCP2_lowpriorityset();
        interrupt_globalinterruptlowenable();
    }
    #else
    interrupt_globalinterruptenable();//these happens if priority is not enabled 
    interrupt_peripheralinterruptenable();
    #endif
    #endif

    
    #if ccp1_interrupt_feature_enable ==interrupt_feature_enable
    ccp1_interruptenable();
    ccp1_interruptflagclear();
    ccp1_interrupthandler=ccp_obj->ccp1_InterruptHandler;
    #if interrupt_priority_levels_enable == interrupt_feature_enable
    interrupt_prioritylevelsenable();
    if(interrupt_high_priority==ccp_obj->ccp1_priority){
        CCP1_highpriorityset();
        interrupt_globalinterrupthighenable();//in interrupt module if we didnot enable our priority interrupt we need to enable gieh but if we enabled priority we need to enable gieh too 
    }else if(interrupt_low_priority==ccp_obj->ccp1_priority){
        CCP1_lowpriorityset();
        interrupt_globalinterruptlowenable();
    }else{
        
    }    
    #else
    interrupt_globalinterruptenable();//these happens if priority is not enabled 
    interrupt_peripheralinterruptenable();
    #endif
    #endif
}
 #if (ccp1_cfg_selected_mode==ccp_cfg_pwm_mode_selected) || (ccp2_cfg_selected_mode==ccp_cfg_pwm_mode_selected)
static void ccp_pwm_mode_config(const ccp_t* ccp_obj){
    
    if(ccp1_inst==ccp_obj->ccp_inst){
        if(ccp_pwm_mode==ccp_obj->ccp_mode_variant){
            ccp_set_mode(ccp_pwm_mode);
        }else{

        }
    }else if(ccp2_inst==ccp_obj->ccp_inst){
        if(ccp_pwm_mode==ccp_obj->ccp_mode_variant){
            ccp2_set_mode(ccp_pwm_mode);
        }else{

        }
    }else{
                
    }   
    PR2=(uint8)((_XTAL_FREQ /(ccp_obj->pwm_frequency *4.0 *ccp_obj->timer2_postscaler_value *ccp_obj->timer2_prescaler_value))-1);
}
#endif 
static void ccp_mode_timer_select(const ccp_t* ccp_obj){
    if(ccp1_ccp2_timer3==ccp_obj->ccp_capture_timer){
        T3CONbits.T3CCP1=0;
        T3CONbits.T3CCP2=1;//these bits written in timer3
    }else if(ccp1_timer1__ccp2_timer3==ccp_obj->ccp_capture_timer){
        T3CONbits.T3CCP1=1;
        T3CONbits.T3CCP2=0;
    }else if(ccp1_ccp2_timer1==ccp_obj->ccp_capture_timer){
        T3CONbits.T3CCP1=0;
        T3CONbits.T3CCP2=0;
    }else{
        
    }
}

static std_returntype ccp_capture_mode_config(const ccp_t* ccp_obj){
    std_returntype ret=E_OK;
    if(ccp1_inst==ccp_obj->ccp_inst){
        switch(ccp_obj->ccp_mode_variant){
            case ccp_capture_mode_1_falling_edge:ccp_set_mode(ccp_capture_mode_1_falling_edge);break;
            case ccp_capture_mode_1_rising_edge:ccp_set_mode(ccp_capture_mode_1_rising_edge);break;   
            case ccp_capture_mode_4_rising_edge:ccp_set_mode(ccp_capture_mode_4_rising_edge);break;
            case ccp_capture_mode_16_rising_edge:ccp_set_mode(ccp_capture_mode_16_rising_edge);break;
            default: ret=E_NOT_OK;
        }
        ccp_mode_timer_select(ccp_obj);
    }else if(ccp2_inst==ccp_obj->ccp_inst){
         switch(ccp_obj->ccp_mode_variant){
            case ccp_capture_mode_1_falling_edge:ccp2_set_mode(ccp_capture_mode_1_falling_edge);break;
            case ccp_capture_mode_1_rising_edge:ccp2_set_mode(ccp_capture_mode_1_rising_edge);break;   
            case ccp_capture_mode_4_rising_edge:ccp2_set_mode(ccp_capture_mode_4_rising_edge);break;
            case ccp_capture_mode_16_rising_edge:ccp2_set_mode(ccp_capture_mode_16_rising_edge);break;
            default: ret=E_NOT_OK;
        }
        ccp_mode_timer_select(ccp_obj);
    }else{
                
    }
    return ret;
}

static std_returntype ccp_compare_mode_config(const ccp_t* ccp_obj){
    std_returntype ret=E_OK;
    if(ccp1_inst==ccp_obj->ccp_inst){
        switch(ccp_obj->ccp_mode_variant){
            case ccp_compare_mode_set_pin_low:ccp_set_mode(ccp_compare_mode_set_pin_low);break;
            case ccp_compare_mode_set_pin_high:ccp_set_mode(ccp_compare_mode_set_pin_high);break;   
            case ccp_compare_mode_toggle_on_match:ccp_set_mode(ccp_compare_mode_toggle_on_match);break;
            case ccp_compare_mode_gen_sw_interrupt:ccp_set_mode(ccp_compare_mode_gen_sw_interrupt);break;
            case ccp_compare_mode_gen_event:ccp_set_mode(ccp_compare_mode_gen_event);break;
            default: ret=E_NOT_OK;
        }
    }else if(ccp2_inst==ccp_obj->ccp_inst){
         switch(ccp_obj->ccp_mode_variant){
            case ccp_compare_mode_set_pin_low:ccp2_set_mode(ccp_compare_mode_set_pin_low);break;
            case ccp_compare_mode_set_pin_high:ccp2_set_mode(ccp_compare_mode_set_pin_high);break;   
            case ccp_compare_mode_toggle_on_match:ccp2_set_mode(ccp_compare_mode_toggle_on_match);break;
            case ccp_compare_mode_gen_sw_interrupt:ccp2_set_mode(ccp_compare_mode_gen_sw_interrupt);break;
            case ccp_compare_mode_gen_event:ccp2_set_mode(ccp_compare_mode_gen_event);break;
            default: ret=E_NOT_OK;
        }
    }else{
                
    }
    return ret;
}
