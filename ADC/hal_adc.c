/* 
 * File:   hal_adc.c
 * Author: Asem
 *
 * Created on February 23, 2024, 10:58 PM
 */
#include "hal_adc.h"

#if ADC_interrupt_feature_enable ==interrupt_feature_enable
    static interrupthandler ADC_interrupthandler=NULL;// will contain address of ADC APP
#endif


static inline void adc_input_channel_port_configure(adc_channel_select_t channel);
static inline void select_result_format(const adc_conf_t *_adc);
static inline void configure_voltage_reference(const adc_conf_t *_adc);
/**
 * 
 * @param _adc
 * @return 
 */
std_returntype ADC_init(const adc_conf_t* _adc){
    std_returntype ret=E_OK;
    if(NULL==_adc){
        ret=E_NOT_OK;
    }else{
        /*disable adc*/
        ADC_CONVERTOR_DISABLE();
        /*configure acquisition time*/
        ADCON2bits.ACQT=_adc->acquisition_time;
        /*configure conversion clock*/
        ADCON2bits.ADCS=_adc->conversion_clock;
        /*select channel then configure it's bin to be input*/
        ADCON0bits.CHS=_adc->adc_channel;
        //adc_input_channel_port_configure(_adc);//as we made function take only channel not whole object:
        adc_input_channel_port_configure(_adc->adc_channel);
        /*CONFIGURE INTERRUPT*/
#if ADC_interrupt_feature_enable ==interrupt_feature_enable
        ADC_interruptenable();
        ADC_interruptflagclear();
        #if interrupt_priority_levels_enable == interrupt_feature_enable
        interrupt_prioritylevelsenable();
        if(interrupt_high_priority==_adc->priority){
            ADC_highpriorityset();
            interrupt_globalinterrupthighenable();
        }else if(interrupt_low_priority==_adc->priority){
            ADC_lowpriorityset();
            interrupt_globalinterruptlowenable();
        }else{
            
        }
        #else
        interrupt_globalinterruptenable();//these happens if priority is not enabled 
        interrupt_peripheralinterruptenable();
        #endif
        ADC_interrupthandler=_adc->ADC_InterruptHandler;
#endif
        /*all previous from if must be done for all modules*/
        /*configure result format right or left*/
        select_result_format(_adc);
        /*cofigure voltage reference*/
        configure_voltage_reference(_adc);
        /*enable adc*/
        ADC_CONVERTOR_ENABLE();
    }
    return ret;
}
/**
 * 
 * @param _adc
 * @return 
 */
std_returntype ADC_deinit(const adc_conf_t* _adc){
    std_returntype ret=E_OK;
    if(NULL==_adc){
        ret=E_NOT_OK;
    }else{
        /*disable adc*/
        ADC_CONVERTOR_DISABLE();
        /*disable interrupt*/
        ADC_interruptdisable();
        
    }
    return ret;
}
/**
 * 
 * @param _adc
 * @param channel
 * @return 
 */
std_returntype ADC_selectchannel(const adc_conf_t* _adc,adc_channel_select_t channel){
    std_returntype ret=E_OK;
    if(NULL==_adc){
        ret=E_NOT_OK;
    }else{
        ADCON0bits.CHS=channel;
        //adc_input_channel_port_configure(_adc);//here we take default channel we can change default so we make follwing func.:
        adc_input_channel_port_configure(channel);
    }
    return ret;
}
/**
 * 
 * @param _adc
 * @return 
 */
std_returntype ADC_startconversion(const adc_conf_t* _adc){
    std_returntype ret=E_OK;
    if(NULL==_adc){
        ret=E_NOT_OK;
    }else{
        ADC_START_CONVERSION();
        ret=E_OK;
    }
    return ret;
}
/**
 * 
 * @param _adc
 * @param conversion_status
 * @return 
 */
std_returntype ADC_isconversiondone(const adc_conf_t* _adc,uint8 *conversion_status){
    std_returntype ret=E_OK;
    if((NULL==_adc) || (NULL==conversion_status)){
        ret=E_NOT_OK;
    }else{
        *conversion_status=(uint8)(!(ADCON0bits.GO_nDONE));//we made not(!) to say that we return true if it is completed
    }
    return ret;
}
/**
 * 
 * @param _adc
 * @param conversion_result
 * @return 
 */
