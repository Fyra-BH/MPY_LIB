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
#include <signal.h>
#include <time.h>
#include <string.h>

#define mem_alloc malloc
#define mem_free free

static void simu_lowlevel_dac_init(uint16_t bitwidth);
static void simu_lowlevel_timer_init(uint16_t freq);
static void simu_timer_cb_register(void (*cb)(void *args), void *args);
static void simu_dac_out(void *p);

#define lowlevel_dac_init simu_lowlevel_dac_init
#define lowlevel_timer_init simu_lowlevel_timer_init
#define timer_cb_register simu_timer_cb_register
#define dac_out simu_dac_out

#else
#include "py/runtime.h"
#define mem_alloc m_malloc
#define mem_free m_free

#endif

//DEBUG_ON_PC

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
    dac->state_flag = STA_IDLE; //初始空闲状态
    dac->bitwidth = bitwidth;
    dac->buff_size = buff_size;
    dac->buff1_sta = buff_writable;
    dac->buff2_sta = buff_writable;
    dac->sample_freq = sample_freq;
    dac->buff_size = buff_size;
    dac->buff1 = (uint8_t *)mem_alloc(buff_size);
    dac->buff2 = (uint8_t *)mem_alloc(buff_size);
    dac->buff1_offset = 0;
    dac->buff2_offset = 0;
    if (dac->buff1 == NULL || dac->buff2 == NULL)
    {
        printf("malloc failed!\n");
        return 1;
    }

    lowlevel_dac_init(bitwidth);             //配置dac
    lowlevel_timer_init(sample_freq);        //配置定时器，使能中断，频率为 sample_freq Hz
    timer_cb_register(work_cb, (void *)dac); //注册回调函数

    return 0;
}
/**
 * @brief 向dac模块中写数据（可能阻塞）
 * 
 * @param dac dac_ctrl_word指针
 * @param buff 要写入的缓冲区
 * @param size 数据长度
 * @return uint8_t 0代表成功
 */
uint8_t powerDaWrite(dac_ctrl_word *dac, uint8_t *buff, uint16_t size)
{
    // if (dac->state_flag == STA_IDLE)
    //     return 1; //还未开始

    uint16_t b_wrtitten = 0;
    while (b_wrtitten != size)
    {
        if (dac->buff1_sta == buff_writable)
        {
            uint16_t remain = dac->buff_size - dac->buff1_offset;
            if (size > remain) //一次写满还有剩余
            {
                memcpy(dac->buff1 + dac->buff1_offset, buff, remain);
                dac->buff1_sta = buff_full; //标记满
                dac->buff1_offset = 0;
                b_wrtitten += remain;
            }
            else //一次写不满
            {
                memcpy(dac->buff1 + dac->buff1_offset, buff, size);
                b_wrtitten += size;
                dac->buff1_offset += size;
            }
        }
        else if (dac->buff2_sta == buff_writable)
        {
            uint16_t remain = dac->buff_size - dac->buff2_offset;
            if (size > remain) //一次写满还有剩余
            {
                memcpy(dac->buff2 + dac->buff2_offset, buff, remain);
                dac->buff2_sta = buff_full; //标记满
                dac->buff2_offset = 0;
                b_wrtitten += remain;
            }
            else //一次写不满
            {
                memcpy(dac->buff2 + dac->buff2_offset, buff, size);
                b_wrtitten += size;
                dac->buff2_offset += size;
            }
        }
    }
}

//工作回调函数
void work_cb(void *args)
{
    dac_ctrl_word *dac = (dac_ctrl_word *)args;
    // printf("work cb\n");
    // printf("dac->bitwidth = %d\n", dac->bitwidth);
    // printf("dac->bitwidth = %d\n", dac->buff_size);
    dac->state_flag = STA_WORKING;

    if (dac->buff1_sta == buff_full) //开始传输
        dac->buff1_sta = busy_reading;
    else if (dac->buff2_sta == buff_full)
        dac->buff2_sta = busy_reading;

    if (dac->buff1_sta == busy_reading)
    {
        if (dac->buff1_offset == dac->buff_size)
            dac->buff1_sta = buff_writable;
        else
        {
            dac_out(dac->buff1 + dac->buff1_offset);
            dac->buff1_offset += 2; //dac数据为uint16型
        }
    }
    else if (dac->buff2_sta == busy_reading)
    {
        if (dac->buff2_offset == dac->buff_size)
            dac->buff2_sta = buff_writable;
        else
        {
            dac_out(dac->buff2 + dac->buff2_offset);
            dac->buff2_offset += 2; //dac数据为uint16型
        }
    }
}

#ifdef DEBUG_ON_PC

uint16_t simu_bitwidth = 0;
void (*timer_cb)(void *args) = NULL;
void *timer_cb_args = NULL;

void handle()
{
    // time_t t;
    // char p[32];

    // time(&t);
    // strftime(p, sizeof(p), "%T", localtime(&t));

    // printf("time is %s\n", p);
    if (timer_cb != NULL)
    {
        timer_cb(timer_cb_args);
    }
}

static void simu_lowlevel_dac_init(uint16_t bitwidth)
{
    simu_bitwidth = bitwidth;
}

/**
 * @brief 设置中断频率
 * 
 * @param freq 
 */
static void simu_lowlevel_timer_init(uint16_t freq)
{
    struct sigevent evp;
    struct itimerspec ts;
    timer_t timer;
    int ret;
    memset(&evp, 0, sizeof(struct sigevent));

    evp.sigev_value.sival_ptr = &timer;
    evp.sigev_notify = SIGEV_SIGNAL;
    evp.sigev_signo = SIGUSR1;
    signal(SIGUSR1, handle);

    ret = timer_create(CLOCK_REALTIME, &evp, &timer);
    if (ret)
        perror("timer_create");

    ts.it_interval.tv_sec = 0;
    ts.it_interval.tv_nsec = (long)(1000000000 / freq);
    ts.it_value.tv_sec = 0;
    ts.it_value.tv_nsec = 1000000;

    ret = timer_settime(timer, 0, &ts, NULL);
    if (ret)
        perror("timer_settime");
}
static void simu_timer_cb_register(void (*cb)(void *args), void *args)
{
    timer_cb = cb;
    timer_cb_args = args;
}

static void simu_dac_out(void *p)
{
    uint16_t *temp = (uint16_t *)p;
    printf("%d\n", *temp);
}

#endif

int main(int argc, char const *argv[])
{
    dac_ctrl_word dac;
    powerDaInit(&dac, 12, 1024, 20);
    uint16_t test_buff[128];
    for (size_t i = 0; i < 128; i++)
    {
        test_buff[i] = i;
    }

    /* code */
    while (1)
    {
        powerDaWrite(&dac, (uint8_t *)test_buff, 128 * 2);
    }

    return 0;
}