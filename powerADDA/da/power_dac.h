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
#ifndef POWER_DAC__H
#define POWER_DAC__H

#include <stdint.h>

#define STA_IDLE        1       //空闲状态
#define STA_WAITING     2       //等待状态（缓冲区未满）
#define STA_BUSY        3       //忙状态（正在读缓存）

typedef enum 
{
    buff_empty = 1,
    busy_writing,
    buff_full,
    busy_reading,
}enum_dabuff_state;

/**
 * @brief use this to control dac
 * 
 */
typedef struct 
{
    uint16_t    state_flag;         //模块当前状态
    uint16_t    bitwidth;           //dac位数
    uint16_t    buff_size;          //缓冲区大小(以字节为单位)
    uint16_t    sample_freq;        //采样率
    void        *buff1;             //缓冲区1
    void        *buff2;             //缓冲区2
    enum_dabuff_state buff1_sta;    //缓冲区1状态
    enum_dabuff_state buff2_sta;    //缓冲区2状态
    void (*callback)(void *args);   //回调函数
}dac_ctrl_word;

int8_t powerDaInit(dac_ctrl_word *dac, uint16_t bitwidth, uint16_t buff_size, uint16_t sample_freq);

#endif // POWER_DAC__H
