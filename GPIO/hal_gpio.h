
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef HAL_GPIO_H
#define	HAL_GPIO_H
/*section : includes*/
#include "../proc/pic18f4620.h"
#include "../mcal_std_types.h"
#include "../device_config.h"
#include "hal_gpio_config.h"
/*section : macro declarations*/
#define port_pin_max_numbers 8
#define port_max_numbers 5
#define portc_mask    0xFF

/*section : macro functions*/
#define HWREG(_x)  (*((volatile unsigned char *)(_x)))
#define setbit(reg,bit_postn)  (reg |= (1<<bit_postn))
#define clearbit(reg,bit_postn)  (reg &= ~(1<<bit_postn))
#define togglebit(reg,bit_postn)  (reg ^= (1<<bit_postn))
#define readbit(reg,bit_postn)  ((reg >> bit_postn)&1)

/*section : data type declarartion*/
typedef enum{
    low=0,high
}_logic;
typedef enum{
    gpio_Direction_output=0,gpio_Direction_input
}direction;
typedef enum{
    pin0=0,
    pin1,
    pin2,
    pin3,
    pin4,
    pin5,
    pin6,  
    pin7        
}pin_index;
typedef enum{
    porta_index=0,portb_index,portc_index,portd_index,porte_index
}port_index;
typedef struct{
    uint8 port:3;/* @ref port_index */
    uint8 pin:3;/* @ref pin_index */
    uint8 direction:1;/* @ref direction */
    uint8 logic:1;/* @ref logic */
}pin_config;
/*section :functions declaration*/
std_returntype gpio_pin_direction_initilaize (const pin_config *pin_config_);
std_returntype gpio_pin_direction_status (const pin_config *pin_config_,direction *directionstatus);
std_returntype gpio_pin_write_logic (const pin_config *pin_config_,_logic logic);
std_returntype gpio_pin_read_logic (const pin_config *pin_config_,_logic *logic);
std_returntype gpio_pin_toggle_logic (const pin_config *pin_config_);

std_returntype gpio_pin_initilaize (const pin_config *pin_config_);

std_returntype gpio_port_direction_initilaize (port_index port,uint8 direction);
std_returntype gpio_port_get_direction_status (port_index port,uint8 *directionstatus);
std_returntype gpio_port_write_logic (port_index port,uint8 logicc);
std_returntype gpio_port_read_logic (port_index port,uint8 *logicc);
std_returntype gpio_port_toggle_logic (port_index port);
#endif	/* HAL_GPIO_H */

