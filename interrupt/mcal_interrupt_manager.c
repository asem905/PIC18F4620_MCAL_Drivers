/* 
 * File:   mcal_interrupt_manager.c
 * Author: Asem
 *
 * Created on February 8, 2024, 11:21 PM
 */
#include "mcal_interrupt_manager.h"
static volatile uint8 RB4_FLAG =1;
static volatile uint8 RB5_FLAG =1;
static volatile uint8 RB6_FLAG =1;
static volatile uint8 RB7_FLAG =1;
#if interrupt_priority_levels_enable == interrupt_feature_enable
void __interrupt() interruptmanagerhigh(void){//we came here due to interrupt qualifier that tell sus to come here each time interrupt happens
    if((INTCONbits.INT0IE == interrupt_enable)&&(interrupt_occur==INTCONbits.INT0IF)){
        INT0_ISR();//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }// we made interrupt 2 here not down because we configure it as high
    if((INTCON3bits.INT2E == interrupt_enable)&&(interrupt_occur==INTCON3bits.INT2F)){
        INT2_ISR();//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
} 
void __interrupt(low_priority) interruptmanagerlow(void){
    if((INTCON3bits.INT1E == interrupt_enable)&&(interrupt_occur==INTCON3bits.INT1F)){
        INT1_ISR();//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
}    
#else 
void __interrupt() interruptmanagerhigh(void){
    if((INTCONbits.INT0IE == interrupt_enable)&&(interrupt_occur==INTCONbits.INT0IF)){
        INT0_ISR();//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
    if((INTCON3bits.INT1E == interrupt_enable)&&(interrupt_occur==INTCON3bits.INT1F)){
        INT1_ISR();//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
    if((INTCON3bits.INT2E == interrupt_enable)&&(interrupt_occur==INTCON3bits.INT2F)){
        INT2_ISR();//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
    
    /*ADC MODULE*/
    if((PIE1bits.ADIE == interrupt_enable)&&(interrupt_occur==PIR1bits.ADIF)){
        ADC_ISR();//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
    
    /*timer0 MODULE*/
    if((INTCONbits.TMR0IE == interrupt_enable)&&(interrupt_occur==INTCONbits.TMR0IF)){
        tmr0_ISR();//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
    /*timer1 MODULE*/
    if((PIE1bits.TMR1IE == interrupt_enable)&&(interrupt_occur==PIR1bits.TMR1IF)){
        tmr1_ISR();//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
    /*timer2 MODULE*/
    if((PIE1bits.TMR2IE == interrupt_enable)&&(interrupt_occur==PIR1bits.TMR2IF)){
        tmr2_ISR();//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
    /*timer3 MODULE*/
    if((PIE2bits.TMR3IE == interrupt_enable)&&(interrupt_occur==PIR2bits.TMR3IF)){
        tmr3_ISR();//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
    
     /*ccp1 MODULE*/
    if((PIE1bits.CCP1IE == interrupt_enable)&&(interrupt_occur==PIR1bits.CCP1IF)){
        ccp1_ISR();//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
    
      /*ccp2 MODULE*/
    if((PIE2bits.CCP2IE == interrupt_enable)&&(interrupt_occur==PIR2bits.CCP2IF)){
        ccp2_ISR();//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
     /*eusart MODULE*/
    if((PIE1bits.TXIE == interrupt_enable)&&(interrupt_occur==PIR1bits.TXIF)){
        eusrat_tx_ISR();//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
    if((PIE1bits.RCIE == interrupt_enable)&&(interrupt_occur==PIR1bits.RCIF)){
        eusrat_rx_ISR();//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
    if((PIE1bits.SSPIE == interrupt_enable)&&(interrupt_occur==PIR1bits.SSPIF)){
        mssp_spi_ISR();//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
    if((PIE1bits.SSPIE == interrupt_enable)&&(interrupt_occur==PIR1bits.SSPIF)){
        mssp_i2c_ISR();//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
    
    if((PIE2bits.BCLIE == interrupt_enable)&&(interrupt_occur==PIR2bits.BCLIF)){
        mssp_i2c_bc_ISR();//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
    
    
    /*portb on change methods*/
    /*========================*/
    
    if((INTCONbits.RBIE == interrupt_enable)&&(interrupt_occur==INTCONbits.RBIF)&& (PORTBbits.RB4 == high)&&(RB4_FLAG ==1)){//this means when i pressed pin carry 5V
        RB4_FLAG =0;
        RB4_ISR(0);//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
    if((INTCONbits.RBIE == interrupt_enable)&&(interrupt_occur==INTCONbits.RBIF)&& (PORTBbits.RB4 == low)&&(RB4_FLAG ==0)){//this means when i removed my hand pin carry 0v
        RB4_FLAG =1;
        RB4_ISR(1);//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
    
    if((INTCONbits.RBIE == interrupt_enable)&&(interrupt_occur==INTCONbits.RBIF)&& (PORTBbits.RB5 == high)&&(RB5_FLAG ==1)){//this means when i pressed pin carry 5V
        RB5_FLAG =0;
        RB5_ISR(0);//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
    if((INTCONbits.RBIE == interrupt_enable)&&(interrupt_occur==INTCONbits.RBIF)&& (PORTBbits.RB5 == low)&&(RB5_FLAG ==0)){//this means when i removed my hand pin carry 0v
        RB5_FLAG =1;
        RB5_ISR(1);//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
    
    
    if((INTCONbits.RBIE == interrupt_enable)&&(interrupt_occur==INTCONbits.RBIF)&& (PORTBbits.RB6 == high)&&(RB6_FLAG ==1)){//this means when i pressed pin carry 5V
        RB6_FLAG =0;
        RB6_ISR(0);//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
    if((INTCONbits.RBIE == interrupt_enable)&&(interrupt_occur==INTCONbits.RBIF)&& (PORTBbits.RB6 == low)&&(RB6_FLAG ==0)){//this means when i removed my hand pin carry 0v
        RB6_FLAG =1;
        RB6_ISR(1);//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
    
    if((INTCONbits.RBIE == interrupt_enable)&&(interrupt_occur==INTCONbits.RBIF)&& (PORTBbits.RB7 == high)&&(RB7_FLAG ==1)){//this means when i pressed pin carry 5V
        RB7_FLAG =0;
        RB7_ISR(0);//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
    if((INTCONbits.RBIE == interrupt_enable)&&(interrupt_occur==INTCONbits.RBIF)&& (PORTBbits.RB7 == low)&&(RB7_FLAG ==0)){//this means when i removed my hand pin carry 0v
        RB7_FLAG =1;
        RB7_ISR(1);//but he doesnot know it so we must declare it in header file
    }else{
        //nothing
    }
    
     /*========================*/
    /*portb on change methods*/
} 
#endif