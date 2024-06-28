/* 
 * File:   hal_eeprom.h
 * Author: Asem
 *
 * Created on February 17, 2024, 8:17 PM
 */

#ifndef HAL_EEPROM_H
#define	HAL_EEPROM_H

/*section : includes*/
#include "../mcal_std_types.h"
#include "../../MCAL_Layer/interrupt/mcal_internal_interrupt.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
#include "../proc/pic18f4620.h"

/*section : macro declarations*/
/*flash or eeprom select*/
#define access_flash_program_memory 1
#define access_eeprom_program_memory 0
/*flash/eeprom or configuration registers select*/
#define access_config_registers 1
#define access_eeprom_flash_registers  0
/*flash/eeprom write data enable*/
#define allow_write_cycles_flash_eeprom 1
#define inhibt_write_cycles_flash_eeprom  0
/*write control*/
#define initiate_data_eeprom_write_erase 1
#define data_eeprom_write_completed 0
/*read control*/
#define initiate_data_eeprom_read 1
#define data_eeprom_read_completed 0
/*section : macro functions*/

/*section : data type declarartion*/

/*section :functions declaration*/
std_returntype data_eeprom_writebyte(uint16 badd,uint8 bData);
std_returntype data_eeprom_readbyte(uint16 badd,uint8 *bData);//to return data i need
#endif	/* HAL_EEPROM_H */

