#ifndef POWER_DAC__H
#define POWER_DAC__H

#include <stdint.h>

/**
 * @file power_dac.h
 * @author Fyra-BH (fyra@foxmail.com)
 * @brief a better dac than pyb.dac
 * @version 0.1
 * @date 2021-04-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

typedef enum 
{
    empty = 1,
    busy_writing,
    full,
    busy_reading,
}enum_dabuff_state;

/**
 * @brief used to control dac
 * 
 */
typedef struct 
{
    uint8_t     state_flag;     //模块当前状态
    uint8_t     dac_bitwidth;   //dac位数
    uint16_t    buff_size;      //缓冲区大小
    uint32_t    sample_freq;    //采样率
    void        *buff1;         //缓冲区1
    void        *buff2;         //缓冲区2
    enum_dabuff_state buff1_sta;//缓冲区1状态
    enum_dabuff_state buff2_sta;//缓冲区2状态
}dac_ctrl_word;


#endif // POWER_DAC__H
