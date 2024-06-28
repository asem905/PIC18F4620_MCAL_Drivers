/* 
 * File:   mcal_external_interrupt.c
 * Author: Asem
 *
 * Created on February 8, 2024, 11:20 PM
 */
#include "mcal_external_interrupt.h"
//static void (*INT0_interrupthandler)(void)=NULL;//what was before doing type interrupthandler
static interrupthandler INT0_interrupthandler=NULL;// will contain address of ISR_app
static interrupthandler INT1_interrupthandler=NULL;
static interrupthandler INT2_interrupthandler=NULL;

static interrupthandler RB4_interrupthandler_high=NULL;// will contain address of RB_app
static interrupthandler RB4_interrupthandler_low=NULL;
static interrupthandler RB5_interrupthandler_high=NULL;// will contain address of RB_app
static interrupthandler RB5_interrupthandler_low=NULL;
static interrupthandler RB6_interrupthandler_high=NULL;// will contain address of RB_app
static interrupthandler RB6_interrupthandler_low=NULL;
static interrupthandler RB7_interrupthandler_high=NULL;// will contain address of RB_app
static interrupthandler RB7_interrupthandler_low=NULL;
// we choose one of previous according to which pin is pressed


static std_returntype interrupt_INTx_enable (const interrupt_INTx_t *int_obj);
static std_returntype interrupt_INTx_disable (const interrupt_INTx_t *int_obj);
static std_returntype interrupt_INTx_priority_init (const interrupt_INTx_t *int_obj);
static std_returntype interrupt_INTx_edge_init (const interrupt_INTx_t *int_obj);
static std_returntype interrupt_INTx_pin_init (const interrupt_INTx_t *int_obj);
static std_returntype interrupt_INTx_clear_flag (const interrupt_INTx_t *int_obj);

//in these functions we will put function pointers we get in above function pointers: INT0_interrupthandler,...
static std_returntype INT0_setinterrupthandler(void (*interrupthandler)(void));
static std_returntype INT1_setinterrupthandler(void (*interrupthandler)(void));
static std_returntype INT2_setinterrupthandler(void (*interrupthandler)(void));
static std_returntype interrupt_INTx_setinterrupthandler(const interrupt_INTx_t *int_obj);//this func calls either one of above 3 func.s based on what she has

static std_returntype interrupt_RBx_enable (const interrupt_RBx_t *int_obj);
static std_returntype interrupt_RBx_disable (const interrupt_RBx_t *int_obj);
static std_returntype interrupt_RBx_priority_init (const interrupt_RBx_t *int_obj);
static std_returntype interrupt_RBx_pin_init (const interrupt_RBx_t *int_obj);

/**
 * 
 * @param int_obj
 * @return 
 */
std_returntype interrupt_INTx_init(const interrupt_INTx_t *int_obj){
    std_returntype ret=E_OK;//E_OK represents number zero so to make sure of all functions down we do or operator :
    if(NULL==int_obj){
        ret=E_NOT_OK;
    }else{
        //disable firstto make sure that it was not working at first
        ret |=interrupt_INTx_disable(int_obj);
        //clear flag bit
        ret |=interrupt_INTx_clear_flag(int_obj);
        //determine edge
        ret |=interrupt_INTx_edge_init(int_obj);
//        #if interrupt_priority_levels_enable == interrupt_feature_enable
//        //priority
//        ret=interrupt_INTx_priority_init(int_obj);//it doesnot matter any more because we check priority in enable function
//        #endif
        //pin
        ret |=interrupt_INTx_pin_init(int_obj);
        //call back:
        ret |=interrupt_INTx_setinterrupthandler(int_obj);
        //enable interrupt
        ret |=interrupt_INTx_enable(int_obj);
    }
    return ret;
}

