/* 
 * File:   hal_usart.c
 * Author: Asem
 *
 * Created on April 6, 2024, 2:35 PM
 */
#include "hal_usart.h"
#if eusart_tx_interrupt_feature_enable ==interrupt_feature_enable
    static void (*eusart_tx_interrupthandler)(void)=NULL;
#endif
#if eusart_rx_interrupt_feature_enable ==interrupt_feature_enable
    static void (*eusart_rx_interrupthandler)(void)=NULL;
    static void (*eusart_framingerrorhandler)(void)=NULL;
    static void (*eusart_overrunerrorhandler)(void)=NULL;
#endif

static void eusart_baud_rate_calculation(const usart_t *_eusart);
static void eusart_async_tx_init(const usart_t *_eusart);
static void eusart_async_rx_init(const usart_t *_eusart);
std_returntype eusart_async_init(const usart_t *_eusart){
    std_returntype ret=E_OK;
    if((NULL==_eusart)){
        ret=E_NOT_OK;
    }else{
        RCSTAbits.SPEN=eusart_module_disable;
        TRISCbits.RC7=1;
        TRISCbits.RC6=1;//to configure them as intput then it is automatically configured as out if transmitting by usart itself
        eusart_baud_rate_calculation(_eusart);
        eusart_async_tx_init(_eusart);
        eusart_async_rx_init(_eusart);
        RCSTAbits.SPEN=eusart_module_enable;
    }
    return ret;
}
std_returntype eusart_async_deinit(const usart_t *_eusart){
    std_returntype ret=E_OK;
    if((NULL==_eusart)){
        ret=E_NOT_OK;
    }else{
        RCSTAbits.SPEN=eusart_module_disable; 
    }
    return ret;
}
std_returntype eusart_async_readbyteblocking(uint8 *_data){
    std_returntype ret=E_OK;
    while(!PIR1bits.RCIF);//as long as it is empty block
    *_data=RCREG;
    return ret;
}
std_returntype eusart_async_readbytenonblocking(uint8 *_data){//no while used at all which is much better
    std_returntype ret=E_OK;
    if(1==PIR1bits.RCIF){
        *_data=RCREG;//after reading it will be cleared automatically
    }else{
        ret=E_NOT_OK;
    }
    
    return ret;
}
std_returntype eusart_async_writebyteblocking(uint8 _data){
    std_returntype ret=E_OK;
    /*we put blocking word because we need to check if tsr is empty first or not and block any thing wriiten again before that*/
    while(!TXSTAbits.TRMT);//when it is empty
    #if eusart_tx_interrupt_feature_enable ==interrupt_feature_enable
    PIE1bits.TXIE=1;//we did that to make sure that interrupt happens only when TRMT IS EMPTY NOT BEFORE as many words can be transmitted many times before even sending one word 
    #endif
    TXREG=_data;  
    return ret;
}
std_returntype eusart_async_writestringblocking(uint8 *_data,uint16 str_len){
    std_returntype ret=E_OK;
    uint8 char_counter=0;
    for(char_counter=0;char_counter<str_len;char_counter++){
        ret=eusart_async_writebyteblocking(_data[char_counter]);
    }
    return ret;
}
std_returntype eusart_async_writebytenonblocking(uint8 _data){
    std_returntype ret=E_OK;
    if(1==TXSTAbits.TRMT){
        TXREG=_data;//after reading it will be cleared automatically
        #if eusart_tx_interrupt_feature_enable ==interrupt_feature_enable
        PIE1bits.TXIE=1;//we did that to make sure that interrupt happens only when TRMT IS EMPTY NOT BEFORE as many words can be transmitted many times before even sending one word 
        #endif
    }else{
        ret=E_NOT_OK;
    }
    
    return ret;
}
std_returntype eusart_async_writestringnonblocking(uint8 *_data,uint16 str_len){
    std_returntype ret=E_OK;
    uint8 char_counter=0;
    if(1==PIR1bits.TXIF){
        for(char_counter=0;char_counter<str_len;char_counter++){
            ret=eusart_async_writebytenonblocking(_data[char_counter]);
        }
    }else{
        ret=E_NOT_OK;
    }
    
    return ret;
}





