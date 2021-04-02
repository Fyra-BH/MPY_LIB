/**
 * @file power_dac.c
 * @author Fyra-BH (fyra@foxmail.com)
 * @brief a better dac than pyb.dac
 * @version 0.1
 * @date 2021-04-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "power_dac.h"
#include <stdio.h>

#define DEBUG_ON_PC

#ifdef DEBUG_ON_PC
    #include <stdlib.h>
    #define mem_alloc malloc
    #define mem_free free

    static void simu_lowlevel_dac_init(uint16_t bitwidth);
    static void simu_lowlevel_timer_init(uint16_t freq);
    static void simu_timer_cb_register(void (*cb)(void *args), void *args);
    static void simu_work_cb(void *args);

    #define lowlevel_dac_init       simu_lowlevel_dac_init
    #define lowlevel_timer_init     simu_lowlevel_timer_init
    #define timer_cb_register       simu_timer_cb_register
    #define work_cb            simu_work_cb
#else
    #include "py/runtime.h"
    #define mem_alloc m_malloc
    #define mem_free m_free
#endif


/**
 * @brief 初始化一个dac_ctrl_word结构体
 * 
 * @param dac           结构体指针
 * @param bitwidth      dac的精度
 * @param buff_size     缓冲区大小
 * @param sample_freq   采样率
 * @return int8_t       返回0代表成功
 */
int8_t powerDaInit(dac_ctrl_word *dac, uint16_t bitwidth, uint16_t buff_size, uint16_t sample_freq)
{
    dac->state_flag     =   STA_IDLE; //初始空闲状态
    dac->bitwidth       =   bitwidth;
    dac->buff_size      =   buff_size;
    dac->buff1_sta      =   buff_empty;
    dac->buff2_sta      =   buff_empty;
    dac->sample_freq    =   sample_freq;

    lowlevel_dac_init(bitwidth);                    //配置dac
    lowlevel_timer_init(sample_freq);               //配置定时器，使能中断，频率为 sample_freq Hz
    timer_cb_register(work_cb, NULL);                //注册回调函数

    dac->buff_size = buff_size;
    dac->buff1 = mem_alloc(buff_size);
    dac->buff2 = mem_alloc(buff_size);
    if(dac->buff1 == NULL || dac->buff2 == NULL)
    {
        printf("malloc failed!\n");
        return 1;
    }
    return 0;
}


#ifdef DEBUG_ON_PC

uint16_t simu_bitwidth = 0;
void (*timer_cb)(void *args) = NULL;
void *timer_cb_args = NULL;

static void simu_lowlevel_dac_init(uint16_t bitwidth)
{
    simu_bitwidth = bitwidth;
}

static void simu_lowlevel_timer_init(uint16_t freq)
{
    ;
}
static void simu_timer_cb_register(void (*cb)(void *args), void *args)
{
    timer_cb = cb;
}
static void simu_work_cb(void *args)
{
    ;
}

#endif

int main(int argc, char const *argv[])
{
    dac_ctrl_word dac;
    powerDaInit(&dac, 12, 1024,10000);
    return 0;
}