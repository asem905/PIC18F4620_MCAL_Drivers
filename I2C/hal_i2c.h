/* 
 * File:   hal_i2c.h
 * Author: Asem
 *
 * Created on April 13, 2024, 2:36 PM
 */

#ifndef HAL_I2C_H
#define	HAL_I2C_H
/*==========section : includes==========*/
#include "../mcal_std_types.h"
#include "../proc/pic18f4620.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "../interrupt/mcal_internal_interrupt.h"
/*==========section : macro declarations==========*/
#define i2c_slew_rate_enable 0
#define i2c_slew_rate_disable 1

#define i2c_smbus_enable 1
#define i2c_smbus_disable 0
/*to detect what i recieved data or address*/
#define i2c_last_byte_data 1
#define i2c_last_byte_address 0
 
#define i2c_stop_bit_detected 1
#define i2c_stop_bit_not_detected 0
#define i2c_start_bit_detected 1
#define i2c_start_bit_not_detected 0

#define i2c_mssp_master_mode 1
#define i2c_mssp_slave_mode 0

/*mssp modes:*/
#define i2c_slave_mode_7bit_address 0x06U
#define i2c_slave_mode_10bit_address 0x07U
#define i2c_slave_mode_7bit_address_ss_int_enable 0x0EU
#define i2c_slave_mode_10bit_address_ss_int_enable 0x0FU
#define i2c_master_mode_defined_clock 0x08U
#define i2c_master_mode_firmware_controlled 0x0BU

#define i2c_general_call_enable 1
#define i2c_general_call_disable 0

#define i2c_master_recieve_enable 1
#define i2c_master_recieve_disable 0

#define i2c_ack_recieved_from_slave 0
#define i2c_ack_not_recieved_from_slave 1

#define i2c_master_send_ack 0
#define i2c_master_send_not_ack 1
/*==========section : macro functions==========*/
#define i2c_slew_rate_enable_cfg()       (SSPSTATbits.SMP=0)
#define i2c_slew_rate_disable_cfg()      (SSPSTATbits.SMP=1)

#define i2c_smbus_enable_cfg()           (SSPSTATbits.CKE=1)
#define i2c_smbus_disable_cfg()          (SSPSTATbits.CKE=0)

#define i2c_general_call_enable_cfg()    (SSPCON2bits.GCEN=1)
#define i2c_general_call_disable_cfg()   (SSPCON2bits.GCEN=0) 

#define i2c_master_recieve_enable_cfg()  (SSPCON2bits.RCEN=1)
#define i2c_master_recieve_disable_cfg() (SSPCON2bits.RCEN=0)   
#define mssp_module_enable_cfg()         (SSPCON1bits.SSPEN=1)
#define mssp_module_disable_cfg()        (SSPCON1bits.SSPEN=0)
/*clock stretch*/
#define i2c_clock_stretch_enable()       (SSPCON1bits.CKP=0)
/*release clock*/
#define i2c_clock_release_enable()       (SSPCON1bits.CKP=1)
/*==========section : data type declarartion==========*/
typedef struct{
    uint8 i2c_mode_cfg;//msssp mode select which has 6modes slave 7bit or slave 10 bit or....
    uint8 i2c_slave_address;
    uint8 i2c_mode:1;//master or slave
    uint8 i2c_slew_rate:1;//enable or disable it
    uint8 i2c_smbus_control:1;//enable or disable it
    uint8 i2c_general_call:1;//enable or disable it
    uint8 i2c_master_rec_mode:1;//master as reciever or not
    uint8 i2c_reserved:3;
    #if mssp_i2c_interrupt_feature_enable ==interrupt_feature_enable
        interrupt_priority_cfg mssp_i2c_priority;
        interrupt_priority_cfg mssp_i2c_bus_collission_priority;
    #endif
}i2c_configs_t;
typedef struct{
    uint32 i2c_clock;
    #if mssp_i2c_interrupt_feature_enable ==interrupt_feature_enable
        void (*i2c_report_write_collision)(void);
        void (*i2c_defaultinterrupthandler)(void);
    #endif
    void (*i2c_report_recieve_overflow)(void);//we may put it in inyerrupt part but it doesnot have to be as we may put it as reference that overflow happens
    i2c_configs_t i2c_cfg;
}mssp_i2c_t;
/*==========section :functions declaration==========*/
std_returntype mssp_i2c_init(const mssp_i2c_t *i2c_obj);
std_returntype mssp_i2c_deinit(const mssp_i2c_t *i2c_obj);
std_returntype mssp_i2c_master_send_start(const mssp_i2c_t *i2c_obj);
std_returntype mssp_i2c_master_send_repeated_start(const mssp_i2c_t *i2c_obj);
std_returntype mssp_i2c_master_send_stop(const mssp_i2c_t *i2c_obj);
std_returntype mssp_i2c_master_write_blocking(const mssp_i2c_t *i2c_obj,uint8 i2c_data,uint8 *_ack);
std_returntype mssp_i2c_master_read_blocking(const mssp_i2c_t *i2c_obj,uint8 ack,uint8 *i2c_data);//ack is done after receiving by reciever
#endif	/* HAL_I2C_H */

