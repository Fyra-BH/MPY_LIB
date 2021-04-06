/**
 * @file rque_fifo.c
 * @author Fyra-BH (fyra@foxmail.com)
 * @brief 环形队列
 * @version 0.1
 * @date 2021-04-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "rque_fifo.h"
#include "stdlib.h"
/**
 * @brief 初始化一个队列
 * 
 * @param rque 
 * @return uint8_t 
 */
uint8_t rque_init(rque_t *rque)
{
    uint16_t len = MAX_BUFF_SIZE;
#ifdef USE_MALLOC
    if (!(rque->buff = (uint8_t)malloc(len)))
    {
        return 1;
    }
#else
    for (size_t i = 0; i < len; i++)
    {
        rque->buff[i] = 0;
    }
#endif
    rque->head = 0;
    rque->rear = 0;
    rque->full = 0;
    rque->empty = 1;

    return 0;
}

/**
 * @brief 队列写入
 * 
 * @param rque 队列结构体指针
 * @param data 要写入的数据
 * @return uint8_t 返回0代表成功
 */
uint8_t rque_write(rque_t *rque, uint8_t data)
{
    uint16_t max_size = MAX_BUFF_SIZE;
    uint8_t ret = 0;
    if (rque->empty)
    {
        rque->empty = 0;
    }

    if(rque->full == 1) //阻塞
    {
        ret = 1;
        return ret;
    }
    rque->buff[rque->head++ % max_size ] = data;

    if (rque->head % max_size == rque->rear % max_size)
    {
        rque->full = 1;
    }
    return ret;
}


/**
 * @brief 从队列读一个数据
 * 
 * @param rque 队列结构体指针
 * @param data 要读出的数据
 * @return uint8_t 返回0代表成功
 */
uint8_t rque_read(rque_t *rque, uint8_t *data)
{
    uint16_t max_size = MAX_BUFF_SIZE;
    uint8_t ret = 0;
    if (rque->empty == 1)
    {
        ret = 1;
        return ret;
    }
    if (rque->full)
    {
        rque->full = 0;
    }
    *data = rque->buff[rque->rear++ % max_size ];
    if (rque->head % max_size == rque->rear % max_size)
    {
        rque->empty = 1;
    }
    return ret;
}
