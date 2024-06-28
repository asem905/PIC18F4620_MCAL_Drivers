/* 
 * File:   hal_adc.h
 * Author: Asem
 *
 * Created on February 23, 2024, 10:58 PM
 */

#ifndef HAL_ADC_H
#define	HAL_ADC_H

/*section : includes*/
#include "../proc/pic18f4620.h"
#include "../mcal_std_types.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "../../MCAL_Layer/interrupt/mcal_internal_interrupt.h"
#include "hal_adc_cfg.h"
/*section : macro declarations*/
#define ADC_CONVERSION_STATUS() (ADCON0bits.GO_nDONE)//tell me about adc conversion status
#define ADC_START_CONVERSION() (ADCON0bits.GO_nDONE=1)

/*======================================================================*/
#define ADC_CONVERTOR_ENABLE() (ADCON0bits.ADON=1)//to enable ADC module
#define ADC_CONVERTOR_DISABLE() (ADCON0bits.ADON=0)

/*======================================================================*/
#define ADC__ENABLE_VOLTAGE_REFERENCE() do{ADCON1bits.VCFG1=1;\
                                           ADCON1bits.VCFG0=1;\
                                        }while(0);//if doing this so we depend on: VREFERENCE FROM OUTSIDE
#define ADC__DISABLE_VOLTAGE_REFERENCE() do{ADCON1bits.VCFG1=0;\
                                           ADCON1bits.VCFG0=0;\
                                        }while(0);
/*======================================================================*/
//port configuration analog or digital:
#define ADC_ANO_ANALOG_FUNCTIONALITY 0x0E// we determine which is analog and all before it is analog too look at data sheet 
#define ADC_AN1_ANALOG_FUNCTIONALITY 0x0D
#define ADC_AN2_ANALOG_FUNCTIONALITY 0x0C
#define ADC_AN3_ANALOG_FUNCTIONALITY 0x0B
#define ADC_AN4_ANALOG_FUNCTIONALITY 0x0A
#define ADC_AN5_ANALOG_FUNCTIONALITY 0x09
#define ADC_AN6_ANALOG_FUNCTIONALITY 0x08
#define ADC_AN7_ANALOG_FUNCTIONALITY 0x07
#define ADC_AN8_ANALOG_FUNCTIONALITY 0x06
#define ADC_AN9_ANALOG_FUNCTIONALITY 0x05
#define ADC_AN10_ANALOG_FUNCTIONALITY 0x04
#define ADC_AN11_ANALOG_FUNCTIONALITY 0x03
#define ADC_AN12_ANALOG_FUNCTIONALITY 0x02
#define ADC_ALL_ANALOG_FUNCTIONALITY 0x00
#define ADC_ALL_DIGITAL_FUNCTIONALITY 0x0F

#define ADC_ANALOG_DIGITAL_PORT_CONFIG(_CONFIG) (ADCON1bits.PCFG=_CONFIG)//IT ATKE SONE OF CHANNELS AS PARAMETER AND PUT IT IN PCFG BITS
/*======================================================================*/
#define ADC_RESULT_RIGHT_FORMAT() (ADCON2bits.ADFM=1)//RIGHT JUSTIFIED FORMAT
#define ADC_RESULT_LEFT_FORMAT() (ADCON2bits.ADFM=0)
/*======================================================================*/
#define ADC_RESULT_RIGHT 0x01U
#define ADC_RESULT_left 0x00U

#define ADC__VOLTAGE_REFERENCE_ENABLE 0x01U
#define ADC__VOLTAGE_REFERENCE_DISABLE 0x00U

#define ADC_conversion_completed 1
#define ADC_conversion_inprogress 0
/*section : macro functions*/

/*section : data type declarartion*/
typedef enum{
    ADC_CHANNEL_AN0=0,
    ADC_CHANNEL_AN1, 
    ADC_CHANNEL_AN2,
    ADC_CHANNEL_AN3,
    ADC_CHANNEL_AN4,
    ADC_CHANNEL_AN5,
    ADC_CHANNEL_AN6,
    ADC_CHANNEL_AN7,
    ADC_CHANNEL_AN8,
    ADC_CHANNEL_AN9,
    ADC_CHANNEL_AN10,
    ADC_CHANNEL_AN11,
    ADC_CHANNEL_AN12    
}adc_channel_select_t;

typedef enum{
    ADC_0_TAD=0,
    ADC_2_TAD, 
    ADC_4_TAD,
    ADC_6_TAD, 
    ADC_8_TAD,   
    ADC_12_TAD, 
    ADC_16_TAD,
    ADC_20_TAD           
}adc_acquisition_time_t;

typedef enum{
    ADC_CONVERSION_CLOCK_FOSC_DIV_2,
    ADC_CONVERSION_CLOCK_FOSC_DIV_8,
    ADC_CONVERSION_CLOCK_FOSC_DIV_32,
    ADC_CONVERSION_CLOCK_FOSC_DIV_FRC,    
    ADC_CONVERSION_CLOCK_FOSC_DIV_4,
    ADC_CONVERSION_CLOCK_FOSC_DIV_16,
    ADC_CONVERSION_CLOCK_FOSC_DIV_64       
}adc_conversion_clock_t;//to determine value of TAD

typedef struct{
#if ADC_interrupt_feature_enable ==interrupt_feature_enable
    void (*ADC_InterruptHandler)(void);//to call back to upper layer
    interrupt_priority_cfg priority;
#endif
    adc_acquisition_time_t acquisition_time;/*@ref:adc_acquisition_time_t*/
    adc_conversion_clock_t conversion_clock;/*@ref:adc_conversion_clock_t*/
    adc_channel_select_t adc_channel; /*@ref:adc_channel_select_t*/
    uint8 voltage_reference:1;/*voltage reference configuration*/
    uint8 result_format:1;/*A/D result format select*/
    uint8 ADC_RESERVED:6;//to complete my 1byte
}adc_conf_t;
typedef uint16 adc_result_t;
/*section :functions declaration*/
std_returntype ADC_init(const adc_conf_t* _adc);
std_returntype ADC_deinit(const adc_conf_t* _adc);
std_returntype ADC_selectchannel(const adc_conf_t* _adc,adc_channel_select_t channel);
std_returntype ADC_startconversion(const adc_conf_t* _adc);
std_returntype ADC_isconversiondone(const adc_conf_t* _adc,uint8 *conversion_status);
std_returntype ADC_getconversionresult(const adc_conf_t* _adc,adc_result_t *conversion_result);//size of my result is 10bits so return iy from this funtion
std_returntype ADC_getconversion_blocking(const adc_conf_t* _adc,adc_channel_select_t channel,uint8 *conversion_result);//combination of is conversiondone andselect channnel
std_returntype ADC_startconversion_interrupt(const adc_conf_t* _adc,adc_channel_select_t channel);//start conversion but not waiting for result that's why no blocking here and after conversion interrupt appears
#endif	/* HAL_ADC_H */

