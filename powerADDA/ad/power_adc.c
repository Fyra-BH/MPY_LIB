/**
 * @file power_adc.c
 * @author Fyra-BH (fyra@foxmail.com)
 * @brief a better adc than pyb.adc
 * @version 0.1
 * @date 2021-04-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "power_adc.h"

#define DEBUG_ON_PC

#ifdef DEBUG_ON_PC
    #include <stdlib.h>
    #define mem_alloc malloc
    #define mem_free free
#else
    #include "py/runtime.h"
    #define mem_alloc m_malloc
    #define mem_free m_free
#endif



