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

#ifdef DEBUG_ON_PC
    #include <stdlib.h>
    #include <stdio.h>
    #define mem_alloc malloc
    #define mem_free free
#else
    #include "py/runtime.h"
    #define mem_alloc m_malloc
    #define mem_free m_free
#endif

// int main(int argc, char const *argv[])
// {
//     enum_dabuff_state sta;
//     sta = busy_writing;
// #ifdef DEBUG_ON_PC
//     printf("sta = %d\n", sta);
// #endif
//     return 0;
// }



