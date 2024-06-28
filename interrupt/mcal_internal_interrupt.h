/* 
 * File:   mcal_internal_interrupt.h
 * Author: Asem
 *
 * Created on February 8, 2024, 11:21 PM
 */

#ifndef MCAL_INTERNAL_INTERRUPT_H
#define	MCAL_INTERNAL_INTERRUPT_H
/*section : includes*/
#include "mcal_interrupt_config.h"

/*section : macro declarations*/

/*section : macro functions*/
#if ADC_interrupt_feature_enable ==interrupt_feature_enable
    /*to clear interrupt enable for adc*/
    #define ADC_interruptdisable() (PIE1bits.ADIE=0)//from ADIE BIT WHICH IS AD INTERRUPT ENABLE
    /*to enable interrutp ADC*/
    #define ADC_interruptenable() (PIE1bits.ADIE=1)
    /*clear flag of adc interrupt*/
    #define ADC_interruptflagclear() (PIR1bits.ADIF=0)//FROM ADIF FLAG BIT WHICH IS CONVERSION BIT SO SET IT IF IT WORKS 
#if interrupt_priority_levels_enable == interrupt_feature_enable
/*set adc module as high priority interrupt*/
    #define ADC_highpriorityset() (IPR1bits.ADIP=1)
/*set adc module as low priority interrupt*/
    #define ADC_lowpriorityset() (IPR1bits.ADIP=0)
#endif
#endif

/*usart*/
#if eusart_tx_interrupt_feature_enable ==interrupt_feature_enable
    /*to clear interrupt enable for eusart*/
    #define eusart_tx_interruptdisable() (PIE1bits.TXIE=0)
    /*to enable interrutp eusart*/
    #define eusart_tx_interruptenable() (PIE1bits.TXIE=1)
    /*clear flag of eusart interrupt*/
    //#define eusart_tx_interruptflagclear() (PIR1bits.TXIF=0)//HERE it is cleared not manually but automatically when writting in tx reg again so not used 
#if interrupt_priority_levels_enable == interrupt_feature_enable
/*set eusart module as high priority interrupt*/
    #define eusart_tx_highpriorityset() (IPR1bits.TXIP=1)
/*set eusart module as low priority interrupt*/
    #define eusart_tx_lowpriorityset() (IPR1bits.TXIP=0)
#endif
#endif

#if eusart_rx_interrupt_feature_enable ==interrupt_feature_enable
    /*to clear interrupt enable for eusart*/
    #define eusart_rx_interruptdisable() (PIE1bits.RCIE=0)
    /*to enable interrutp eusart*/
    #define eusart_rx_interruptenable() (PIE1bits.RCIE=1)
    /*clear flag of eusart_rx interrupt*/
    //#define eusart_tx_interruptflagclear() (PIR1bits.RCIF=0)//HERE it is cleared not manually but automatically when writting in rc reg again so not used 
#if interrupt_priority_levels_enable == interrupt_feature_enable
/*set eusart module as high priority interrupt*/
    #define eusart_rx_highpriorityset() (IPR1bits.RCIP=1)
/*set eusart module as low priority interrupt*/
    #define eusart_rx_lowpriorityset() (IPR1bits.RCIP=0)
#endif
#endif

/*=========timers interrupt feature========*/
#if timer0_interrupt_feature_enable ==interrupt_feature_enable
    /*to clear interrupt enable for timer0*/
    #define timer0_interruptdisable() (INTCONbits.TMR0IE=0)//from TMR0IE BIT WHICH IS AD INTERRUPT ENABLE
    /*to enable interrutp timer0*/
    #define timer0_interruptenable() (INTCONbits.TMR0IE=1)
    /*clear flag of timer0 interrupt*/
    #define timer0_interruptflagclear() (INTCONbits.TMR0IF=0)//FROM TMR0IF FLAG BIT WHICH IS CONVERSION BIT SO SET IT IF IT WORKS 
