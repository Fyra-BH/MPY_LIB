#include "py/runtime.h"
#include "py/objarray.h"

#include "stdio.h"
#include "rque_fifo.h"

extern void POWERADDA_TIM4_Init(void);
extern void timer4_start_it(void);
extern void POWERADDA_DAC_Init(void);
extern void dac_start(void);
extern void POWERADDA_ADC1_Init(void);
extern void adc_start(void);

rque_t Q_dac = {0};
rque_t Q_adc = {0};

STATIC mp_obj_t powerADDA_init(void)
{
    //初始化两个队列
    rque_init(&Q_dac);
    rque_init(&Q_adc);

    POWERADDA_DAC_Init();
    dac_start();
    POWERADDA_ADC1_Init();
    adc_start();

    POWERADDA_TIM4_Init();
    timer4_start_it();

    return mp_obj_new_int(0);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(powerADDA_init_obj, powerADDA_init);

STATIC mp_obj_t powerADDA_send_to_dac(mp_obj_t buffer)
{
    mp_obj_array_t *array = (mp_obj_array_t *)buffer;
    if (array->typecode != 'H')
    {
        printf("please input uin16 array!\n");
        printf("eg : array.array('H',)\n");
        return mp_obj_new_int(0);
    }

    uint16_t *p = (uint16_t *)array->items;
    for (int i = 0; i < array->len; i++)
    {
        // printf("write %d\n",*p);
        rque_set_lock(&Q_dac,1);
        while (rque_write(&Q_dac, *p / 256)) //先写高字节
            ;                                //do nothing
        while (rque_write(&Q_dac, *p % 256)) //再写低字节
            ;                                //do nothing
        rque_set_lock(&Q_dac,0);
        p++;
    }

    return mp_obj_new_int(array->len);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(powerADDA_send_to_dac_obj, powerADDA_send_to_dac);

STATIC mp_obj_t powerADDA_read_from_adc(mp_obj_t buffer)
{
    mp_obj_array_t *array = (mp_obj_array_t *)buffer;
    if (array->typecode != 'H')
    {
        printf("ERROR \n\n");
        printf("please input a uin16 array to receive data!\n");
        printf("eg :\n");
        printf("\timport array\n");
        printf("\tbuff = array.array('H',[0]*1024)\n");
        printf("\tpowerADDA.read_from_adc(buff) # now data in buff\n\n");

        return mp_obj_new_bool(0);
    }
    uint16_t *p = (uint16_t *)array->items;
    uint8_t tmp = 0;
    for (int i = 0; i < array->len; i++)
    {
        // printf("write %d\n",*p);
        while (rque_read(&Q_adc, &tmp))
            ; //do nothing
        *p = tmp * 256; //先读高字节

        while (rque_read(&Q_adc, &tmp))
            ; //do nothing
        *p += tmp; //再读低字节

        p++;
    }
    return buffer;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_1(powerADDA_read_from_adc_obj, powerADDA_read_from_adc);

STATIC mp_obj_t powerADDA_status(void)
{
    printf("Q_adc.full = %d\n",Q_adc.full);
    printf("Q_adc.empty = %d\n",Q_adc.empty);
    printf("Q_dac.full = %d\n",Q_dac.full);
    printf("Q_dac.empty = %d\n",Q_dac.empty);

    return  mp_obj_new_bool(1);
}

STATIC MP_DEFINE_CONST_FUN_OBJ_0(powerADDA_status_obj, powerADDA_status);

STATIC const mp_rom_map_elem_t powerADDA_module_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_powerADDA)},
    {MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&powerADDA_init_obj)},
    {MP_ROM_QSTR(MP_QSTR_status), MP_ROM_PTR(&powerADDA_status_obj)},
    {MP_ROM_QSTR(MP_QSTR_send_to_dac), MP_ROM_PTR(&powerADDA_send_to_dac_obj)},
    {MP_ROM_QSTR(MP_QSTR_read_from_adc), MP_ROM_PTR(&powerADDA_read_from_adc_obj)},
};
STATIC MP_DEFINE_CONST_DICT(powerADDA_module_globals, powerADDA_module_globals_table);

// Define module object.
const mp_obj_module_t powerADDA_user_cmodule = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&powerADDA_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_powerADDA, powerADDA_user_cmodule, 1);
