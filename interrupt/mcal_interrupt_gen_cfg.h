/* 
 * File:   mcal_interrupt_gen_cfg.h
 * Author: Asem
 *
 * Created on February 9, 2024, 3:53 PM
 */

#ifndef MCAL_INTERRUPT_GEN_CFG_H
#define	MCAL_INTERRUPT_GEN_CFG_H

/*section : includes*/


/*section : macro declarations*/
#define interrupt_feature_enable 1
//#define interrupt_priority_levels_enable               interrupt_feature_enable
#define external_interrupt_INTx_feature_enable         interrupt_feature_enable
#define external_interrupt_onchange_feature_enable     interrupt_feature_enable
#define ADC_interrupt_feature_enable                   interrupt_feature_enable
#define timer0_interrupt_feature_enable                interrupt_feature_enable
#define timer1_interrupt_feature_enable                interrupt_feature_enable
#define timer2_interrupt_feature_enable                interrupt_feature_enable
#define timer3_interrupt_feature_enable                interrupt_feature_enable
#define ccp1_interrupt_feature_enable                  interrupt_feature_enable
#define ccp2_interrupt_feature_enable                  interrupt_feature_enable
#define eusart_tx_interrupt_feature_enable             interrupt_feature_enable
#define eusart_rx_interrupt_feature_enable             interrupt_feature_enable
#define mssp_i2c_interrupt_feature_enable              interrupt_feature_enable
#define mssp_spi_interrupt_feature_enable              interrupt_feature_enable
/*section : macro functions*/

/*section : data type declarartion*/

/*section :functions declaration*/

#endif	/* MCAL_INTERRUPT_GEN_CFG_H */

