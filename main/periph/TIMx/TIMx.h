#ifndef TIM5_H
#define TIM5_H
#include "stm32.h"
#include "GPIO.h"
enum{
    TIME_BASE_UNIT = 0,
    ENCODER_MODE,
};



struct Encoder_mode_settings{
    //void;
    uint16_t array;
    struct
    {
       //-- port A of encoder
       unsigned TI1_en:1;
       uint8_t TI1_pin;
       uint8_t TI1_port;

       //-- port B of encoder
       unsigned TI2_en:1;
       uint8_t TI2_pin;
       uint8_t TI2_port;
    };
    
};

struct Timer_mode_settings{
    uint16_t prescaller;
    uint16_t array;
};
struct TIMx_chx
{
    u32 TIMx;
    u8 ch;
};


//void TIM5_init(uint16_t prescaller,uint16_t array);
stm32_api void TIM5_init(uint32_t mode, void* settings);
stm32_api void TIM5_time_base_unit(uint16_t prescaller,uint16_t array);
stm32_api void TIM5_encoder_interface_mode(struct Encoder_mode_settings* cfg);
// void TIM5_PWM_output_mode();
stm32_api void PWM_output_mode(struct TIMx_chx* ret_handle,uint8_t pin, uint8_t port);
stm32_api void PWM_setup(struct TIMx_chx* tim, u16 T_us, u16 duty_cycle);
/** Examples:

void example(){

    struct Encoder_mode_settings cfg;
    cfg.array = 0xffff;
    //--in A
    cfg.TI1_en = true;
    cfg.TI1_pin = 5;
    cfg.TI1_port ='A';
    //--in B
    cfg.TI2_en = true;
    cfg.TI1_pin = 15;
    cfg.TI1_port ='C';
    //--call initialization;    
    TIM5_init(ENCODER_MODE,&cfg);

}







*/

#endif // !TIM5_H
