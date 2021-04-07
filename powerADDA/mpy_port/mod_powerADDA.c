#include "py/runtime.h"
#include "py/objarray.h"

#include "stdio.h"
#include "rque_fifo.h"

extern void POWERADDA_TIM4_Init(void);
extern void timer4_start_it(void);
extern void POWERADDA_DAC_Init(void);
extern void dac_start(void);

rque_t Q_dac = {0};
rque_t Q_adc = {0};

STATIC mp_obj_t powerADDA_init(void)
{
    POWERADDA_TIM4_Init();
    timer4_start_it();
    POWERADDA_DAC_Init();
    dac_start();
    rque_init(&Q_dac);
    rque_init(&Q_adc);
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
        while (rque_write(&Q_dac, *p / 256))
            ; //do nothing
        while (rque_write(&Q_dac, *p % 256))
            ; //do nothing
        p++;
    }

    return mp_obj_new_int(array->len);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(powerADDA_send_to_dac_obj, powerADDA_send_to_dac);

STATIC const mp_rom_map_elem_t powerADDA_module_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_powerADDA)},
    {MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&powerADDA_init_obj)},
    {MP_ROM_QSTR(MP_QSTR_send_to_dac), MP_ROM_PTR(&powerADDA_send_to_dac_obj)},
};
STATIC MP_DEFINE_CONST_DICT(powerADDA_module_globals, powerADDA_module_globals_table);

// Define module object.
const mp_obj_module_t powerADDA_user_cmodule = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&powerADDA_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_powerADDA, powerADDA_user_cmodule, 1);
