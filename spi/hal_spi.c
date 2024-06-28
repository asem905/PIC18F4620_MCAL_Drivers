/* 
 * File:   hal_spi.h
 * Author: Asem
 *
 * Created on April 20, 2024, 9:09 PM
 */
#include "hal_spi.h"
#if mssp_i2c_interrupt_feature_enable ==interrupt_feature_enable
    static void (*spi_defaultinterrupthandler)(void)=NULL;
#endif 
static inline void mssp_spi_interrupt_configuration(const mssp_spi_t *spi_obj);
static void mssp_spi_clock_config(const mssp_spi_t *spi_obj);
static void mssp_spi_master_pin_config();
static void mssp_spi_master_mode_config(const mssp_spi_t *spi_obj);
static void mssp_spi_slave_pin_mode_config(const mssp_spi_t *spi_obj);
static void mssp_spi_sample_config(const mssp_spi_t *spi_obj);
std_returntype mssp_spi_init(const mssp_spi_t *spi_obj){
    std_returntype ret=E_NOT_OK;
    if((NULL== spi_obj)){
        ret=E_NOT_OK;
    }else{ 
        mssp_module_disable_cfg(); 
        if(spi_obj->spi_cfg.spi_mode==spi_mssp_master_mode){//
            mssp_spi_master_mode_config(spi_obj);
            mssp_spi_master_pin_config();
            
        }else if(spi_obj->spi_cfg.spi_mode==spi_mssp_slave_mode){//
            
            mssp_spi_slave_pin_mode_config(spi_obj);
               
        }else{
            
        }

        mssp_spi_sample_config(spi_obj);
        mssp_spi_clock_config(spi_obj);
        #if mssp_spi_interrupt_feature_enable ==interrupt_feature_enable
            mssp_spi_interrupt_configuration(spi_obj);
        #endif
        mssp_module_enable_cfg();       
        ret=E_OK;
    }
    return ret;
}
std_returntype mssp_spi_deinit(const mssp_spi_t *spi_obj){
    std_returntype ret=E_NOT_OK;
    if((NULL== spi_obj)){
        ret=E_NOT_OK;
    }else{
        mssp_module_disable_cfg();
        #if mssp_spi_interrupt_feature_enable ==interrupt_feature_enable
            mssp_spi_interruptdisable();
        #endif
        ret=E_OK;
    }
    return ret;
}

