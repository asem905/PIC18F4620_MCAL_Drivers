/* 
 * File:   app.c
 * Author: mm
 *
 * Created on November 27, 2023, 3:23 AM
 */
#include "hal_gpio.h"

volatile uint8 *tris_registers[]={&TRISA,&TRISB,&TRISC,&TRISD,&TRISE};
volatile uint8 *lat_registers[]={&LATA,&LATB,&LATC,&LATD,&LATE};
volatile uint8 *port_registers[]={&PORTA,&PORTB,&PORTC,&PORTD,&PORTE};
#if (port_pin_configurations==config_enable)
/**
 * @brief initialize specific direction of pin
 * @param pin_config_pointer to configuration
 * @return status of function
 *          E_OK:if done successfully
 *          E_NOT_OK:if fails
 */
std_returntype gpio_pin_direction_initilaize (const pin_config *pin_config_){
    std_returntype ret=E_OK;
    if((NULL==pin_config_ )|| ((pin_config_->pin) >(port_pin_max_numbers-1))){
        ret=E_NOT_OK;
    }else{
        switch(pin_config_->direction){
            case gpio_Direction_output:
                clearbit(*tris_registers[pin_config_->port],pin_config_->pin);//because macro function takes actual value so derefernce address
                break;
            case gpio_Direction_input:
                setbit(*tris_registers[pin_config_->port],pin_config_->pin);//because macro function takes actual value
                break;
            defaut: ret=E_NOT_OK  ; 
        }
    }
    return ret;
}
#endif

#if (port_pin_configurations==config_enable)
// to make us know what on this pin from tris registers of it:
/**
 * 
 * @param pin_config_
 * @param directionstatus
 * @return 
 */
std_returntype gpio_pin_direction_status (const pin_config *pin_config_,direction *directionstatus){
    std_returntype ret=E_OK;
    if(NULL==pin_config_ || NULL==directionstatus || ((pin_config_->pin) >(port_pin_max_numbers-1))){
        ret=E_NOT_OK;
    }else{
        *directionstatus=readbit(*tris_registers[pin_config_->port],pin_config_->pin);
    }
    return ret;
}
#endif

#if (port_pin_configurations==config_enable)
/**
 * 
 * @param pin_config_
 * @param logic
 * @return 
 */
std_returntype gpio_pin_write_logic (const pin_config *pin_config_,_logic logic){
    std_returntype ret=E_OK;
    if(NULL==pin_config_ || ((pin_config_->pin) >(port_pin_max_numbers-1))){
        ret=E_NOT_OK;
    }else{
        switch(logic){
            case low:
                clearbit(*lat_registers[pin_config_->port],pin_config_->pin);
                break;
            case high:
                setbit(*lat_registers[pin_config_->port],pin_config_->pin);
                break;
            defaut: ret=E_NOT_OK  ;     
        }
    }
    return ret;
}
#endif

#if (port_pin_configurations==config_enable)
/**
 * 
 * @param pin_config_
 * @param logic
 * @return 
 */
std_returntype gpio_pin_read_logic (const pin_config *pin_config_,_logic *logic){
    std_returntype ret=E_OK;
    if(NULL==pin_config_ || NULL==logic || ((pin_config_->pin) >(port_pin_max_numbers-1))){
        ret=E_NOT_OK;
    }else{
        *logic=readbit(*port_registers[pin_config_->port],pin_config_->pin);
    }
    return ret;
}
#endif

#if (port_pin_configurations==config_enable)
/**
 * 
 * @param pin_config_
 * @return 
 */
std_returntype gpio_pin_toggle_logic (const pin_config *pin_config_){
    std_returntype ret=E_OK;
    if(NULL==pin_config_ || ((pin_config_->pin) >(port_pin_max_numbers-1))){
        ret=E_NOT_OK;
    }else{
        togglebit(*lat_registers[pin_config_->port],pin_config_->pin);
    }
    return ret;
}
#endif
#if (port_pin_configurations==config_enable)
/**
 * 
 * @param pin_config_
 * @return 
 */
std_returntype gpio_pin_initilaize (const pin_config *pin_config_){
    std_returntype ret=E_OK;
    if(NULL==pin_config_ || ((pin_config_->pin) >(port_pin_max_numbers-1))){
        ret=E_NOT_OK;
    }else{
        ret=gpio_pin_direction_initilaize(pin_config_);
        ret=gpio_pin_write_logic(pin_config_,pin_config_->logic);
    }
    return ret;
}
#endif

#if (gpio_port_configurations==config_enable)
/**
 * 
 * @param port
 * @param direction
 * @return 
 */
std_returntype gpio_port_direction_initilaize (port_index port,uint8 direction){
    std_returntype ret=E_OK;
    if(port > port_max_numbers-1){
        ret=E_NOT_OK;
    }else{
        *tris_registers[port]=direction;
    }
    return ret;
}
#endif

#if (gpio_port_configurations==config_enable)
/**
 * 
 * @param port
 * @param directionstatus
 * @return 
 */
std_returntype gpio_port_get_direction_status (port_index port,uint8 *directionstatus){
    std_returntype ret=E_OK;
     if(NULL==directionstatus || (port > port_max_numbers-1)){
        ret=E_NOT_OK;
    }else{
         *directionstatus=*tris_registers[port];
    }
    return ret;
}
#endif

#if (gpio_port_configurations==config_enable)
/**
 * 
 * @param port
 * @param logicc
 * @return 
 */
std_returntype gpio_port_write_logic (port_index port,uint8 logicc){
    std_returntype ret=E_OK;
     if((port > port_max_numbers-1)){
        ret=E_NOT_OK;
    }else{
         *lat_registers[port]=logicc;
    }
    return ret;
}
#endif

#if (gpio_port_configurations==config_enable)
/**
 * 
 * @param port
 * @param logicc
 * @return 
 */
std_returntype gpio_port_read_logic (port_index port,uint8 *logicc){
    std_returntype ret=E_OK;
     if(NULL==logicc){
        ret=E_NOT_OK;
    }else{
         *logicc=*lat_registers[port];
    }
    return ret;
}
#endif

#if (gpio_port_configurations==config_enable)
/**
 * 
 * @param port
 * @return 
 */
std_returntype gpio_port_toggle_logic (port_index port){
    std_returntype ret=E_OK;
    if((port > port_max_numbers-1)){
        ret=E_NOT_OK;
    }else{
         *lat_registers[port] ^= portc_mask;
    }
    return ret;
}
#endif