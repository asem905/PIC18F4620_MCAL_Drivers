/* 
 * File:   mcal_interrupt_manager.h
 * Author: Asem
 *
 * Created on February 8, 2024, 11:21 PM
 */

#ifndef MCAL_INTERRUPT_MANAGER_H
#define	MCAL_INTERRUPT_MANAGER_H

/*section : includes*/
#include "mcal_interrupt_config.h"

/*section : macro declarations*/

/*section : macro functions*/

/*section : data type declarartion*/

/*section :functions declaration*/
void INT0_ISR();
void INT1_ISR();
void INT2_ISR();
void RB4_ISR(uint8 RB4_source);
void RB5_ISR(uint8 RB5_source);
void RB6_ISR(uint8 RB6_source);
void RB7_ISR(uint8 RB7_source);
void ADC_ISR(void);
void tmr0_ISR();
void tmr1_ISR();
void tmr2_ISR();
void tmr3_ISR();
void ccp1_ISR();
void ccp2_ISR();
void eusrat_tx_ISR(void);
void eusrat_rx_ISR(void);
void mssp_i2c_bc_ISR();
void mssp_i2c_ISR();
void mssp_spi_ISR();
#endif


