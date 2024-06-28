/* 
 * File:   hal_usart.h
 * Author: Asem
 *
 * Created on April 6, 2024, 2:35 PM
 */

#ifndef HAL_USART_H
#define	HAL_USART_H

/*section : includes*/
#include "hal_usart_cfg.h"
#include "../proc/pic18f4620.h"
#include "../mcal_std_types.h"
#include "../interrupt/mcal_internal_interrupt.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
/*================section : macro declarations==============*/
/*enable disable usart module*/
#define eusart_module_enable 1
#define eusart_module_disable 0

#define eusart_synchronous_mode 1
#define eusart_asynchronous_mode 0
/*baud rate gen async. speed mode*/
#define eusart_asynchronous_high_speed  1
#define eusart_asynchronous_low_speed   0
/*baud rate gen register size*/
#define eusart_16bit_baudrate_gen 1
#define eusart_08bit_baudrate_gen 0
/*eusart transmit enable*/
#define eusart_asynchronous_tx_enable  1
#define eusart_asynchronous_tx_disable   0
/*eusart transmit interrupt enable*/
#define eusart_asynchronous_interrupt_tx_enable  1
#define eusart_asynchronous_interrupt_tx_disable   0
/*eusart 9_bit transmit enable*/
#define eusart_asynchronous_9bit_tx_enable  1
#define eusart_asynchronous_9bit_tx_disable   0

/*eusart reciever enable*/
#define eusart_asynchronous_rx_enable  1
#define eusart_asynchronous_rx_disable   0
/*eusart reciever interrupt enable*/
#define eusart_asynchronous_interrupt_rx_enable  1
#define eusart_asynchronous_interrupt_rx_disable   0
/*eusart 9_bit reciever enable*/
#define eusart_asynchronous_9bit_rx_enable  1
#define eusart_asynchronous_9bit_rx_disable   0

/*eusart framing error*/
#define eusart_framing_error_detected 1
#define eusart_framing_error_cleared 0
/*eusart overrun error*/
#define eusart_overrun_error_detected 1
#define eusart_overrun_error_cleared 0
/*section : macro functions*/

/*section : data type declarartion*/
typedef enum{
    baudrate_asyn_8bit_low_speed,
    baudrate_asyn_8bit_high_speed,
    baudrate_asyn_16bit_low_speed,
    baudrate_asyn_16bit_high_speed,
    baudrate_syn_8bit,
    baudrate_syn_16bit,      
}baudrate_gen_t;//choose according to it's error percentage
typedef struct{
    uint8 usart_tx_reserved:5;
    uint8 usart_tx_enable:1;
    uint8 usart_tx_interrupt_enable:1;
    uint8 usart_tx_9bit_enable:1;
    interrupt_priority_cfg usart_tx_priority;
}usart_tx_cfg_t;
typedef struct{
    uint8 usart_rx_reserved:5;
    uint8 usart_rx_enable:1;
    uint8 usart_rx_interrupt_enable:1;
    uint8 usart_rx_9bit_enable:1;
    interrupt_priority_cfg usart_rx_priority;
}usart_rx_cfg_t;
typedef union{
    struct{
        uint8 usart_tx_reserved:6;
        uint8 usart_ferr:1;
        uint8 usart_oerr:1;
    };
    uint8 status;
}usart_error_status;
typedef struct{
    uint32 baud_rate;
    baudrate_gen_t baudrate_gen_config;
    usart_tx_cfg_t usart_tx_cfg;
    usart_rx_cfg_t usart_rx_cfg;
    usart_error_status error_status;
    void (*eusart_txdefaultinterrupthandler)(void);
    void (*eusart_rxdefaultinterrupthandler)(void);
    void (*eusart_framingerrorhandler)(void);
    void (*eusart_overrunerrorhandler)(void);
}usart_t;
/*section :functions declaration*/
std_returntype eusart_async_init(const usart_t *_eusart);
std_returntype eusart_async_deinit(const usart_t *_eusart);
std_returntype eusart_async_readbyteblocking(uint8 *_data);//write data you want to recieve
std_returntype eusart_async_readbytenonblocking(uint8 *_data);//write data you want to recieve
std_returntype eusart_async_rx_restart(void);//to restart reciver when overrun error happens

std_returntype eusart_async_writebyteblocking(uint8 _data);//write data you want to transmit
std_returntype eusart_async_writestringblocking(uint8 *_data,uint16 str_len);//to write any string we need pointer and length of string

std_returntype eusart_async_writebytenonblocking(uint8 _data);//write data you want to transmit
std_returntype eusart_async_writestringnonblocking(uint8 *_data,uint16 str_len);//to write any string we need pointer and length of string
#endif	/* HAL_USART_H */