static void eusart_baud_rate_calculation(const usart_t *_eusart){
    float baud_rate_temp=0;
    switch(_eusart->baudrate_gen_config){
        case baudrate_asyn_8bit_low_speed:
            TXSTAbits.SYNC=eusart_asynchronous_mode;
            TXSTAbits.BRGH=eusart_asynchronous_low_speed;
            BAUDCONbits.BRG16=eusart_08bit_baudrate_gen;
            baud_rate_temp=((_XTAL_FREQ/(float)_eusart->baud_rate)/64)-1;
            break;
        case baudrate_asyn_8bit_high_speed:
            TXSTAbits.SYNC=eusart_asynchronous_mode;
            TXSTAbits.BRGH=eusart_asynchronous_high_speed;
            BAUDCONbits.BRG16=eusart_08bit_baudrate_gen;
            baud_rate_temp=((_XTAL_FREQ/(float)_eusart->baud_rate)/16)-1;
            break;
        case baudrate_asyn_16bit_low_speed:
            TXSTAbits.SYNC=eusart_asynchronous_mode;
            TXSTAbits.BRGH=eusart_asynchronous_low_speed;
            BAUDCONbits.BRG16=eusart_16bit_baudrate_gen;
            baud_rate_temp=((_XTAL_FREQ/(float)_eusart->baud_rate)/16)-1;
            break;
        case baudrate_asyn_16bit_high_speed:
            TXSTAbits.SYNC=eusart_asynchronous_mode;
            TXSTAbits.BRGH=eusart_asynchronous_high_speed;
            BAUDCONbits.BRG16=eusart_16bit_baudrate_gen;
            baud_rate_temp=((_XTAL_FREQ/(float)_eusart->baud_rate)/4)-1;
            break;
        case baudrate_syn_8bit:
            TXSTAbits.SYNC=eusart_synchronous_mode;
            BAUDCONbits.BRG16=eusart_08bit_baudrate_gen;
            baud_rate_temp=((_XTAL_FREQ/(float)_eusart->baud_rate)/4)-1;
            break;
        case baudrate_syn_16bit:
            TXSTAbits.SYNC=eusart_asynchronous_mode;
            BAUDCONbits.BRG16=eusart_16bit_baudrate_gen;
            baud_rate_temp=((_XTAL_FREQ/(float)_eusart->baud_rate)/4)-1;
            break;
        default: ;    
        
    }
    /*the only important bits are first two bytes*/
    SPBRG=(uint8)((uint32)baud_rate_temp);//to take only first 8bits
    SPBRGH=(uint8)(((uint32)baud_rate_temp)>>8);//to take second 8bits
}
static void eusart_async_tx_init(const usart_t *_eusart){
    if(eusart_asynchronous_tx_enable==_eusart->usart_tx_cfg.usart_tx_enable){
        TXSTAbits.TXEN=eusart_asynchronous_tx_enable;
        eusart_tx_interrupthandler=(_eusart->eusart_txdefaultinterrupthandler);
        if(eusart_asynchronous_interrupt_tx_enable==_eusart->usart_tx_cfg.usart_tx_interrupt_enable){
            PIE1bits.TXIE=eusart_asynchronous_interrupt_tx_enable;
            #if eusart_tx_interrupt_feature_enable ==interrupt_feature_enable
            eusart_tx_interruptenable();
            //eusart_tx_interruptflagclear();//automatically cleared
            
            #if interrupt_priority_levels_enable == interrupt_feature_enable
            interrupt_prioritylevelsenable();
            if(interrupt_high_priority==_eusart->usart_tx_cfg.usart_tx_priority){
                eusart_tx_highpriorityset();
                interrupt_globalinterrupthighenable();//in interrupt module if we didnot enable our priority interrupt we need to enable gieh but if we enabled priority we need to enable gieh too 
            }else if(interrupt_low_priority==_eusart->usart_tx_cfg.usart_tx_priority){
                eusart_tx_lowpriorityset();
                interrupt_globalinterruptlowenable();
            }
            #else
            interrupt_globalinterruptenable();//these happens if priority is not enabled 
            interrupt_peripheralinterruptenable();
            #endif
            #endif
            
        }else if(eusart_asynchronous_interrupt_tx_disable==_eusart->usart_tx_cfg.usart_tx_interrupt_enable){
            PIE1bits.TXIE=eusart_asynchronous_interrupt_tx_disable;
        }else{/*nothing*/}

        /*eusart 9bit transmit configuration*/
        if(eusart_asynchronous_9bit_tx_enable==_eusart->usart_tx_cfg.usart_tx_9bit_enable){
            TXSTAbits.TX9=eusart_asynchronous_9bit_tx_enable;
        }else if(eusart_asynchronous_9bit_tx_disable==_eusart->usart_tx_cfg.usart_tx_9bit_enable){
            TXSTAbits.TX9=eusart_asynchronous_9bit_tx_disable;
        }else{/*nothing*/}
    }else{
        
    }
    
   
}
static void eusart_async_rx_init(const usart_t *_eusart){
    if(eusart_asynchronous_rx_enable==_eusart->usart_rx_cfg.usart_rx_enable){
        RCSTAbits.CREN=eusart_asynchronous_rx_enable;
        eusart_rx_interrupthandler=(_eusart->eusart_rxdefaultinterrupthandler);
        eusart_framingerrorhandler=_eusart->eusart_framingerrorhandler;
        eusart_overrunerrorhandler=_eusart->eusart_overrunerrorhandler;
        if(eusart_asynchronous_interrupt_rx_enable==_eusart->usart_rx_cfg.usart_rx_interrupt_enable){
            PIE1bits.RCIE=eusart_asynchronous_interrupt_rx_enable;
            #if eusart_rx_interrupt_feature_enable ==interrupt_feature_enable
            eusart_rx_interruptenable();
            //eusart_rx_interruptflagclear();//automatically cleared
            #if interrupt_priority_levels_enable == interrupt_feature_enable
            interrupt_prioritylevelsenable();
            if(interrupt_high_priority==_eusart->usart_rx_cfg.usart_rx_priority){
                eusart_rx_highpriorityset();
                interrupt_globalinterrupthighenable();//in interrupt module if we didnot enable our priority interrupt we need to enable gieh but if we enabled priority we need to enable gieh too 
            }else if(interrupt_low_priority==_eusart->usart_rx_cfg.usart_rx_priority){
                eusart_rx_lowpriorityset();
                interrupt_globalinterruptlowenable();
            }
            #else
            interrupt_globalinterruptenable();//these happens if priority is not enabled 
            interrupt_peripheralinterruptenable();
            #endif
            #endif
        }else if(eusart_asynchronous_interrupt_rx_disable==_eusart->usart_rx_cfg.usart_rx_interrupt_enable){
            PIE1bits.RCIE=eusart_asynchronous_interrupt_rx_disable;
        }else{/*nothing*/}

        /*eusart 9bit reciever configuration*/
        if(eusart_asynchronous_9bit_rx_enable==_eusart->usart_rx_cfg.usart_rx_9bit_enable){
            RCSTAbits.RX9=eusart_asynchronous_9bit_rx_enable;
        }else if(eusart_asynchronous_9bit_rx_disable==_eusart->usart_rx_cfg.usart_rx_9bit_enable){
            RCSTAbits.RX9=eusart_asynchronous_9bit_rx_disable;
        }else{/*nothing*/}
    }else{
        
    }
}
void eusrat_tx_ISR(void){
    PIE1bits.TXIE=0;
    if(eusart_tx_interrupthandler){
        eusart_tx_interrupthandler();
    }else{
        
    }
}
void eusrat_rx_ISR(void){
    if(eusart_rx_interrupthandler){
        eusart_rx_interrupthandler();
    }else{
        
    }
    if(eusart_framingerrorhandler){
        eusart_framingerrorhandler();
    }else{
        
    }
    if(eusart_overrunerrorhandler){
        eusart_overrunerrorhandler();
    }else{
        
    }
}
std_returntype eusart_async_rx_restart(void){
    std_returntype ret=E_OK;
    RCSTAbits.CREN=0;
    RCSTAbits.CREN=1;
    return ret;
}