void INT0_ISR(){
    //clear flag to not come back to it again
    ext_INT0_interruptflagclear();
    //code:
    //callback function:
    if(INT0_interrupthandler){
        INT0_interrupthandler();//then here we return to function ISR_APP of main app.c 
    }
}
void INT1_ISR(){
    //clear flag
    ext_INT1_interruptflagclear();
    //code:
    //callback function:
    if(INT1_interrupthandler){
        INT1_interrupthandler();
    }
}
void INT2_ISR(){
    //clear flag
    ext_INT2_interruptflagclear();
    //code:
    //callback function:
    if(INT2_interrupthandler){
        INT2_interrupthandler();
    }
}
void RB4_ISR(uint8 RB4_source){
    ext_RBx_interruptflagclear();
    
    if(RB4_source==0){
        RB4_interrupthandler_high();
        if(RB4_interrupthandler_high){
            RB4_interrupthandler_high();
        }
    }else if(RB4_source==1){
        RB4_interrupthandler_low();
        if(RB4_interrupthandler_low){
            RB4_interrupthandler_low();
        }
    }else{
        /*nothing*/
    }
}
void RB5_ISR(uint8 RB5_source){
    ext_RBx_interruptflagclear();
    
    if(RB5_source==0){
        RB5_interrupthandler_high();
        if(RB5_interrupthandler_high){
            RB5_interrupthandler_high();
        }
    }else if(RB5_source==1){
        RB5_interrupthandler_low();
        if(RB5_interrupthandler_low){
            RB5_interrupthandler_low();
        }
    }else{
        /*nothing*/
    }
}

void RB6_ISR(uint8 RB6_source){
    ext_RBx_interruptflagclear();
    
    if(RB6_source==0){
        RB6_interrupthandler_high();
        if(RB6_interrupthandler_high){
            RB6_interrupthandler_high();
        }
    }else if(RB6_source==1){
        RB6_interrupthandler_low();
        if(RB6_interrupthandler_low){
            RB6_interrupthandler_low();
        }
    }else{
        /*nothing*/
    }
}

void RB7_ISR(uint8 RB7_source){
    ext_RBx_interruptflagclear();
    
    if(RB7_source==0){
        RB7_interrupthandler_high();
        if(RB7_interrupthandler_high){
            RB7_interrupthandler_high();
        }
    }else if(RB7_source==1){
        RB7_interrupthandler_low();
        if(RB7_interrupthandler_low){
            RB7_interrupthandler_low();
        }
    }else{
        /*nothing*/
    }
}
/**
 * 
 * @param int_obj
 * @return 
 */
std_returntype interrupt_INTx_deinit(const interrupt_INTx_t *int_obj){
    std_returntype ret=E_OK;
    if(NULL==int_obj){
        ret=E_NOT_OK;
    }else{
        ret=interrupt_INTx_disable(int_obj);
    }
    return ret;
}
/**
 * 
 * @param int_obj
 * @return 
 */
std_returntype interrupt_RBx_init(const interrupt_RBx_t *int_obj){
    std_returntype ret=E_OK;
    if(NULL==int_obj){
        ret=E_NOT_OK;
    }else{
        //same as INTX
        ext_RBx_interruptdisable();
        
        ext_RBx_interruptflagclear();
        
        #if interrupt_priority_levels_enable != interrupt_feature_enable
        interrupt_peripheralinterruptenable();
        interrupt_globalinterruptenable();// these keys to make interrupt happens
        #else
        interrupt_prioritylevelsenable();
        if(interrupt_low_priority==int_obj->priority){
            //this line to enable all low priority:
            interrupt_globalinterruptlowenable();
            //this line to enable RBx low priority
            ext_RBx_priority_low();
        }else if(interrupt_high_priority==int_obj->priority){
            //this line to enable all high priority:
            interrupt_globalinterrupthighenable();
            //this line to enable RBx high priority
            ext_RBx_priority_high();
        }else{
                    
        }
        #endif
        //configure pins as inputs:
        ret=gpio_pin_direction_initilaize(&(int_obj->mcu_pin));
        //initialize call back function:
        switch(int_obj->mcu_pin.pin){
            case pin4:
                RB4_interrupthandler_high=int_obj->ext_interrupthandler_high;
                RB4_interrupthandler_low=int_obj->ext_interrupthandler_low;
                break;
            case pin5:
                RB5_interrupthandler_high=int_obj->ext_interrupthandler_high;
                RB5_interrupthandler_low=int_obj->ext_interrupthandler_low;
                break;
            case pin6:
                RB6_interrupthandler_high=int_obj->ext_interrupthandler_high;
                RB6_interrupthandler_low=int_obj->ext_interrupthandler_low;
                break;
            case pin7:
                RB7_interrupthandler_high=int_obj->ext_interrupthandler_high;
                RB7_interrupthandler_low=int_obj->ext_interrupthandler_low;
                break;
            default:
                ret=E_NOT_OK;;    
        }
        
        ext_RBx_interruptenable();
    }
    return ret;
}
/**
 * 
 * @param int_obj
 * @return 
 */
