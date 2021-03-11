#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_spi_flash.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_partition.h"
#include "driver/i2s.h"
#include "driver/adc.h"


//i2s number
#define EXAMPLE_I2S_NUM           (0)
//i2s sample rate
#define EXAMPLE_I2S_SAMPLE_RATE   (16000)
//i2s data bits
#define EXAMPLE_I2S_SAMPLE_BITS   (16)
//enable display buffer for debug
#define EXAMPLE_I2S_BUF_DEBUG     (0)
//I2S read buffer length
#define EXAMPLE_I2S_READ_LEN      (16 * 1024)
//I2S data format
#define EXAMPLE_I2S_FORMAT        (I2S_CHANNEL_FMT_RIGHT_LEFT)
//I2S channel number
#define EXAMPLE_I2S_CHANNEL_NUM   ((EXAMPLE_I2S_FORMAT < I2S_CHANNEL_FMT_ONLY_RIGHT) ? (2) : (1))
//I2S built-in ADC unit
#define I2S_ADC_UNIT              ADC_UNIT_1
//I2S built-in ADC channel
#define I2S_ADC_CHANNEL           ADC1_CHANNEL_0

/**
 * @brief I2S ADC/DAC mode init.
 */
void example_i2s_init(int fs)
{
     int i2s_num = EXAMPLE_I2S_NUM;
     i2s_config_t i2s_config = {
        .mode = I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN | I2S_MODE_ADC_BUILT_IN,
        .sample_rate =  fs,
        .bits_per_sample = EXAMPLE_I2S_SAMPLE_BITS,
        .communication_format = I2S_COMM_FORMAT_I2S_MSB,
        .channel_format = EXAMPLE_I2S_FORMAT,
        .intr_alloc_flags = 0,
        .dma_buf_count = 2,
        .dma_buf_len = 1024,
        .use_apll = 1,
     };
     i2s_driver_uninstall(i2s_num);
     //install and start i2s driver
     i2s_driver_install(i2s_num, &i2s_config, 0, NULL);
     //init DAC pad
     i2s_set_dac_mode(I2S_DAC_CHANNEL_BOTH_EN);
     //init ADC pad
     i2s_set_adc_mode(I2S_ADC_UNIT, I2S_ADC_CHANNEL);

   return;
}

//uint16数组转float数组
void s16_to_float(int len_of_int, void* src, void* dst)
{
   short unsigned int * _src = (short unsigned int*) src;
   float* _dst = (float*) dst;
   for (size_t i = 0; i < len_of_int; i++)
   {
      * _dst++ = (float)(* _src++) *2 / 4096;
   }
}

size_t bytes_read;
//读取i2s
int my_i2s_read(int Nbytes, void* buff)
{
   int ret;
   i2s_adc_enable(EXAMPLE_I2S_NUM);
   i2s_read(EXAMPLE_I2S_NUM, (void*) buff, Nbytes, &bytes_read, portMAX_DELAY);
   ret = bytes_read;
   i2s_adc_disable(EXAMPLE_I2S_NUM);
   return ret;
}