#if interrupt_priority_levels_enable == interrupt_feature_enable
/*set timer0 module as high priority interrupt*/
    #define timer0_highpriorityset() (INTCON2bits.TMR0IP=1)
/*set timer0 module as low priority interrupt*/
    #define timer0_lowpriorityset() (INTCON2bits.TMR0IP=0)
#endif
#endif

#if timer1_interrupt_feature_enable ==interrupt_feature_enable
    /*to clear interrupt enable for timer1*/
    #define timer1_interruptdisable() (PIE1bits.TMR1IE=0)//from TMR0IE BIT WHICH IS AD INTERRUPT ENABLE
    /*to enable interrutp timer1*/
    #define timer1_interruptenable() (PIE1bits.TMR1IE=1)
    /*clear flag of timer1 interrupt*/
    #define timer1_interruptflagclear() (PIR1bits.TMR1IF=0)//FROM TMR0IF FLAG BIT WHICH IS CONVERSION BIT SO SET IT IF IT WORKS 
#if interrupt_priority_levels_enable == interrupt_feature_enable
/*set timer1 module as high priority interrupt*/
    #define timer1_highpriorityset() (IPR1bits.TMR1IP=1)
/*set timer1 module as low priority interrupt*/
    #define timer1_lowpriorityset() (IPR1bits.TMR1IP=0)
#endif
#endif

#if timer2_interrupt_feature_enable ==interrupt_feature_enable
    /*to clear interrupt enable for timer2*/
    #define timer2_interruptdisable() (PIE1bits.TMR2IE=0)//from TMR0IE BIT WHICH IS AD INTERRUPT ENABLE
    /*to enable interrutp timer2*/
    #define timer2_interruptenable() (PIE1bits.TMR2IE=1)
    /*clear flag of timer2 interrupt*/
    #define timer2_interruptflagclear() (PIR1bits.TMR2IF=0)//FROM TMR0IF FLAG BIT WHICH IS CONVERSION BIT SO SET IT IF IT WORKS 
#if interrupt_priority_levels_enable == interrupt_feature_enable
/*set timer2 module as high priority interrupt*/
    #define timer2_highpriorityset() (IPR1bits.TMR2IP=1)
/*set timer2 module as low priority interrupt*/
    #define timer2_lowpriorityset() (IPR1bits.TMR2IP=0)
#endif
#endif

#if timer1_interrupt_feature_enable ==interrupt_feature_enable
    /*to clear interrupt enable for timer3*/
    #define timer3_interruptdisable() (PIE2bits.TMR3IE=0)//from TMR0IE BIT WHICH IS AD INTERRUPT ENABLE
    /*to enable interrutp timer3*/
    #define timer3_interruptenable() (PIE2bits.TMR3IE=1)
    /*clear flag of timer3 interrupt*/
    #define timer3_interruptflagclear() (PIR2bits.TMR3IF=0)//FROM TMR0IF FLAG BIT WHICH IS CONVERSION BIT SO SET IT IF IT WORKS 
#if interrupt_priority_levels_enable == interrupt_feature_enable
/*set timer3 module as high priority interrupt*/
    #define timer3_highpriorityset() (IPR2bits.TMR3IP=1)
/*set timer3 module as low priority interrupt*/
    #define timer3_lowpriorityset() (IPR2bits.TMR3IP=0)
#endif
#endif
/*=========ccp1 interrupt feature========*/
#if ccp1_interrupt_feature_enable ==interrupt_feature_enable
    /*to clear interrupt enable for ccp1*/
    #define ccp1_interruptdisable() (PIE1bits.CCP1IE=0)//from TMR0IE BIT WHICH IS AD INTERRUPT ENABLE
    /*to enable interrutp ccp1*/
    #define ccp1_interruptenable() (PIE1bits.CCP1IE=1)
    /*clear flag of ccp1 interrupt*/
    #define ccp1_interruptflagclear() (PIR1bits.CCP1IF=0)//FROM TMR0IF FLAG BIT WHICH IS CONVERSION BIT SO SET IT IF IT WORKS 
