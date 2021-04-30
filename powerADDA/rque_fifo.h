/**
 * @file rque_fifo.h
 * @author Fyra-BH (fyra@foxmail.com)
 * @brief 环形队列
 * @version 0.1
 * @date 2021-04-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef __RQUE__H
#define __RQUE__H

#include <stdint.h>

#ifndef MAX_BUFF_SIZE
#define MAX_BUFF_SIZE 1024
#endif

typedef struct
{
    uint8_t full;
    uint8_t empty;
    uint16_t head;
    uint16_t rear;
    uint16_t len;
#ifdef R_QUE_USE_MALLOC
    uint16_t *buff;
#else
    uint16_t buff[MAX_BUFF_SIZE];
#endif
} rque_t;

uint8_t rque_init(rque_t *rque);
uint8_t rque_write(rque_t *rque, uint16_t data);
uint8_t rque_read(rque_t *rque, uint16_t *data);

#endif
