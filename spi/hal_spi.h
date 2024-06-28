/* 
 * File:   hal_spi.h
 * Author: Asem
 *
 * Created on April 20, 2024, 9:09 PM
 */

#ifndef HAL_SPI_H
#define	HAL_SPI_H

/*section : includes*/
#include "../proc/pic18f4620.h"
#include "../mcal_std_types.h"
#include "../interrupt/mcal_internal_interrupt.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
/*section : macro declarations*/
#define spi_mssp_master_mode 1
#define spi_mssp_slave_mode 0

#define spi_mssp_master_sampleatmiddle 0
#define spi_mssp_master_sampleatend 1
#define spi_mssp_clock_polarity_high_level 1
#define spi_mssp_clock_polarity_low_level 0
#define spi_mssp_active_idle 1
#define spi_mssp_idle_active 0
/*mssp modes of spi*/
#define spi_slave_mode_ss_disabled 0x05
#define spi_slave_mode_ss_enabled 0x04//used
#define spi_master_mode_tmr2 0x03
#define spi_master_mode_div_by64 0x02
#define spi_master_mode_div_by16 0x01
#define spi_master_mode_div_by4 0x00


#define spi_mssp_slave_sample 0
/*section : macro functions*/
#define mssp_module_enable_cfg()         (SSPCON1bits.SSPEN=1)
#define mssp_module_disable_cfg()        (SSPCON1bits.SSPEN=0)
#define mssp_ss_enable()                 (LATAbits.LA5=0)  
#define mssp_ss_disable()                 (LATAbits.LA5=1)   
/*section : data type declarartion*/
typedef struct{
    uint8 spi_mode_cfg;
    uint8 spi_mode:1;//master or slave
    uint8 spi_sample_phase:1;
    uint8 spi_clock_edge:1;
    uint8 spi_clock_polarity:1;    
    uint8 spi_reserved:4;
}spi_configs_t;
typedef struct{
    #if mssp_spi_interrupt_feature_enable ==interrupt_feature_enable
        interrupt_priority_cfg mssp_spi_priority;
        void (*spi_defaultinterrupthandler_func)(void);
    #endif

    spi_configs_t spi_cfg;
}mssp_spi_t;
/*section :functions declaration*/
std_returntype mssp_spi_init(const mssp_spi_t *spi_obj);
std_returntype mssp_spi_deinit(const mssp_spi_t *spi_obj);
std_returntype mssp_spi_write_blocking(const mssp_spi_t *spi_obj,uint8 spi_data);
std_returntype mssp_spi_read_blocking(const mssp_spi_t *spi_obj,uint8 *spi_data);
#endif	/* HAL_SPI_H */