std_returntype interrupt_RBx_deinit(const interrupt_RBx_t *int_obj){
    std_returntype ret=E_OK;
    if(NULL==int_obj){
        ret=E_NOT_OK;
    }else{
         ext_RBx_interruptdisable();
        
        ext_RBx_interruptflagclear();
    }
    return ret;
}




static std_returntype interrupt_INTx_enable (const interrupt_INTx_t *int_obj){
    std_returntype ret=E_OK;
    if(NULL==int_obj){
        ret=E_NOT_OK;
    }else{
        switch(int_obj->source){
            case interrupt_external_INT0:
                #if interrupt_priority_levels_enable != interrupt_feature_enable
                interrupt_peripheralinterruptenable();
                interrupt_globalinterruptenable();// these keys to make interrupt happens
                #else
                interrupt_globalinterrupthighenable();//to enable high interrupt enable as int0 is always high
                #endif
                ext_INT0_interruptenable();
                ret=E_OK;
                break;
            case interrupt_external_INT1:
                #if interrupt_priority_levels_enable != interrupt_feature_enable
                interrupt_peripheralinterruptenable();
                interrupt_globalinterruptenable();// these keys to make interrupt happens
                #else
                interrupt_prioritylevelsenable();
                if(interrupt_low_priority==int_obj->priority){
                    interrupt_globalinterruptlowenable();//to enable global low interrupt
                }else if(interrupt_high_priority==int_obj->priority){
                    interrupt_globalinterrupthighenable();
                }else{
                    
                }
                #endif
                ext_INT1_interruptenable();
                ret=E_OK;
                break;
            case interrupt_external_INT2:
                #if interrupt_priority_levels_enable != interrupt_feature_enable
                //this above to make us enable priority featur bu one down it to enable it in micro controller
                interrupt_peripheralinterruptenable();
                interrupt_globalinterruptenable();// these keys to make interrupt happens
                #else
                interrupt_prioritylevelsenable();//we must enable priority feature if we want interrupt priority
                if(interrupt_low_priority==int_obj->priority){
                    interrupt_globalinterruptlowenable();
                }else if(interrupt_high_priority==int_obj->priority){
                    interrupt_globalinterrupthighenable();
                }else{
                    
                }
                #endif
                ext_INT2_interruptenable();
                ret=E_OK;
                break;    
            default:ret=E_NOT_OK;break;    
        }
        
    }
    return ret;
}
static std_returntype interrupt_INTx_disable (const interrupt_INTx_t *int_obj){
    std_returntype ret=E_OK;
    if(NULL==int_obj){
        ret=E_NOT_OK;
    }else{
        switch(int_obj->source){
            case interrupt_external_INT0:
                ext_INT0_interruptdisable();
                ret=E_OK;
                break;
            case interrupt_external_INT1:
                ext_INT1_interruptdisable();
                ret=E_OK;
                break;
            case interrupt_external_INT2:
                ext_INT2_interruptdisable();
                ret=E_OK;
                break;    
            default:ret=E_NOT_OK;break;    
        }
    }
    return ret;
}
#if interrupt_priority_levels_enable==interrupt_feature_enable
//as we said int0 is always high
static std_returntype interrupt_INTx_priority_init (const interrupt_INTx_t *int_obj){
    std_returntype ret=E_OK;
    if(NULL==int_obj){
        ret=E_NOT_OK;
    }else{
        switch(int_obj->source){
            case interrupt_external_INT1:
                if(interrupt_low_priority==int_obj->priority){
                    ext_INT1_lowpriorityset();
                }else if(interrupt_high_priority==int_obj->priority){
                    ext_INT1_highpriorityset();
                }else{
                    
                }
                ret=E_OK;
                break;
            case interrupt_external_INT2:
                if(interrupt_low_priority==int_obj->priority){
                    ext_INT2_lowpriorityset();
                }else if(interrupt_high_priority==int_obj->priority){
                    ext_INT2_highpriorityset();
                }else{
                    
                }
                ret=E_OK;
                break;    
            default:ret=E_NOT_OK;break;    
        }
    }
    return ret;
}
#endif
static std_returntype interrupt_INTx_edge_init (const interrupt_INTx_t *int_obj){
    std_returntype ret=E_OK;
    if(NULL==int_obj){
        ret=E_NOT_OK;
    }else{
        switch(int_obj->source){
            case interrupt_external_INT0:
                if(interrupt_falling_edge==int_obj->edge){
                    ext_INT0_fallingedgeset();
                }else if(interrupt_rising_edge==int_obj->edge){
                    ext_INT0_risingedgeset();
                }else{
                    
                }
                ret=E_OK;
                break;
            case interrupt_external_INT1:
                if(interrupt_falling_edge==int_obj->edge){
                    ext_INT1_fallingedgeset();
                }else if(interrupt_rising_edge==int_obj->edge){
                    ext_INT1_risingedgeset();
                }else{
                    
                }
                ret=E_OK;
                break;
            case interrupt_external_INT2:
                if(interrupt_falling_edge==int_obj->edge){
                    ext_INT2_fallingedgeset();
                }else if(interrupt_rising_edge==int_obj->edge){
                    ext_INT2_risingedgeset();
                }else{
                    
                }
                ret=E_OK;
                break;    
            default:ret=E_NOT_OK;break;    
        }
    }
    return ret;
}
/**
 * 
 * @param int_obj
 * @return 
 */