std_returntype ADC_getconversionresult(const adc_conf_t* _adc,adc_result_t *conversion_result){
    std_returntype ret=E_OK;
    if((NULL==_adc) || (NULL==conversion_result)){
        ret=E_NOT_OK;
    }else{
        if(ADC_RESULT_RIGHT == _adc->result_format){
            *conversion_result=(adc_result_t)((ADRESH<<8)+ADRESL);
        }else if(ADC_RESULT_left == _adc->result_format){
            *conversion_result=(adc_result_t)((ADRESH << 8)+(ADRESL>>6));
        }else{
            *conversion_result=(adc_result_t)((ADRESH<<8)+ADRESL);
        }
    }
    return ret;
}
/**
 * 
 * @param _adc
 * @param channel
 * @param conversion_status
 * @return 
 */
std_returntype ADC_getconversion_blocking(const adc_conf_t* _adc,adc_channel_select_t channel,uint8 *conversion_result){
    std_returntype ret=E_OK;
    uint8 l_conversion_status=0;
    if((NULL==_adc) || (NULL==conversion_result)){
        ret=E_NOT_OK;
    }else{
        ret &= ADC_selectchannel(_adc,channel);
        ret=ADC_startconversion(_adc);
        /*check conversion*/
        while(ADCON0bits.GO_nDONE);//IT IS called blocking state we can do it instead of if condition
        ret=ADC_getconversionresult(_adc,conversion_result);
//       ret=ADC_isconversiondone(_adc,&l_conversion_status);
//        if(ADC_conversion_completed==l_conversion_status){
//            ret=ADC_getconversionresult(_adc,conversion_result);      
//        }else{
//            ret=E_NOT_OK;
//        }
    }
    return ret;
}
static inline void adc_input_channel_port_configure(adc_channel_select_t channel){//inline functions written as normal functions but acts as macro ones
    switch(channel){
        case ADC_CHANNEL_AN0: setbit(TRISA,_TRISA_RA0_POSN);//so this pin is input
            break;
        case ADC_CHANNEL_AN1:setbit(TRISA,_TRISA_RA1_POSN);
            break;
        case ADC_CHANNEL_AN2:setbit(TRISA,_TRISA_RA2_POSN);
            break; 
         case ADC_CHANNEL_AN3: setbit(TRISA,_TRISA_RA3_POSN);//so this pin is input
            break;
        case ADC_CHANNEL_AN4:setbit(TRISA,_TRISA_RA5_POSN);
            break;
        case ADC_CHANNEL_AN5:setbit(TRISE,_TRISE_RE0_POSN);
            break;    
         case ADC_CHANNEL_AN6: setbit(TRISE,_TRISE_RE1_POSN);//so this pin is input
            break;
        case ADC_CHANNEL_AN7:setbit(TRISE,_TRISE_RE2_POSN);
            break;
        case ADC_CHANNEL_AN8:setbit(TRISB,_TRISB_RB2_POSN);
            break;    
         case ADC_CHANNEL_AN9: setbit(TRISB,_TRISB_RB3_POSN);//so this pin is input
            break;
        case ADC_CHANNEL_AN10:setbit(TRISB,_TRISB_RB1_POSN);
            break;
        case ADC_CHANNEL_AN11:setbit(TRISB,_TRISB_RB4_POSN);
            break;    
        case ADC_CHANNEL_AN12: setbit(TRISB,_TRISB_RB0_POSN);//so this pin is input
            break;    
    }
}

std_returntype ADC_startconversion_interrupt(const adc_conf_t* _adc,adc_channel_select_t channel){
    std_returntype ret=E_OK;
    uint8 l_conversion_status=0;
    if((NULL==_adc)){
        ret=E_NOT_OK;
    }else{
        ret &= ADC_selectchannel(_adc,channel);
        ret &= ADC_startconversion(_adc);/*adc start with no blocking when conversion ends adc interrupt flag is set if 
                                          interrupt is enabled then interrupt manager is called then it calls call back function
                                          so there you read value in adc register
                                           */
    }
    return ret;
}

static inline void select_result_format(const adc_conf_t *_adc){
    if(ADC_RESULT_RIGHT == _adc->result_format){
        ADC_RESULT_RIGHT_FORMAT();
    }else if(ADC_RESULT_left == _adc->result_format){
        ADC_RESULT_LEFT_FORMAT();
    }else{
        ADC_RESULT_RIGHT_FORMAT();
    }
}
static inline void configure_voltage_reference(const adc_conf_t *_adc){
    if(ADC__VOLTAGE_REFERENCE_ENABLE==_adc->voltage_reference){
        ADC__ENABLE_VOLTAGE_REFERENCE();
    }else if(ADC__VOLTAGE_REFERENCE_DISABLE==_adc->voltage_reference){
        ADC__DISABLE_VOLTAGE_REFERENCE();
    }else{
        ADC__DISABLE_VOLTAGE_REFERENCE();
    }
}
void ADC_ISR(void){
    ADC_interruptflagclear();
    if(ADC_interrupthandler){
        ADC_interrupthandler();
    }
}