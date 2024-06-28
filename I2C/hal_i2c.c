/* 
 * File:   hal_i2c.c
 * Author: Asem
 *
 * Created on April 13, 2024, 2:36 PM
 */
#include "hal_i2c.h"
#if mssp_i2c_interrupt_feature_enable ==interrupt_feature_enable
    static void (*i2c_report_write_collision_interrupthandler)(void)=NULL;
    static void (*i2c_defaultinterrupthandler)(void)=NULL;
    static void (*i2c_report_recieve_overflow_interrupthandler)(void)=NULL;
#endif 
static inline void mssp_i2c_interrupt_configuration(const mssp_i2c_t *i2c_obj);
static inline void mssp_i2c_mode_gpio_cfg(void);
static inline void i2c_master_mode_clock_cfg(const mssp_i2c_t *i2c_obj);
static inline void i2c_slave_mode_cfg(const mssp_i2c_t *i2c_obj);
std_returntype mssp_i2c_init(const mssp_i2c_t *i2c_obj){
    std_returntype ret=E_OK;
    if((NULL== i2c_obj)){
        ret=E_NOT_OK;
    }else{
        mssp_module_disable_cfg();
        /*mode select of i2c */
        if(i2c_mssp_master_mode==i2c_obj->i2c_cfg.i2c_mode){
            i2c_master_mode_clock_cfg(i2c_obj);
        }else if(i2c_mssp_slave_mode==i2c_obj->i2c_cfg.i2c_mode){
            if(i2c_general_call_enable==i2c_obj->i2c_cfg.i2c_general_call){
                i2c_general_call_enable_cfg();
            }else if(i2c_general_call_disable==i2c_obj->i2c_cfg.i2c_general_call){
                i2c_general_call_disable_cfg();
            }else{/*nothing*/}
            /*explained in notebook 7 in coding part of i2c*/
            SSPCON1bits.WCOL=0;//write collission as slave transmitter
            SSPCON1bits.SSPOV=0;//over flow condn.
            SSPCON1bits.CKP=1;//means that you release clock
            /*assign slave address by sspadd that used in clock generation in master*/
            SSPADD=i2c_obj->i2c_cfg.i2c_slave_address;
            /*i2c slave configuration */
            i2c_slave_mode_cfg(i2c_obj);
        }else{
            
        }
        mssp_i2c_mode_gpio_cfg();//it is general for master or slave as it is said by data sheet
        /*slew rate control:*/
        if(i2c_slew_rate_enable==i2c_obj->i2c_cfg.i2c_slew_rate){
            i2c_slew_rate_enable_cfg();
        }else if(i2c_slew_rate_disable==i2c_obj->i2c_cfg.i2c_slew_rate){
            i2c_slew_rate_disable_cfg();
        }else{
                
        }
        /*smBUS control:*/
        if(i2c_smbus_enable==i2c_obj->i2c_cfg.i2c_smbus_control){
            i2c_smbus_enable_cfg();
        }else if(i2c_smbus_disable==i2c_obj->i2c_cfg.i2c_smbus_control){
            i2c_smbus_disable_cfg();
        }else{
                
        }
        /*interrupt feature*/
        #if mssp_i2c_interrupt_feature_enable ==interrupt_feature_enable
            mssp_i2c_interrupt_configuration(i2c_obj);
        #endif

        
        mssp_module_enable_cfg();
    }
    return ret;
}
std_returntype mssp_i2c_deinit(const mssp_i2c_t *i2c_obj){
    std_returntype ret=E_OK;
    if((NULL== i2c_obj)){
        ret=E_NOT_OK;
    }else{
        mssp_module_disable_cfg();
        #if mssp_i2c_interrupt_feature_enable ==interrupt_feature_enable
            mssp_i2c_interruptdisable();
            mssp_i2c_bus_collission_interruptdisable();
        #endif
    }
    return ret;
}
std_returntype mssp_i2c_master_send_start(const mssp_i2c_t *i2c_obj){
    std_returntype ret=E_OK;
    if((NULL== i2c_obj)){
        ret=E_NOT_OK;
    }else{
        SSPCON2bits.SEN=1;
        while(SSPCON2bits.SEN);//done by hardware it self
        PIR1bits.SSPIF=0;
        if(i2c_start_bit_detected==SSPSTATbits.S){
            ret=E_OK;
        }else{
            ret=E_NOT_OK;
        }
    }
    return ret;
}
std_returntype mssp_i2c_master_send_repeated_start(const mssp_i2c_t *i2c_obj){
    std_returntype ret=E_NOT_OK;
    if((NULL== i2c_obj)){
        ret=E_NOT_OK;
    }else{
        SSPCON2bits.RSEN=1;
        while(SSPCON2bits.RSEN);//done by hardware it self
        PIR1bits.SSPIF=0;
        ret=E_OK;
    }
    return ret;
}
std_returntype mssp_i2c_master_send_stop(const mssp_i2c_t *i2c_obj){
    std_returntype ret=E_OK;
    if((NULL== i2c_obj)){
        ret=E_NOT_OK;
    }else{
        SSPCON2bits.PEN=1;
        while(SSPCON2bits.PEN);//done by hardware it self
        PIR1bits.SSPIF=0;
        if(i2c_stop_bit_detected==SSPSTATbits.P){
            ret=E_OK;
        }else{
            ret=E_NOT_OK;
        }
    }
    return ret;
}
std_returntype mssp_i2c_master_write_blocking(const mssp_i2c_t *i2c_obj,uint8 i2c_data,uint8 *_ack){
    std_returntype ret=E_NOT_OK;
    if((NULL== i2c_obj)||(_ack==NULL)){
        ret=E_NOT_OK;
    }else{
        /*write data in sspbuf*/
        SSPBUF=i2c_data;
        while(!PIR1bits.SSPIF);//transmission of data is ok when sspif=1 so wait till that happen
        PIR1bits.SSPIF=0;
        if(i2c_ack_recieved_from_slave==SSPCON2bits.ACKSTAT){
            *_ack=i2c_ack_recieved_from_slave;
        }else{
            *_ack=i2c_ack_not_recieved_from_slave;
        }
        ret=E_OK;
    }
    return ret;
}
std_returntype mssp_i2c_master_read_blocking(const mssp_i2c_t *i2c_obj,uint8 ack,uint8 *i2c_data){
    std_returntype ret=E_OK;
    if((NULL== i2c_obj)|| (NULL==i2c_data)){
        ret=E_NOT_OK;
    }else{
        i2c_master_recieve_enable_cfg();
        /*in recieve mode we check if data is recieved*/
        while(!SSPSTATbits.BF);
        /*then copy data from buffer reg to read it*/
        *i2c_data=SSPBUF;
        /*send ack or not ack*/
        if(i2c_master_send_ack==ack){
            SSPCON2bits.ACKDT=0;
            SSPCON2bits.ACKEN=1;//to initiate ack sequence
        }else if(i2c_master_send_not_ack==ack){
            SSPCON2bits.ACKDT=1;
            SSPCON2bits.ACKEN=1;//to initiate ack sequence 
        }
        
    }
    return ret;
}