#if interrupt_priority_levels_enable == interrupt_feature_enable
/*set ccp1 module as high priority interrupt*/
    #define ccp1_highpriorityset() (IPR1bits.CCP1IP=1)
/*set ccp1 module as low priority interrupt*/
    #define ccp1_lowpriorityset() (IPR1bits.CCP1IP=0)
#endif
#endif
/*=========ccp2 interrupt feature========*/
#if ccp2_interrupt_feature_enable ==interrupt_feature_enable
    /*to clear interrupt enable for ccp2*/
    #define ccp2_interruptdisable() (PIE2bits.CCP2IE=0)//from  BIT WHICH IS AD INTERRUPT ENABLE
    /*to enable interrutp ccp2*/
    #define ccp2_interruptenable() (PIE2bits.CCP2IE=1)
    /*clear flag of ccp1 interrupt*/
    #define ccp2_interruptflagclear() (PIR2bits.CCP2IF=0)//FROM  FLAG BIT WHICH IS CONVERSION BIT SO SET IT IF IT WORKS 
#if interrupt_priority_levels_enable == interrupt_feature_enable
/*set ccp2 module as high priority interrupt*/
    #define ccp2_highpriorityset() (IPR2bits.CCP2IP=1)
/*set ccp2 module as low priority interrupt*/
    #define ccp2_lowpriorityset() (IPR2bits.CCP2IP=0)
#endif
#endif
/*=========mssp_i2c interrupt feature========*/
#if mssp_i2c_interrupt_feature_enable ==interrupt_feature_enable
    /*to clear interrupt enable */
    #define mssp_i2c_interruptdisable() (PIE1bits.SSPIE=0)
    #define mssp_i2c_bus_collission_interruptdisable() (PIE2bits.BCLIE=0)//for collision interrupt
    /*to enable interrutp */
    #define mssp_i2c_interruptenable() (PIE1bits.SSPIE=1)
    #define mssp_i2c_bus_collission_interruptdenable() (PIE2bits.BCLIE=1)//for collision interrupt
    /*clear flag of  interrupt*/
    #define mssp_i2c_interruptflagclear() (PIR1bits.SSPIF=0)
    #define mssp_i2c_bus_collission_interruptflagclear() (PIR2bits.BCLIF=0)//for collision interrupt
#if interrupt_priority_levels_enable == interrupt_feature_enable
/*set mssp_i2c module as high priority interrupt*/
    #define mssp_i2c_highpriorityset() (IPR1bits.SSPIP=1)
    #define mssp_i2c_bus_collission_highpriorityset() (IPR2bits.BCLIP=1)//for collision interrupt
/*set mssp_i2c module as low priority interrupt*/
    #define mssp_i2c_lowpriorityset() (IPR1bits.SSPIP=0)
    #define mssp_i2c_bus_collission_highpriorityset() (IPR2bits.BCLIP=0)//for collision interrupt
#endif
#endif

/*====================SPI===================*/
#if mssp_spi_interrupt_feature_enable ==interrupt_feature_enable
    /*to clear interrupt enable */
    #define mssp_spi_interruptdisable() (PIE1bits.SSPIE=0)
    /*to enable interrutp */
    #define mssp_spi_interruptenable() (PIE1bits.SSPIE=1)
    /*clear flag of  interrupt*/
    #define mssp_spi_interruptflagclear() (PIR1bits.SSPIF=0)
#if interrupt_priority_levels_enable == interrupt_feature_enable
/*set mssp_spi module as high priority interrupt*/
    #define mssp_spi_highpriorityset() (IPR1bits.SSPIP=1)
/*set mssp_spi module as low priority interrupt*/
    #define mssp_spi_lowpriorityset() (IPR1bits.SSPIP=0)
#endif
#endif


/*section : data type declarartion*/
typedef void (*interrupthandler)(void);
/*section :functions declaration*/

#endif