std_returntype mssp_spi_write_blocking(const mssp_spi_t *spi_obj,uint8 spi_data){
    std_returntype ret=E_NOT_OK;
    if((NULL== spi_obj)){
        ret=E_NOT_OK;
    }else{
        SSPCON1bits.WCOL=0;
        SSPBUF=spi_data;
        while(SSPSTATbits.BF==1);
        PIR1bits.SSPIF=0;
        if(spi_obj->spi_cfg.spi_mode==spi_mssp_slave_mode){
            SSPCON1bits.SSPOV=0;
        }
        //mssp_ss_disable();
        ret=E_OK;
    }
    return ret;
}
std_returntype mssp_spi_read_blocking(const mssp_spi_t *spi_obj,uint8 *spi_data){
    std_returntype ret=E_NOT_OK;
    if((NULL== spi_obj)){
        ret=E_NOT_OK;
    }else{
        //mssp_ss_enable();
        /*in recieve mode we check if data is recieved*/
        
        while(SSPSTATbits.BF==1);
        /*then copy data from buffer reg to read it*/
        *spi_data=SSPBUF;
        if(spi_obj->spi_cfg.spi_mode==spi_mssp_slave_mode){
            SSPCON1bits.SSPOV=0;
        }
        //mssp_ss_disable();
        ret=E_OK;
    }
    return ret;
}
static void mssp_spi_clock_config(const mssp_spi_t *spi_obj){
    if(spi_obj->spi_cfg.spi_clock_polarity==spi_mssp_clock_polarity_high_level){
        SSPCON1bits.CKP=1;
    }else if(spi_obj->spi_cfg.spi_clock_polarity==spi_mssp_clock_polarity_low_level){
        SSPCON1bits.CKP=0;
    }else{ }
    if(spi_obj->spi_cfg.spi_clock_edge==spi_mssp_active_idle){
        SSPSTATbits.CKE=1;
    }else if(spi_obj->spi_cfg.spi_clock_edge==spi_mssp_idle_active){
        SSPSTATbits.CKE=0;
    }else{ }
}
static void mssp_spi_master_pin_config(){
    std_returntype ret=E_NOT_OK;
    pin_config SPI_SDO = {.port = portc_index, .pin = pin5, .direction = gpio_Direction_output};
    pin_config SPI_SDI = {.port = portc_index, .pin = pin4, .direction = gpio_Direction_output};
    pin_config SPI_CLK = {.port = portc_index, .pin = pin3, .direction = gpio_Direction_output};
    
    ret = gpio_pin_direction_initilaize(&SPI_SDO); 
    ret = gpio_pin_direction_initilaize(&SPI_SDI);
    ret = gpio_pin_direction_initilaize(&SPI_CLK);
}
static void mssp_spi_slave_pin_mode_config(const mssp_spi_t *spi_obj){
    std_returntype ret=E_NOT_OK;
    SSPCON1bits.SSPOV=0;
    pin_config SPI_SDO = {.port = portc_index, .pin = pin5, .direction = gpio_Direction_output};
    pin_config SPI_SDI = {.port = portc_index, .pin = pin4, .direction = gpio_Direction_output};
    pin_config SPI_CLK = {.port = portc_index, .pin = pin3, .direction = gpio_Direction_output};
    pin_config SPI_SS = {.port = porta_index, .pin = pin5, .direction = gpio_Direction_output};
    ret = gpio_pin_direction_initilaize(&SPI_SDO); 
    ret = gpio_pin_direction_initilaize(&SPI_SDI);
    ret = gpio_pin_direction_initilaize(&SPI_CLK);
    if(spi_slave_mode_ss_enabled==spi_obj->spi_cfg.spi_mode_cfg){
        ret = gpio_pin_direction_initilaize(&SPI_SS);
    }
}
static void mssp_spi_sample_config(const mssp_spi_t *spi_obj){
    if(spi_obj->spi_cfg.spi_sample_phase==spi_mssp_master_sampleatend){
        SSPSTATbits.SMP=1;
    }else if(spi_obj->spi_cfg.spi_sample_phase==spi_mssp_master_sampleatmiddle){
        SSPSTATbits.SMP=0;
    }else if(spi_obj->spi_cfg.spi_sample_phase==spi_mssp_slave_sample){
        SSPSTATbits.SMP=spi_mssp_slave_sample;
    }else{
            
    }
}
static void mssp_spi_master_mode_config(const mssp_spi_t *spi_obj){
    if(spi_master_mode_div_by64==spi_obj->spi_cfg.spi_mode_cfg){
        SSPCON1bits.SSPM=spi_master_mode_div_by64;
    }else if(spi_master_mode_div_by4==spi_obj->spi_cfg.spi_mode_cfg){
        SSPCON1bits.SSPM=spi_master_mode_div_by4;
    }else if(spi_master_mode_div_by16==spi_obj->spi_cfg.spi_mode_cfg){
        SSPCON1bits.SSPM=spi_master_mode_div_by16;
    }else if(spi_master_mode_tmr2==spi_obj->spi_cfg.spi_mode_cfg){
        SSPCON1bits.SSPM=spi_master_mode_tmr2;
    }else{
                
    }
}
static inline void mssp_spi_interrupt_configuration(const mssp_spi_t *spi_obj){
    #if mssp_spi_interrupt_feature_enable ==interrupt_feature_enable
        mssp_spi_interruptenable(); 
        mssp_spi_interruptflagclear();
        spi_defaultinterrupthandler=spi_obj->spi_defaultinterrupthandler_func;
        #if interrupt_priority_levels_enable == interrupt_feature_enable
        interrupt_prioritylevelsenable();
        if(interrupt_high_priority==spi_obj->mssp_spi_priority){
            mssp_spi_highpriorityset();
            interrupt_globalinterrupthighenable();//in interrupt module if we didnot enable our priority interrupt we need to enable gieh but if we enabled priority we need to enable gieh too 
        }else if(interrupt_low_priority==spi_obj->mssp_spi_priority){
            mssp_spi_lowpriorityset();
            interrupt_globalinterruptlowenable();
        }else{/*nothing*/}
        #else
        interrupt_globalinterruptenable();//these happens if priority is not enabled 
        interrupt_peripheralinterruptenable();
        #endif
    #endif
}
void mssp_spi_ISR(void){
    mssp_spi_interruptflagclear();
    if(spi_defaultinterrupthandler){
        spi_defaultinterrupthandler();
    }
}