void mssp_i2c_ISR(void){//this interrupt happens when overflow happens
#if mssp_i2c_interrupt_feature_enable ==interrupt_feature_enable
    mssp_i2c_interruptflagclear();
    if(i2c_defaultinterrupthandler){
        i2c_defaultinterrupthandler();
    }
#endif 
}
void mssp_i2c_bc_ISR(void){//for bus collision
#if mssp_i2c_interrupt_feature_enable ==interrupt_feature_enable
    mssp_i2c_bus_collission_interruptflagclear();
    if(i2c_report_write_collision_interrupthandler){
        i2c_report_write_collision_interrupthandler();
    }
#endif 
}


static inline void mssp_i2c_mode_gpio_cfg(void){
    /*PIN3,PIN4 AS inputs as he told me in data sheet*/
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
}

static inline void i2c_master_mode_clock_cfg(const mssp_i2c_t *i2c_obj){
    SSPCON1bits.SSPM=i2c_obj->i2c_cfg.i2c_mode_cfg;
    SSPADD=(uint8)(((_XTAL_FREQ/4)/i2c_obj->i2c_clock)-1);//of crystal 8mega
}
static inline void i2c_slave_mode_cfg(const mssp_i2c_t *i2c_obj){
    SSPCON1bits.SSPM=i2c_obj->i2c_cfg.i2c_mode_cfg;
}



static inline void mssp_i2c_interrupt_configuration(const mssp_i2c_t *i2c_obj){
    #if mssp_i2c_interrupt_feature_enable ==interrupt_feature_enable
        mssp_i2c_interruptenable();
        mssp_i2c_bus_collission_interruptdenable();
        mssp_i2c_interruptflagclear();
        mssp_i2c_bus_collission_interruptflagclear();
        i2c_report_write_collision_interrupthandler=i2c_obj->i2c_report_write_collision;
        i2c_defaultinterrupthandler=i2c_obj->i2c_defaultinterrupthandler;
        i2c_report_recieve_overflow_interrupthandler=i2c_obj->i2c_report_recieve_overflow;
        #if interrupt_priority_levels_enable == interrupt_feature_enable
        interrupt_prioritylevelsenable();
        if(interrupt_high_priority==i2c_obj->i2c_cfg.mssp_i2c_priority){
            mssp_i2c_highpriorityset();
            interrupt_globalinterrupthighenable();//in interrupt module if we didnot enable our priority interrupt we need to enable gieh but if we enabled priority we need to enable gieh too 
        }else if(interrupt_low_priority==i2c_obj->i2c_cfg.mssp_i2c_priority){
            mssp_i2c_lowpriorityset();
            interrupt_globalinterruptlowenable();
        }else{/*nothing*/}
            
        if(interrupt_high_priority==i2c_obj->i2c_cfg.mssp_i2c_priority){
            mssp_i2c_bus_collission_highpriorityset()
            interrupt_globalinterrupthighenable();//in interrupt module if we didnot enable our priority interrupt we need to enable gieh but if we enabled priority we need to enable gieh too 
        }else if(interrupt_low_priority==i2c_obj->i2c_cfg.mssp_i2c_priority){
            mssp_i2c_bus_collission_lowpriorityset();
            interrupt_globalinterruptlowenable();
        }else{/*nothing*/}
        #else
        interrupt_globalinterruptenable();//these happens if priority is not enabled 
        interrupt_peripheralinterruptenable();
        #endif
    #endif
}