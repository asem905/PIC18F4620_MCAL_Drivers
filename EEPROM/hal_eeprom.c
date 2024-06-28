/* 
 * File:   hal_eeprom.c
 * Author: Asem
 *
 * Created on February 17, 2024, 8:17 PM
 */
#include "hal_eeprom.h"

std_returntype data_eeprom_writebyte(uint16 badd,uint8 bData){
    
    std_returntype ret=E_OK;//E_OK represents number zero so to make sure of all functions down we do or operator :
    /*read interrupt state first enabled or disabled */
    uint8 global_interrupt_status=INTCONbits.GIE;//to know what was state of interrupt
    /*update address register to be able to access eeprom*/
    EEADRH=(uint8)((badd>>8) & 0x03);
    EEADR=(uint8)(badd & 0xff);
    /*update data register*/
    EEDATA=bData;
    /*select access data eeprom memory*/
    EECON1bits.EEPGD=access_eeprom_program_memory;//To access eeprom memory not flash 
    EECON1bits.CFGS=access_eeprom_flash_registers;//choosing between flash/eeprom or configuration registers
    /*allow write cycles*/
    EECON1bits.WREN=allow_write_cycles_flash_eeprom;
    /*disable all interrupts*/
    interrupt_globalinterruptdisable();
    /*write required sequence :0x55 --> 0xAA*/
    EECON2=0x55;
    EECON2=0xAA;
    /*initiates adata eeprom erase/write cycle*/
    EECON1bits.WR=initiate_data_eeprom_write_erase;
    /*wait for writ to complete*/
    while(EECON1bits.WR);
    /*inhibt write cycles*/
    EECON1bits.WREN=inhibt_write_cycles_flash_eeprom;
    /*restore interrupt status*/
    INTCONbits.GIE=global_interrupt_status;
    return ret;
}
std_returntype data_eeprom_readbyte(uint16 badd,uint8 *bData){
     std_returntype ret=E_OK;//E_OK represents number zero so to make sure of all functions down we do or operator :
    if(NULL==bData){
        
        ret=E_NOT_OK;
    }else{
        /*update address register*/
        EEADRH=(uint8)((badd>>8) & 0x03);
        EEADR=(uint8)(badd & 0xff);
         /*select access data eeprom memory*/
        EECON1bits.EEPGD=access_eeprom_program_memory;//To access eeprom memory not flash 
        EECON1bits.CFGS=access_eeprom_flash_registers;//choosing between flash/eeprom or configuration registers
        /*initiates adata eeprom read*/
        EECON1bits.RD=initiate_data_eeprom_read;
        NOP();
        NOP();//WE GIVE here delay time to separate between operations
        /*return data*/
        *bData=EEDATA;
        ret=E_OK;
    }
    return ret;
}