static std_returntype interrupt_INTx_pin_init (const interrupt_INTx_t *int_obj){
    std_returntype ret=E_OK;
    if(NULL==int_obj){
        ret=E_NOT_OK;
    }else{
        ret=gpio_pin_direction_initilaize(&(int_obj->mcu_pin));// we only need to configure it's direction
    }
    return ret;
}
static std_returntype interrupt_INTx_clear_flag (const interrupt_INTx_t *int_obj){
    std_returntype ret=E_OK;
    if(NULL==int_obj){
        ret=E_NOT_OK;
    }else{
        switch(int_obj->source){
            case interrupt_external_INT0:
                ext_INT0_interruptflagclear();
                ret=E_OK;
                break;
            case interrupt_external_INT1:
                ext_INT1_interruptflagclear();
                ret=E_OK;
                break;
            case interrupt_external_INT2:
                ext_INT2_interruptflagclear();
                ret=E_OK;
                break;    
            default:ret=E_NOT_OK;break;    
        }
    }
    return ret;
}

static std_returntype INT0_setinterrupthandler(void (*interrupthandler)(void)){
    std_returntype ret=E_OK;
    if(NULL==interrupthandler){
        ret=E_NOT_OK;
    }else{
       INT0_interrupthandler=interrupthandler;//like that we assign int_app in main.c in this pointer to function
       ret=E_OK;
    }
    return ret;
}
static std_returntype INT1_setinterrupthandler(void (*interrupthandler)(void)){
    std_returntype ret=E_OK;
    if(NULL==interrupthandler){
        ret=E_NOT_OK;
    }else{
       INT1_interrupthandler=interrupthandler;
       ret=E_OK;
    }
    return ret;
}
static std_returntype INT2_setinterrupthandler(void (*interrupthandler)(void)){
    std_returntype ret=E_OK;
    if(NULL==interrupthandler){
        ret=E_NOT_OK;
    }else{
       INT2_interrupthandler=interrupthandler;
       ret=E_OK;
    }
    return ret;
}
static std_returntype interrupt_INTx_setinterrupthandler(const interrupt_INTx_t *int_obj){
    std_returntype ret=E_OK;
    if(NULL==int_obj){
        ret=E_NOT_OK;
    }else{
        switch(int_obj->source){
            case interrupt_external_INT0:
                ret=INT0_setinterrupthandler(int_obj->ext_interrupthandler);
                ret=E_OK;
                break;
            case interrupt_external_INT1:
                ret=INT1_setinterrupthandler(int_obj->ext_interrupthandler);
                ret=E_OK;
                break;
            case interrupt_external_INT2:
                ret=INT2_setinterrupthandler(int_obj->ext_interrupthandler);
                ret=E_OK;
                break;    
            default:ret=E_NOT_OK;break;    
        }
    }
}