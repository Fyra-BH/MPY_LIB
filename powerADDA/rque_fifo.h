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
#ifndef RQUE__H
#define RQUE__H

#include <stdint.h>

#define MAX_BUFF_SIZE 4096

typedef struct
{
    uint8_t full;
    uint8_t empty;
    uint32_t head;
    uint32_t rear;
#ifdef R_QUE_USE_MALLOC
    uint8_t *buff;
#else
    uint8_t buff[MAX_BUFF_SIZE];
#endif
} rque_t;

uint8_t rque_init(rque_t *rque);
uint8_t rque_write(rque_t *rque, uint8_t data);
uint8_t rque_read(rque_t *rque, uint8_t *data);

#endif
